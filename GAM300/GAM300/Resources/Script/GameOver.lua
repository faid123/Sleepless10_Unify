local pointerID
local pTransform
local pointerNo = 0

local canTriggerW_Key
local canTriggerS_Key

local buttonsID
local buttonsSprite

function Awake()
end

function Start()
end

function update(dt)
	pointerID = ScriptSystem:getLuaEntityID()
	pTransform = ECS:getComponent_Transform(pointerID)

	buttonsID = ECS:getEntityID("gameover_buttons")
	buttonsSprite = ECS:getComponent_SpriteCom(buttonsID)

	if Inputs.get_Key_Triggered(InputKey.W_KEY) or Inputs.get_Controller(InputControllerKey.UP_DPAD) 
		and canTriggerW_Key then
			if pointerNo == 0 then
				pointerNo = 1
			else
				pointerNo = 0
			end
		canTriggerW_Key = false
	
	elseif Inputs.get_Key_Triggered(InputKey.S_KEY) or Inputs.get_Controller(InputControllerKey.DOWN_DPAD)
		and canTriggerS_Key then
			if pointerNo == 0 then
				pointerNo = 1
			else
				pointerNo = 0
			end
		canTriggerS_Key = false
	end

	if pointerNo == 0 then
		buttonsSprite.spriteName = "game_over_menu1"
	else
		buttonsSprite.spriteName = "game_over_menu2"
end