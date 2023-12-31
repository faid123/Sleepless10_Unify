/**********************************************************************************
* \file UI_System.h
* \brief File contains the function definitions for all functions that are used
		 in UI System.
* \author Willy Lau, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include <vector>
#include "System/System.h"
#include "UI_Logic.h"
#include "UICom.h"
#include "Engine.h"

#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

class UISystem : public ISystem
{
private:
	EntityID _UIcamID{};
	float _storedTime = 0.0f;
	const float TIMECHANGE = 5.0f;
public:
	void init() override;
	void load() override;
	void update(float delta_time) override;
	//void menuLogic();
	void victoryMenuLogic();
	void levelSelectMenuLogic();
	void OptionsLogic();
	void unload() override;
	void release() override;


	static void RegisterLua(sol::state& lua);

	void update_v2(float delta_time);

	bool keyActive = false;
	bool isFullScreen = false;
	bool ReAdjust = false;

	Vector2 InitalEditorViewPortSize;
	Vector2 InitalWindowSize;
	UI_Logic _UILogic{};

	int pausePointer = 0;
	int pauseState = 0;
	void HandlePauseMenu();
	void loadPauseMenuUI();
	void unloadPauseMenuUI();
	void ResetUI();
	void resizeWindow();
	bool addToList(std::shared_ptr<UICom> cPtr, EntityID eId);

	std::map<EntityID, std::shared_ptr<UICom>> _logics;

	//ui logic specific

	//health
	float _healthPercent = 100;
	float _threatPercent = 0;
	float _powerPercent = 19.53f;
	void increaseHealth(float amount);
	void decreaseHealth(float amount);
	void setHealth(float amount);
	void increaseThreat(float amount);
	void decreaseThreat(float amount);
	void setThreat(float amount);
	void findHealth();
	void findPower();
	void findThreat();
	void increasePower(float amount);
	void decreasePower(float amount);
	void setPower(float amount);
	void slowlyDecreasePower(float dt);
	float powerTimer = 0.0f;
	std::shared_ptr<SpriteCom> threat_sprite;
	std::shared_ptr<SpriteCom> health_sprite;
	std::shared_ptr<SpriteCom> power_sprite;
	std::shared_ptr<SpriteCom> powerMax_sprite;
	//some values
	int bar_XOffsetAmount = 60;
	float damageamount = 8.33f;
	float poweramount =10.0f;
	//ammo
	int _currentAmmoType = WeaponType_UI::Pistol;
	void swapToWeapon(int weapType);
	std::map<WeaponType_UI, std::shared_ptr<TransformCom>> weaponList{};

	// Health Bar for ui
	void findGeneratorHealth();
	void increasePistolHealth(float amount);
	void decreasePistolHealth(float amount);
	void increaseLaserHealth(float amount);
	void decreaseLaserHealth(float amount);
	void increaseRocketHealth(float amount);
	void decreaseRocketHealth(float amount);
	std::shared_ptr<SpriteCom> pistol_barHealth;
	std::shared_ptr<SpriteCom> laser_barHealth;
	std::shared_ptr<SpriteCom> rocket_barHealth;

	void activateAfterPause();

	//get a map of the transform of the weapons in the current scene
	std::map<WeaponType_UI, std::shared_ptr<TransformCom>> findWeapons();
	int _button = 0;

	//function to get the current gamestate
	int getGameState();
	void setGameState(int gameState);
	void setPrevState();
	void ResetLevel();
	
	// an option to change scene while ensuring there is a fade to black efect
	void setGameState_wF2B(int gameState);	
	int getNextState();
	bool getChangingSceneState();
	void setChangingSceneState(bool);


	void goToPrevState();

	int prevState;
	int getPrevState();

	//function to toggle pausing of cutscene.
	void toggleCutscene(bool val);
	bool getCutsceneBool();//return cutscene bool

	//function for turning minimap off
	//so i dont call for rendersystem multiple times everyframe
	bool rendersystem_Minimap_State;
	bool get_RenderSystem_Minimap();
	void set_RenderSystem_Minimap(bool val);

	void setFullscreen(bool b);
	bool getFullscreenState();

	int _menuPointer			= 0;
	int _optionsPointer			= 0;
	bool canTriggerW_Key		= true;
	bool canTriggerS_Key		= true;
	bool canTriggerA_Key		= true;
	bool canTriggerD_Key		= true;
	bool canTriggerEnter_Key	= true;
	bool _isPaused				= false;
	bool _changedSkybox			= false;
	bool _playingCutscene		= false;
	bool _changingScene			= false ;
};