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
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

enum class GameState
{
	MAIN_MENU = 0,
	LV1,
	LV2,
	LV3,
	PAUSE_MENU,
	GAMEOVER,
	VICTORY,
	LOADING_MENULV1,
	LOADING_MENULV2,
	LOADING_MENULV3,
	LEVEL_SELECT,
	OPTIONS,
	QUIT,
	NONE,
	LOADING_START,
	DIGIPEN_LOGO,
	FMOD_LOGO,
	TEAM_LOGO
};

class GameStateManager
{
public:
	static int _curState;
	static int _preState;
	static int _nState;
};