local eEntID
local eTransform
local eSprite
local pointerNo = 0

local canTriggerW_Key
local canTriggerS_Key
local canTriggerEnter_Key = false

local buttonsID
local buttonsSprite

local volBarID
local gammaBarID
local yesnoID
local htpID
local logoID

local volBarSprite
local gammaBarSprite
local yesnoSprite
local htpSprite
local logoSprite

local enteredFromMainMenu = false
local isFullscreen

local vol
local gamma

local state = 0
-- 0 - main menu
-- 1 - options
-- 2 - are you sure prompt

local no = true
local counter = 0

function Awake()
end

function Start()
	state = 0
	pointerNo = 0
end

function Update(dt)
	eEntID = ScriptSystem:getLuaEntityID()
	eTransform = ECS:getComponent_Transform(eEntID)
	eSprite = ECS:getComponent_SpriteCom(eEntID)
	buttonsID = ECS:getEntityID("mainmenu_buttons")
	buttonsSprite = ECS:getComponent_SpriteCom(buttonsID)

	volBarID = ECS:getEntityID("volume_bar")
	gammaBarID = ECS:getEntityID("brightness_bar")
	yesnoID = ECS:getEntityID("yesno")
	htpID = ECS:getEntityID("how_to_play")
	logoID = ECS:getEntityID("mainmenu_logo")

	volBarSprite = ECS:getComponent_SpriteCom(volBarID)
	gammaBarSprite = ECS:getComponent_SpriteCom(gammaBarID)
	yesnoSprite = ECS:getComponent_SpriteCom(yesnoID)
	htpSprite = ECS:getComponent_SpriteCom(htpID)
	logoSprite = ECS:getComponent_SpriteCom(logoID)

	isFullscreen = UISystem:getFullscreenState()

	vol = AudioSystem:getMasterVolume() * 100.0 / 5.0
	volBarSprite.barPercentage = vol

	gamma = RenderSystem:getGammaVal()
	gammaBarSprite.barPercentage = gamma * 100 + 60

	if Inputs.get_Key_Triggered(InputKey.W_KEY) or Inputs.get_Controller_Trigger(InputControllerKey.UP_DPAD) 
		and canTriggerW_Key then
		if state == 0 then
			AudioSystem:PlayUISound("SFX_MenuSelect")
			if pointerNo == 0 then
				pointerNo = 5
			else
				pointerNo = pointerNo - 1
			end
		elseif state == 1 then
			AudioSystem:PlayUISound("SFX_MenuSelect")
			if pointerNo == 0 then
			pointerNo = 3
			else
				pointerNo = pointerNo - 1
			end
		end

		canTriggerW_Key = false
	
	elseif Inputs.get_Key_Triggered(InputKey.S_KEY) or Inputs.get_Controller_Trigger(InputControllerKey.DOWN_DPAD)
		and canTriggerS_Key then
		if state == 0 then
			AudioSystem:PlayUISound("SFX_MenuSelect")
			if pointerNo == 5 then
				pointerNo = 0
			else
				pointerNo = pointerNo + 1
			end
		elseif state == 1 then
			AudioSystem:PlayUISound("SFX_MenuSelect")
			if pointerNo == 3 then
				pointerNo = 0
			else
				pointerNo = pointerNo + 1
			end
		end

		canTriggerS_Key = false
	end

	if Inputs.get_Key_Triggered(InputKey.ENTER_KEY) or Inputs.get_Controller_Trigger(InputControllerKey.A_BUTTON) then
		if pointerNo == 0 then
			if state == 0 then
				AudioSystem:PlayUISound("SFX_MenuEnter")
				UISystem:setGameState_wF2B(7)
			end
		elseif pointerNo == 1 then
			if state == 0 then
				AudioSystem:PlayUISound("SFX_MenuEnter")
				UISystem:setGameState(10)
			end
		elseif pointerNo == 2 then
			if state == 0 then
				AudioSystem:PlayUISound("SFX_MenuEnter")
				buttonsSprite.SpriteName = "main_menu_options"
				volBarSprite._drawSprite = true
				gammaBarSprite._drawSprite = true
				yesnoSprite._drawSprite = true
				state = 1
				pointerNo = 0
			end
		elseif pointerNo == 3 then
			if state == 0 then
				AudioSystem:PlayUISound("SFX_MenuEnter")
				state = 3
			end
		elseif pointerNo == 4 then
			AudioSystem:PlayUISound("SFX_MenuEnter")
			if RenderSystem:getStartVideo() == false then
				UISystem:toggleCutscene(true)
				RenderSystem:setVideoStart(true)
				RenderSystem:setVideoName("credits.mp4")
				RenderSystem:setVideoEndTime(31.0)
				RenderSystem:setPlayVideo(true)
				RenderSystem:setVideoMinimapAfter(false)
			end
		elseif pointerNo == 5 then
			if state == 0 then
				AudioSystem:PlayUISound("SFX_MenuEnter")
				state = 2
				canTriggerEnter_Key = true
			end
		elseif pointerNo == 6 then
			if state == 2 then
				AudioSystem:PlayUISound("SFX_MenuEnter")
				state = 0
				pointerNo = 5
			end
		elseif pointerNo == 7 then
			UISystem:setGameState(12)
		elseif pointerNo == 8 then
			AudioSystem:PlayUISound("SFX_MenuEnter")
			htpSprite._drawSprite = false
			buttonsSprite._drawSprite = true
			eSprite._drawSprite = true
			logoSprite._drawSprite = true
			state = 0
			pointerNo = 3
		end
	end	

	if pointerNo == 0 then
		eTransform.pos.x = -158.84
		eTransform.pos.y = 3.210
		if state == 0 then
			buttonsSprite.SpriteName = "main_menu_front"
		elseif state == 1 then
			buttonsSprite.SpriteName = "main_menu_options_1"
		end
	elseif pointerNo == 1 then
		eTransform.pos.x = -158.84
		eTransform.pos.y = -9.46
		if state == 0 then
			buttonsSprite.SpriteName = "main_menu_front_2"
		elseif state == 1 then
			buttonsSprite.SpriteName = "main_menu_options_2"
		end
	elseif pointerNo == 2 then
		eTransform.pos.x = -158.84
		eTransform.pos.y = -22.13
		if state == 0 then
			buttonsSprite.SpriteName = "main_menu_front_3"
		elseif state == 1 then
			buttonsSprite.SpriteName = "main_menu_options_3"
		end
	elseif pointerNo == 3 then
		eTransform.pos.x = -158.84
		eTransform.pos.y = -34.8
		if state == 0 then
			buttonsSprite.SpriteName = "main_menu_front_4"
		elseif state == 1 then
			buttonsSprite.SpriteName = "main_menu_options"
		end
	elseif pointerNo == 4 then
		eTransform.pos.x = -158.84
		eTransform.pos.y = -47.47
		if state == 0 then
			buttonsSprite.SpriteName = "main_menu_front_5"
		end
	elseif pointerNo == 5 then
		eTransform.pos.x = -158.84
		eTransform.pos.y = -60.14
		if state == 0 then
			buttonsSprite.SpriteName = "main_menu_front_6"
		end
	elseif pointerNo == 6 then
		eTransform.pos.x = -108.49
		eTransform.pos.y = -34.65
		buttonsSprite.SpriteName = "are_you_sure_no"
	elseif pointerNo == 7 then
		eTransform.pos.x = -153.5
		eTransform.pos.y = -34.65
		buttonsSprite.SpriteName = "are_you_sure_yes"
	end

	if Inputs.get_KeyReleaseTrigger(InputKey.W_KEY) or Inputs.get_Controller_Trigger(InputControllerKey.UP_DPAD) then
		canTriggerW_Key = true
	elseif Inputs.get_KeyReleaseTrigger(InputKey.S_KEY) or Inputs.get_Controller_Trigger(InputControllerKey.DOWN_DPAD) then
		canTriggerS_Key = true
	end

	if state == 1 then
		handleOptionsMenu()
	elseif state == 2 then
		handleQuit()
	elseif state == 3 then
		handleHowToPlay()
	end
end

function handleOptionsMenu()
	if Inputs.get_Key_Triggered(InputKey.A_KEY) or Inputs.get_Controller_Trigger(InputControllerKey.LEFT_DPAD) then
		AudioSystem:PlayUISound("SFX_MenuSelect")
		if pointerNo == 0 then
			if isFullscreen then
				UISystem:setFullscreen(false)
			else
				UISystem:setFullscreen(true)
			end
		elseif pointerNo == 1 then
			AudioSystem:decreaseMasterVolume()
		elseif pointerNo == 2 then
			RenderSystem:addGamma(-0.2)
		end
	end

	if Inputs.get_Key_Triggered(InputKey.D_KEY) or Inputs.get_Controller_Trigger(InputControllerKey.RIGHT_DPAD) then
		AudioSystem:PlayUISound("SFX_MenuSelect")
		if pointerNo == 0 then
			if isFullscreen then
				UISystem:setFullscreen(false)
			else
				UISystem:setFullscreen(true)
			end
		elseif pointerNo == 1 then
			AudioSystem:increaseMasterVolume()
		elseif pointerNo == 2 then
			RenderSystem:addGamma(0.2)

		end
	end

	if Inputs.get_Key_Triggered(InputKey.ENTER_KEY) or Inputs.get_Controller_Trigger(InputControllerKey.A_BUTTON) then
		if pointerNo == 3 then
			AudioSystem:PlayUISound("SFX_MenuEnter")
			UISystem:goToPrevState()
			buttonsSprite.SpriteName = "main_menu_front"
			volBarSprite._drawSprite = false
			gammaBarSprite._drawSprite = false
			yesnoSprite._drawSprite = false
			state = 0
			pointerNo = 0
		end
	end

	if Inputs.get_Key_Triggered(InputKey.ESC_KEY) then
		UISystem:goToPrevState()
		buttonsSprite.SpriteName = "main_menu_front"
		volBarSprite._drawSprite = false
		gammaBarSprite._drawSprite = false
		yesnoSprite._drawSprite = false
		state = 0
		pointerNo = 0
	end

	if isFullscreen then
		yesnoSprite.SpriteName = "main_menu_yes"
		--UISystem:setFullscreen(true)
	else
		yesnoSprite.SpriteName = "main_menu_no"
		--UISystem:setFullscreen(false)
	end
end

function handleQuit()
	if Inputs.get_Key_Triggered(InputKey.A_KEY) or Inputs.get_Controller_Trigger(InputControllerKey.LEFT_DPAD) then
		AudioSystem:PlayUISound("SFX_MenuSelect")
		no = false
	end
	
	if Inputs.get_Key_Triggered(InputKey.D_KEY) or Inputs.get_Controller_Trigger(InputControllerKey.RIGHT_DPAD) then
		AudioSystem:PlayUISound("SFX_MenuSelect")
		no = true
	end

	if no then
		buttonsSprite.SpriteName = "are_you_sure_no"
		pointerNo = 6
	elseif not no then
		buttonsSprite.SpriteName = "are_you_sure_yes"
		pointerNo = 7
	end
end

function handleHowToPlay()
	htpSprite._drawSprite = true
	buttonsSprite._drawSprite = false
	logoSprite._drawSprite = false
	pointerNo = 8
	eTransform.pos.x = 135.0
	eTransform.pos.y = 94.0

	if Inputs.get_Key_Triggered(InputKey.ESC_KEY) then
		htpSprite._drawSprite = false
		buttonsSprite._drawSprite = true
		logoSprite._drawSprite = true
		state = 0
		pointerNo = 3
	end
end