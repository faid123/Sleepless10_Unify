local eEntID
local eEmitter

local lifetime = 1.0

-- Awake is called once and only once when script is loaded.
function Awake()
end

-- Start is called once and only once after Awake() is called.
function Start()
end

-- Updates the Game Object per game loop
function Update(dt)
	eEntID = ScriptSystem:getLuaEntityID()
	eEmitter = ECS:getComponent_Emitter(eEntID)

	eEmitter.colorBegin.x = Random.Float()
    eEmitter.colorBegin.y = Random.Float()
	eEmitter.colorBegin.z = Random.Float()

	eEmitter.colorEnd.x = Random.Float()
    eEmitter.colorEnd.y = Random.Float()
	eEmitter.colorEnd.z = Random.Float()

	eEmitter.sizeBegin = 1.0
	eEmitter.sizeEnd   = 0
	
	eEmitter.velocityVariation.x = 100.0
	eEmitter.velocityVariation.y = 100.0
	eEmitter.velocityVariation.z = 100.0
	
	eEmitter.sizeVariation = 5.0
	
	eEmitter.lifeTime = 1.0

	if lifetime > 0.0 then
		lifetime = lifetime - dt
	end

	if lifetime <= 0.0 then
		ECS:addEntityToDestroyQueue(eEntID)
	end
end