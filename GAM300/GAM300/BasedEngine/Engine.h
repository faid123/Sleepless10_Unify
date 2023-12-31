/******************************************************************************
* \file			Engine.h
* \brief		Contain all the Engine related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2021 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#pragma once

#include "ECS/ECS.h"
#define SHOW_IMGUI

class Engine
{
public:
	/// <summary>
	/// this will init all properties need in the engine
	/// </summary>
	void init();
	/// <summary>
	/// loading things needed for a lv
	/// </summary>
	void load();
	/// <summary>
	/// calling all active system to update
	/// </summary>
	/// <param name="dt">delta time</param>
	void update();
	/// <summary>
	/// unloading things that was previously loading
	/// </summary>
	void unload();
	/// <summary>
	/// releasing properies from the init
	/// </summary>
	void release();
	/// <summary>
	/// condition for restarting
	/// </summary>
	/// <returns>true when it should restart else false</returns>
	bool isRestarting();
	/// <summary>
	/// condition for quiting
	/// </summary>
	/// <returns>true when it should quit else false</returns>
	bool isQuiting();

	void updateTime();

	static ECS _ecs;
	static bool _isPlay;
	static bool _isGameMode;
	bool _once = true;
	static bool _justQuit;
	bool canTriggerEsc;
	/*void computeFDT(double dt);
	double getDelta_time() { return delta_time; }*/
private:
	//Need to add the timer
	//double delta_time;
	//double accumulated_Time = 0.0;
	//double Fixed_accumulated_Time = 0.0;
	//int currentNumberofSteps = 0;
	////FIXED DELTA TIME (Proper implementation to be added later)
	//const double fixed_delta_time = 1.0 / 60.0;
};