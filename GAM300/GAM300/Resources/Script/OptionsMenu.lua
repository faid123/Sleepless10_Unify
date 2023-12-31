local eEntID
local eTransform
local pointerNo = 0
local canTriggerW_Key
local canTriggerS_Key

local volBarID
local gammaBarID
local yesnoID

local volBarSprite
local gammaBarSprite
local yesnoSprite

local isFullscreen = false

function Awake()
end

function Start()
end

function Update(dt)
	eEntID = ScriptSystem:getLuaEntityID()
	eTransform = ECS:getComponent_Transform(eEntID)

	volBarID = ECS:getEntityID("volume_bar")
	gammaBarID = ECS:getEntityID("brightness_bar")
	yesnoID = ECS:getEntityID("yesno")

	volBarSprite = ECS:getComponent_SpriteCom(volBarID)
	gammaBarSprite = ECS:getComponent_SpriteCom(gammaBarID)
	yesnoSprite = ECS:getComponent_SpriteCom(yesnoID)

	if Inputs.get_Key_Triggered(InputKey.W_KEY) or Inputs.get_Controller(InputControllerKey.UP_DPAD) 
		and canTriggerW_Key then
		if pointerNo == 0 then
			pointerNo = 3
		else
			pointerNo = pointerNo - 1
		end

		canTriggerW_Key = false
	
	elseif Inputs.get_Key_Triggered(InputKey.S_KEY) or Inputs.get_Controller(InputControllerKey.DOWN_DPAD)
		and canTriggerS_Key then
		if pointerNo == 3 then
			pointerNo = 0
		else
			pointerNo = pointerNo + 1
		end

		canTriggerS_Key = false
	end

	if Inputs.get_Key_Triggered(InputKey.A_KEY) or Inputs.get_Controller(InputControllerKey.LEFT_DPAD) then
		if pointerNo == 0 then
			if isFullscreen then
				isFullscreen = false
			else
				isFullscreen = true
			end
		elseif pointerNo == 1 then
			volBarSprite.barPercentage = volBarSprite.barPercentage - 10
			AudioSystem:decreaseMasterVolume()
		elseif pointerNo == 2 then
			gammaBarSprite.barPercentage = gammaBarSprite.barPercentage - 20
		end
	end

	if Inputs.get_Key_Triggered(InputKey.D_KEY) or Inputs.get_Controller(InputControllerKey.RIGHT_DPAD) then
		if pointerNo == 0 then
			if isFullscreen then
				isFullscreen = false
			else
				isFullscreen = true
			end
		elseif pointerNo == 1 then
			volBarSprite.barPercentage = volBarSprite.barPercentage + 10
			AudioSystem:increaseMasterVolume()
		elseif pointerNo == 2 then
			gammaBarSprite.barPercentage = gammaBarSprite.barPercentage + 20
		end
	end

	if Inputs.get_Key_Triggered(InputKey.ENTER_KEY) or Inputs.get_Controller(InputControllerKey.B_BUTTON) then
		if pointerNo == 3 then
			UISystem:goToPrevState()
		end
	end

	if Inputs.get_Key_Triggered(InputKey.ESC_KEY) then
		UISystem:goToPrevState()
	end

	if isFullscreen then
		yesnoSprite.SpriteName = "main_menu_yes"
	else
		yesnoSprite.SpriteName = "main_menu_no"
	end

	if pointerNo == 0 then
		eTransform.pos.y = 3.210
	elseif pointerNo == 1 then
		eTransform.pos.y = -9.46
	elseif pointerNo == 2 then
		eTransform.pos.y = -22.13
	elseif pointerNo == 3 then
		eTransform.pos.y = -34.8
	end

	if Inputs.get_KeyReleaseTrigger(InputKey.W_KEY) then
		canTriggerW_Key = true
	elseif Inputs.get_KeyReleaseTrigger(InputKey.S_KEY) then
		canTriggerS_Key = true
	end
end