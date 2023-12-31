--stuff goes here

-- Component Variables
local bTransform
local bRigidDynamic
local bEmitter
local eEntID

local hasFired      = false
local lifeTime      = 1
local bulletForce   = 2500.0

local bForwardPrime = Vector3:new()
local bForward      = Vector3:new(0.0, 0.0, -1.0)

local bRotNew       = Quat:new()

-- Awake is called once and only once when script is loaded.
function Awake()

end

-- Start is called once and only once after Awake() is called.
function Start()
    ----Console.WriteLine("Rocket Behavior script loaded")   
end

-- Updates the Game Object per game loop
function Update(dt)
    eEntID = ScriptSystem:getLuaEntityID()
    bTransform = ECS:getComponent_Transform(eEntID)
    bRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
    bEmitter = ECS:getComponent_Emitter(eEntID)
    bRotNew = PhysicsSystem:getGlobalPoseRotation(eEntID)
    local bRotTemp = bRotNew
    bRotTemp.x = bRotTemp.x * -1.0
    bRotTemp.z = bRotTemp.z * -1.0
    bTransform.rotate = PhysicsSystem:convertQuatToEuler(bRotTemp)
    --fireBullet()
    handleBulletLifeTime(dt)
    onCollide()
    updateParticleColor()
end

function onCollide()
    if bRigidDynamic.getHit then
        bRigidDynamic.rigidScale.x = 0.01
        bRigidDynamic.rigidScale.y = 0.01
        bRigidDynamic.rigidScale.z = 0.01
    end

end

function handleBulletLifeTime(dt)
    if lifeTime > 0.0 then
        lifeTime = lifeTime - dt        
    end

    if lifeTime <= 0.0 then
        hasFired = false
        ECS:addEntityToDestroyQueue(eEntID)
        PhysicsSystem:LuaRemoveActor(eEntID)
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

function updateParticleColor()
    --bEmitter.colorBegin.x = 1.0
    --bEmitter.colorBegin.y = Random.Float()
    bEmitter.colorBegin.x = 1.0
    bEmitter.colorBegin.y = Random.Float()
	bEmitter.colorBegin.z = 0.0
	
	bEmitter.sizeBegin = 1
	bEmitter.sizeEnd   = 0
	
	bEmitter.velocityVariation.x = 50.0
	bEmitter.velocityVariation.y = 50.0
	bEmitter.velocityVariation.z = 50.0
	
	bEmitter.lifeTime = 0.5
end

