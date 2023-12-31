local eRigidBody
local eEntID
function Awake()
end

function Start(  )

end


function initInUpdate()
    eEntID = ScriptSystem:getLuaEntityID()
    if ECS:hasComponent_RigidStatic(eEntID) then
        eRigidBody = ECS:getComponent_RigidStatic(eEntID)
    end
end

function onHit()

    if eRigidBody.getHit then
        ----Console.WriteLine("TriggerTouched")
        --change state to level 2
        local level = 2
        ScriptSystem:setGameState(level)

        eRigidBody.getHit = false
    end
end


function Update( dt )

	initInUpdate()
    onHit()
end
