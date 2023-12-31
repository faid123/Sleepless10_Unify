/******************************************************************************
* \file			Time.cpp
* \brief		Contain all the time related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2021 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#include "Time.h"
#include <iostream>
#include <iomanip>

GLfloat Time::FPS= 0.0f;
float Time::DELTA_TIME = 0.0f;
float Time::APP_TIME = 0.0f;
float Time::TIME_STAMP = 1.0f;
float Time::TIME_STAMP_UI = 1.0f;
float Time::TIME_STAMP_CUTSCENE = 1.0f;
float Time::UI_DELTA_TIME = 0.0f;
float Time::CUTSCENE_DELTA_TIME = 0.0f;
float Time::SYSTEM_START_TIME = 0.0f;
std::map<std::string, float> Time::SYSTEM_DURATION{};
std::map<std::string, std::pair<float, float>> Time::SYSTEM_PERCENTAGE{};
const float Time::_fixedDeltaTime = 1.0f / 60.0f;
int Time::_currentNumberOfSteps = 0;
float Time::_accumulatedTime = 0.0f;



/*****************************************************************************/
/*
 \fn static double update_time(double FPSCalcInt = 1.0);
 \brief Update delta time
*/
/*****************************************************************************/
float Time::update_time(float fps_calc_interval)
{
	// get elapsed time (in seconds) between previous and current frames
	static float prev_time = static_cast<float>(glfwGetTime());
	float curr_time = static_cast<float>(glfwGetTime());
	float _DELTA_TIME = curr_time - prev_time;
	prev_time = curr_time;

	Time::APP_TIME += Time::DELTA_TIME;
	UI_DELTA_TIME = _DELTA_TIME * TIME_STAMP_UI;
	CUTSCENE_DELTA_TIME = _DELTA_TIME * TIME_STAMP_CUTSCENE;
	// FPS calculations
	static float count = 0.0f; // number of game loop iterations
	static float start_time = static_cast<float>(glfwGetTime());
	// get elapsed time since very beginning (in seconds) ...
	float elapsed_time = curr_time - start_time;

	++count;

	// update FPS at least every 10 seconds ...
	fps_calc_interval = (fps_calc_interval < 0.0f) ? 0.0f : fps_calc_interval;
	fps_calc_interval = (fps_calc_interval > 10.0f) ? 10.0f : fps_calc_interval;
	if (elapsed_time > fps_calc_interval)
	{
		// calculate FPS
		FPS = count / elapsed_time;
		start_time = curr_time;
		count = 0.0f;

		// calculate % usage for each system
		for (auto const& system : SYSTEM_DURATION)
		{
			// get the % of time taken for this system in a each frame of delta time.
			SYSTEM_PERCENTAGE[system.first] = std::pair<float,float>(((system.second / _DELTA_TIME) * 100.0f), system.second);
		}
	}
	
	// reset system duration timing every frame
	resetSystemTime();

	// done calculating FPS ...
	return (_DELTA_TIME * TIME_STAMP);
}
/*****************************************************************************/
/*
 \fn static GLdouble get_fps();
 \brief getter function for FPS
*/
/*****************************************************************************/
GLfloat Time::get_fps()
{
	return FPS;
}
/// <summary>
/// getting the system time percentage
/// </summary>
/// <returns></returns>
/// SYSTEM_PERCENTAGE
std::map<std::string, std::pair<float,float>> Time::getSystemTime()
{
	return SYSTEM_PERCENTAGE;
}
/// <summary>
/// pausing the time
/// </summary>
void Time::pauseTime()
{
	TIME_STAMP = 0.0;
}
/// <summary>
/// continuing the time
/// </summary>
void Time::playTime()
{
	TIME_STAMP = 1.0;
}
/// <summary>
/// setting the time
/// </summary>
/// <param name="time"></param>
/// time we want
void Time::changeTime(float time)
{
	TIME_STAMP = time;
}
/// <summary>
/// start accumminating systems time
/// </summary>
void Time::pauseGame(bool pause)
{
	if (pause)
	{
		TIME_STAMP = 0.0;
		TIME_STAMP_CUTSCENE = 0.0;
	}
	else
	{
		TIME_STAMP = 1.0;
		TIME_STAMP_CUTSCENE = 1.0;
	}
}
/// <summary>
/// stop accumminating system time
/// </summary>
/// <param name="systemName"></param>
/// name of the system
void Time::pauseForCutScene(bool pause)
{
	if (pause)
	{
		TIME_STAMP = 0.0;
	}
	else
	{
		TIME_STAMP = 1.0;
	}
}

void Time::pauseUIDelta(bool pause)
{
	if (pause)
	{
		TIME_STAMP_UI = 0.0;
	}
	else
	{
		TIME_STAMP_UI = 1.0;
	}
}
float Time::getTimeStamp()
{
	return TIME_STAMP;
}
/// <summary>
/// reseting the time to 0
/// </summary>
void Time::startSystemTime()
{
	SYSTEM_START_TIME = static_cast<float>(glfwGetTime());
}
/// <summary>
/// setting the delta time 
/// </summary>
void Time::endSystemTime(std::string systemName)
{
	float SYSTEM_END_TIME = static_cast<float>(glfwGetTime());
	SYSTEM_DURATION[systemName] = SYSTEM_DURATION[systemName] + (SYSTEM_END_TIME - SYSTEM_START_TIME);
}
/// <summary>
/// pausing normal dt and cutscene
/// </summary>
/// <param name="pause"></param>
/// if true pause the game
void Time::resetSystemTime()
{
	SYSTEM_DURATION.clear();
}
/// <summary>
/// pause normal dt
/// </summary>
/// <param name="pause"></param>
/// if true pause it
void Time::fixedDeltaTime()
{
	//Fixed DeltaTime
	_currentNumberOfSteps = 0; //reset
	_accumulatedTime += DELTA_TIME; //adding the actual game loop time
	while (_accumulatedTime >= _fixedDeltaTime)
	{
		_accumulatedTime -= _fixedDeltaTime; //this will save the exact accumulated time differences, among all game loops
		_currentNumberOfSteps++;
	}
}