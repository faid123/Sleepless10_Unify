#include "Engine.h"

// graphic
#include "Graphics/Window/Window/Window.h"
#include "Graphics/Graphics/Graphics.h"
#include "Graphics/RenderSystem/RenderSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Camera/CameraSystem/CameraSystem.h"
#include "Particles/ParticleSystem.h"
#include "Audio/AudioSystem.h"
#include "Input.h"
#include "UserIntered/UI_System.h"
#include "Animation/AnimationSystem.h"

// Components
#include "Physics/RigidBodyComponent.h"
#include "Camera/CameraCom.h"
#include "Graphics/TransformCom.h"
#include "Graphics/LightCom.h"
#include "Graphics/MaterialCom.h"
#include "Particles/ParticleComponent.h"
#include "Particles/EmitterComponent.h"
#include "Audio/AudioSrcCom.h"
#include "Animation/AnimationCom.h"
#include "Graphics/SpriteCom.h"
#include "UserIntered/UICom.h"
#include "Graphics/SpriteCom.h"
#include "Graphics/TextCom.h"
#include "Animation/AnimationCom.h"

#include "Serializer/ISerializer.h"
#include "ECS/Serializer/SerializerSystem.h"
// editor
#include "Editor/EditorSystem/EditorSystem.h"

// scripting system
#include "Logic/Lua/ScriptingSystem.h"
#include "Logic/Lua/LuaScriptCom.h"

#include "Event/Testevent.h"
#include <ResourceManager/ResourceManager.h>

#include "GameState.h"
#include <Graphics/Window/Time/Time.h>

sol::state ScriptingSystem::lua;	// The Lua State
ECS Engine::_ecs{};
bool Engine::_isPlay = false;

#ifdef SHOW_IMGUI
bool Engine::_isGameMode = false;
int GameStateManager::_curState = (int)GameState::NONE;
int GameStateManager::_preState = (int)GameState::NONE;
int GameStateManager::_nState = (int)GameState::NONE;
#else
bool Engine::_isGameMode = true;
int GameStateManager::_curState = (int)GameState::LOADING_START;
int GameStateManager::_preState = (int)GameState::LOADING_START;
int GameStateManager::_nState = (int)GameState::NONE;
#endif
bool Engine::_justQuit = false;


void Engine::init()
{
	_ecs.init();
	//graphic
	auto windowSystem = _ecs.registerSystem<WindowSystem>();
	auto serializerSystem = _ecs.registerSystem<SerializerSystem>();
	auto scriptingSystem = _ecs.registerSystem<ScriptingSystem>();
	auto physicsSystem = _ecs.registerSystem<PhysicsSystem>();
	auto audioSystem = _ecs.registerSystem<AudioSystem>();
	//auto testSystem = _ecs.registerSystem<TestSystem>();
	auto cameraSystem = _ecs.registerSystem<CameraSystem>();
	auto particleSystem = _ecs.registerSystem<ParticleSystem>();
	auto resourceSystem = _ecs.registerSystem<ResourceManager>();
#ifdef SHOW_IMGUI
	auto editorSystem = _ecs.registerSystem<EditorSystem>();
#endif
	auto animation = _ecs.registerSystem<AnimationSystem>();
	auto renderSystem = _ecs.registerSystem<RenderSystem>();
	auto uiSystem = _ecs.registerSystem<UISystem>();


	_ecs.registerNewComponent<TransformCom>();
	_ecs.registerNewComponent<CameraCom>();
	_ecs.registerNewComponent<RigidStaticCom>();
	_ecs.registerNewComponent<RigidDynamicCom>();
	_ecs.registerNewComponent<LightCom>();
	_ecs.registerNewComponent<MaterialCom>();
	_ecs.registerNewComponent<LuaScriptCom>();
	_ecs.registerNewComponent<ParticleCom>();
	_ecs.registerNewComponent<EmitterCom>();
	_ecs.registerNewComponent<AudioSrcCom>();
	_ecs.registerNewComponent<AnimationCom>();
	_ecs.registerNewComponent<SpriteCom>();
	_ecs.registerNewComponent<UICom>();
	_ecs.registerNewComponent<TextCom>();

#ifdef SHOW_IMGUI
#else
	//Engine::_ecs.getSystem<WindowSystem>()->setViewableCustor();
	//Engine::_ecs.getSystem<SerializerSystem>()->setFileName("NewMainMenu(w civilians).scene");
	Engine::_ecs.getSystem<SerializerSystem>()->setFileName("Loading_StartScreen.scene");
	WindowSystem::_isFullScreen = true;
#endif
	_ecs.systemInit();
#ifdef SHOW_IMGUI
#else
	Engine::_ecs.getSystem<WindowSystem>()->setViewableCustor();
#endif
}

void Engine::load()
{
	_ecs.systemLoad();
#ifdef SHOW_IMGUI
#else
	//Engine::_ecs.getSystem<EditorSystem>()->_isActive = false;
	_isPlay = true;
	Engine::_ecs.getSystem<CameraSystem>()->_camType = CameraSystem::CameraType::PLAYER;
	Engine::_ecs.getSystem<ScriptingSystem>()->LoadSceneScripts();
	Engine::_ecs.getSystem<RenderSystem>()->toggle_renderFramebuffer(!_isPlay);
#endif


	GameStateManager::_preState = GameStateManager::_curState;
}

void Engine::update()
{
	float dt = Time::DELTA_TIME;
	/*std::vector<std::string> names;
	std::vector<std::shared_ptr<Component>> ptr;
	_ecs.getEntityComponentPtrAndName(1, names, ptr);*/

	if (_isGameMode && _once)
	{
		Engine::_ecs.getSystem<ScriptingSystem>()->LoadSceneScripts();
		_once = false;
	}

#ifdef SHOW_IMGUI
	if (EditorSystem::_PlayInFullScreen)
	{
		Engine::_ecs.getSystem<EditorSystem>()->_isActive = !_isPlay;
		Engine::_ecs.getSystem<RenderSystem>()->toggle_renderFramebuffer(!_isPlay);
	}
#endif

	auto windowSys = _ecs.getSystem<WindowSystem>();
	//_ecs.getSystem<PhysicsSystem>()->_isActive = isPlay;
	_ecs.getSystem<ScriptingSystem>()->_isActive = _isPlay;

	Time::fixedDeltaTime();
	glfwPollEvents();
	_ecs.systemUpdate(dt);
	//glfwSwapBuffers(windowSys->getWindowPtr());

	//if (Fixed_accumulated_Time >= fixed_delta_time)
	//{
	//	_ecs.systemFixedUpdate(static_cast<float>(fixed_delta_time), currentNumberofSteps);
	//	Fixed_accumulated_Time = 0.0;
	//}
	//else
	//{
	//	float temp = 1.0f / 60.0f;
	//	//std::cout << "TIME: " << Fixed_accumulated_Time << std::endl;
	//	Fixed_accumulated_Time += dt;
	//}

#ifdef SHOW_IMGUI
	if (_isPlay && EditorSystem::_PlayInFullScreen)
	{
		if (Inputs::get_Key_Triggered(GLFW_KEY_ESCAPE) && !_isGameMode)
		{
			Engine::_ecs.getSystem<SerializerSystem>()->callReset();
		}
	}
	if (Inputs::get_Key_Triggered(GLFW_KEY_ESCAPE)
		&& Inputs::get_Key(GLFW_KEY_LEFT_SHIFT) && _isGameMode && _isPlay)
	{
		auto RENDERSYSTEM = Engine::_ecs.getSystem<RenderSystem>();
		if (RENDERSYSTEM->getPlayVideo())
		{
			RENDERSYSTEM->closeVideoNow();
		}
		Engine::_ecs.getSystem<SerializerSystem>()->callReset();
	}
#else
	if (Inputs::get_Key_Triggered(GLFW_KEY_ESCAPE)
		&& Inputs::get_Key(GLFW_KEY_LEFT_SHIFT))
		_justQuit = true;
#endif
	// for pause
	if (_isPlay && _isGameMode)
	{
		if (Inputs::get_KeyReleaseTrigger(GLFW_KEY_ESCAPE))
			canTriggerEsc = true;

		if ((Inputs::get_Key_Triggered(GLFW_KEY_ESCAPE) || Inputs::get_GamePatButton_Trigger(GLFW_GAMEPAD_BUTTON_START)))
		{
			if (GameStateManager::_curState != (int)GameState::MAIN_MENU
				&& GameStateManager::_curState != (int)GameState::GAMEOVER
				&& GameStateManager::_curState != (int)GameState::LEVEL_SELECT
				&& GameStateManager::_curState != (int)GameState::DIGIPEN_LOGO
				&& GameStateManager::_curState != (int)GameState::LOADING_START
				&& GameStateManager::_curState != (int)GameState::FMOD_LOGO
				&& GameStateManager::_curState != (int)GameState::TEAM_LOGO
				&& !Engine::_ecs.getSystem<UISystem>()->_isPaused
				&& !Engine::_ecs.getSystem<UISystem>()->_isPaused
				&& !Engine::_ecs.getSystem<UISystem>()->_playingCutscene
				&& GameStateManager::_curState != (int)GameState::LOADING_MENULV1
				&& GameStateManager::_curState != (int)GameState::LOADING_MENULV2
				&& GameStateManager::_curState != (int)GameState::LOADING_MENULV3)
			{
				Engine::_ecs.getSystem<UISystem>()->pausePointer = 0;
				//GameStateManager::_nState = GameStateManager::_curState;
				Engine::_ecs.getSystem<UISystem>()->loadPauseMenuUI();
				//GameStateManager::_curState = (int)GameState::PAUSE_MENU;
				//_isPlay = false;
				Engine::_ecs.getSystem<UISystem>()->_isPaused = true;
				Engine::_ecs.getSystem<ScriptingSystem>()->_isActive = false;
				Engine::_ecs.getSystem<PhysicsSystem>()->_isActive = false;
				Engine::_ecs.getSystem<AnimationSystem>()->_isActive = false;
				Engine::_ecs.getSystem<ParticleSystem>()->_isActive = false;
				Engine::_ecs.getSystem<CameraSystem>()->_isActive = false;
//#ifdef SHOW_IMGUI
//				Engine::_ecs.getSystem<WindowSystem>()->setViewableCustor();
//#endif
			}
			else if (Engine::_ecs.getSystem<UISystem>()->_isPaused && Engine::_ecs.getSystem<UISystem>()->pauseState == 0)
			{
//#ifdef SHOW_IMGUI
//				Engine::_ecs.getSystem<WindowSystem>()->setViewableCustor();
//#endif
				Engine::_ecs.getSystem<UISystem>()->_isPaused = false;
				Engine::_ecs.getSystem<UISystem>()->activateAfterPause();
				Engine::_ecs.getSystem<UISystem>()->unloadPauseMenuUI();
			}
			canTriggerEsc = false;
		}
	}
	
	// changing gamestate lv
	if (_isGameMode)
	{
		if (Inputs::get_Key_Triggered(GLFW_KEY_7) && 
			((GameStateManager::_curState != (int)GameState::LOADING_MENULV1) &&
			 (GameStateManager::_curState != (int)GameState::LOADING_MENULV2) && 
			 (GameStateManager::_curState != (int)GameState::LOADING_MENULV3) && 
			 (GameStateManager::_curState != (int)GameState::LOADING_START))
			)
			GameStateManager::_curState = (int)GameState::LOADING_MENULV1;
		if (Inputs::get_Key_Triggered(GLFW_KEY_8) && 
			((GameStateManager::_curState != (int)GameState::LOADING_MENULV1) &&
			 (GameStateManager::_curState != (int)GameState::LOADING_MENULV2) && 
			 (GameStateManager::_curState != (int)GameState::LOADING_MENULV3) && 
			 (GameStateManager::_curState != (int)GameState::LOADING_START)))
		{
			if (GameStateManager::_curState == (int)GameState::LV1)
			{
				//skip the cutscene
				Engine::_ecs.getSystem<UISystem>()->toggleCutscene(false);
				Engine::_ecs.getSystem<RenderSystem>()->setVideoStart(false);
				Engine::_ecs.getSystem<RenderSystem>()->setPlayVideo(false);
			}
			GameStateManager::_curState = (int)GameState::LOADING_MENULV2;
		}

		if (Inputs::get_Key_Triggered(GLFW_KEY_9) &&  
			((GameStateManager::_curState != (int)GameState::LOADING_MENULV1) &&
			 (GameStateManager::_curState != (int)GameState::LOADING_MENULV2) && 
			 (GameStateManager::_curState != (int)GameState::LOADING_MENULV3) && 
			 (GameStateManager::_curState != (int)GameState::LOADING_START)))
		{
			if (GameStateManager::_curState == (int)GameState::LV1)
			{
				//skip the cutscene
				Engine::_ecs.getSystem<UISystem>()->toggleCutscene(false);
				Engine::_ecs.getSystem<RenderSystem>()->setVideoStart(false);
				Engine::_ecs.getSystem<RenderSystem>()->setPlayVideo(false);
			}
			GameStateManager::_curState = (int)GameState::LOADING_MENULV3;
		}
	}
	_ecs.entityToDestroyUpdate();
}

void Engine::unload()
{
	_ecs.systemUnload();
	_once = true;
}

void Engine::release()
{
	//graphic_static.exit();
	_ecs.systemRelease();
}

bool Engine::isRestarting()
{
	if (_isGameMode && (GameStateManager::_curState != GameStateManager::_preState))
	{
		Engine::_ecs.getSystem<SerializerSystem>()->setIsLoaded(true);
		auto serializer = Engine::_ecs.getSystem<SerializerSystem>();
		switch ((GameState)GameStateManager::_curState)
		{
		case GameState::MAIN_MENU:
			_ecs.getSystem<UISystem>()->_playingCutscene = false;
			serializer->setFileName("NewMainMenu(w civilians).scene");
			Engine::_ecs.getSystem<AudioSystem>()->StopBGM("BG_MainLoading");
			//Engine::_ecs.getSystem<AudioSystem>()->StopAllBGM();
			Engine::_ecs.getSystem<AudioSystem>()->PlayBGM("BG_MainMenu");
			break;
		case GameState::LV1:
			serializer->setFileName("Final_Level1.scene");
			//Engine::_ecs.getSystem<AudioSystem>()->StopAllSounds();
			Engine::_ecs.getSystem<AudioSystem>()->StopAllBGM();
			Engine::_ecs.getSystem<AudioSystem>()->PlayBGM("BG_Level1");
			//Engine::_ecs.getSystem<AudioSystem>()->StopBGM("BG_MainMenu");
			//RESOURCE->getChannel("BG_Level1")->setVolume(0.2f);
			break;
		case GameState::LV2:
			serializer->setFileName("Final_Level2.scene");
			//Engine::_ecs.getSystem<AudioSystem>()->StopAllSounds();
			Engine::_ecs.getSystem<AudioSystem>()->StopAllBGM();
			Engine::_ecs.getSystem<AudioSystem>()->PlayBGM("BG_Level2");
			//Engine::_ecs.getSystem<AudioSystem>()->StopBGM("BG_MainMenu");
			//RESOURCE->getChannel("BG_Level2")->setVolume(0.2f);
			break;
		case GameState::LV3:
			serializer->setFileName("Final_Level3.scene");

			//if (!Engine::_ecs.getSystem<UISystem>()->_playingCutscene)
				//Engine::_ecs.getSystem<AudioSystem>()->StopAllSounds();
			Engine::_ecs.getSystem<AudioSystem>()->StopAllBGM();
			Engine::_ecs.getSystem<AudioSystem>()->PlayBGM("BG_Level3");
			//Engine::_ecs.getSystem<AudioSystem>()->StopBGM("BG_MainMenu");
				//RESOURCE->getChannel("BG_Level3")->setVolume(0.2f);
			break;
		case GameState::PAUSE_MENU:
			//serializer->setFileName("PauseMenu.scene");
			//Engine::_ecs.getSystem<UISystem>()->loadPauseMenuUI();
			/*Engine::_ecs.getSystem<AudioSystem>()->PauseBGM("Game_BG");
			Engine::_ecs.getSystem<AudioSystem>()->PauseBGM("BG_Level2");*/
			break;
		case GameState::LOADING_MENULV1:
			Engine::_ecs.getSystem<AudioSystem>()->StopAllSounds();
			serializer->setFileName("Level1_LoadingScreen.scene");
			break;
		case GameState::LOADING_MENULV2:
			Engine::_ecs.getSystem<AudioSystem>()->StopAllSounds();
			serializer->setFileName("Level2_LoadingScreen.scene");
			break;
		case GameState::LOADING_MENULV3:
			Engine::_ecs.getSystem<AudioSystem>()->StopAllSounds();
			serializer->setFileName("Level3_LoadingScreen.scene");
			break;
		case GameState::LOADING_START:
			Engine::_ecs.getSystem<AudioSystem>()->StopAllSounds();
			serializer->setFileName("Loading_StartScreen.scene");
			break;
		case GameState::DIGIPEN_LOGO:
			serializer->setFileName("DigipenLogo_StartScreen.scene");
			//Engine::_ecs.getSystem<AudioSystem>()->PlayBGM(" somethinjgmusicnsomething "); //play main menu music or something
			Engine::_ecs.getSystem<AudioSystem>()->StopAllSounds();
			Engine::_ecs.getSystem<AudioSystem>()->PlayBGM("BG_MainLoading");
			break;
		case GameState::FMOD_LOGO:
			serializer->setFileName("FModLogo_StartScreen.scene");
			break;
		case GameState::TEAM_LOGO:
			serializer->setFileName("TeamLogo_StartScreen.scene");
			break;
		case GameState::GAMEOVER:
			serializer->setFileName("GameOver.scene");
			Engine::_ecs.getSystem<AudioSystem>()->StopAllSoundsWhenDead();
			break;
		case GameState::VICTORY:
			serializer->setFileName("Win.scene");
			break;
		case GameState::LEVEL_SELECT:
			serializer->setFileName("LevelSelect.scene");
			break;
		case GameState::OPTIONS:
			serializer->setFileName("OptionsMenu.scene");
			break;
		case GameState::QUIT:
			_justQuit = true;
			break;
		case GameState::NONE:
			break;
		default:
			break;
		}
		return true;
	}
	else if (isQuiting()
		|| Engine::_ecs.getSystem<SerializerSystem>()->getIsLoaded())
	{
		Engine::_ecs.getSystem<SerializerSystem>()->setIsLoaded(false);
		Engine::_ecs.getSystem<AudioSystem>()->StopAllSounds();
		return true;
	}
	return false;
}

bool Engine::isQuiting()
{
	if (Inputs::get_Key_Triggered(GLFW_KEY_LEFT_SHIFT) && !_isPlay)
		_isGameMode = false;
	auto windowSys = _ecs.getSystem<WindowSystem>();
	return glfwWindowShouldClose(windowSys->getWindowPtr())
		|| (Inputs::get_Key_Triggered(GLFW_KEY_ESCAPE)
			&& Inputs::get_Key(GLFW_KEY_LEFT_SHIFT)
			&& (!_isPlay && !_isGameMode))
		|| (_isGameMode && _justQuit);
}

void Engine::updateTime()
{
	Time::DELTA_TIME = Time::update_time(1.0);
}
//
//void Engine::computeFDT(double dt)
//{
//	delta_time = dt;
//	currentNumberofSteps = 0;
//	accumulated_Time += dt;
//
//	while (accumulated_Time >= fixed_delta_time)
//	{
//		accumulated_Time -= fixed_delta_time;
//		++currentNumberofSteps;
//	}
//}
