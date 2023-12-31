local playerID
local player_env
local entit
local max = 0.5
local min = 0
local spriteCom

function Awake()
end

function Start()
	playerID = ECS:getEntityID("Player")
	player_env = ECS:getComponent_LuaScriptCom(playerID):GetEnvironment()
end

function Update(dt)
	ID = ScriptSystem:getLuaEntityID()
	--Console.PrintFloat(ID)
	if ECS:hasComponent_SpriteCom(ID) then
	spriteCom = ECS:getComponent_SpriteCom(ID)
	--Console.PrintFloat(player_env.dashLifetime)
	spriteCom._alphaVal  = lerp(min, max, player_env.dashLifetime )
	end
end

function lerp(a,b,t)
	return a + (t * (b - a))
end