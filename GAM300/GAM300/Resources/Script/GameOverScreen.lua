local pointerID
local pTransform
local pointerNo = 0

local canTriggerW_Key
local canTriggerS_Key

local buttonsID
local buttonsSprite

local timer = 0.0
local canPlayMusic = true

function Awake()
end

function Start()
end

function Update(dt)
	pointerID = ScriptSystem:getLuaEntityID()
	pTransform = ECS:getComponent_Transform(pointerID)
	
	buttonsID = ECS:getEntityID("gameover_buttons")
	buttonsSprite = ECS:getComponent_SpriteCom(buttonsID)

	if canPlayMusic then
		if timer <= 3.0 then
			timer = timer + dt
		else
			AudioSystem:PlayBGM("UNI-FY_GameOver")
			canPlayMusic = false
			timer = 0.0
		end
	end
	
	if Inputs.get_Key_Triggered(InputKey.W_KEY) or Inputs.get_Controller(InputControllerKey.UP_DPAD)
		and canTriggerW_Key then
			AudioSystem:PlayUISound("SFX_MenuSelect")
			if pointerNo == 0 then
				pointerNo = 1
			else
				pointerNo = 0
			end
			canTriggerW_Key = false
	
	elseif Inputs.get_Key_Triggered(InputKey.S_KEY) or Inputs.get_Controller(InputControllerKey.DOWN_DPAD)
		and canTriggerS_Key then
			AudioSystem:PlayUISound("SFX_MenuSelect")
			if pointerNo == 0 then
				pointerNo = 1
			else
				pointerNo = 0
			end
			canTriggerS_Key = false
	end

	if Inputs.get_Key_Triggered(InputKey.ENTER_KEY) or Inputs.get_Controller(InputControllerKey.B_BUTTON) then
		AudioSystem:StopUISound("SFX_GameOver")
		AudioSystem:PlayUISound("SFX_MenuEnter")
		AudioSystem:StopBGM("UNI-FY_GameOver")
		if pointerNo == 0 then
			UISystem:ResetLevel()
		else
			UISystem:setGameState_wF2B(15)
		end
	end
	
	if Inputs.get_KeyReleaseTrigger(InputKey.W_KEY) then
		canTriggerW_Key = true
	elseif Inputs.get_KeyReleaseTrigger(InputKey.S_KEY) then
		canTriggerS_Key = true
	end
	
	if pointerNo == 0 then
		buttonsSprite.SpriteName = "game_over_menu1"
		pTransform.pos.x = -55.87
		pTransform.pos.y = -32.39
	else
		buttonsSprite.SpriteName = "game_over_menu2"
		pTransform.pos.x = -43.52
		pTransform.pos.y = -46.01
	end
end