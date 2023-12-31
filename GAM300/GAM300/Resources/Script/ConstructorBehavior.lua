local eWalkForce            = 15.0
local eSpeedCap             = 25.0
local eHealth               = 6
local eFlashTimer           = 1
local eUnified              = false
local canApplyLaserDamage   = true
local lifeTime              = 5.0
local laserdmgdelayTime     = 0.0
local hasTarget = false

local deathTimer = 10
local death = 0.0
local dying = false
local randomDirection   = Vector3:new()
local randomForce       = 10.0

-- Component Variables
local eEntID
local eRigidDynamic
local eTransform
local eMaterial
local eAnim
local bEmitter
local emitTimer             = 3
local eRotQuat
local enemyVeloctiy
local addForce = Vector3:new(0.0, 0.0, 0.0)
local newPos
local newRot
local dir = "up"
local random
local timer = 0

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

-- Scenario
local cScenarioID

local UIObjID

-- player
local pEntID
local playerLuaScript

-- Enemy Table
enemy = {}

local laser_CivilianHit
local explode_CivilianHit

local unifySoundPlayed = false

local invincible


-- Awake is called once and only once when script is loaded.
function Awake()
    --Console.WriteLine("EnemyChase script loaded")
end

-- Start is called once and only once after Awake() is called.
function Start()
    cScenarioID = ECS:getEntityTagged("CivilianScenario")
	UIObjID = ECS:getEntityID("UI_Challenge")
    pEntID = ECS:getEntityID("Player")
end


function randomMovement(dt)
    -- Adding force towards the player direction
     eEntID = ScriptSystem:getLuaEntityID()
    if ECS:hasComponent_RigidDynamic(eEntID) then
        eRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
        eTransform = ECS:getComponent_Transform(eEntID)
      
        if mContactReportCallback.civilianHitBuilding then 
            hasTarget = false
        end

        if hasTarget == false then
             random =  Random.randIntInRange(0,3)

             addForce.x = 0
             addForce.z = 0
            if random == 0 then --up 
                addForce.z = Random.randInRange(0.5, 1)
                eSetEnemyRot.y = 180
            elseif random == 1 then --down
                addForce.z = Random.randInRange(-1, -0.5)
                eSetEnemyRot.y = 0
            elseif random == 2 then --left
                addForce.x = Random.randInRange(0.5, 1)
                eSetEnemyRot.y = 270
            elseif random == 3 then --right
                addForce.x = Random.randInRange(-1, -0.5)
                eSetEnemyRot.y = 90
            end
            hasTarget = true
        end
        if not ECS:hasTagInEntity(eEntID, "Trapped") then
            PhysicsSystem:setGlobalPose(eEntID, newPos + Vector3:new(addForce.x, 0, addForce.z) * dt * eWalkForce, eRotQuat)
        end
        --PhysicsSystem:addForceToBody(eEntID, Vector3:new(addForce.x, 0, addForce.z), eWalkForce, 0)
       
        enemyVeloctiy = PhysicsSystem:getBodyLinearVelocity(eEntID)
        PhysicsSystem:setBodyLinearVelocity(eEntID, enemyVeloctiy, eSpeedCap)

        ePosNew = PhysicsSystem:getGlobalPosePosition(eEntID)
       
         eRotQuat = PhysicsSystem:convertEulerToQuat(eSetEnemyRot)
          PhysicsSystem:setGlobalPose(eEntID,  ePosNew, eRotQuat)
          eTransform.rotate = eSetEnemyRot
        eRigidDynamic.rigidRot = eRotQuat
    end
end
-- Updates the Game Object per game loop
function Update(dt)

    initInUpdate()
    bEmitter = ECS:getComponent_Emitter(eEntID)
    laser_CivilianHit = mContactReportCallback:getEntityHitID("Civilian","Laser")
    explode_CivilianHit = mContactReportCallback:getEntityHitID("Civilian","Explosion")

    eAnim = ECS:getComponent_AnimationCom(eEntID)


    enemyRotateVectorByQuaternion()
    updateEnemyPos()
    updateEnemyRot()

    updateTrappedBehavior()


    if eFlashTimer > 1.5 then
        eFlashTimer = eFlashTimer - dt*10
        flashOnHit()
    end
    onHit()

    if timer <= 0 then 
        timer =  Random.randInRange(1,6)
        hasTarget = false
    else
        timer = timer - dt * 3
        hasTarget = true
    end
    
    

    if (eHealth > 0 and eUnified == false) then
        eAnim.currentAnim = 2
         randomMovement(dt)
         setSatZero()
    end

    if (eUnified == true) then
        randomMovement(dt)
        eHealth = 0
    end

    if eHealth <= 0 then
        --Unify()
       --randomMovement()
       deathAnim()
    end
    
    if death >= 0 then
        death = death - dt*10
    end
    changing()
    if bEmitter.emit == true then
		emitTimer = emitTimer - dt*10
	end
	
	if emitTimer < 0 then
		bEmitter.emit = false
		emitTimer = 3
	end
    deathExplosion()
    handleLaserDelay(dt)



end

-- Updates the rigidbody on Enemy position
function updateEnemyPos()
    eTransform = ECS:getComponent_Transform(eEntID)
    eRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
    newPos = PhysicsSystem:getGlobalPosePosition(eEntID)

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


function initInUpdate()
    eEntID = ScriptSystem:getLuaEntityID()

    if ECS:hasComponent_Material(eEntID) and ECS:hasComponent_Transform(eEntID) and ECS:hasComponent_RigidDynamic(eEntID) then
            eMaterial = ECS:getComponent_Material(eEntID)
            eTransform = ECS:getComponent_Transform(eEntID)
            eRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
    end

	if ECS:hasComponent_LuaScriptCom(pEntID) then
        playerLuaScript = ECS:getComponent_LuaScriptCom(pEntID)
    end
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
    if not invincible then
        if eRigidDynamic.getHit and mContactReportCallback.bulletHitCivilian and eEntID == mContactReportCallback._detectedCivilian then
            if eUnified == false then
                AudioSystem:PlaySFX("SFX_HitMarker", eEntID)
                eHealth = eHealth - 5
                eFlashTimer = 8
            end
        elseif eRigidDynamic.getHit and mContactReportCallback.laserHitCivilian and canApplyLaserDamage then
            for i = 1, #laser_CivilianHit do
                if eUnified == false and laser_CivilianHit[i] == eEntID and not ECS:hasTagInEntity(mContactReportCallback._detectedLaser, "Blocked") then
                    AudioSystem:PlaySFX("SFX_HitMarker", eEntID)
                    eHealth = eHealth - 6
                    eFlashTimer = 8
                    if playerLuaScript:GetEnvironment().laserUlt then
                        laserdmgdelayTime = 0.4
                    else
                        laserdmgdelayTime = 0.7
                    end
                    canApplyLaserDamage = false
                    break
                end
            end
        elseif eRigidDynamic.getHit and mContactReportCallback.explosionHitCivilian then
            for i = 1, #explode_CivilianHit do
                if eUnified == false and explode_CivilianHit[i] == eEntID then
                    AudioSystem:PlaySFX("SFX_HitMarker", eEntID)
                    eHealth = eHealth - 8
                    eFlashTimer = 8
                    break
                end
            end
        end
    end
    eRigidDynamic.getHit = false
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
    if not eUnified then
        randomHue()
        eUnified = true
        --ECS:addEntityTag(eEntID, "Unified")
        bEmitter.emit = true
        for i = 1, #cScenarioID do
            if ECS:hasEntity(cScenarioID[i]) then
                if ECS:hasTagInEntity(cScenarioID[i], "CivilianScenario") then
                    if ECS:getComponent_LuaScriptCom(cScenarioID[i]):GetEnvironment().civiSceneActivated then
                        ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().civiUnifyCount = 
                            ECS:getComponent_LuaScriptCom(UIObjID):GetEnvironment().civiUnifyCount + 1
                    end
                end
            end
        end
        
        setSatOne()
        eMaterial._saturation.z = 1.5
        eMaterial.m_model = "Constructor_Happy_Bone"
        eMaterial.m_texture = "ConstructorHappy"
        eMaterial.isInitialised = false
        eAnim.currentAnim = 0
        eAnim.speed = 30

        playerLuaScript:GetEnvironment().wantedLevel = 
            playerLuaScript:GetEnvironment().wantedLevel + 0.1

        if playerLuaScript:GetEnvironment().pHealth < 75 then
		playerLuaScript:GetEnvironment().pHealth = playerLuaScript:GetEnvironment().pHealth + 2
        playerLuaScript:GetEnvironment().healTimer = 0.5
        AudioSystem:PlaySFX("SFX_HealthRegen", pEntID)
		end
        
    end
end

function handleLaserDelay(dt)
    if laserdmgdelayTime > 0.0 then
        laserdmgdelayTime = laserdmgdelayTime - dt        
    end

    if laserdmgdelayTime <= 0.0 then
        canApplyLaserDamage = true
    end
end


function randomDirectionSpawn()

     if ECS:hasComponent_RigidDynamic(eEntID) then
        randomDirection.x = Random.randInRange(-100.0, 100.0)

        randomDirection.y = 300.0
        randomDirection.z = Random.randInRange(-100.0, 100.0)

        ----Console.PrintFloat(randomDirection.x)
        ----Console.PrintFloat(randomDirection.y)
        ----Console.PrintFloat(randomDirection.z)

        PhysicsSystem:addForceToBody(eEntID, randomDirection, randomForce, 0)
    end
end

function deathAnim()
    if death <= 0  and dying == false then
        death = deathTimer
        eAnim.speed = 15
        eAnim.currentAnim = 0
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

function updateTrappedBehavior()
    if ECS:hasTagInEntity(eEntID, "Trapped") then
        invincible = true
    else
        invincible = false
    end
end
