--stuff goes here

-- Component Variables
local bTransform
local bRigidDynamic
local eEntID
local player_env
local playerID
local pTransform

local hasFired      = false
local lifeTime      = 1.0
local bulletForce   = 2500.0

local bForwardPrime = Vector3:new()
local bForward      = Vector3:new(0.0, 0.0, -1.0)

local bRotNew       = Quat:new()
local bPos          = Vector3:new()

-- Awake is called once and only once when script is loaded.
function Awake()

end

-- Start is called once and only once after Awake() is called.
function Start()
    ------Console.WriteLine("Bullet Behavior script loaded")
    playerID = ECS:getEntityID("Player")
    player_env = ECS:getComponent_LuaScriptCom(playerID):GetEnvironment()
    pTransform = ECS:getComponent_Transform(playerID)
end

-- Updates the Game Object per game loop
function Update(dt)
    eEntID = ScriptSystem:getLuaEntityID()
    bTransform = ECS:getComponent_Transform(eEntID)
    bRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
    bRotNew = PhysicsSystem:getGlobalPoseRotation(eEntID)
    bPos = PhysicsSystem:getGlobalPosePosition(eEntID)

    local bRotTemp = bRotNew
    --bRotTemp.x = bRotTemp.x * -1.0
    --bRotTemp.z = bRotTemp.z * -1.0

    bTransform.rotate = PhysicsSystem:convertQuatToEuler(bRotTemp)
    fireBullet()
    onCollide()
    handleBulletLifeTime(dt)
end

function handleBulletLifeTime(dt)
    if lifeTime > 0.0 then
        lifeTime = lifeTime - dt        
    end

    if lifeTime <= 0.0 then
        ------Console.WriteLine("Bullet Destroyed")
        hasFired = false
        ECS:addEntityToDestroyQueue(eEntID)
        PhysicsSystem:LuaRemoveActor(eEntID)
    end
end

function onCollide()
    if bRigidDynamic.getHit then
        ------Console.WriteLine("Bullet Destroyed")
        PhysicsSystem:LuaRemoveActor(eEntID)
        ECS:addEntityToDestroyQueue(eEntID)
    end

end


function fireBullet()
    if not hasFired then 
        PhysicsSystem:setGravityFlag(eEntID, true)
        PhysicsSystem:rotate_vector_by_quaternion(bForward, bRotNew, bForwardPrime)
        PhysicsSystem:addForceToBody(eEntID, bForwardPrime, bulletForce, 0)
        hasFired = true
    end
end