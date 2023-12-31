local eEntID
local timer = 0.8

-- Awake is called once and only once when script is loaded.
function Awake()
end

-- Start is called once and only once after Awake() is called.
function Start()
end

-- Updates the Game Object per game loop
function Update(dt)
	eEntID = ScriptSystem:getLuaEntityID() 
	if timer > 0.0 then
		timer = timer - dt
	else
		ECS:addEntityToDestroyQueue(eEntID)
	end
end