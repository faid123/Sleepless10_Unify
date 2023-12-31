--stuff goes here

-- Editable Variable

local eWalkForce            = 170.0
local eSpeedCap             = 25.0
local eHealth               = 50
local eFlashTimer           = 1
eUnified              = false
local spawnedCubes          = false
local canApplyLaserDamage   = true
local offsetPos             = 10.0
local lifeTime              = 5.0
local laserdmgdelayTime     = 0.7
local attackDistance        = 30.0
local emitTimer             = 2

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
local bEmitter
local targetPrefabID

local pEntID
local pCubePrefabID
local pRigidDynamic
local pTransform
local pPos
local pRot
local pMaterial
local playerLuaScript

local ent1Pos
local ent2Pos
local newPos
local newRot
local targetRotation
local rotationDifference
local addForce

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

local forwardOffest     = 3.0
local rightwardOffset   = 0.8

local E_RocketPrefabID
local fireDelayTimer    = 0.0
local fireDelayTime     = 3.0
local canShoot          = true

local timer = 0
local turnNow = false
local maxDistApart = 80.0
local currDistApart

local E_RocketID
local rocketBody
local targetID

local heliHeight = 35.0

local E_ParticleDispenserPrefabID

local pEmitter

local inRangeToFire = false

local soundPlayed = false

-- Awake is called once and only once when script is loaded.
function Awake()
    --Console.WriteLine("EnemyChase script loaded")
end

-- Start is called once and only once after Awake() is called.
function Start()
    --------Console.WriteLine("EnemyChase script runnning")
    --pCubePrefabID = ECS:getPrefabID("Prefab_AmmoCube")
    pEntID = ECS:getEntityID("Player")
    pMaterial = ECS:getComponent_Material(pEntID)
    pTransform = ECS:getComponent_Transform(pEntID)

    E_RocketPrefabID = ECS:getPrefabID("Prefab_EnemyRocket")
    targetPrefabID = ECS:getPrefabID("Prefab_Target")
    E_ParticleDispenserPrefabID = ECS:getPrefabID("Prefab_ParticleDispenser")

end

-- Updates the Game Object per game loop
function Update(dt)

    initInUpdate()

    if not soundPlayed then
        AudioSystem:Play3DSFX("Helicopter_Entrance", eEntID)
        AudioSystem:Play3DSFX("SFX_Helicopter", eEntID)
        soundPlayed = true
    end

    if ECS:hasComponent_RigidDynamic(eEntID) then
        PhysicsSystem:setGravityFlag(eEntID, true)
    end
    
    enemyRotateVectorByQuaternion()
    updateEnemyPos()
    updateEnemyRot()
    handleTurnTimer(dt)
    lookTowards(eEntID, pEntID)
    chasingTowards(eEntID, pEntID, dt)

    fireWeapon()
    
    handleshootingcooldown(dt)

    if eFlashTimer > 1.5 then
        eFlashTimer = eFlashTimer - dt*10
        flashOnHit()
    end

    onHit()
    handleLaserDelay(dt)
    destroyEnemy(dt)
    destroyTarget()
    handleLaserDelay(dt)

    bEmitter = ECS:getComponent_Emitter(eEntID)
    if bEmitter.emit == true then
		emitTimer = emitTimer - dt*10
	end
	
	if emitTimer < 0 then
		bEmitter.emit = false
		emitTimer = 2
	end
    updateParticleColor()
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

    newPos.y = heliHeight

    eTransform.pos.x = newPos.x
    eTransform.pos.y = newPos.y
    eTransform.pos.z = newPos.z

    eRigidDynamic.rigidPos.x = newPos.x
    eRigidDynamic.rigidPos.y = newPos.y
    eRigidDynamic.rigidPos.z = newPos.z

    PhysicsSystem:setGlobalPose(eEntID, newPos, eRotQuat)
end

-- Updates the rigidbody on Enemy rotation
function updateEnemyRot()
    
    if eSetEnemyRot.y > 360 then
        eSetEnemyRot.y = 0
    elseif eSetEnemyRot.y < 0 then
        eSetEnemyRot.y = 360
    end
    
    if turnNow == true then
        eRotQuat = PhysicsSystem:convertEulerToQuat(eSetEnemyRot)
        eRigidDynamic.rigidRot = eRotQuat
        eTransform.rotate = eSetEnemyRot
        newPos.y = heliHeight
        eRigidDynamic.rigidPos.y = heliHeight
        PhysicsSystem:setGlobalPose(eEntID, newPos, eRotQuat)
    end
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

    -- Follow player if out of range
    if ECS:hasComponent_RigidDynamic(EntID1) then
        eRigidDynamic = ECS:getComponent_RigidDynamic(EntID1)
        eTransform = ECS:getComponent_Transform(EntID1)
        ePos = eRigidDynamic.rigidPos
        eRot = eRigidDynamic.rigidRot
        --ePos.y = 0
        local eFlatPos = Vector3:new(ePos.x, 0, ePos.z)

        currDistApart = Vector3.Length(pPos - eFlatPos)
        addForce = Vector3.Normalized(pPos - eFlatPos)
        addForce.y = 0
        
        eRotQuat = PhysicsSystem:convertEulerToQuat(eSetEnemyRot)

        if currDistApart > maxDistApart then
            PhysicsSystem:setGlobalPose(EntID1, newPos + addForce * dt * 50, eRotQuat)
        else
            inRangeToFire = true
        end
    end
end

-- Ent1 look at ent2 function
function lookTowards(EntID1, EntID2)
    -- Getting player id and their position
    if ECS:hasComponent_RigidDynamic(EntID2) then
        pRigidDynamic = ECS:getComponent_RigidDynamic(EntID2)
        ent2Pos = pRigidDynamic.rigidPos
    end
    
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

function initInUpdate()
    eEntID = ScriptSystem:getLuaEntityID()

    if ECS:hasComponent_Material(eEntID) and ECS:hasComponent_Transform(eEntID) and ECS:hasComponent_RigidDynamic(eEntID)
        and ECS:hasComponent_LuaScriptCom(eEntID) then
            eMaterial = ECS:getComponent_Material(eEntID)
            eTransform = ECS:getComponent_Transform(eEntID)
            eRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
            eLuaScript = ECS:getComponent_LuaScriptCom(eEntID)
    end

    if ECS:hasComponent_Material(pEntID) and ECS:hasComponent_Transform(pEntID) and ECS:hasComponent_RigidDynamic(pEntID) and ECS:hasComponent_LuaScriptCom(pEntID) then
            pTransform = ECS:getComponent_Transform(pEntID)
            pRigidDynamic = ECS:getComponent_RigidDynamic(pEntID)
            playerLuaScript = ECS:getComponent_LuaScriptCom(pEntID)
    end

    ePos = PhysicsSystem:getGlobalPosePosition(eEntID)
    eRotQuat = PhysicsSystem:getGlobalPoseRotation(eEntID)
    eRot = eTransform.rotate

    pPos = PhysicsSystem:getGlobalPosePosition(pEntID)
end

-- On hit function
function onHit()
    if eRigidDynamic.getHit and mContactReportCallback.bulletHitHelicopter then
        AudioSystem:PlaySFX("SFX_HitMarker", eEntID)
        eHealth = eHealth - 2
        eFlashTimer = 8
        bEmitter.emit = true
    elseif eRigidDynamic.getHit and mContactReportCallback.laserHitHelicopter and canApplyLaserDamage then
        AudioSystem:PlaySFX("SFX_HitMarker", eEntID)
        eHealth = eHealth - 8
        eFlashTimer = 8
        if playerLuaScript:GetEnvironment().laserUlt then
            laserdmgdelayTime = 0.4
        else
            laserdmgdelayTime = 0.7
        end
        canApplyLaserDamage = false
        bEmitter.emit = true
    elseif eRigidDynamic.getHit and mContactReportCallback.explosionHitHelicopter and canApplyLaserDamage then
        AudioSystem:PlaySFX("SFX_HitMarker", eEntID)
        eHealth = eHealth - 25
        eFlashTimer = 8
        canApplyLaserDamage = false
        bEmitter.emit = true
    end
    eRigidDynamic.getHit = false
end

-- Flashing on hit function
function flashOnHit()
    if eFlashTimer > 1 then
            eMaterial._saturation.z = eFlashTimer
    end
end

function Unify()
    if eUnified == false then
        randomHue()
        eUnified = true
        ECS:addEntityTag(eEntID, "Unified")
        setSatOne()
        eMaterial._saturation.z = 1.5
        if spawnedCubes == false then
            spawnCubes()
        end
        ECS:getComponent_LuaScriptCom(pEntID):GetEnvironment().wantedLevel = 
            ECS:getComponent_LuaScriptCom(pEntID):GetEnvironment().wantedLevel + 0.1
    end
end

function handleEnemyLifeTime(dt)
    if lifeTime > 0.0 then
        lifeTime = lifeTime - dt        
    end

    if lifeTime <= 0.0 then
        ------Console.WriteLine("Enemy Destroyed")
        ECS:addEntityToDestroyQueue(eEntID)
        PhysicsSystem:LuaRemoveActor(eEntID)
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
    eTransform = ECS:getComponent_Transform(eEntID)

    local x = eTransform.pos.x - pTransform.pos.x
    local z = eTransform.pos.z - pTransform.pos.z

    if x < 0 then 
        x = -x
    end
    
    if z < 0 then 
        z = -z
    end

    if  pMaterial._saturation.y == 0 then
         eAnim.currentAnim = 1
    elseif  eUnified == true then 
        eAnim.currentAnim = 1
    elseif eIsWalking then
        eAnim.currentAnim = 2
    else 
        eAnim.currentAnim = 1
    end
end

function fireWeapon()
    if canShoot and inRangeToFire then
        AudioSystem:Play3DSFX("SFX_HeliRocket", eEntID)
        eTransform = ECS:getComponent_Transform(eEntID)
        pTransform = ECS:getComponent_Transform(pEntID)
        local rocketRotQuat = PhysicsSystem:convertEulerToQuat(eTransform.rotate)
        local rocketSpawnPos = (eTransform.pos + forwardPrime * forwardOffest) + rightwardPrime * rightwardOffset
        local targetRotQuat = PhysicsSystem:convertEulerToQuat(pTransform.rotate)
        local targetSpawnPos = Vector3:new(pPos.x, 1.2, pPos.z)
        E_RocketID = ECS:instantiatePrefab(E_RocketPrefabID, "EnemyRocket", rocketSpawnPos, rocketRotQuat, false, false, false)
        rocketBody = ECS:getComponent_RigidDynamic(E_RocketID)
        targetID = ECS:instantiatePrefab(targetPrefabID, "Target", targetSpawnPos, targetRotQuat, false, false, false)
        local targetTransform = ECS:getComponent_Transform(targetID)
        targetTransform.pos = Vector3:new(pPos.x, 1.2, pPos.z)
        canShoot = false
        fireDelayTimer = fireDelayTime
    end
end

-- Update shooting cooldown
function handleshootingcooldown(dt)
    if fireDelayTimer > 0.0 then
        fireDelayTimer = fireDelayTimer - dt
    end

    if fireDelayTimer <= 0.0 then
        canShoot = true
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

function handleTurnTimer(dt)
    if timer < 0.5 then
		timer = timer + dt * 10
		turnNow = false
	end
	
	if timer > 0.5 then
		timer = 0
		turnNow = true
	end
end

function destroyEnemy(dt)
    if eHealth <= 0 then
        --bEmitter.emit = true
        AudioSystem:Play3DSFX("SFX_HelicopterDeath", eEntID)
        AudioSystem:Stop3DSFX("SFX_Helicopter", eEntID)
        AudioSystem:Stop3DSFX("Helicopter_Entrance", eEntID)
        local ParticleDispenserID = ECS:instantiatePrefab(E_ParticleDispenserPrefabID, "Particles", ePos, eRotQuat, false, false, false)
        --Console.WriteLine("particle")
        local particleTransform = ECS:getComponent_Transform(ParticleDispenserID)
        pEmitter = ECS:getComponent_Emitter(ParticleDispenserID)
        particleTransform.pos = ePos

        ECS:addEntityToDestroyQueue(eEntID)
        PhysicsSystem:LuaRemoveActor(eEntID)
    end
end

function destroyTarget()
    if rocketBody then
        if rocketBody.getHit and not mContactReportCallback.enemyRocketHitHeli then
            ECS:addEntityToDestroyQueue(targetID)
        end
    end
end