local eEntID
local eTransform
local startingX = -215.0
local destinationX = -130.0
local travelSpeed = 350.0

function Awake()
end

-- Start is called once and only once after Awake() is called.
function Start()
end

-- Updates the Game Object per game loop
function Update(dt)
    eEntID = ScriptSystem:getLuaEntityID()
    eTransform = ECS:getComponent_Transform(eEntID)

    if Inputs.get_Key(InputKey.TAB_KEY) then
        eTransform.pos.x = eTransform.pos.x + 1 * dt * travelSpeed
    else
        eTransform.pos.x = eTransform.pos.x - 1 * dt * travelSpeed
    end

    if eTransform.pos.x >= destinationX then
        eTransform.pos.x = destinationX
    end

    if eTransform.pos.x <= startingX then
        eTransform.pos.x = startingX
    end
end