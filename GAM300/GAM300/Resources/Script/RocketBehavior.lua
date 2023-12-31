--stuff goes here

-- Component Variables
local bTransform
local bRigidDynamic
local bEmitter
local eEntID
local pExplosionPrefabID
local player_env
local playerID
local bMaterial

local hasFired      = false

local lifeTime      = 10.0
local bulletForce   = 6000.0

local bForwardPrime = Vector3:new()
local bForward      = Vector3:new(0.0, 0.0, -1.0)

local bRotNew       = Quat:new()
local bPos          = Vector3:new()

-- Explosion Effect
local effectPrefabID
local effectEmitSpawned = false
local effRotQuat        = Quat:new()

-- Awake is called once and only once when script is loaded.
function Awake()
    pExplosionPrefabID = ECS:getPrefabID("Prefab_Explosion")
end

-- Start is called once and only once after Awake() is called.
function Start()
    ----Console.WriteLine("Rocket Behavior script loaded")
    playerID = ECS:getEntityID("Player")
    player_env = ECS:getComponent_LuaScriptCom(playerID):GetEnvironment()
    effectPrefabID = ECS:getPrefabID("Prefab_ExplosionEffect")
end

-- Updates the Game Object per game loop
function Update(dt)
    eEntID = ScriptSystem:getLuaEntityID()
    bTransform = ECS:getComponent_Transform(eEntID)
    bRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
    bEmitter = ECS:getComponent_Emitter(eEntID)
    bRotNew = PhysicsSystem:getGlobalPoseRotation(eEntID)
    bPos = PhysicsSystem:getGlobalPosePosition(eEntID)
    bMaterial = ECS:getComponent_Material(eEntID)
    local bRotTemp = bRotNew
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
        ------Console.WriteLine("Rocket Destroyed")
        hasFired = false
        ECS:addEntityToDestroyQueue(eEntID)
        PhysicsSystem:LuaRemoveActor(eEntID)
    end
end


function onCollide()
    if bRigidDynamic.getHit then
        ----Console.WriteLine("rocket Destroyed")
        local explosionID = ECS:instantiatePrefab(pExplosionPrefabID, "Explosion", bRigidDynamic.rigidPos,bRigidDynamic.rigidRot,false, false, false)
        
        if not effectEmitSpawned then
            effRotQuat = PhysicsSystem:convertEulerToQuat(bTransform.rotate)
            local effectEntID = ECS:instantiatePrefab(effectPrefabID, "ExplosionEffect", bTransform.pos,effRotQuat,false, false, false)
            local effectTransform = ECS:getComponent_Transform(effectEntID)
		    effectTransform.pos = bTransform.pos
            effectEmitSpawned = true
        end
        
        AudioSystem:Play3DSFX("PlayerRocketHit_SFX", eEntID)
        ECS:addEntityToDestroyQueue(eEntID)
        PhysicsSystem:LuaRemoveActor(eEntID)
        
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

function fireBullet()
    if not hasFired then 
        PhysicsSystem:setGravityFlag(eEntID, true)
        local bulletDir = (player_env.TargetPoint - bPos):Normalized()
        PhysicsSystem:addForceToBody(eEntID, bulletDir, bulletForce, 0)
        hasFired = true
    end
end

function updateParticleColor()
    bEmitter.colorBegin.x = 1.0
    bEmitter.colorBegin.y = 0.3
    bEmitter.colorBegin.z = 1.0

    bEmitter.colorEnd.x = 0.7
    bEmitter.colorEnd.y = 0.7

    bEmitter.sizeBegin = 0.7
	bEmitter.sizeEnd   = 0
	bEmitter.lifeTime = 0.7
	bEmitter.velocityVariation.x = 10.0
	bEmitter.velocityVariation.y = 10.0
	bEmitter.velocityVariation.z = 10.0
end