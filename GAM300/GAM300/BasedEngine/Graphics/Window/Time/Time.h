/******************************************************************************
* \file			Time.h
* \brief		Contain all the time related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2021 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#pragma once

#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <map>
#include <string>
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"
/*****************************************************************************/
/*
 \fn class Time
 \brief Transform class with all the needed function
*/
/*****************************************************************************/


class Time
{
public:
	/*****************************************************************************/
	/*
	 \fn static double update_time(double FPSCalcInt = 1.0);
	 \brief Update delta time
	*/
	/*****************************************************************************/
	static float update_time(float FPSCalcInt = 1.0f);
	/*****************************************************************************/
	/*
	 \fn static GLdouble get_fps();
	 \brief getter function for FPS
	*/
	/*****************************************************************************/
	static GLfloat get_fps();
	/// <summary>
	/// getting the system time percentage
	/// </summary>
	/// <returns></returns>
	/// SYSTEM_PERCENTAGE
	static std::map<std::string, std::pair<float, float>> getSystemTime();
	/// <summary>
	/// pausing the time
	/// </summary>
	static void pauseTime();
	/// <summary>
	/// continuing the time
	/// </summary>
	static void playTime();
	/// <summary>
	/// setting the time
	/// </summary>
	/// <param name="time"></param>
	/// time we want
	static void changeTime(float time);
	/// <summary>
	/// start accumminating systems time
	/// </summary>
	static void startSystemTime();
	/// <summary>
	/// stop accumminating system time
	/// </summary>
	/// <param name="systemName"></param>
	/// name of the system
	static void endSystemTime(std::string systemName);
	/// <summary>
	/// reseting the time to 0
	/// </summary>
	static void resetSystemTime();
	/// <summary>
	/// setting the delta time 
	/// </summary>
	static void fixedDeltaTime();
	/// <summary>
	/// pausing normal dt and cutscene
	/// </summary>
	/// <param name="pause"></param>
	/// if true pause the game
	static void pauseGame(bool pause);
	/// <summary>
	/// pause normal dt
	/// </summary>
	/// <param name="pause"></param>
	/// if true pause it
	static void pauseForCutScene(bool pause);
	static void pauseUIDelta(bool pause);
	static float getTimeStamp();

	static void RegisterLua(sol::state& lua);

	static float DELTA_TIME;
	static float UI_DELTA_TIME;
	static float CUTSCENE_DELTA_TIME;
	static float APP_TIME; // total time
	static const float _fixedDeltaTime;
	static int _currentNumberOfSteps;
	static float _accumulatedTime;

private:
	static GLfloat FPS;
	static float TIME_STAMP;
	static float TIME_STAMP_UI;
	static float TIME_STAMP_CUTSCENE;
	static std::map<std::string, float> SYSTEM_DURATION;
	static std::map<std::string, std::pair<float, float>> SYSTEM_PERCENTAGE;
	static float SYSTEM_START_TIME;
};