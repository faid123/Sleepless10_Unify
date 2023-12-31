-- local variables

local playerID
local player_env

local pExplosionPrefabID
local eEntID
-- Component Variables
local bTransform
local bRigidDynamic
local bEmitter
local bMaterial
local bForwardPrime = Vector3:new()
local bForward      = Vector3:new(0.0, 0.0, -1.0)

local bRotNew       = Quat:new()
local bPos          = Vector3:new()

-- taret position of where rocket will launch to
local playerTransform
local newPlayerPos  = Vector3:new()
local playerPos
-- the speed to change the angle
local angleChangeSpeed = 0.5
-- movement speed of the rocket towards the player
local rocketSpeed = 20000.0 -- this is for angular velocity, when the rocket turns
local rocketForce = 10.0
local hasFired      = false
local lifeTime      = 10.0

local pRigidDynamic
local ent2Pos
local eRigidDynamic
local ent1Pos
local targetRotation = 0.0
local prevTargetRotation = 0.0
local eSetEnemyRot  = Vector3:new(0.0, 0.0, 0.0)

-- global variables

-- Awake is called once and only once when script is loaded.
function Awake()
	pExplosionPrefabID = ECS:getPrefabID("Prefab_EnemyExplosion")
end

-- Start is called once and only once after Awake() is called.
function Start()
	-- get the player entity id
	playerID = ECS:getEntityID("Player")
    playerTransform = ECS:getComponent_Transform(playerID)
	player_env = ECS:getComponent_LuaScriptCom(playerID):GetEnvironment()
end

-- Updates the Game Object per game loop
function Update(dt)
	eEntID = ScriptSystem:getLuaEntityID()
    if ECS:hasEntity(eEntID) == true then
        bTransform = ECS:getComponent_Transform(eEntID)
        bRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
        bEmitter = ECS:getComponent_Emitter(eEntID)
        bRotNew = PhysicsSystem:getGlobalPoseRotation(eEntID)
        bPos = PhysicsSystem:getGlobalPosePosition(eEntID)
        bMaterial = ECS:getComponent_Material(eEntID)

        playerPos = PhysicsSystem:getGlobalPosePosition(playerID)
        bPos = PhysicsSystem:getGlobalPosePosition(eEntID)
        
        --local bRotTemp = bRotNew commented
        --bTransform.rotate = PhysicsSystem:convertQuatToEuler(bRotTemp)  commented

        fireRocket()
        onCollide()
        -- handleBulletLifeTime(dt)
        updateParticleColor()
        newPlayerPos = playerPos;
    end
end


function destory()
	-- destroy the lua entity itself
    ECS:addEntityToDestroyQueue(eEntID)
end

-- function fireRocket()
-- 	if not hasFired then 
--         PhysicsSystem:setGravityFlag(eEntID, true)
-- 		-- get the direction of the rocket to the player
-- 		-- normalize the direction rocket to unit vector
--         local bulletDir = Vector3.Normalized(playerPos - bPos)
--         --Console.WriteLine("Bullet Dir: ")
--         --Console.PrintFloat(bulletDir)
-- 		-- find the right amount of rotation
-- 		local rotateAmt = bulletDir:Cross(bTransform).z
-- 		local angularVelocity = - (angleChangeSpeed * rotateAmt)
--         --Console.WriteLine("Angular Amount: ")
--         --Console.PrintFloat(angularVelocity)
--         -- Console.WriteLine("Rotation Amount: ")
--         -- Console.PrintFloat(rotateAmt)
--         local angularVec = Vector3:new()
--         angularVec.z = angularVelocity
-- 		local velocity = bTransform.pos.z * rocketSpeed
--         PhysicsSystem:setAngularVelocity(eEntID, angularVec, 50.0)
--         PhysicsSystem:addForceToBody(eEntID, bulletDir, velocity, 0)
--         -- hasFired = true
--     end
-- end

-- function fireRocket()
--     PhysicsSystem:setGravityFlag(eEntID , true)
--     local targetDir = Vector3.new()
--     local currentDir = Vector3.Normalized(playerPos - bPos)
--     local crossProduct = targetDir:Cross(currentDir)
--     local magnitude = crossProduct:LengthSq()
--     local angularVelocity  = crossProduct:Normalized() * rocketSpeed * magnitude
--     Console.WriteLine(angularVelocity.x)
--     Console.WriteLine(angularVelocity.z)
--     local angularVec = Vector3:new()
--     angularVec.z = angularVelocity
--     angularVec.x = angularVelocity
--     --Console.WriteLine(angularVec.x)
--     -- Console.WriteLine(angularVec.z)
--     PhysicsSystem:setAngularVelocity(eEntID, angularVec, 10000.0)
--     PhysicsSystem:addForceToBody(eEntID, currentDir, 30.0, 0)
--     local newPosition = bPos
--     newPosition.y = 5.0
--     PhysicsSystem:setGlobalPose(eEntID, newPosition, bRotNew)
-- end

function fireRocket()
    PhysicsSystem:setGravityFlag(eEntID , true)

    if newPlayerPos.x - playerPos.x <= 0.01 and newPlayerPos.z - playerPos.z <= 0.01 then
        local targetDelta = Vector3:new()
        local DirVector = Vector3:new()
        -- take the new target direction dir
        targetDelta = playerPos - bPos
        
        -- get the angle between rocket transform forward and target delta
        local currentDir = Vector3.Normalized(targetDelta)
        local angleDiff = Vector3.Angle(currentDir, targetDelta)
        
        -- get its cross product, which is the axis of rotation to
        -- get from one vector to the other
        local cross = currentDir:Cross(targetDelta)
        
        -- apply torque along that axis according to the magnitude of the angle.
        local torque = cross * angleDiff * 10.0
        PhysicsSystem:addTorqueToBody(eEntID, torque, rocketForce)
        PhysicsSystem:addForceToBody(eEntID, currentDir, 50.0, 0)
        lookTowards(eEntID, playerID)
        if ECS:hasComponent_Transform(eEntID) then
            local bTransform = ECS:getComponent_Transform(eEntID) 
            bTransform.rotate = eSetEnemyRot
        end
    end
    local maintainY = bPos
    if maintainY.y < 5 then        
        maintainY.y = 5.0
        PhysicsSystem:setGlobalPose(eEntID, maintainY, bRotNew)
    end
end

function onCollide()
    if bRigidDynamic.getHit then
        ----Console.WriteLine("rocket Destroyed")
        local explosionID = ECS:instantiatePrefab(pExplosionPrefabID, "Boss_Explosion", bRigidDynamic.rigidPos,bRigidDynamic.rigidRot,false, false, false)
        -- ECS:addEntityTag(explosionID, "Boss_Explosion")
        AudioSystem:Play3DSFX("PlayerRocketHit_SFX", eEntID)
        ECS:addEntityToDestroyQueue(eEntID)
        PhysicsSystem:LuaRemoveActor(eEntID)
        
    end
end

function handleBulletLifeTime(dt)
    if lifeTime > 0.0 then
        lifeTime = lifeTime - dt
    end

    if lifeTime <= 0.0 then
        -- Console.WriteLine("Rocket Destroyed")
        hasFired = false
        ECS:addEntityToDestroyQueue(eEntID)
        PhysicsSystem:LuaRemoveActor(eEntID)
    end
end

function updateParticleColor()
    bEmitter.colorBegin.x = 1.0
    bEmitter.colorBegin.y = Random.Float()
    bEmitter.colorBegin.z = Random.Float()

    bEmitter.sizeBegin = 0.7
	bEmitter.sizeEnd   = 0
	
	bEmitter.velocityVariation.x = 10.0
	bEmitter.velocityVariation.y = 10.0
	bEmitter.velocityVariation.z = 10.0
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

        if targetRotation ~= prevTargetRotation then
            local rotationDifference = eSetEnemyRot.y + targetRotation -90

            --rotationDifference = rotationDifference % 360.0
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
        prevTargetRotation = targetRotation
    end
end

function updateRocketRot()
    
    if eSetEnemyRot.y > 360 then
        eSetEnemyRot.y = 0
    elseif eSetEnemyRot.y < 0 then
        eSetEnemyRot.y = 360
    end

    local newPos = PhysicsSystem:getGlobalPosePosition(EntID1)
    local eRotQuat = PhysicsSystem:convertEulerToQuat(eSetEnemyRot)
        
    PhysicsSystem:setGlobalPose(EntID1, newPos, eRotQuat)
end