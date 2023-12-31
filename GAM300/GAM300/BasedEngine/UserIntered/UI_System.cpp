#include "UI_System.h"
#include "Input.h"
#include "Engine.h"
#include "Graphics/RenderSystem/RenderSystem.h"
#include "Editor/EditorSystem/EditorSystem.h"
#include <vector>
#include "GameState.h"
#include <Animation/AnimationSystem.h>
bool once = true;

void UISystem::init()
{
	resizeWindow();

}

void UISystem::load()
{
	/*for (auto const& entity : _entities)
	{
		auto _ui = Engine::_ecs.getComponent<UICom>(entity);
		if (_ui != nullptr)
		{
			auto sprite = Engine::_ecs.getComponent<SpriteCom>(entity);
			_UILogic.SetMaxHealthBarScale(sprite->_scale.x);
			sprite->_initalpos = sprite->_pos;
		}
	}*/
	weaponList = findWeapons();
	findHealth();
	findPower();
	findThreat();
	findGeneratorHealth();
}

void UISystem::update(float delta_time)
{
	(void)delta_time;

	//Engine::_ecs.getSystem<WindowSystem>()->setFullScreen();
	
	//get minimap status
	get_RenderSystem_Minimap();

	if (_isPaused)
	{
		HandlePauseMenu();
		/*RESOURCE->getChannel("Enemy_Dog")->setMute(true);
		RESOURCE->getChannel("SFX_ClownHorn")->setMute(true);*/
	}
	std::shared_ptr<TransformCom> transform = nullptr;
	if (Engine::_isPlay && GameStateManager::_curState == GameStateManager::_preState && !_isPaused)
	{
		switch ((GameState)GameStateManager::_curState)
		{
		case GameState::MAIN_MENU:
			// in lua
			set_RenderSystem_Minimap(false);
			break;
		case GameState::LV1:
			if (!_changedSkybox)
				Engine::_ecs.getSystem<RenderSystem>()->ChangeSkybox("Cloud_1");
			break;
		case GameState::LV2:
			if (!_changedSkybox)
				Engine::_ecs.getSystem<RenderSystem>()->ChangeSkybox("Cloud_2");
			//will change once level 2 gets an intro cutscene or something
			//set_RenderSystem_Minimap(true);
			break;
		case GameState::LV3:
			if (!_changedSkybox) //for the future level
				Engine::_ecs.getSystem<RenderSystem>()->ChangeSkybox("skybox");

			//will change once level 3 gets an intro cutscene or something
			//set_RenderSystem_Minimap(true);
			break;
		case GameState::PAUSE_MENU:
			// ui logic
			break;
		case GameState::GAMEOVER:
			// ui logic
			//menuLogic();
			set_RenderSystem_Minimap(false);
			break;
		case GameState::VICTORY:
			// ui logic
			victoryMenuLogic();
			set_RenderSystem_Minimap(false);
			break;
		case GameState::LEVEL_SELECT:
			levelSelectMenuLogic();
			break;
		case GameState::LOADING_MENULV1:
			_storedTime += delta_time;
			if (_storedTime > TIMECHANGE)
			{
				_storedTime = 0.0f;
				setGameState_wF2B((int)GameState::LV1);
			}
			set_RenderSystem_Minimap(false);
			break;
		case GameState::LOADING_MENULV2:
			_storedTime += delta_time;
			if (_storedTime > TIMECHANGE)
			{
				_storedTime = 0.0f;
				setGameState_wF2B((int)GameState::LV2);
			}
			set_RenderSystem_Minimap(false);
			break;
		case GameState::LOADING_MENULV3:
			_storedTime += delta_time;
			if (_storedTime > TIMECHANGE)
			{
				_storedTime = 0.0f; 
				setGameState_wF2B((int)GameState::LV3);
			}

			set_RenderSystem_Minimap(false);
			break;
		case GameState::OPTIONS:
			//OptionsLogic();
			if (!_changedSkybox) //for the future level
				Engine::_ecs.getSystem<RenderSystem>()->ChangeSkybox("skybox");
			break;
		case GameState::DIGIPEN_LOGO:
		case GameState::FMOD_LOGO:
		case GameState::LOADING_START:
		case GameState::TEAM_LOGO:
			set_RenderSystem_Minimap(false);
			break;
		case GameState::NONE:
			break;
		default:
			break;
		}

		//if (Inputs::get_Key_Triggered(GLFW_KEY_1))
		//	swapToWeapon(WeaponType_UI::Pistol);
		//if (Inputs::get_Key_Triggered(GLFW_KEY_2))
		//	swapToWeapon(WeaponType_UI::Laser);
		//if (Inputs::get_Key_Triggered(GLFW_KEY_3))
		//	swapToWeapon(WeaponType_UI::Rocket);


		/*if (health_sprite == nullptr)
			findHealth();
		else
		{
			if (Inputs::get_Key_Triggered(GLFW_KEY_4))
			{
				if(_healthPercent > 0.0f)
					decreaseHealth(damageamount);

			}
			if (Inputs::get_Key_Triggered(GLFW_KEY_5))
			{
				if (_healthPercent < 100.0f)
					increaseHealth(damageamount);

			}
		}*/

		//if (pistol_barHealth == nullptr || 
		//	laser_barHealth == nullptr ||
		//	rocket_barHealth == nullptr
		//	)
		//	findGeneratorHealth();
		//else
		//{
		//	if (Inputs::get_Key_Triggered(GLFW_KEY_4))
		//	{
		//		decreasePistolHealth(damageamount);
		//		decreaseLaserHealth(damageamount);
		//		decreaseRocketHealth(damageamount);
		//	}
		//	if (Inputs::get_Key_Triggered(GLFW_KEY_5))
		//	{
		//		increasePistolHealth(damageamount);
		//		increaseLaserHealth(damageamount);
		//		increaseRocketHealth(damageamount);
		//	}
		//}

	}
	else
	{
		ResetUI();
	}


}

//void UISystem::menuLogic()
//{
//	std::shared_ptr<TransformCom> transform = nullptr;
//	transform = Engine::_ecs.getComponent<TransformCom>(
//		Engine::_ecs.getEntityID("Pointer"));
//	if (Inputs::get_Key_Triggered(GLFW_KEY_W) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_DPAD_UP))
//	{
//		++_button;
//		if (_button > 1)
//			_button = 0;
//	}
//	else if (Inputs::get_Key_Triggered(GLFW_KEY_S) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_DPAD_DOWN))
//	{
//		--_button;
//		if (_button < 0)
//			_button = 1;
//	}
//	if (transform)
//	{
//		if (_button == 0)
//			transform->pos.y = -2.0f;
//		else
//			transform->pos.y = -24.0f;
//	}
//
//	if (Inputs::get_Key_Triggered(GLFW_KEY_ENTER) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_A))
//	{
//		if (_button == 0)
//			GameStateManager::_curState = GameStateManager::_nState;
//		else if (_button == 1)
//			GameStateManager::_curState = (int)GameState::MAIN_MENU;
//	}
//}

void UISystem::victoryMenuLogic()
{
	std::shared_ptr<TransformCom> transform = nullptr;
	transform = Engine::_ecs.getComponent<TransformCom>(
		Engine::_ecs.getEntityID("Pointer"));
	if (Inputs::get_Key_Triggered(GLFW_KEY_W) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_DPAD_UP))
	{
		++_button;
		if (_button > 1)
			_button = 0;
	}
	else if (Inputs::get_Key_Triggered(GLFW_KEY_S) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_DPAD_DOWN))
	{
		--_button;
		if (_button < 0)
			_button = 1;
	}

	if (_button == 0)
		transform->pos.y = -2.0f;
	else
		transform->pos.y = -24.0f;

	if (Inputs::get_Key_Triggered(GLFW_KEY_ENTER) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_A))
	{
		if (_button == 0)
			GameStateManager::_curState = GameStateManager::_nState;
		else if (_button == 1)
			GameStateManager::_curState = (int)GameState::MAIN_MENU;
	}
}

void UISystem::update_v2(float)
{
	/*
		if (Inputs::get_Key_Triggered(49) && keyActive == false)
		{
			_UILogic.SetUIActive();
			keyActive = true;
		}
		else if (Inputs::get_Key_Triggered(50) && keyActive == false)
		{
			_UILogic.SetUITypeActive(Type_UI::Health);
			keyActive = true;
		}
		else if (Inputs::get_Key_Triggered(51) && keyActive == false)
		{
			_UILogic.SetUITypeActive(Type_UI::Ammo);
			keyActive = true;
		}
		else if (Inputs::get_Key_Triggered(52) && keyActive == false)
		{
			_UILogic.SetUITypeActive(Type_UI::ColourBar);
			keyActive = true;
		}

		if ((Inputs::get_KeyReleaseTrigger(49) ||
			Inputs::get_KeyReleaseTrigger(50) ||
			Inputs::get_KeyReleaseTrigger(51) ||
			Inputs::get_KeyReleaseTrigger(49)) && keyActive)
		{
			keyActive = false;
		}*/

		/*if (_UILogic.IsUIActive())
		{
			if (_UILogic.IsUITypeActive(Type_UI::Health))
			{
				_UILogic.HealthUILogic();
			}

			if (_UILogic.IsUITypeActive(Type_UI::Ammo))
			{
				_UILogic.AmmoUILogic();
			}

			if (_UILogic.IsUITypeActive(Type_UI::ColourBar))
			{
				_UILogic.ColourUILogic();
			}
		}*/

		//std::cout << _UILogic.getHealth() << std::endl;
		//std::cout << _UILogic.GetMaxHealthBarScale() << std::endl;

		//for (auto const& entity : _entities)
		//{
		//	auto _ui = Engine::_ecs.getComponent<UICom>(entity);
		//	if (_ui != nullptr)
		//	{
		//		auto sprite = Engine::_ecs.getComponent<SpriteCom>(entity);
		//		Vector2 CamOffSet;

		//		if (_UILogic.IsHit())
		//		{
		//			_UILogic.SetCamoffSetX((_UILogic.GetMaxHealthBarScale() - (_UILogic.GetMaxHealthBarScale() * (_UILogic.getHealth() * 0.01f)))
		//			);
		//			//_UILogic.SetIsHit(false);
		//			CamOffSet = { _UILogic.GetCamOffSetX() , 0.0f };
		//		}
		//		else if (_UILogic.IsHeal())
		//		{
		//			_UILogic.SetCamoffSetX((_UILogic.GetMaxHealthBarScale() - (_UILogic.GetMaxHealthBarScale() * (_UILogic.getHealth() * 0.01f))));
		//			//_UILogic.SetIsHeal(false);
		//			CamOffSet = { _UILogic.GetCamOffSetX() , 0.0f };
		//		}
		//		else {
		//			CamOffSet = { _UILogic.GetCamOffSetX() , 0.0f };
		//		}

		//		if (_UILogic.IsHit() || _UILogic.IsHeal())
		//		{
		//			_UILogic.SetIsHit(false);
		//			_UILogic.SetIsHeal(false);
		//			_UILogic.SetCamoffSetX((_UILogic.GetMaxHealthBarScale() - (_UILogic.GetMaxHealthBarScale() * (_UILogic.getHealth() * 0.01f))));
		//			CamOffSet = { _UILogic.GetCamOffSetX() , 0.0f };

		//			sprite->_pos = sprite->_initalpos - CamOffSet;
		//			sprite->_scale.x = _UILogic.GetMaxHealthBarScale() * (_UILogic.getHealth() * 0.01f);
		//		}
		//		
		//		std::cout << sprite->_pos.x << "  " << sprite->_pos.y << std::endl;
		//		//std::cout << (float)Engine::_ecs.getSystem<WindowSystem>()->getWindowWidth() << "  " << (float)Engine::_ecs.getSystem<WindowSystem>()->getWindowHeight() << std::endl;
		//		//std::cout << InitalWindowSize.x << "  " << InitalWindowSize.y << std::endl;
		//		//std::cout << Engine::_ecs.getSystem<EditorSystem>()->editorWindow._viewportPanalSize.x << "  " << Engine::_ecs.getSystem<EditorSystem>()->editorWindow._viewportPanalSize.y << std::endl;
		//		//std::cout << InitalEditorViewPortSize.x << "  " << InitalEditorViewPortSize.y << std::endl;

		//		auto& imguiVeiwPort = Engine::_ecs.getSystem<EditorSystem>()->editorWindow._viewportPanalSize;
		//		if (InitalWindowSize.x == (float)Engine::_ecs.getSystem<WindowSystem>()->getWindowWidth() && InitalWindowSize.y == (float)Engine::_ecs.getSystem<WindowSystem>()->getWindowHeight())
		//		{
		//			ReAdjust = false;
		//		}
		//		else {
		//			ReAdjust = true;
		//		}

		//		//std::cout << imguiVeiwPort.x << "  " << imguiVeiwPort.y << std::endl;

		//		if (Engine::isPlay)
		//		{
		//			if (ReAdjust)
		//			{
		//				if (EditorSystem::_PlayInFullScreen && !isFullScreen)
		//				{
		//					isFullScreen = true;
		//					float imguiX = sprite->_pos.x / imguiVeiwPort.x;
		//					float imguiY = sprite->_pos.y / imguiVeiwPort.y;
		//					float imguiScaleX = (float)Engine::_ecs.getSystem<WindowSystem>()->getWindowWidth() / imguiVeiwPort.x;
		//					float imguiScaleY = (float)Engine::_ecs.getSystem<WindowSystem>()->getWindowHeight() / imguiVeiwPort.y;
		//					
		//					sprite->_pos.x = (float)Engine::_ecs.getSystem<WindowSystem>()->getWindowWidth() * imguiX;
		//					sprite->_pos.y = (float)Engine::_ecs.getSystem<WindowSystem>()->getWindowHeight() * imguiY;
		//					sprite->_scale.x *= imguiScaleX;
		//					sprite->_scale.y *= imguiScaleY;
		//				}
		//			}
		//		}
		//		else {
		//			if (isFullScreen)
		//			{
		//				isFullScreen = false;
		//				float imguiX = sprite->_pos.x / (float)Engine::_ecs.getSystem<WindowSystem>()->getWindowWidth();
		//				float imguiY = sprite->_pos.y / (float)Engine::_ecs.getSystem<WindowSystem>()->getWindowHeight();
		//				float imguiScaleX = imguiVeiwPort.x / (float)Engine::_ecs.getSystem<WindowSystem>()->getWindowWidth();
		//				float imguiScaleY = imguiVeiwPort.y / (float)Engine::_ecs.getSystem<WindowSystem>()->getWindowHeight();

		//				sprite->_pos.x = imguiVeiwPort.x * imguiX;
		//				sprite->_pos.y = imguiVeiwPort.y * imguiY;
		//				sprite->_scale.x *= imguiScaleX;
		//				sprite->_scale.y *= imguiScaleY;
		//			}
		//		}
		//	}
		//}


}


void UISystem::unload()
{
	ResetUI();
}

void UISystem::release()
{

}

void UISystem::HandlePauseMenu()
{
	std::shared_ptr<TransformCom> pTransform = Engine::_ecs.getComponent<TransformCom>(
		Engine::_ecs.getEntityID("Prefab_pause_pointer"));
	std::shared_ptr<SpriteCom> pointerSprite = Engine::_ecs.getComponent<SpriteCom>(
		Engine::_ecs.getEntityID("Prefab_pause_pointer"));
	std::shared_ptr<SpriteCom> logoSprite = Engine::_ecs.getComponent<SpriteCom>(
		Engine::_ecs.getEntityID("Prefab_pause_logo"));
	std::shared_ptr<SpriteCom> buttonsSprite = Engine::_ecs.getComponent<SpriteCom>(
		Engine::_ecs.getEntityID("Prefab_pause_buttons"));
	std::shared_ptr<SpriteCom> optionsSprite = Engine::_ecs.getComponent<SpriteCom>(
		Engine::_ecs.getEntityID("Prefab_pause_options"));
	std::shared_ptr<SpriteCom> yesnoSprite = Engine::_ecs.getComponent<SpriteCom>(
		Engine::_ecs.getEntityID("Prefab_pause_yesno"));
	std::shared_ptr<SpriteCom> volBarSprite = Engine::_ecs.getComponent<SpriteCom>(
		Engine::_ecs.getEntityID("Prefab_pause_vol_bar"));
	std::shared_ptr<SpriteCom> gammaBarSprite = Engine::_ecs.getComponent<SpriteCom>(
		Engine::_ecs.getEntityID("Prefab_pause_gamma_bar"));
	std::shared_ptr<SpriteCom> htpSprite = Engine::_ecs.getComponent<SpriteCom>(
		Engine::_ecs.getEntityID("Prefab_pause_htp"));

	if (Engine::_ecs.getSystem<WindowSystem>()->_isFullScreen)
		yesnoSprite->SpriteName = "main_menu_yes";
	else
		yesnoSprite->SpriteName = "main_menu_no";

	volBarSprite->barPercentage = Engine::_ecs.getSystem<AudioSystem>()->getMasterVolume() * 100 / 5.0f;
	gammaBarSprite->barPercentage = Engine::_ecs.getSystem<RenderSystem>()->getGammaVal() * 100 + 60;

	if (Inputs::get_Key_Triggered(GLFW_KEY_W) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_DPAD_UP) && canTriggerW_Key)
	{
		if (pauseState != 2 && pauseState != 3)
		{
			Engine::_ecs.getSystem<AudioSystem>()->PlayUISound("SFX_MenuSelect");
			if (pausePointer == 0)
				pausePointer = 3;
			else
				pausePointer--;
		}
		canTriggerW_Key = false;
	}
	else if (Inputs::get_Key_Triggered(GLFW_KEY_S) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_DPAD_DOWN) && canTriggerS_Key)
	{
		if (pauseState != 2 && pauseState != 3)
		{
			Engine::_ecs.getSystem<AudioSystem>()->PlayUISound("SFX_MenuSelect");
			if (pausePointer == 3)
				pausePointer = 0;
			else
				pausePointer++;
		}
		canTriggerS_Key = false;
	}

	if (pauseState == 1 || pauseState == 3)
	{
		if (Inputs::get_Key_Triggered(GLFW_KEY_A) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_DPAD_LEFT))
		{
			Engine::_ecs.getSystem<AudioSystem>()->PlayUISound("SFX_MenuSelect");
			if (pausePointer == 0)
			{
				if (getFullscreenState())
					setFullscreen(false);
				else
					setFullscreen(true);
			}
			else if (pausePointer == 1)
				Engine::_ecs.getSystem<AudioSystem>()->decreaseMasterVolume();
			else if (pausePointer == 2)
				Engine::_ecs.getSystem<RenderSystem>()->addGamma(-0.2f);
			else if (pausePointer == 5)
				pausePointer = 6;
		}
		else if (Inputs::get_Key_Triggered(GLFW_KEY_D) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_DPAD_RIGHT))
		{
			Engine::_ecs.getSystem<AudioSystem>()->PlayUISound("SFX_MenuSelect");
			if (pausePointer == 0)
			{
				if (getFullscreenState())
					setFullscreen(false);
				else
					setFullscreen(true);
			}
			else if (pausePointer == 1)
				Engine::_ecs.getSystem<AudioSystem>()->increaseMasterVolume();
			else if (pausePointer == 2)
				Engine::_ecs.getSystem<RenderSystem>()->addGamma(0.2f);
			else if (pausePointer == 6)
				pausePointer = 5;
		}
	}
	

	switch (pausePointer)
	{
	case 0:
		pTransform->pos.x = -158.84f;
		pTransform->pos.y = 3.210f;
		if (pauseState == 0)
			buttonsSprite->SpriteName = "pause_menu_1";
		else if (pauseState == 1)
			buttonsSprite->SpriteName = "main_menu_options_1";
		break;
	case 1:
		pTransform->pos.x = -158.84f;
		pTransform->pos.y = -9.46f;
		if (pauseState == 0)
			buttonsSprite->SpriteName = "pause_menu_2";
		else if (pauseState == 1)
			buttonsSprite->SpriteName = "main_menu_options_2";
		break;
	case 2:
		pTransform->pos.x = -158.84f;
		pTransform->pos.y = -22.13f;
		if (pauseState == 0)
			buttonsSprite->SpriteName = "pause_menu_3";
		else if (pauseState == 1)
			buttonsSprite->SpriteName = "main_menu_options_3";
		break;
	case 3:
		pTransform->pos.x = -158.84f;
		pTransform->pos.y = -34.8f;
		if (pauseState == 0)
			buttonsSprite->SpriteName = "pause_menu_4";
		else if (pauseState == 1)
			buttonsSprite->SpriteName = "main_menu_options";
		break;
	case 4:
		pTransform->pos.x = 135.0f;
		pTransform->pos.y = 94.0f;
		break;
	case 5:
		pTransform->pos.x = -109.49f;
		pTransform->pos.y = -34.65f;
		buttonsSprite->SpriteName = "are_you_sure_no";
		break;
	case 6:
		pTransform->pos.x = -154.5f;
		pTransform->pos.y = -34.65f;
		buttonsSprite->SpriteName = "are_you_sure_yes";
		break;
	}

	if (Inputs::get_Key_Triggered(GLFW_KEY_ENTER) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_A))
	{
		if (pausePointer == 0)
		{
			if (pauseState == 0)
			{
				Engine::_ecs.getSystem<AudioSystem>()->PlayUISound("SFX_MenuEnter");
				_isPaused = false;
				activateAfterPause();
				unloadPauseMenuUI();
			}
		}
		else if (pausePointer == 1)
		{
			if (pauseState == 0)
			{
				Engine::_ecs.getSystem<AudioSystem>()->PlayUISound("SFX_MenuEnter");
				pausePointer = 0;
				pauseState = 1;
				yesnoSprite->_drawSprite = true;
				volBarSprite->_drawSprite = true;
				gammaBarSprite->_drawSprite = true;
			}
		}
		else if (pausePointer == 2)
		{
			if (pauseState == 0)
			{
				Engine::_ecs.getSystem<AudioSystem>()->PlayUISound("SFX_MenuEnter");
				pauseState = 2;
				pausePointer = 4;
				logoSprite->_drawSprite = false;
				buttonsSprite->_drawSprite = false;
				htpSprite->_drawSprite = true;
			}
		}
		else if (pausePointer == 3)
		{
			if (pauseState == 0)
			{
				Engine::_ecs.getSystem<AudioSystem>()->PlayUISound("SFX_MenuEnter");
				pauseState = 3;
				pausePointer = 5;

			}
			else if (pauseState == 1)
			{
				Engine::_ecs.getSystem<AudioSystem>()->PlayUISound("SFX_MenuEnter");
				pausePointer = 1;
				pauseState = 0;
				buttonsSprite->_drawSprite = true;
				optionsSprite->_drawSprite = false;
				yesnoSprite->_drawSprite = false;
				volBarSprite->_drawSprite = false;
				gammaBarSprite->_drawSprite = false;
			}
		}
		else if (pausePointer == 4)
		{
			Engine::_ecs.getSystem<AudioSystem>()->PlayUISound("SFX_MenuEnter");
			pauseState = 0;
			pausePointer = 2;
			logoSprite->_drawSprite = true;
			buttonsSprite->_drawSprite = true;
			htpSprite->_drawSprite = false;
		}
		// no
		else if (pausePointer == 5)
		{
			Engine::_ecs.getSystem<AudioSystem>()->PlayUISound("SFX_MenuEnter");
			pauseState = 0;
			pausePointer = 3;
		}
		// yes
		else if (pausePointer == 6)
		{
			if (_playingCutscene)
				_playingCutscene = false;

			Engine::_ecs.getSystem<AudioSystem>()->PlayUISound("SFX_MenuEnter");
			pauseState = 0;
			_isPaused = false;
			activateAfterPause();
			unloadPauseMenuUI();
			setPrevState();
			setGameState_wF2B((int)GameState::DIGIPEN_LOGO);
		}
	}

	if (Inputs::get_Key_Triggered(GLFW_KEY_ESCAPE))
	{
		if (pauseState == 1)
		{
			pausePointer = 1;
			pauseState = 0;
			buttonsSprite->_drawSprite = true;
			optionsSprite->_drawSprite = false;
			yesnoSprite->_drawSprite = false;
			volBarSprite->_drawSprite = false;
			gammaBarSprite->_drawSprite = false;
		}	
		else if (pauseState == 2)
		{
			pauseState = 0;
			pausePointer = 2;
			logoSprite->_drawSprite = true;
			pointerSprite->_drawSprite = true;
			buttonsSprite->_drawSprite = true;
			htpSprite->_drawSprite = false;
		}
	}

	if (Inputs::get_KeyReleaseTrigger(GLFW_KEY_W) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_DPAD_UP))
		canTriggerW_Key = true;
	else if (Inputs::get_KeyReleaseTrigger(GLFW_KEY_S) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_DPAD_DOWN))
		canTriggerS_Key = true;
}

void UISystem::loadPauseMenuUI()
{
	PrefabID pauseLogo = Engine::_ecs.getPrefabID("Prefab_pause_logo");
	PrefabID buttons = Engine::_ecs.getPrefabID("Prefab_pause_buttons");
	PrefabID pointer = Engine::_ecs.getPrefabID("Prefab_pause_pointer");
	PrefabID options = Engine::_ecs.getPrefabID("Prefab_pause_options");
	PrefabID yesno = Engine::_ecs.getPrefabID("Prefab_pause_yesno");
	PrefabID volBar = Engine::_ecs.getPrefabID("Prefab_pause_vol_bar");
	PrefabID gammaBar = Engine::_ecs.getPrefabID("Prefab_pause_gamma_bar");
	PrefabID htp = Engine::_ecs.getPrefabID("Prefab_pause_htp");

	Engine::_ecs.instantiatePrefab(pauseLogo, Engine::_ecs.getPrefabName(pauseLogo));
	Engine::_ecs.instantiatePrefab(buttons, Engine::_ecs.getPrefabName(buttons));
	Engine::_ecs.instantiatePrefab(pointer, Engine::_ecs.getPrefabName(pointer));
	Engine::_ecs.instantiatePrefab(options, Engine::_ecs.getPrefabName(options));
	Engine::_ecs.instantiatePrefab(yesno, Engine::_ecs.getPrefabName(yesno));
	Engine::_ecs.instantiatePrefab(volBar, Engine::_ecs.getPrefabName(volBar));
	Engine::_ecs.instantiatePrefab(gammaBar, Engine::_ecs.getPrefabName(gammaBar));
	Engine::_ecs.instantiatePrefab(htp, Engine::_ecs.getPrefabName(htp));

	Engine::_ecs.getSystem<AudioSystem>()->PauseBGM("BG_Level1");
	Engine::_ecs.getSystem<AudioSystem>()->PauseBGM("BG_Level2");
	Engine::_ecs.getSystem<AudioSystem>()->PauseBGM("BG_Level3");

	//turn off minimap
	set_RenderSystem_Minimap(false);
}

void UISystem::unloadPauseMenuUI()
{
	std::vector<Entity> activeEntity = Engine::_ecs.getActiveEntities();
	for (auto active : activeEntity)
	{
		if (active._entityName.find("pause") != std::string::npos)
		{
			Engine::_ecs.destroyEntity(active._entityID);
		}
	}

	if (GameStateManager::_curState == (int)GameState::MAIN_MENU)
		Engine::_ecs.getSystem<AudioSystem>()->UnpauseBGM("BG_MainMenu");
	else if (GameStateManager::_curState == (int)GameState::LV1)
		Engine::_ecs.getSystem<AudioSystem>()->UnpauseBGM("BG_Level1");
	else if (GameStateManager::_curState == (int)GameState::LV2)
		Engine::_ecs.getSystem<AudioSystem>()->UnpauseBGM("BG_Level2");
	else if (GameStateManager::_curState == (int)GameState::LV3)
	{
		Engine::_ecs.getSystem<AudioSystem>()->UnpauseBGM("BG_Level3");
		RESOURCE->getChannel("Enemy_Dog")->setMute(false);
		RESOURCE->getChannel("SFX_ClownHorn")->setMute(false);
	}
	//turn on minimap
	set_RenderSystem_Minimap(true);
}

void UISystem::ResetUI()
{
	//reset health
	_healthPercent = 100;
	_threatPercent = 0;
	_powerPercent = 19.53f;
	_currentAmmoType = WeaponType_UI::Pistol;
	if (health_sprite == nullptr)
		findHealth();
	if (threat_sprite == nullptr)
		findThreat();
	if (power_sprite == nullptr || powerMax_sprite == nullptr)
		findPower();
	if (pistol_barHealth == nullptr ||
		laser_barHealth == nullptr ||
		rocket_barHealth == nullptr
		)
		findGeneratorHealth();
	else
	{
		pistol_barHealth->barPercentage = 100.0f;
		laser_barHealth->barPercentage = 100.0f;
		rocket_barHealth->barPercentage = 100.0f;
	}
}

void UISystem::resizeWindow()
{
	InitalWindowSize.x = (float)Engine::_ecs.getSystem<WindowSystem>()->getWindowWidth();
	InitalWindowSize.y = (float)Engine::_ecs.getSystem<WindowSystem>()->getWindowHeight();
}

bool UISystem::addToList(std::shared_ptr<UICom> cPtr, EntityID eId)
{
	if (eId != UINT_MAX && cPtr != nullptr)
	{
		static uint ID = 0;
		eId = ID++;
		_logics.insert(std::make_pair(eId, cPtr));
		return true;
	}

	return false;
}


void UISystem::increaseHealth(float amount)
{
	_healthPercent += amount;
	if (_healthPercent > 100)
		_healthPercent = 100;
	health_sprite->barPercentage = _healthPercent;
}

void UISystem::decreaseHealth(float amount)
{
	_healthPercent -= amount;
	if (_healthPercent < 0)
		_healthPercent = 0;
	health_sprite->barPercentage = _healthPercent;
}

void UISystem::setHealth(float amount)
{
	health_sprite->barPercentage = _healthPercent = amount;
}

void UISystem::increaseThreat(float amount)
{
	_threatPercent -= amount;
	if (_threatPercent > 100)
		_threatPercent = 100;
	threat_sprite->barPercentage = _threatPercent;
}

void UISystem::decreaseThreat(float amount)
{
	_threatPercent -= amount;
	if (_threatPercent < 0)
		_threatPercent = 0;
	threat_sprite->barPercentage = _threatPercent;
}

void UISystem::setThreat(float amount)
{
	threat_sprite->barPercentage = _threatPercent = amount;
}

void UISystem::findHealth()
{
	for (auto& entity : _entities)
	{
		auto tags = Engine::_ecs.getEntityTag(entity);

		if (tags.find("U_Health_Colour") != tags.end())
		{
			health_sprite = Engine::_ecs.getComponent<SpriteCom>(entity);
			health_sprite->barXAmount = (float)bar_XOffsetAmount;
		}
	}
}

void UISystem::findPower()
{
	for (auto& entity : _entities)
	{
		auto tags = Engine::_ecs.getEntityTag(entity);

		if (tags.find("U_Power_Colour") != tags.end())
		{
			power_sprite = Engine::_ecs.getComponent<SpriteCom>(entity);
			power_sprite->barPercentage = _powerPercent;
		}
		else if (tags.find("U_Power_Max") != tags.end())
		{
			powerMax_sprite = Engine::_ecs.getComponent<SpriteCom>(entity);
			powerMax_sprite->_drawSprite = false;
		}
	}
}

void UISystem::findThreat()
{
	for (auto& entity : _entities)
	{
		auto tags = Engine::_ecs.getEntityTag(entity);

		if (tags.find("U_Threat_Color") != tags.end())
		{
			threat_sprite = Engine::_ecs.getComponent<SpriteCom>(entity);
		}
	}
}

void UISystem::increasePower(float amount)
{
	_powerPercent += amount;
	if (_powerPercent >= 75.1f)
		_powerPercent = 75.1f;
	power_sprite->barPercentage = _powerPercent;
	if (_powerPercent >= 75.1f)
		powerMax_sprite->_drawSprite = true;
}

void UISystem::decreasePower(float amount)
{
	_powerPercent -= amount;
	if (_powerPercent <= 19.53f)
		_powerPercent = 19.53f;
	power_sprite->barPercentage = _powerPercent;
	if (_powerPercent < 75.1f)
		powerMax_sprite->_drawSprite = false;
}

void UISystem::setPower(float amount)
{
	powerMax_sprite->barPercentage = _powerPercent = amount;
	if (_powerPercent < 100.0f)
		powerMax_sprite->_drawSprite = false;
	else
		powerMax_sprite->_drawSprite = true;
}

void UISystem::slowlyDecreasePower(float dt)
{
	if (powerMax_sprite->barPercentage > 0.0f)
	{
		powerMax_sprite->barPercentage -= dt;
	}
}

void UISystem::swapToWeapon(int wType)
{
	WeaponType_UI weapType = (WeaponType_UI)wType;
	//get 
	if (weaponList.empty() || weaponList.size() != 3)
		findWeapons();

	if (weapType != _currentAmmoType && weaponList.size() == 3)
	{
		//this is just swapping the draw order for the sprites
		std::swap(weaponList[(WeaponType_UI)_currentAmmoType]->pos.z, weaponList[weapType]->pos.z);
		_currentAmmoType = weapType;
	}
}

void UISystem::findGeneratorHealth()
{
	for (auto& entity : _entities)
	{
		auto tags = Engine::_ecs.getEntityTag(entity);

		if (tags.find("UI_Cage_Pistol_Health") != tags.end())
		{
			pistol_barHealth = Engine::_ecs.getComponent<SpriteCom>(entity);
			pistol_barHealth->barPercentage = 100.0f;
		}
		else if (tags.find("UI_Cage_Laser_Health") != tags.end())
		{
			laser_barHealth = Engine::_ecs.getComponent<SpriteCom>(entity);
			laser_barHealth->barPercentage = 100.0f;
		}
		else if (tags.find("UI_Cage_Rocket_Health") != tags.end())
		{
			rocket_barHealth = Engine::_ecs.getComponent<SpriteCom>(entity);
			rocket_barHealth->barPercentage = 100.0f;
		}
	}
}

void UISystem::increasePistolHealth(float amount)
{
	pistol_barHealth->barPercentage += amount;
}

void UISystem::decreasePistolHealth(float amount)
{
	pistol_barHealth->barPercentage -= amount;
}

void UISystem::increaseLaserHealth(float amount)
{
	laser_barHealth->barPercentage += amount;
}

void UISystem::decreaseLaserHealth(float amount)
{
	laser_barHealth->barPercentage -= amount;
}

void UISystem::increaseRocketHealth(float amount)
{
	rocket_barHealth->barPercentage += amount;
}

void UISystem::decreaseRocketHealth(float amount)
{
	rocket_barHealth->barPercentage -= amount;
}

void UISystem::activateAfterPause()
{
	Engine::_ecs.getSystem<ScriptingSystem>()->_isActive = true;
	Engine::_ecs.getSystem<PhysicsSystem>()->_isActive = true;
	Engine::_ecs.getSystem<AnimationSystem>()->_isActive = true;
	Engine::_ecs.getSystem<ParticleSystem>()->_isActive = true;
	Engine::_ecs.getSystem<CameraSystem>()->_isActive = true;
}

std::map<WeaponType_UI, std::shared_ptr<TransformCom>> UISystem::findWeapons()
{
	std::map<WeaponType_UI, std::shared_ptr<TransformCom>> weaponMap;

	for (auto& entity : _entities)
	{
		auto tags = Engine::_ecs.getEntityTag(entity);

		if (tags.find("Weapon_Pistol") != tags.end())
			weaponMap[WeaponType_UI::Pistol] = Engine::_ecs.getComponent<TransformCom>(entity);
		else if (tags.find("Weapon_Laser") != tags.end())
			weaponMap[WeaponType_UI::Laser] = Engine::_ecs.getComponent<TransformCom>(entity);
		else if (tags.find("Weapon_Rocket") != tags.end())
			weaponMap[WeaponType_UI::Rocket] = Engine::_ecs.getComponent<TransformCom>(entity);
		if (weaponMap.size() == 3)
			break;
	}

	return weaponMap;
}

int UISystem::getGameState()
{
	return GameStateManager::_curState;
}

void UISystem::setGameState(int gameState)
{
	GameStateManager::_curState = gameState;
}

void UISystem::setGameState_wF2B(int gameState)
{
	GameStateManager::_nState = gameState;
	_changingScene = true;
	int screenID = Engine::_ecs.getPrefabID("Prefab_simple_FadeIn_Screen");
	Engine::_ecs.instantiatePrefab(screenID, "simple_FadeIn_Screen");
	switch ((GameState)gameState)
	{
	case(GameState::LV1):
	case(GameState::LV2):
	case(GameState::LV3):
			toggleCutscene(false);
			break;
	default:
		break;
	}
}

int UISystem::getNextState()
{
	return GameStateManager::_nState;
}

bool UISystem::getChangingSceneState()
{
	return _changingScene;
}

void UISystem::setChangingSceneState(bool val)
{
	_changingScene = val;
}

void UISystem::setPrevState()
{
	prevState = GameStateManager::_curState;
}

void UISystem::ResetLevel()
{
	setGameState_wF2B(GameStateManager::_nState);
}

void UISystem::goToPrevState()
{
	GameStateManager::_curState = prevState;
}

int UISystem::getPrevState()
{
	return prevState;
}

void UISystem::toggleCutscene(bool val)
{
	_playingCutscene = val;
	if (_playingCutscene)
	{
		//notify graphics
		set_RenderSystem_Minimap(false);
		Engine::_ecs.getSystem<RenderSystem>()->toggleCutscene(true);
		//notify physics?
		Engine::_ecs.getSystem<PhysicsSystem>()->_isActive = false;
	}
	else
	{
		//notify graphics
		set_RenderSystem_Minimap(true);
		Engine::_ecs.getSystem<RenderSystem>()->toggleCutscene(false);
		//notify physics?
		Engine::_ecs.getSystem<PhysicsSystem>()->_isActive = true;

	}
}

bool UISystem::getCutsceneBool()
{
	return _playingCutscene;
}

bool UISystem::get_RenderSystem_Minimap()
{
	//get the state of minimap in rendersystem
	rendersystem_Minimap_State = Engine::_ecs.getSystem<RenderSystem>()->getMinimap();
	return rendersystem_Minimap_State;
}

void UISystem::set_RenderSystem_Minimap(bool val)
{
	//doing this cos the current UI System with how the gamestate is done
	//calls getsystem many times every single frame
	// so to reduce calling ecs getsystem many times 
	if (rendersystem_Minimap_State != val)
	{
		rendersystem_Minimap_State = val;
		Engine::_ecs.getSystem<RenderSystem>()->toggleMinimap(val);
	}

}

void UISystem::setFullscreen(bool b)
{
	WindowSystem::_isFullScreen = b;
	Engine::_ecs.getSystem<WindowSystem>()->setFullScreen();
}

bool UISystem::getFullscreenState()
{
	return WindowSystem::_isFullScreen;
}

void UISystem::levelSelectMenuLogic()
{
	std::shared_ptr<TransformCom> transformLv1 = nullptr;
	std::shared_ptr<TransformCom> transformLv2 = nullptr;
	std::shared_ptr<TransformCom> transformLv3 = nullptr;
	std::shared_ptr<TransformCom> transformPointer = nullptr;
	std::shared_ptr<SpriteCom> spriteLv1 = nullptr;
	std::shared_ptr<SpriteCom> spriteLv2 = nullptr;
	std::shared_ptr<SpriteCom> spriteLv3 = nullptr;
	std::shared_ptr<SpriteCom> backsprite = nullptr;

	transformLv1 = Engine::_ecs.getComponent<TransformCom>(
		Engine::_ecs.getEntityID("Level1Map"));
	transformLv2 = Engine::_ecs.getComponent<TransformCom>(
		Engine::_ecs.getEntityID("Level2Map"));
	transformLv3 = Engine::_ecs.getComponent<TransformCom>(
		Engine::_ecs.getEntityID("Level3Map"));
	transformPointer = Engine::_ecs.getComponent<TransformCom>(
		Engine::_ecs.getEntityID("LevelPointer"));
	spriteLv1 = Engine::_ecs.getComponent<SpriteCom>(
		Engine::_ecs.getEntityID("Level1Map"));	
	spriteLv2 = Engine::_ecs.getComponent<SpriteCom>(
		Engine::_ecs.getEntityID("Level2Map"));	
	spriteLv3 = Engine::_ecs.getComponent<SpriteCom>(
		Engine::_ecs.getEntityID("Level3Map"));
	backsprite = Engine::_ecs.getComponent<SpriteCom>(
		Engine::_ecs.getEntityID("BackToMainMenu")); 
	static int counter = 0;
	
	if ((Inputs::get_Key_Triggered(GLFW_KEY_A) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_DPAD_LEFT)) && canTriggerA_Key)
	{
		Engine::_ecs.getSystem<AudioSystem>()->PlayUISound("SFX_MenuSelect");
		--_menuPointer;
		if (_menuPointer < 0)
			_menuPointer = 3;
		canTriggerA_Key = false;
	}
	else if ((Inputs::get_Key_Triggered(GLFW_KEY_D) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_DPAD_RIGHT)) && canTriggerD_Key)
	{
		Engine::_ecs.getSystem<AudioSystem>()->PlayUISound("SFX_MenuSelect");
		++_menuPointer;
		if (_menuPointer > 3)
			_menuPointer = 0;
		canTriggerD_Key = false;
	}

	if (transformLv1 && transformLv2 && transformLv3)
	{
		if (_menuPointer == 0)
		{
			spriteLv1->setName("map_area1");

			spriteLv2->setName("map_unselect_area2");
			transformLv2->pos.z = 0.0f;

			spriteLv3->setName("map_unselect_area3");
			transformLv3->pos.x = 0.0f;
			transformLv3->pos.y = 0.0f;

			transformPointer->pos.x = -89.0f;
			transformPointer->pos.y = -20.0f;

			backsprite->setName("level_select_back2");
		}
		else if (_menuPointer == 1)
		{
			spriteLv1->setName("map_unselect_area1");

			spriteLv2->setName("map_area2");
			transformLv2->pos.z = 0.1f;

			spriteLv3->setName("map_unselect_area3");
			transformLv3->pos.x = 0.0f;
			transformLv3->pos.y = 0.0f;

			transformPointer->pos.x = 26.5f;
			transformPointer->pos.y = 41.0f;
			transformPointer->pos.z = 0.1f;

			backsprite->setName("level_select_back2");
		}
		else if (_menuPointer == 2)
		{
			spriteLv1->setName("map_unselect_area1");

			spriteLv2->setName("map_unselect_area2");
			transformLv2->pos.z = 0.0f;

			spriteLv3->setName("map_area3");
			transformLv3->pos.x = 0.0f;
			transformLv3->pos.y = 0.0f;

			transformPointer->pos.x = 89.0f;
			transformPointer->pos.y = -41.0f;

			backsprite->setName("level_select_back2");
		}
		else if (_menuPointer == 3)
		{
			spriteLv1->setName("map_unselect_area1");

			spriteLv2->setName("map_unselect_area2");
			transformLv2->pos.z = 0.0f;

			spriteLv3->setName("map_unselect_area3");
			transformLv3->pos.x = 0.0f;
			transformLv3->pos.y = 0.0f;

			transformPointer->pos.x = 135.0f;
			transformPointer->pos.y = 94.0f;

			backsprite->setName("level_select_back1");
		}
	}

	if (Inputs::get_Key_Triggered(GLFW_KEY_ENTER) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_A))
	{
		Engine::_ecs.getSystem<AudioSystem>()->PlayUISound("SFX_MenuEnter");
		if (_menuPointer == 0)
		{
			_changedSkybox = false;
			setGameState_wF2B((int)GameState::LOADING_MENULV1);
		}
		else if (_menuPointer == 1)
		{
			_changedSkybox = false;
			setGameState_wF2B((int)GameState::LOADING_MENULV2);
		}
		else if (_menuPointer == 2)
		{
			_changedSkybox = false;
			setGameState_wF2B((int)GameState::LOADING_MENULV3);
		}
		else if (_menuPointer == 3)
			GameStateManager::_curState = (int)GameState::MAIN_MENU;
	}

	if (Inputs::get_KeyReleaseTrigger(GLFW_KEY_A) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_DPAD_LEFT))
		canTriggerA_Key = true;

	if (Inputs::get_KeyReleaseTrigger(GLFW_KEY_D) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_DPAD_RIGHT))
		canTriggerD_Key = true;

	if(Inputs::get_KeyReleaseTrigger(GLFW_KEY_ESCAPE))
		GameStateManager::_curState = (int)GameState::MAIN_MENU;
}

void UISystem::OptionsLogic()
{
	std::shared_ptr<TransformCom> transform = nullptr;
	transform = Engine::_ecs.getComponent<TransformCom>(
		Engine::_ecs.getEntityID("Prefab_Pointer"));

	if (transform)
	{
		if (Inputs::get_Key_Triggered(GLFW_KEY_W) || Inputs::get_GamePatButton(GLFW_GAMEPAD_BUTTON_RIGHT_THUMB))
		{
			Engine::_ecs.getSystem<AudioSystem>()->PlayUISound("SFX_MenuSelect");

			if (_optionsPointer == 0)
				_optionsPointer = 2;
			else
				--_optionsPointer;
		}
		else if (Inputs::get_Key_Triggered(GLFW_KEY_S) || Inputs::get_GamePatButton(GLFW_GAMEPAD_BUTTON_DPAD_RIGHT))
		{
			Engine::_ecs.getSystem<AudioSystem>()->PlayUISound("SFX_MenuSelect");
			if (_optionsPointer == 2)
				_optionsPointer = 0;
			else
				++_optionsPointer;
		}

		// 0 - volume
		// 1 - gamma
		// 2 - fullscreen

		/*if (_button == 0)
			transform->pos.y = -2.0f;
		else if (_button == 1)
			transform->pos.y = -24.0f;
		else
			transform->pos.y = -46.0f;*/

		// volume
		if (_optionsPointer == 0)
		{
			if (Inputs::get_Key_Triggered(GLFW_KEY_A))
			{
				Engine::_ecs.getSystem<AudioSystem>()->decreaseMasterVolume();
			}

			else if (Inputs::get_Key_Triggered(GLFW_KEY_D))
			{
				Engine::_ecs.getSystem<AudioSystem>()->increaseMasterVolume();
			}
		}
	}
}
