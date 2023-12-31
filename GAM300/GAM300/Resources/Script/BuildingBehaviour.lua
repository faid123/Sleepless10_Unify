-- Component Variables
local eTransform
local eMaterial
local eEntID
local eHealth
local eFlashTimer
local Unified
local offsetPos = 10.0
local eRigidBody
local canApplyLaserDamage = true
local laserDmgDelayTimer  = 0.0
local pCubePrefabID
local pos = Vector3:new(0.0, 7.0, 0.0)
local rot = Quat:new(0,0,0,1)

local scale = Vector3:new(0.1, 0.1, 0.1)

local pAmmoCubePrefabID

local newRot        = Quat:new()
local forwardPrime  = Vector3:new()
local forward       = Vector3:new(0.0, 0.0, -1.0)

local spawnedCubes = false
local spawnedAmmoCubes = false

-- for wanted level
local playerLuaScript
local playerID
local changeWanted = false

-- Building Scenario
local buildScenarioID

-- Tree Scenario
local treeScenarioID

local UIObjID

local laser_BuildingHit
local explode_BuildingHit

-- Building Shake
local shakeTimer = 0.0
local shakeTime = 0.5
local startingPos = Vector3:new()
local startPosStored = false

local particulate = false
local lifetime = 2.0

-- Awake is called once and only once when script is loaded.
function Awake()
    eHealth = 25
    eFlashTimer = 1
    Unified = false
end

-- Start is called once and only once after Awake() is called.
function Start()
    ------Console.WriteLine("building script loaded")
    pCubePrefabID = ECS:getPrefabID("Prefab_Cube")
    pAmmoCubePrefabID = ECS:getPrefabID("Prefab_AmmoCube")
    playerID = ECS:getEntityID("Player")

    buildScenarioID = ECS:getEntityTagged("BuildingScenario")
    treeScenarioID = ECS:getEntityTagged("TreeScenario")
    UIObjID = ECS:getEntityID("UI_Challenge")

end

-- Updates the Game Object per game loop
function Update(dt)
    -- 
    eEntID = ScriptSystem:getLuaEntityID()
    initInUpdate()
    
    if not startPosStored then
        startingPos.x = eTransform.pos.x
        startingPos.y = eTransform.pos.y
        startingPos.z = eTransform.pos.z
        startPosStored = true
    end

    newRot = PhysicsSystem:getGlobalPoseRotation(eEntID)
    PhysicsSystem:rotate_vector_by_quaternion(forward, newRot, forwardPrime)

    laser_BuildingHit = mContactReportCallback:getEntityHitID("Building","Laser")
    explode_BuildingHit = mContactReportCallback:getEntityHitID("Building","Explosion")

    ----Console.PrintEntityID(eEntID)
    

    if eFlashTimer > 1.5 then
        eFlashTimer = eFlashTimer - dt*10
        flashOnHit()
    end

    -- BUILDING SHAKE LOGIC
    handleShakeTimer(dt)
    buildingShake()

    --if Inputs.get_Key_Triggered(InputKey.W_KEY) then
        onHit()
    --end
    
    if (eMaterial._saturation.y > 0.0) then 
        Unified = true
        if changeWanted == false then
            local hasTag = ECS:hasTagInEntity(eEntID, "Tree")
            if not(hasTag) then
                --playerLuaScript:GetEnvironment().wantedLevel = playerLuaScript:GetEnvironment().wantedLevel + 0.1
                -- Console.WriteLine("Building WANTED LEVEL")
                -- Console.PrintFloat(playerLuaScript:GetEnvironment().wantedLevel)
                changeWanted = true
            end
        end
    end

    if (eHealth > 0 and Unified == false) then
        setSatZero()
    end

    if eHealth <= 0 then
        Unify(dt)
    end
	
	if spawnedCubes == true then
		lifetime = lifetime - dt
		--Console.PrintFloat(lifetime)
	end
	
	if lifetime < 0 then
		StopSpawnParticles()
	end

    handleLaserDmgDelay(dt)
	
end


function initInUpdate()
    if ECS:hasComponent_Material(eEntID) and ECS:hasComponent_Transform(eEntID) and ECS:hasComponent_RigidStatic(eEntID) then
        eMaterial = ECS:getComponent_Material(eEntID)
        eTransform = ECS:getComponent_Transform(eEntID)
        eRigidBody = ECS:getComponent_RigidStatic(eEntID)
    end
    -- check and get the luascriptcomponent
    if ECS:hasComponent_LuaScriptCom(playerID) then
        playerLuaScript = ECS:getComponent_LuaScriptCom(playerID)
    end
end

function setSatZero()

    eMaterial._saturation.y = 0
end

function setSatOne()

    eMaterial._saturation.y = 1
end

function onHit()
    if eRigidBody.getHit and mContactReportCallback.bulletHitBuilding and eEntID == mContactReportCallback._detectedBuilding  then
        if Unified == false then
            AudioSystem:PlaySFX("SFX_HitMarker", playerID)
            eHealth = eHealth - 5
            eFlashTimer = 8
            shakeTimer = shakeTime
        end
    elseif eRigidBody.getHit and mContactReportCallback.explosionHitBuilding then
        for i = 1, #explode_BuildingHit do
            if Unified == false and explode_BuildingHit[i] == eEntID then
                local explosionID = mContactReportCallback._detectedExplosion
                if not ECS:hasTagInEntity(explosionID, "Boss_Explosion") then
                    if Unified == false then
                        AudioSystem:PlaySFX("SFX_HitMarker", playerID)
                        eHealth = eHealth - 13
                        eFlashTimer = 8
                        shakeTimer = shakeTime
                        break
                    end
                end
            end
        end
    elseif eRigidBody.getHit and mContactReportCallback.laserHitBuilding and canApplyLaserDamage then
        for i = 1, #laser_BuildingHit do
            if Unified == false and laser_BuildingHit[i] == eEntID and not ECS:hasTagInEntity(mContactReportCallback._detectedLaser, "Blocked") then
                AudioSystem:PlaySFX("SFX_HitMarker", playerID)
                eHealth = eHealth - 9
                eFlashTimer = 8
                if playerLuaScript:GetEnvironment().laserUlt then
                    laserDmgDelayTimer = 0.4
                else
                    laserDmgDelayTimer = 0.7
                end
                canApplyLaserDamage = false
                shakeTimer = shakeTime
                break
            end
        end
    end
    eRigidBody.getHit = false
end

function flashOnHit()

    if eFlashTimer > 1 then
            eMaterial._saturation.z = eFlashTimer
    end
end

function randomHue()

    if Unified == false then
            eMaterial._saturation.x = Random.Float() * 360
    end
end

function Unify(dt)
    if Unified == false then
        randomHue()
        Unified = true
        setSatOne()
        eMaterial._saturation.z = 1.5

        local isTree = ECS:hasTagInEntity(eEntID, "Tree")
        
        if isTree then
            for i = 1, #treeScenarioID do
                if ECS:hasEntity(treeScenarioID[i]) then
                    if ECS:hasTagInEntity(treeScenarioID[i], "TreeScenario") then
                        if ECS:getComponent_LuaScriptCom(treeScenarioID[i]):GetEnvironment().treeSceneActivated then
                            ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().treeUnifyCount = 
                                ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().treeUnifyCount + 1
                        end
                    end
                end
            end
        elseif not isTree then
            for i = 1, #buildScenarioID do
                if ECS:hasEntity(buildScenarioID[i]) then
                    if ECS:hasTagInEntity(buildScenarioID[i], "BuildingScenario") then
                        if ECS:getComponent_LuaScriptCom(buildScenarioID[i]):GetEnvironment().buildSceneActivated then
                            ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().buildUnifyCount = 
                                ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().buildUnifyCount + 1
                        end
                    end
                end
            end
        end

        if spawnedCubes == false then
            spawnCubes()
			spawnParticles()
			
			-- incr health
			
			if ECS:hasComponent_LuaScriptCom(playerID) then
				if playerLuaScript:GetEnvironment().pHealth < 75 then
				playerLuaScript:GetEnvironment().pHealth = playerLuaScript:GetEnvironment().pHealth + 5
				playerLuaScript:GetEnvironment().healTimer = 0.5
                AudioSystem:PlaySFX("SFX_HealthRegen", pEntID)
                end
                
			end		
        end	
		

        playerLuaScript:GetEnvironment().wantedLevel = playerLuaScript:GetEnvironment().wantedLevel + 0.1
    end
end

function spawnCubes()
    local hasTree = ECS:hasTagInEntity(eEntID, "Tree")
    if hasTree == false then
        for i = 10, 1, -1
        do
            --local bulletSpawnPos = pRigidDynamic.rigidPos + rightwardPrime * offsetPos
            local spawnPos = eTransform.pos + forwardPrime * offsetPos
            spawnPos.y = spawnPos.y - 3
            --spawnPos.z = spawnPos.z + 10
            local cubeID = ECS:instantiatePrefab(pCubePrefabID, "Cube", spawnPos,rot,false, false, false)
            if ECS:hasComponent_Transform(cubeID) then
        
                --PhysicsSystem:setGlobalPose(cubeID, pos, rot)
                --PhysicsSystem:setGravityFlag(cubeID, true)
            end
        end
       ------Console.WriteLine("cube spawned")
       spawnedCubes = true
    end
end

function spawnAmmoCubes()
    for i = 5, 1, -1
    do
        --local bulletSpawnPos = pRigidDynamic.rigidPos + rightwardPrime * offsetPos
        local spawnPos = eTransform.pos + forwardPrime * offsetPos
        spawnPos.y = spawnPos.y - 3
        --spawnPos.z = spawnPos.z + 10
        local AmmoCubeID = ECS:instantiatePrefab(pAmmoCubePrefabID, "AmmoCube", spawnPos,rot,false, false, false)
        if ECS:hasComponent_Transform(AmmoCubeID) then
            
            --PhysicsSystem:setGlobalPose(cubeID, pos, rot)
            --PhysicsSystem:setGravityFlag(cubeID, true)
        end
    end
   ------Console.WriteLine("AmmoCube spawned")
   spawnedAmmoCubes = true
end

function spawnParticles()

	-- Emit the particles
   if ECS:hasComponent_Emitter(eEntID) then
   
		eEmitter = ECS:getComponent_Emitter(eEntID)
		
		eEmitter.colorBegin.x = Random.Float()
		eEmitter.colorBegin.y = Random.Float()
		eEmitter.colorBegin.z = Random.Float()
		
		eEmitter.sizeBegin = 1.0
		eEmitter.sizeEnd   = 0
		
		eEmitter.velocityVariation.x = 100.0
		eEmitter.velocityVariation.y = 100.0
		eEmitter.velocityVariation.z = 100.0
		
		eEmitter.sizeVariation = 5.0
	
		eEmitter.lifeTime = 1.0
		
		
		eEmitter.emit = true	
   end
   
   --Console.WriteLine("Particulate On")

end

function StopSpawnParticles()

	-- Emit the particles
   if ECS:hasComponent_Emitter(eEntID) then	
		eEmitter.emit = false	
   end
   
   --Console.WriteLine("Particulate Off")

end

function handleLaserDmgDelay(dt)
    if laserDmgDelayTimer > 0.0 then
        laserDmgDelayTimer = laserDmgDelayTimer - dt        
    end

    if laserDmgDelayTimer <= 0.0 then
        canApplyLaserDamage = true
    end
end

function buildingShake()
    if shakeTimer > 0.0 then
        eTransform.pos.x = startingPos.x + Random.randInRange(-1.0, 1.0) * shakeTimer
        --eTransform.pos.y = startingPos.y + Random.randInRange(-1.0, 1.0) * shakeTimer
        eTransform.pos.z = startingPos.z + Random.randInRange(-1.0, 1.0) * shakeTimer
    else
        eTransform.pos.x = startingPos.x
        eTransform.pos.y = startingPos.y
        eTransform.pos.z = startingPos.z
    end
end

function handleShakeTimer(dt)
    if shakeTimer > 0.0 then
        shakeTimer = shakeTimer - dt
    end
end