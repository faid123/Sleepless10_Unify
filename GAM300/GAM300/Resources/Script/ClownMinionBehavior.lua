--stuff goes here

-- Editable Variable

local eWalkForce            = 20.0
local eSpeedCap             = 25.0
local eHealth               = 5
local eFlashTimer           = 1
eUnified              = false
local spawnedCubes          = false
local canApplyLaserDamage   = true
local offsetPos             = 10.0
local lifeTime              = 2.0
local laserdmgdelayTime     = 0.0
local eIsWalking            = false
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

-- Self explosion
local E_ExplosionPrefabID
local exFlashTimer      = 2.0
local explodeNow        = false
local isFlashing        = false
local CCdelay           = 0.3
local CCdelay2          = 0.0
local explodeTriggerDist    = 5.0
local exEffectPrefabID
local effectEmitSpawned = false

local laser_EnemyHit
local explode_EnemyHit

local soundTimer = 0.0

-- Awake is called once and only once when script is loaded.
function Awake()
    --Console.WriteLine("EnemyChase script loaded")
end

-- Start is called once and only once after Awake() is called.
function Start()
    --------Console.WriteLine("EnemyChase script runnning")
    pCubePrefabID = ECS:getPrefabID("Prefab_AmmoCube")
    pEntID = ECS:getEntityID("Player")
    pMaterial = ECS:getComponent_Material(pEntID)
    pTransform = ECS:getComponent_Transform(pEntID)
    E_ExplosionPrefabID = ECS:getPrefabID("Prefab_MinionExplosion")
    exEffectPrefabID = ECS:getPrefabID("Prefab_ExplosionEffect")
end

-- Updates the Game Object per game loop
function Update(dt)

    initInUpdate()

    if soundTimer > 0.0 then
        soundTimer = soundTimer - dt
    elseif soundTimer <= 0.0 then
        AudioSystem:Play3DSFX("Enemy_Dog", eEntID)
        soundTimer = 2.0
    end

    laser_EnemyHit = mContactReportCallback:getEntityHitID("Enemy","Laser")
    explode_EnemyHit = mContactReportCallback:getEntityHitID("Enemy","Explosion")

    enemyRotateVectorByQuaternion()
    updateEnemyPos()
    updateEnemyRot()
     if ECS:hasComponent_AnimationCom(eEntID) then
        eAnim = ECS:getComponent_AnimationCom(eEntID)
    end

    --if Inputs.get_Key_Triggered(InputKey.W_KEY) then
        onHit()
    --end
    bEmitter = ECS:getComponent_Emitter(eEntID)
    if bEmitter.emit == true then
		emitTimer = emitTimer - dt*10
	end
	
	if emitTimer < 0 then
		bEmitter.emit = false
		emitTimer = 2
	end

    if eHealth > 0 then
    
        --Enemy looking at Player
        lookTowards(eEntID, pEntID)

        --Enemy chase at Player
        chasingTowards(eEntID, pEntID, dt)

        if inRangeToExplode() then
            eWalkForce = 0
        else
            eWalkForce = 20
        end

        if inRangeToExplode() == true and not isFlashing then
            isFlashing = true
        end

        if isFlashing then
            startFlashing()
            handleCountdown(dt)
            updateCCdelay(dt)
        end

        if explodeNow then
            explodeSelf()
        end
    end

    if eHealth <= 0 then
        AudioSystem:Stop3DSFX("Enemy_Dog", eEntID)
        ECS:addEntityToDestroyQueue(eEntID)
        PhysicsSystem:LuaRemoveActor(eEntID)
    end

    handleLaserDelay(dt)
    updateEnemyAnim()
    updateParticleColor()
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

function initInUpdate()
    eEntID = ScriptSystem:getLuaEntityID()

    if ECS:hasComponent_Material(eEntID) and ECS:hasComponent_Transform(eEntID) and ECS:hasComponent_RigidDynamic(eEntID)
        and ECS:hasComponent_LuaScriptCom(eEntID) then
            eMaterial = ECS:getComponent_Material(eEntID)
            eTransform = ECS:getComponent_Transform(eEntID)
            eRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
            eLuaScript = ECS:getComponent_LuaScriptCom(eEntID)
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
    if eRigidDynamic.getHit and mContactReportCallback.bulletHitEnemy and mContactReportCallback._detectedEnemy == eEntID then
        eHealth = eHealth - 1
        eFlashTimer = 8
        --bEmitter.emit = true
    elseif eRigidDynamic.getHit and mContactReportCallback.laserHitEnemy and canApplyLaserDamage then
        for i = 1, #laser_EnemyHit do
            if laser_EnemyHit[i] == eEntID and not ECS:hasTagInEntity(mContactReportCallback._detectedLaser, "Blocked") then
                AudioSystem:PlaySFX("SFX_HitMarker", eEntID)
                eHealth = eHealth - 2
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
            if explode_EnemyHit[i] == eEntID then
                AudioSystem:PlaySFX("SFX_HitMarker", eEntID)
                eHealth = eHealth - 5
                eFlashTimer = 8
                --bEmitter.emit = true
                break
            end
        end
    end
    eRigidDynamic.getHit = false
end

function spawnCubes()
    for i = 5, 1, -1
    do
        --local bulletSpawnPos = pRigidDynamic.rigidPos + rightwardPrime * offsetPos
        local spawnPos = eTransform.pos + forwardPrime * offsetPos
        --spawnPos.y = spawnPos.y - 3
        spawnPos.z = spawnPos.z + 3
        local cubeID = ECS:instantiatePrefab(pCubePrefabID, "AmmoCube", spawnPos,rot,false, false, false)
        if ECS:hasComponent_Transform(cubeID) then
        
            --PhysicsSystem:setGlobalPose(cubeID, pos, rot)
            --PhysicsSystem:setGravityFlag(cubeID, true)
        end
    end
   ------Console.WriteLine("cube spawned")
   spawnedCubes = true
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
         eAnim.currentAnim = 0
    elseif isFlashing then
        eAnim.currentAnim = 0
    elseif eIsWalking then
        eAnim.currentAnim = 1
    else 
        eAnim.currentAnim = 1
    end
end

function inRangeToExplode()
    eTransform = ECS:getComponent_Transform(eEntID)

    local x = eTransform.pos.x - pTransform.pos.x
    local z = eTransform.pos.z - pTransform.pos.z

    if x < 0 then 
        x = -x
    end
    
    if z < 0 then 
        z = -z
    end

    if x < explodeTriggerDist and z < explodeTriggerDist and eHealth > 0 then
        return true
    else
        return false
    end
end

function startFlashing()
    if CCdelay > 0.0 then
        eMaterial._saturation.x = 310.0
        eMaterial._saturation.z = 1.5
        CCdelay2 = 0.3
    end

    if CCdelay <= 0.0 and CCdelay2 > 0.0 then
        eMaterial._saturation.x = 1.0
        eMaterial._saturation.z = 1.0
    end
end

function updateCCdelay(dt)
    if CCdelay > 0.0 then
        CCdelay = CCdelay - dt
    end

    if CCdelay2 > 0.0 then
        CCdelay2 = CCdelay2 - dt
    elseif CCdelay2 <= 0.0 then
        CCdelay = 0.3
    end
end

function explodeSelf()
    local explosionID = ECS:instantiatePrefab(E_ExplosionPrefabID, "MinionExplosion", eRigidDynamic.rigidPos,eRigidDynamic.rigidRot,false, false, false)
    if not effectEmitSpawned then
            local effectEntID = ECS:instantiatePrefab(exEffectPrefabID, "ExplosionEffect", eRigidDynamic.rigidPos,eRigidDynamic.rigidRot,false, false, false)
            local effectTransform = ECS:getComponent_Transform(effectEntID)
		    effectTransform.pos = eRigidDynamic.rigidPos
            effectEmitSpawned = true
    end
    ECS:addEntityToDestroyQueue(eEntID)
    PhysicsSystem:LuaRemoveActor(eEntID)
end

-- Update flashing countdown
function handleCountdown(dt)
    if exFlashTimer > 0.0 then
        exFlashTimer = exFlashTimer - dt
        eAnim.speed = eAnim.speed + 1.0
    end

    if exFlashTimer <= 0.0 then
        explodeNow = true
    end
end

function updateParticleColor()

    bEmitter.colorBegin.x = Random.Float()
    bEmitter.colorBegin.y = Random.Float()
	bEmitter.colorBegin.z = Random.Float()
	
    bEmitter.colorEnd.x = Random.Float()
    bEmitter.colorEnd.y = Random.Float()
    bEmitter.colorEnd.z = Random.Float()

	bEmitter.sizeBegin = 0.7
	bEmitter.sizeEnd   = 0
	
	bEmitter.velocityVariation.x = 50.0
	bEmitter.velocityVariation.y = 50.0
	bEmitter.velocityVariation.z = 50.0
	
	bEmitter.lifeTime = 0.5
	
	--speed, duration
	
end