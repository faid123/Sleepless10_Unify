/******************************************************************************
* \file			Window.h
* \brief		Contain all the Window related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2021 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
/******************************************************************************/
#pragma once

#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <string>
#include <map>
#include <glm/glm.hpp>
#include "System/System.h"
/*****************************************************************************/
/*
 \fn class Windows
 \brief Windows class with all the needed function
*/
/*****************************************************************************/
class WindowSystem: public ISystem
{
public:
	void init() override;
	void load() override;
	void update(float dt)override;
	void unload() override;
	void release() override;

	int getWindowWidth();
	int	getWindowHeight();
	std::string getWindowTitle();
	GLFWwindow* getWindowPtr();


	/*****************************************************************************/
	/*
	 \fn  static void error_Callback(int error, char const* description);
	 \brief showing the error message for opelgl
	*/
	/*****************************************************************************/
	static void error_Callback(int error, char const* description);
	/*****************************************************************************/
	/*
	 \fn  static void fbsize_Callback(GLFWwindow* ptr_win, int width, int height);ViewPort
	 \brief setting viewport to with width and height
	*/
	/*****************************************************************************/
	static void fbsize_Callback(GLFWwindow* ptr_win, int width, int height);
	
	static void iconify_callback(GLFWwindow* window, int iconified);

	
	/// <summary>
	/// get the visibility of window
	/// </summary>
	/// <returns></returns>
	/// return true when it active. else false its not
	bool get_Focus();
	/// <summary>
	/// setting it to full screen when we want
	/// </summary>
	/// <param name="isFullScreen"></param>
	/// true when we want it to be fullscreen
	void setFullScreen();
	/// <summary>
	/// setting the size of the window
	/// </summary>
	/// <param name="x"></param>
	/// width of the window
	/// <param name="y"></param>
	/// height of the window
	bool getFullScreen();

	/// <summary>
	/// getting the window size that was before the window was minimize
	/// </summary>
	/// <returns></returns>
	/// return _savePreSize
	glm::ivec2 get_savedPreSive();
	//static GLFWwindow* ptr_window;
	void resize_window(const int& Width, const int& Height);

	void setViewableCustor();

	static bool _isFullScreen;

private:
	/*static GLint _width, _height;
	
	static std::string _title;*/
	glm::ivec2 _preSize;
	glm::ivec2 _prePos {100, 100};
	glm::ivec2 _savePreSize;

	int _windowWidth{ 1280 };
	int _windowHeight{ 720 };
	//float _fps{ 0.0f };
	std::string _title{"GAM300"};
	GLFWwindow* _ptrWindow{nullptr};

	GLFWmonitor* _monitor{ nullptr };
	const GLFWvidmode* _mode{ nullptr };

	bool _isHiddingCursor{ false };
	bool InitWindow();
};

struct ViewPort
{
	// lower left of the viewport
	GLint _x{ 0 }, _y{ 0 };
	// size of the viewport
	GLsizei _width{ 0 }, _height{ 0 };
	/// <summary>
	/// default constuctor
	/// </summary>
	ViewPort() = default;
	/// <summary>
	/// implement constuctor
	/// </summary>
	ViewPort(GLint x, GLint y, GLsizei width, GLsizei height);
	/// <summary>
    /// copy constuctor
    /// </summary>
	ViewPort(const ViewPort& rhs);
	/// <summary>
	/// copy asisment
	/// </summary>
	ViewPort& operator=(const ViewPort& rhs);
	/// <summary>
	/// default destructor
	/// </summary>
	~ViewPort() = default;
};
using ViewPort_Map = std::map<std::string, ViewPort>;