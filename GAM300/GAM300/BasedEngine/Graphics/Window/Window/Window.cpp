/******************************************************************************
* \file			Window.cpp
* \brief		Contain all the Window related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2021 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
/******************************************************************************/
#include "Window.h"
#include "Engine.h"
#include "Graphics/RenderSystem/RenderSystem.h"
#include <iostream>
#include "../Input/Input.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <Logic/Lua/ScriptingSystem.h>
#include <Animation/AnimationSystem.h>

//#include <../Assert/MyAssert.h>
//#include "resource.h"
//#include "../../ECS/BBEngine.h"


//glm::ivec2 WindowSystem::_preSize{ 1280,800 };
//glm::ivec2 WindowSystem::_prePos{ 0,0 };
bool WindowSystem::_isFullScreen;
//glm::ivec2 WindowSystem::_savePreSize{ 0 };


/*****************************************************************************/
/*
 \fn  static void error_Callback(int error, char const* description);
 \brief showing the error message for opelgl
*/
/*****************************************************************************/
void WindowSystem::error_Callback(int error, char const* description)
{
#ifdef _DEBUG
	std::cerr << "GLFW error " << error << ": " << description << std::endl;
#endif
	(void)error;
	(void)description;
}
/*****************************************************************************/
/*
 \fn  static void fbsize_Callback(GLFWwindow* ptr_win, int width, int height);
 \brief setting viewport to with width and height
*/
/*****************************************************************************/
void WindowSystem::fbsize_Callback(GLFWwindow*, int Width, int Height)
{
#ifdef _DEBUG
	std::cout << "fbsize_cb getting called!!!" << std::endl;
	/*std::cout << Width << " " << Height << std::endl << std::endl;*/
#endif
	auto windowSys = Engine::_ecs.getSystem<WindowSystem>();
	
	windowSys->resize_window(Width, Height);
	/*if (_windowWidth && _windowHeight)
		_savePreSize = glm::ivec2(_width, _height);*/
		/*_width = Width;
		_height = Height;*/
		//glfwSetWindowAspectRatio(windowSys->getWindowPtr(), 16, 9);
		//glViewport(0, 0, Width, Height);

}

/// <summary>
/// get the visibility of window
/// </summary>
/// <returns></returns>
/// return true when it active. else false its not
bool WindowSystem::get_Focus()
{
	return glfwGetWindowAttrib(_ptrWindow, GLFW_FOCUSED);
}
/// <summary>
/// setting it to full screen when we want
/// </summary>
/// <param name="isFullScreen"></param>
/// true when we want it to be fullscreen
void WindowSystem::setFullScreen()
{
	if (_isFullScreen)
	{
		_preSize = glm::ivec2(_windowWidth, _windowHeight);
		glfwGetWindowPos(_ptrWindow, &_prePos.x, &_prePos.y);
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		_monitor = glfwGetPrimaryMonitor();;
		glfwSetWindowMonitor(_ptrWindow, _monitor, 0, 0, mode->width, mode->height, 0);
	}
	else
	{
		glm::ivec2 temp{ 0 };
		glfwGetWindowSize(_ptrWindow, &temp.x, &temp.y);
		glfwSetWindowMonitor(_ptrWindow, NULL,
			_prePos.x, _prePos.y,
			_preSize.x, _preSize.y, 0);
		
		//glfwGetFramebufferSize(WindowSystem::_ptrWindow, &temp.x, &temp.y);
		//glViewport(0, 0, _preSize.x, _preSize.y);
	}
}

bool WindowSystem::getFullScreen()
{
	return _isFullScreen;
}


glm::ivec2 WindowSystem::get_savedPreSive()
{
	return _savePreSize;
}
void WindowSystem::resize_window(const int& Width, const int& Height)
{
	_windowHeight = Height;
	_windowWidth = Width;
	//Engine::_ecs.getSystem<RenderSystem>()->resize_viewport(Width, Height);
}
void WindowSystem::setViewableCustor()
{
	_isHiddingCursor = !_isHiddingCursor;
	auto windowSys = Engine::_ecs.getSystem<WindowSystem>();
	if (_isHiddingCursor)
	{
		glfwSetInputMode(windowSys->getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		glfwSetInputMode(windowSys->getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}
/// <summary>
/// implement constuctor
/// </summary>
ViewPort::ViewPort(GLint x, GLint y, GLsizei width, GLsizei height) :
	_x{ x },
	_y{ y },
	_width{ width },
	_height{ height }
{
}
/// <summary>
/// copy constuctor
/// </summary>
ViewPort::ViewPort(const ViewPort& rhs) :
	_x{ rhs._x },
	_y{ rhs._y },
	_width{ rhs._width },
	_height{ rhs._height }
{
}
/// <summary>
/// copy asisment
/// </summary>
ViewPort& ViewPort::operator=(const ViewPort& rhs)
{
	if (this != &rhs)
	{
		_x = rhs._x;
		_y = rhs._y;
		_width = rhs._width;
		_height = rhs._height;
	}
	return *this;
}

void WindowSystem::init()
{
	if (!InitWindow())
	{
		std::cerr << "Unable to create OpenGL context" << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void WindowSystem::load()
{
}

void WindowSystem::update(float dt)
{
	(void)dt;
	Inputs::update();
	std::stringstream sstr;

	sstr << std::fixed << std::setprecision(2) << _title;
	glfwSetWindowTitle(_ptrWindow, sstr.str().c_str());
}

void WindowSystem::unload()
{
}

void WindowSystem::release()
{
	Inputs::realase();
	if (_ptrWindow)
	{
		glfwSetWindowShouldClose(_ptrWindow, GLFW_TRUE);
		glfwTerminate();
	}
}

int WindowSystem::getWindowWidth()
{
	int w, h;
	glfwGetFramebufferSize(_ptrWindow, &w, &h);
	_windowHeight = h;
	_windowWidth = w;
	return _windowWidth;
}

int WindowSystem::getWindowHeight()
{
	int w, h;
	glfwGetFramebufferSize(_ptrWindow, &w, &h);
	_windowHeight = h;
	_windowWidth = w;
	return _windowHeight;
}

std::string WindowSystem::getWindowTitle()
{
	return _title;
}

GLFWwindow* WindowSystem::getWindowPtr()
{
	return _ptrWindow;
}

void WindowSystem::iconify_callback(GLFWwindow* window, int isFullScreen)
{
	(void)window;
	if (isFullScreen)
	{
#ifdef _DEBUG
		std::cout << "The window was iconified" << std::endl;
#endif
		// The window was iconified
		//_isFullScreen = true;
	}
	else
	{
#ifdef _DEBUG
		std::cout << "The window was restored" << std::endl;
#endif
		// The window was restored
		//_isFullScreen = false;
	}
}
void window_focus_callback(GLFWwindow* window, int focused);
bool WindowSystem::InitWindow()
{
	if (!glfwInit())
	{
		std::cout << "GLFW init has failed." << std::endl;
		return false;
	}
	glfwSetErrorCallback(WindowSystem::error_Callback);

	// Before asking GLFW to create an OpenGL context, we specify the minimum constraints
	// in that context:
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // window dimensions are static

	if (_isFullScreen)
	{
		_monitor = glfwGetPrimaryMonitor();
		_mode = glfwGetVideoMode(_monitor);

		glfwWindowHint(GLFW_RED_BITS, _mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, _mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, _mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, _mode->refreshRate);

		//FULLSCREEN
		_ptrWindow = glfwCreateWindow(_mode->width, _mode->height, _title.c_str(), _monitor, NULL);
		_preSize = glm::ivec2(_windowWidth, _windowHeight);
		_prePos = glm::ivec2(100, 100);
		_windowHeight = _mode->height;
		_windowWidth = _mode->width;
	}
	else
	{
		_ptrWindow = glfwCreateWindow(_windowWidth, _windowHeight, _title.c_str(), NULL, NULL);
	}

	if (!_ptrWindow) {
		std::cerr << "GLFW unable to create OpenGL context - abort program\n";
		glfwTerminate();
		return false;
	}


	glfwMakeContextCurrent(_ptrWindow);
	glfwSetWindowIconifyCallback(_ptrWindow, iconify_callback);

	glfwSetFramebufferSizeCallback(_ptrWindow, fbsize_Callback);
	glfwSetKeyCallback(_ptrWindow, Inputs::key_Callback);
	glfwSetMouseButtonCallback(_ptrWindow, Inputs::mouseButton_Callback);
	glfwSetCursorPosCallback(_ptrWindow, Inputs::mousePos_Callback);
	glfwSetScrollCallback(_ptrWindow, Inputs::mouseScroll_Callback);

	glfwSetInputMode(_ptrWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetWindowFocusCallback(_ptrWindow, window_focus_callback);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		std::cerr << "Unable to initialize GLEW - error: "
			<< glewGetErrorString(err) << " abort program" << std::endl;
		return false;
	}

	return true;
}

void window_focus_callback(GLFWwindow* /*window*/, int focused)
{
	if (focused)
	{
		Engine::_ecs.getSystem<RenderSystem>()->_graphics._pauseCutScene = false;
		if (GameStateManager::_curState == (int)GameState::MAIN_MENU
			|| GameStateManager::_curState == (int)GameState::LEVEL_SELECT)
			Engine::_ecs.getSystem<AudioSystem>()->UnpauseBGM("BG_MainMenu");
		else if (GameStateManager::_curState == (int)GameState::DIGIPEN_LOGO
			|| GameStateManager::_curState == (int)GameState::TEAM_LOGO
			|| GameStateManager::_curState == (int)GameState::FMOD_LOGO)
			Engine::_ecs.getSystem<AudioSystem>()->UnpauseBGM("BG_MainLoading");
		else if (Engine::_ecs.getSystem<UISystem>()->_isPaused && Engine::_ecs.getSystem<UISystem>()->pauseState == 0
			&& GameStateManager::_curState != (int)GameState::LV1
			&& GameStateManager::_curState != (int)GameState::LV2
			&& GameStateManager::_curState != (int)GameState::LV3)
		{
			Engine::_ecs.getSystem<UISystem>()->_isPaused = false;
			Engine::_ecs.getSystem<UISystem>()->activateAfterPause();
			Engine::_ecs.getSystem<UISystem>()->unloadPauseMenuUI();

		}
	}
	else
	{
		if (Engine::_isPlay)
		{
			if (!Engine::_ecs.getSystem<RenderSystem>()->_graphics._pauseCutScene)
				Engine::_ecs.getSystem<RenderSystem>()->_graphics._pauseCutScene = true;

			if (GameStateManager::_curState == (int)GameState::MAIN_MENU
				|| GameStateManager::_curState == (int)GameState::LEVEL_SELECT)
				Engine::_ecs.getSystem<AudioSystem>()->PauseBGM("BG_MainMenu");
			else if (GameStateManager::_curState == (int)GameState::DIGIPEN_LOGO
				|| GameStateManager::_curState == (int)GameState::TEAM_LOGO
				|| GameStateManager::_curState == (int)GameState::FMOD_LOGO)
				Engine::_ecs.getSystem<AudioSystem>()->PauseBGM("BG_MainLoading");
			else if (GameStateManager::_curState != (int)GameState::MAIN_MENU
				&& GameStateManager::_curState != (int)GameState::GAMEOVER
				&& GameStateManager::_curState != (int)GameState::DIGIPEN_LOGO
				&& GameStateManager::_curState != (int)GameState::LEVEL_SELECT
				&& GameStateManager::_curState != (int)GameState::TEAM_LOGO
				&& GameStateManager::_curState != (int)GameState::FMOD_LOGO
				&& !Engine::_ecs.getSystem<UISystem>()->_isPaused)
			{

				Engine::_ecs.getSystem<UISystem>()->_isPaused = true;
				Engine::_ecs.getSystem<UISystem>()->loadPauseMenuUI();
				Engine::_ecs.getSystem<UISystem>()->pausePointer = 0;
				Engine::_ecs.getSystem<ScriptingSystem>()->_isActive = false;
				Engine::_ecs.getSystem<PhysicsSystem>()->_isActive = false;
				Engine::_ecs.getSystem<AnimationSystem>()->_isActive = false;
				Engine::_ecs.getSystem<ParticleSystem>()->_isActive = false;
				Engine::_ecs.getSystem<CameraSystem>()->_isActive = false;
			}
		}
	}
}