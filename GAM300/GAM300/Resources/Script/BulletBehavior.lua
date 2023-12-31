--stuff goes here

-- Component Variables
local bTransform
local bRigidDynamic
local bMaterial
local eEntID
local player_env
local playerID

local hasFired      = false
local lifeTime      = 1.0
local bulletForce   = 10000.0

local bForwardPrime = Vector3:new()
local bForward      = Vector3:new(0.0, 0.0, -1.0)

local bRotNew       = Quat:new()
local bPos          = Vector3:new()

local CCdelay       = 0.0
local timer         = 2.0
local starTimer     = false

-- Awake is called once and only once when script is loaded.
function Awake()

end

-- Start is called once and only once after Awake() is called.
function Start()
    ------Console.WriteLine("Bullet Behavior script loaded")
    playerID = ECS:getEntityID("Player")
    player_env = ECS:getComponent_LuaScriptCom(playerID):GetEnvironment()
   
end

-- Updates the Game Object per game loop
function Update(dt)
    eEntID = ScriptSystem:getLuaEntityID()
    bTransform = ECS:getComponent_Transform(eEntID)
    bRigidDynamic = ECS:getComponent_RigidDynamic(eEntID)
    bRotNew = PhysicsSystem:getGlobalPoseRotation(eEntID)
    bPos = PhysicsSystem:getGlobalPosePosition(eEntID)
    bMaterial = ECS:getComponent_Material(eEntID)
    local bRotTemp = bRotNew
    --bRotTemp.x = bRotTemp.x * -1.0
    --bRotTemp.z = bRotTemp.z * -1.0

    bTransform.rotate = PhysicsSystem:convertQuatToEuler(bRotTemp)
    fireBullet()
    onCollide()
    handleBulletLifeTime(dt)
    updateCCdelay(dt)
    randomColor()
	
	if starTimer == true then
		timer = timer - 0.1
		--Console.WriteLine("Timer started!")
	end
	
	if timer < 0 then
		if ECS:hasComponent_Emitter(eEntID) then
			eEmitter.emit = false	
			ECS:addEntityToDestroyQueue(eEntID)
			--Console.WriteLine("Bullet Destroyed")
		end
	end

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
        bTransform.scale.x = 0.0
        bTransform.scale.y = 0.0
        bTransform.scale.z = 0.0
        bRigidDynamic.rigidScale.x = 0.0
        bRigidDynamic.rigidScale.y = 0.0
        bRigidDynamic.rigidScale.z = 0.0
        PhysicsSystem:LuaRemoveActor(eEntID)
        
		
		--Emit Particle
		if ECS:hasComponent_Emitter(eEntID) then
		
			eEmitter = ECS:getComponent_Emitter(eEntID)
			eEmitter.colorBegin.x = Random.Float()
			eEmitter.colorBegin.y = Random.Float()
			eEmitter.colorBegin.z = Random.Float()
		
			eEmitter.sizeBegin = 0.3
			eEmitter.sizeEnd   = 0
		
			eEmitter.velocityVariation.x = 100.0
			eEmitter.velocityVariation.y = 100.0
			eEmitter.velocityVariation.z = 100.0
		
			eEmitter.sizeVariation = 1.0
	
			eEmitter.lifeTime = 0.3
			eEmitter.emit = true
			
			starTimer = true
			
		end

        bRigidDynamic.getHit = false
		
		--ECS:addEntityToDestroyQueue(eEntID)
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

function randomColor()
    if CCdelay <= 0.0 then
        bMaterial.colour.x = Random.Float() * 7.0
        bMaterial.colour.y = Random.Float() * 7.0
        bMaterial.colour.z = Random.Float() * 7.0
        CCdelay = 0.05
    end
end

function updateCCdelay(dt)
    if CCdelay > 0.0 then
        CCdelay = CCdelay - dt
    end
end