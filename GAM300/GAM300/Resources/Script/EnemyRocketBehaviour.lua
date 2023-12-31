--stuff goes here

-- Component Variables
local bTransform
local bRigidDynamic
local bEmitter
local eEntID
local player_env
local playerID
local pTransform
local pRigidDynamic

local helicopterID
local helicopterEnv
local hTransform

local eExplosionPrefabID

local hasFired      = false
local lifeTime      = 2.0
local bulletForce   = 2500.0

local bForwardPrime = Vector3:new()
local bForward      = Vector3:new(0.0, 0.0, -1.0)

local bRotNew       = Quat:new()
local bPos          = Vector3:new()

-- Awake is called once and only once when script is loaded.
function Awake()
    eExplosionPrefabID = ECS:getPrefabID("Prefab_EnemyExplosion")
end

-- Start is called once and only once after Awake() is called.
function Start()
    ------Console.WriteLine("Bullet Behavior script loaded")
    playerID = ECS:getEntityID("Player")
    player_env = ECS:getComponent_LuaScriptCom(playerID):GetEnvironment()
    pTransform = ECS:getComponent_Transform(playerID)
    pRigidDynamic = ECS:getComponent_RigidDynamic(playerID)

    helicopterID = ECS:getEntityID("HelicopterEnemy")
    helicopterEnv = ECS:getComponent_LuaScriptCom(helicopterID):GetEnvironment()
    hTransform = ECS:getComponent_Transform(helicopterID)
end

-- Updates the Game Object per game loop
function Update(dt)
    eEntID = ScriptSystem:getLuaEntityID()
    bTransform = ECS:getComponent_Transform(eEntID)
    bRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
    bRotNew = PhysicsSystem:getGlobalPoseRotation(eEntID)
    bPos = PhysicsSystem:getGlobalPosePosition(eEntID)
    bEmitter = ECS:getComponent_Emitter(eEntID)

    local bRotTemp = bRotNew
    --bRotTemp.x = bRotTemp.x * -1.0
    --bRotTemp.z = bRotTemp.z * -1.0

    bTransform.rotate = PhysicsSystem:convertQuatToEuler(bRotTemp)
    fireBullet()
    onCollide()
    handleBulletLifeTime(dt)
    updateParticleColor()
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
    if bRigidDynamic.getHit and not mContactReportCallback.enemyRocketHitHeli then
        --Console.WriteLine("Bullet Destroyed")
        local explosionID = ECS:instantiatePrefab(eExplosionPrefabID, "EnemyExplosion", bRigidDynamic.rigidPos,bRigidDynamic.rigidRot,false, false, false)
        AudioSystem:Play3DSFX("PlayerRocketHit_SFX", eEntID)
        PhysicsSystem:LuaRemoveActor(eEntID)
        ECS:addEntityToDestroyQueue(eEntID)
        bRigidDynamic.getHit = false
    end
end


function fireBullet()
    if not hasFired then 
        if ECS:hasComponent_RigidDynamic(eEntID) then
            PhysicsSystem:setGravityFlag(eEntID, true)
        end
        --PhysicsSystem:rotate_vector_by_quaternion(bForward, bRotNew, bForwardPrime)

        local pPos = PhysicsSystem:getGlobalPosePosition(playerID)
        local targetPos = Vector3:new(pPos.x, 1.2, pPos.z)
        PhysicsSystem:addForceToBody(eEntID, Vector3.Normalized(targetPos - hTransform.pos), bulletForce, 0)
        hasFired = true
    end
end

function randomColor()
    if CCdelay <= 0.0 then
        bMaterial.colour.x = Random.Float() * 7.0
        bMaterial.colour.y = Random.Float() * 7.0
        bMaterial.colour.z = Random.Float() * 7.0
        CCdelay = 0.05
    end
end

function updateParticleColor()
    bEmitter.colorBegin.x = 1.0
    bEmitter.colorBegin.y = Random.Float()
    bEmitter.colorBegin.z = 0.0

    bEmitter.sizeBegin = 0.7
	bEmitter.sizeEnd   = 0
	
	bEmitter.velocityVariation.x = 10.0
	bEmitter.velocityVariation.y = 10.0
	bEmitter.velocityVariation.z = 10.0
end