--stuff goes here

-- Editable Variable

local eWalkForce            = 15
local eSpeedCap             = 25.0
local eHealth               = 12
local eFlashTimer           = 1
local emitTimer             = 3
eUnified              = false
local spawnedCubes          = false
local canApplyLaserDamage   = true
local offsetPos             = 10.0
local lifeTime              = 3.0
local laserdmgdelayTime     = 0.0
local attackDistance        = 3.0
local eIsWalking            = false

-- Component Variables
local eEntID
local eRigidDynamic
local eTransform
local eMaterial
local eLuaScript
local ePos
local eRot
local eRotQuat
local eAnim

local pEntID
local pCubePrefabID
local pLAmmoPrefabID
local pRAmmoPrefabID
local pRigidDynamic
local pPos
local pRot
local pMaterial
local pScript

local ent1Pos
local ent2Pos
local newPos
local newRot
local targetRotation
local rotationDifference
local addForce

local deathTimer = 10
local death = 0.0
local dying = false
local randomDirection   = Vector3:new()
local randomForce       = 10.0

local rot               = Quat:new(0,0,0,1)

local eSetEnemyRot      = Vector3:new(0.0, 0.0, 0.0)

local forward           = Vector3:new(0.0, 0.0, -1.0)
local backward          = Vector3:new(0.0, 0.0, 1.0)
local leftward          = Vector3:new(-1.0, 0.0, 0.0)
local rightward         = Vector3:new(1.0, 0.0, 0.0)

local forwardPrime      = Vector3:new()
local backwardPrime     = Vector3:new()
local leftwardPrime     = Vector3:new()
local rightwardPrime    = Vector3:new()

-- for scenario one
local eLuaScipt_Scenario1
local policeScenarioID

local bEmitter

local UIObjID

-- Enemy Table
enemy = {}
local laser_EnemyHit
local explode_EnemyHit

-- sounds
local unifySoundPlayed = false

-- Awake is called once and only once when script is loaded.
function Awake()
    --Console.WriteLine("EnemyChase script loaded")
end

-- Start is called once and only once after Awake() is called.
function Start()
    --------Console.WriteLine("EnemyChase script runnning")
    pLAmmoPrefabID = ECS:getPrefabID("Prefab_LAmmoCube")
    pRAmmoPrefabID = ECS:getPrefabID("Prefab_RAmmoCube")
    pCubePrefabID = ECS:getPrefabID("Prefab_Cube")
    policeScenarioID = ECS:getEntityTagged("PoliceScenario")
	UIObjID = ECS:getEntityID("UI_Challenge")
	
end

-- Updates the Game Object per game loop
function Update(dt)

    initInUpdate()
	
	bEmitter = ECS:getComponent_Emitter(eEntID)
	
	
    enemy["Enemy_Env"] = eLuaScript:GetEnvironment()


    pMaterial = ECS:getComponent_Material(pEntID)

    laser_EnemyHit = mContactReportCallback:getEntityHitID("Enemy","Laser")
    explode_EnemyHit = mContactReportCallback:getEntityHitID("Enemy","Explosion")

    enemyRotateVectorByQuaternion()
    updateEnemyPos()
    updateEnemyRot()
     if ECS:hasComponent_AnimationCom(eEntID) then
        eAnim = ECS:getComponent_AnimationCom(eEntID)
    end 

    if eFlashTimer > 1.5 then
        eFlashTimer = eFlashTimer - dt*10
        flashOnHit()
    end

    --if Inputs.get_Key_Triggered(InputKey.W_KEY) then
        onHit()
    --end
    
    
    --if (eMaterial._saturation.y > 0.0) then 
       -- eUnified = true
    --end

    if (eHealth > 0 and eUnified == false) then
    
        --Enemy looking at Player
        lookTowards(eEntID, pEntID)

        --Enemy chase at Player
        chasingTowards(eEntID, pEntID, dt)

        setSatZero()
    end

    if (eUnified == true) then
        eHealth = 0
    end

    if eHealth <= 0 then
        
        deathAnim()
        --randomMovement(eEntID)
        handleEnemyLifeTime(dt)
    end
	
    if death >= 0 then
        death = death - dt*10
    end

	if bEmitter.emit == true then
		emitTimer = emitTimer - dt*10
	end
	
	if emitTimer < 0 then
		bEmitter.emit = false
		emitTimer = 3
	end
	--Console.PrintBool(bEmitter.emit)
	deathExplosion()
    
    handleLaserDelay(dt)
    updateEnemyAnim()
	changing()
    ----Console.PrintBool(canApplyLaserDamage)
end

-- If there is collision detected, this physics component runs this function
function onTriggerEnter2D(gameObject, dt, manifold)
 
end

-- If there is collision detected, this physics component runs this function
function onTriggerStay2D(gameObject, dt, manifold)

end

-- Updates the rigidbody on Enemy position
function updateEnemyPos()
    eTransform = ECS:getComponent_Transform(eEntID)
    eRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
    newPos = PhysicsSystem:getGlobalPosePosition(eEntID)
    
    if  eTransform.pos.x == newPos.x and eTransform.pos.y == newPos.y and eTransform.pos.z == newPos.z then
        eIsWalking = true
    else 
        eIsWalking = false 
    end

    eTransform.pos.x = newPos.x
    eTransform.pos.y = newPos.y
    eTransform.pos.z = newPos.z

    eRigidDynamic.rigidPos.x = newPos.x
    eRigidDynamic.rigidPos.y = newPos.y
    eRigidDynamic.rigidPos.z = newPos.z
end

-- Updates the rigidbody on Enemy rotation
function updateEnemyRot()
    
    if eSetEnemyRot.y > 360 then
        eSetEnemyRot.y = 0
    elseif eSetEnemyRot.y < 0 then
        eSetEnemyRot.y = 360
    end
    
    eRotQuat = PhysicsSystem:convertEulerToQuat(eSetEnemyRot)
        
    PhysicsSystem:setGlobalPose(eEntID, newPos, eRotQuat)
end

-- Updates the rotation vector quaternion
function enemyRotateVectorByQuaternion()
    newRot = PhysicsSystem:getGlobalPoseRotation(eEntID)

    PhysicsSystem:rotate_vector_by_quaternion(forward, newRot, forwardPrime)
    PhysicsSystem:rotate_vector_by_quaternion(backward, newRot, backwardPrime)
    PhysicsSystem:rotate_vector_by_quaternion(leftward, newRot, leftwardPrime)
    PhysicsSystem:rotate_vector_by_quaternion(rightward, newRot, rightwardPrime)
end

-- Ent1 chase at Ent2 function
function chasingTowards(EntID1, EntID2, dt)
    -- Getting player id and their position
    if ECS:hasComponent_RigidDynamic(EntID2) then
        pRigidDynamic = ECS:getComponent_RigidDynamic(EntID2)
        pPos = pRigidDynamic.rigidPos
        pRot = pRigidDynamic.rigidRot
    end

    -- Adding force towards the player direction
    if ECS:hasComponent_RigidDynamic(EntID1) then
        eRigidDynamic = ECS:getComponent_RigidDynamic(EntID1)
        eTransform = ECS:getComponent_Transform(EntID1)
        ePos = eRigidDynamic.rigidPos
        eRot = eRigidDynamic.rigidRot

        addForce = Vector3.Normalized(pPos - ePos)
        
        eRotQuat = PhysicsSystem:convertEulerToQuat(eSetEnemyRot)

        local dist = Vector3.Distance(pPos, ePos)

        --Console.PrintBool(mContactReportCallback.enemyHitPlayer and eRigidDynamic.getHit)

        if (pRigidDynamic.getHit and mContactReportCallback.enemyHitPlayer and eEntID == mContactReportCallback.collidedEnemy) then
            eWalkForce = 0
        else
            eWalkForce = 15
        end

        --Console.PrintFloat(eWalkForce)

        --if dist <= 2.6 then
        --    eWalkForce = 0
        --    --Console.WriteLine("contacted")
        --else
        --    eWalkForce = 15
        --end

        PhysicsSystem:setGlobalPose(EntID1, newPos + Vector3:new(addForce.x, 0, addForce.z) * dt * eWalkForce, eRotQuat)
        
        eTransform.rotate = eSetEnemyRot
        --eRot = eRotQuat
        eRigidDynamic.rigidRot = eRotQuat

        ----Console.PrintFloat(addForce.x)
        ----Console.PrintFloat(addForce.z)
        --PhysicsSystem:addForceToBody(EntID1, Vector3:new(addForce.x, 0, addForce.z), eWalkForce, 0)
        
        enemyVeloctiy = PhysicsSystem:getBodyLinearVelocity(EntID1)
        PhysicsSystem:setBodyLinearVelocity(EntID1, enemyVeloctiy, eSpeedCap)
        
        ----Console.PrintFloat(eSetEnemyRot.y)
    end
end

-- Ent1 look at ent2 function
function lookTowards(EntID1, EntID2)
    -- Getting player id and their position
    if ECS:hasComponent_RigidDynamic(EntID2) then
        pRigidDynamic = ECS:getComponent_RigidDynamic(EntID2)
        ent2Pos = pRigidDynamic.rigidPos
    end
    
    -- Adding force towards the player direction
    if ECS:hasComponent_RigidDynamic(EntID1) then
        eRigidDynamic = ECS:getComponent_RigidDynamic(EntID1)
        ent1Pos = eRigidDynamic.rigidPos
        
        targetRotation = radianTopi(atan2(ent1Pos.z - ent2Pos.z, ent1Pos.x - ent2Pos.x))
        --Trigo.floatToint(targetRotation)
        rotationDifference = eSetEnemyRot.y + targetRotation -90

        --rotationDifference = rotationDifference % 180
        Trigo.floatToint(rotationDifference)
        
        if rotationDifference > 180.0 then
            rotationDifference = rotationDifference - 360.0
        end

        if rotationDifference < -180.0 then
            rotationDifference = rotationDifference + 360.0
        end
        
        if rotationDifference < 0  then
            eSetEnemyRot.y = eSetEnemyRot.y + 2.0
        elseif rotationDifference > 0 then
            eSetEnemyRot.y = eSetEnemyRot.y - 2.0
        end
    end
end

-- Ent1 randomly move around function
function randomMovement(EntID1)
    if ECS:hasComponent_RigidDynamic(EntID1) then
        eRigidDynamic = ECS:getComponent_RigidDynamic(EntID1)
        eTransform = ECS:getComponent_Transform(EntID1)
      
        if mContactReportCallback.ememyHitBuilding then 
            hasTarget = false
        end

        if hasTarget == false then
             random =  Random.randIntInRange(0,3)

             addForce.x = 0
             addForce.z = 0
            if random == 0 then --up 
                addForce.z = Random.randInRange(1, 5)
                eSetEnemyRot.y = 180
            elseif random == 1 then --down
                addForce.z = Random.randInRange(-5, -1)
                eSetEnemyRot.y = 0
            elseif random == 2 then --left
                addForce.x = Random.randInRange(1, 5)
                eSetEnemyRot.y = 270
            elseif random == 3 then --right
                addForce.x = Random.randInRange(-5, -1)
                eSetEnemyRot.y = 90
            end
            hasTarget = true
        end

        --PhysicsSystem:addForceToBody(EntID1, Vector3:new(addForce.x, 0, addForce.z), eWalkForce, 0)
       
        enemyVeloctiy = PhysicsSystem:getBodyLinearVelocity(EntID1)
        PhysicsSystem:setBodyLinearVelocity(EntID1, enemyVeloctiy, eSpeedCap)

        ePosNew = PhysicsSystem:getGlobalPosePosition(EntID1)
       
         eRotQuat = PhysicsSystem:convertEulerToQuat(eSetEnemyRot)
          PhysicsSystem:setGlobalPose(EntID1,  ePosNew, eRotQuat)
          eTransform.rotate = eSetEnemyRot
        eRigidDynamic.rigidRot = eRotQuat
    end
end

function initInUpdate()
    eEntID = ScriptSystem:getLuaEntityID()
    pEntID = ECS:getEntityID("Player")
	
	if ECS:hasComponent_LuaScriptCom(pEntID) then
        playerLuaScript = ECS:getComponent_LuaScriptCom(pEntID)
    end

    if ECS:hasComponent_Material(eEntID) and ECS:hasComponent_Transform(eEntID) and ECS:hasComponent_RigidDynamic(eEntID)
        and ECS:hasComponent_LuaScriptCom(eEntID) then
            eMaterial = ECS:getComponent_Material(eEntID)
            eTransform = ECS:getComponent_Transform(eEntID)
            eRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
            eLuaScript = ECS:getComponent_LuaScriptCom(eEntID)
    end

    pRigidDynamic = ECS:getComponent_RigidDynamic(pEntID)
end

-- Flashing dark
function setSatZero()
    eMaterial._saturation.y = 0
end

-- Flashing bright
function setSatOne()
    eMaterial._saturation.y = 1
end

-- On hit function
function onHit()
    --updateParticleColor()
    if dying == false and eUnified == false then
        if eRigidDynamic.getHit and mContactReportCallback.bulletHitEnemy and mContactReportCallback._detectedEnemy == eEntID then
            if eUnified == false then
                AudioSystem:PlaySFX("SFX_HitMarker", eEntID)
                eHealth = eHealth - 5
                eFlashTimer = 8
			    --bEmitter.emit = true
            end
        elseif eRigidDynamic.getHit and mContactReportCallback.laserHitEnemy and canApplyLaserDamage then
            for i = 1, #laser_EnemyHit do
                if eUnified == false and laser_EnemyHit[i] == eEntID and not ECS:hasTagInEntity(mContactReportCallback._detectedLaser, "Blocked") then
                    AudioSystem:PlaySFX("SFX_HitMarker", eEntID)
                    eHealth = eHealth - 6
                    eFlashTimer = 8
                    if playerLuaScript:GetEnvironment().laserUlt then
                        laserdmgdelayTime = 0.4
                    else
                        laserdmgdelayTime = 0.7
                    end
                    canApplyLaserDamage = false
		    	    --bEmitter.emit = true
                    break
                end
            end
        elseif eRigidDynamic.getHit and mContactReportCallback.explosionHitEnemy then
            for i = 1, #explode_EnemyHit do
                if eUnified == false and explode_EnemyHit[i] == eEntID then
                    AudioSystem:PlaySFX("SFX_HitMarker", eEntID)
                    eHealth = eHealth - 12
                    eFlashTimer = 8
		    	    --bEmitter.emit = true
                    break
                end
            end
        end
        eRigidDynamic.getHit = false
    end
end


-- Flashing on hit function
function flashOnHit()
    if eFlashTimer > 1 then
            eMaterial._saturation.z = eFlashTimer
    end
end

function randomHue()
    if eUnified == false then
            eMaterial._saturation.x = 1
    end
end

function Unify()
    if eUnified == false then
        randomHue()
        bEmitter.emit = true
        eUnified = true
        

        

        for i = 1, #policeScenarioID do
            if ECS:hasEntity(policeScenarioID[i]) then
                if ECS:hasTagInEntity(policeScenarioID[i], "PoliceScenario") then
                    if ECS:getComponent_LuaScriptCom(policeScenarioID[i]):GetEnvironment().polSceneActivated then
                        ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().polUnifyCount = 
                            ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().polUnifyCount + 1
                    end
                end
            end
        end

        setSatOne()
        eMaterial._saturation.x = 1.0
        eMaterial._saturation.z = 1.5
        if spawnedCubes == false then
            spawnCubes()
			
			if ECS:hasComponent_LuaScriptCom(pEntID) then
				if playerLuaScript:GetEnvironment().pHealth < 75 then
				playerLuaScript:GetEnvironment().pHealth = playerLuaScript:GetEnvironment().pHealth + 5
				playerLuaScript:GetEnvironment().healTimer = 0.5
                AudioSystem:PlaySFX("SFX_HealthRegen", pEntID)
                end
                
			end
        end

       -- ECS:getComponent_LuaScriptCom(pEntID):GetEnvironment().wantedLevel = 
       --     ECS:getComponent_LuaScriptCom(pEntID):GetEnvironment().wantedLevel + 0.05

        
    end
end

function spawnCubes()
    local spawnPos = eTransform.pos + forwardPrime
    for i = 3, 1, -1
    do
        --local bulletSpawnPos = pRigidDynamic.rigidPos + rightwardPrime * offsetPos
        -- * offsetPos
        --spawnPos.y = spawnPos.y - 3
        --spawnPos.z = spawnPos.z + 3
        local cubeID = ECS:instantiatePrefab(pLAmmoPrefabID, "lAmmo", spawnPos,rot,false, false, false)
        local cube2ID = ECS:instantiatePrefab(pRAmmoPrefabID, "rAmmo", spawnPos,rot,false, false, false)
        local cube3ID = ECS:instantiatePrefab(pCubePrefabID, "Cube", spawnPos,rot,false, false, false)
        if ECS:hasComponent_Transform(cubeID) then
        
            --PhysicsSystem:setGlobalPose(cubeID, pos, rot)
            --PhysicsSystem:setGravityFlag(cubeID, true)
        end
    end
    
   --Console.WriteLine("cube spawned")
   spawnedCubes = true
end

function handleEnemyLifeTime(dt)
    if dying == true and death <= 0 and eUnified == true then
        if lifeTime > 0.0 then
            lifeTime = lifeTime - dt        
        end

        if lifeTime <= 0.0 then
            ------Console.WriteLine("Enemy Destroyed")
            -- eLuaScipt_Scenario1:GetEnvironment().killCount = eLuaScipt_Scenario1:GetEnvironment().killCount + 1
            -- Console.WriteLine("KILLCOUNT in EnemyChase.lua")
            -- Console.PrintFloat(eLuaScipt_Scenario1:GetEnvironment().killCount)
            ECS:addEntityToDestroyQueue(eEntID)
            PhysicsSystem:LuaRemoveActor(eEntID)
        end
    end
end

-- Math Logic --

-- Convert to positive number
function positiveOnly(x)
    if x < 0.0 then
        x = -x
    end
    return x
end

-- Logic for atan2
function atan2(y, x)
    if x > 0.0 then
        return Trigo.atan(y/x)
    end

    if x < 0.0 and y >= 0.0 then
        return Trigo.atan(y/x) + Trigo.pi()
    end

    if x < 0.0 and y < 0.0 then
        return Trigo.atan(y/x) - Trigo.pi()
    end

    if x == 0.0 and y > 0.0 then
        return Trigo.pi() / 2
    end

    if x == 0.0 and y < 0.0 then
        return -Trigo.pi() / 2
    end
    
    if x == 0.0 and y == 0.0 then
        return 0.0
    end

    return 0.0
end

-- Convert radian to pi
function radianTopi(x)
    return 180/Trigo.pi()*x
end

function handleLaserDelay(dt)
    if laserdmgdelayTime > 0.0 then
        laserdmgdelayTime = laserdmgdelayTime - dt        
    end

    if laserdmgdelayTime <= 0.0 then
        canApplyLaserDamage = true
    end
end

function updateEnemyAnim()
    local x = ePos.x - pPos.x
    local z = ePos.z - pPos.z

    if x < 0 then 
        x = -x
    end
    
    if z < 0 then 
        z = -z
    end

    --if  pMaterial._saturation.y == 0 then
         --eAnim.currentAnim = 1
    if x <  attackDistance and z < attackDistance and eHealth > 0 then
        eAnim.speed = 30
        eAnim.currentAnim = 0
    elseif  eUnified == true then 
        eAnim.speed = 30
        eAnim.currentAnim = 2
    elseif dying then
        eAnim.currentAnim = 1
    elseif eIsWalking then
        eAnim.speed = 30
        eAnim.currentAnim = 3

    else
    end
end

function updateParticleColor()

    bEmitter.colorBegin.x = Random.Float()
    bEmitter.colorBegin.y = Random.Float()
	bEmitter.colorBegin.z = Random.Float()
	
	bEmitter.sizeBegin = 0.7
	bEmitter.sizeEnd   = 0
	
	bEmitter.velocityVariation.x = 50.0
	bEmitter.velocityVariation.y = 50.0
	bEmitter.velocityVariation.z = 50.0
	
	bEmitter.lifeTime = 0.5
	
	--speed, duration
	
end

function deathExplosion()

    bEmitter.colorBegin.x = Random.Float()
    bEmitter.colorBegin.y = Random.Float()
	bEmitter.colorBegin.z = Random.Float()
	
     bEmitter.colorEnd.x = Random.Float()
    bEmitter.colorEnd.y = Random.Float()
	bEmitter.colorEnd.z = Random.Float()

	bEmitter.sizeBegin = 1.5
	bEmitter.sizeEnd   = 0
	
	bEmitter.velocityVariation.x = Random.Float() * 100
	bEmitter.velocityVariation.y = Random.Float() * 100
	bEmitter.velocityVariation.z = Random.Float() * 100
	
	bEmitter.lifeTime = 1
	
	--speed, duration
	
end

function randomDirectionSpawn()

     if ECS:hasComponent_RigidDynamic(eEntID) then
        randomDirection.x = Random.randInRange(-100.0, 100.0)

        randomDirection.y = 300.0
        randomDirection.z = Random.randInRange(-100.0, 100.0)

        --Console.PrintFloat(randomDirection.x)
        --Console.PrintFloat(randomDirection.y)
        --Console.PrintFloat(randomDirection.z)

        PhysicsSystem:addForceToBody(eEntID, randomDirection, randomForce, 0)
    end
end


function deathAnim()
    if death <= 0  and dying == false then
        death = deathTimer
        eAnim.speed = 15
        eAnim.currentAnim = 1
        eIsWalking = false
        dying = true
        ECS:addEntityTag(eEntID, "Unified")
        if not unifySoundPlayed then
            AudioSystem:Play3DSFX("SFX_EnemyDeath", eEntID)
            unifySoundPlayed = true
        end
    end
    if(dying == true and death <= 0 and eUnified == false) then
        randomDirectionSpawn()
        Unify()
    end
end

function changing()
    if dying == true and death >= 0 then
        eMaterial._saturation.y = 1
        eMaterial._saturation.x = Random.randInRange(0.0, 250.0)
    --else
    end
end
