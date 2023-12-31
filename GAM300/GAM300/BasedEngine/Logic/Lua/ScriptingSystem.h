/**********************************************************************************
* \file RenderSystem.h
* \brief File contains the function definitions for all functions that are used
		 to provide as an interface for graphics class.
* \author Tan Wei Jie, 50% Code Contribution
* \author Heng Zeng Le, 50% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include <System/System.h>
extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "../ECS/Entity/Entity.h"
#include <type_traits>
#include "Serializer/ISerializer.h"
#include <memory>
#include <type_traits>

#include "Graphics/TransformCom.h"
#include "Graphics/LightCom.h"
#include "Graphics/MaterialCom.h"
#include "Graphics/RenderSystem/RenderSystem.h"
#include "Camera/CameraCom.h"
#include "Physics/RigidBodyComponent.h"
#include "Physics/PhysicsSystem.h"
#include "Camera/CameraSystem/CameraSystem.h"
#include "Audio/AudioSystem.h"
#include "ECS.h"
#include "Particles/ParticleComponent.h"
#include "Particles/EmitterComponent.h"
#include "Particles/ParticleSystem.h"
#include "Serializer/SerializerSystem.h"
#include "Graphics/SpriteCom.h"
#include "GameState.h"
#include "UserIntered/UI_System.h"
#include "MathLib/Random.h"
#include "MathLib/Trigo.h"
#include "Animation/AnimationCom.h"
#include <ResourceManager/ResourceManager.h>

class ScriptingSystem : public ISystem
{
public:
	void init();
	void load();
	void update(float delta_time);
	void unload();
	void release();

	ScriptingSystem() : mbSceneScriptsLoaded{ false }, mnScriptsLoaded{ 0 }
	{
		// Create a log file.
		//log.CreateLogFile("log/script_debug_log.txt");
	}
	~ScriptingSystem() = default;

	// Loads all the scripts in a scene.
	void LoadSceneScripts();
	void RunSceneScripts(float dt);
	static void RegisterLua(sol::state& lua);
	static sol::state lua;	// The Lua State
	sol::table math_table;
	sol::table console_table;
	sol::table input_table;
	sol::table input_controller_table;
	EntityID getLuaEntityID();
	void setLuaEntityID(EntityID eID);
	void initConsoleTable();
	void setGameState(int gameState);
	void setNState(int nState);
	int getGameState();
private:
	bool mbSceneScriptsLoaded{ false };		// Whether scripts are loaded in the scene.
	int mnScriptsLoaded;	// Number of scripts loaded
	EntityID _luaEntityID;
};

template<typename T>
inline void SetLuaGlobalVariable(const char* var_name, const T* ptr) {
	ScriptingSystem::lua[var_name] = ptr;
}
