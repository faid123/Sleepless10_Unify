--stuff goes here

-- Component Variables
local eTransform
local eRigidDynamic
local eEntID
local frandvar
-- Awake is called once and only once when script is loaded.
function Awake()

end

-- Start is called once and only once after Awake() is called.
function Start()
    ----Console.WriteLine("Temptest script loaded")
    Random.Init()
   
end

-- Updates the Game Object per game loop
function Update(dt)
    eEntID = ScriptSystem:getLuaEntityID()
    ------Console.WriteLine("EntityID: ")
    ----Console.PrintEntityID(eEntID)

    -- Move the obj by incrementing transform component
    if ECS:hasComponent_Transform(eEntID) then
        eTransform = ECS:getComponent_Transform(eEntID)
        ----Console.PrintFloat(eTransform.pos.x)
        eTransform.pos.x = eTransform.pos.x + 0.1
        --eTransform.scale.x = eTransform.scale.x + 0.1
        --eTransform.rotate.x = eTransform.rotate.x + 0.1
    end

    frandvar = Random.Float()
    ----Console.PrintFloat(frandvar)
end

-- If there is collision detected, this physics component runs this function
function onTriggerEnter2D(gameObject, dt, manifold)
 
end

-- If there is collision detected, this physics component runs this function
function onTriggerStay2D(gameObject, dt, manifold)

end