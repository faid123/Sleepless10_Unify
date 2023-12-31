-- Emitter
local effectEntID
local effEmitter
local effLifeTime   = 2
local eEmitTime = 0.3

function Awake()
end

function Start()
end

function Update(dt)
    effectEntID = ScriptSystem:getLuaEntityID()
    effEmitter = ECS:getComponent_Emitter(effectEntID)
    handleLifeTime(dt)
    emitTime(dt)
    updateEmitterParticleColor()
end

function updateEmitterParticleColor()
    effEmitter = ECS:getComponent_Emitter(effectEntID)

    effEmitter.colorBegin.x = Random.Float()
    effEmitter.colorBegin.y = Random.Float()
    effEmitter.colorBegin.z = Random.Float()

    effEmitter.colorEnd.x = Random.Float()
    effEmitter.colorEnd.y = Random.Float()
    effEmitter.colorEnd.z = Random.Float()
end

function handleLifeTime(dt)
    if effLifeTime > 0.0 then
        effLifeTime = effLifeTime - dt        
    end

    if effLifeTime <= 0.0 then
        ECS:addEntityToDestroyQueue(effectEntID)
        PhysicsSystem:LuaRemoveActor(effectEntID)
    end
end

function emitTime(dt)
    if eEmitTime >= 0 then
        eEmitTime = eEmitTime - dt
        effEmitter.emit = true
    end

    if eEmitTime <= 0 then
        effEmitter.emit = false
    end

end