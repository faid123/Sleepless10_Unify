#include "ScriptingSystem.h"
#include "GameState.h"
#include "LuaScriptCom.h"
#include <Graphics/TextCom.h>

int readonly_table_error(lua_State* L);

/// <summary>
/// Initializes the Scripting System
/// </summary>
void ScriptingSystem::init()
{
	std::cout << "[Script] Initializing LUA Scripting System" << std::endl;
	std::cout << "[Script] LUA - START Registering User Types" << std::endl;
	Vector3::RegisterLua(lua);
	Vector4::RegisterLua(lua);
	Quat::RegisterLua(lua);
	Random::RegisterLua(lua);
	Trigo::RegisterLua(lua);
	
	initConsoleTable();
	math_table = lua.create_named_table("Math");
	sol::table ob_metatable = lua.create_table_with();
	ob_metatable["Pi"] = 3.14159265358979323846264338327950288;
	ob_metatable["Rad2Deg"] = 57.295779513082320876798154814105;
	ob_metatable["Deg2Rad"] = 0.01745329251994329576923690768489;
	ob_metatable["Clamp"] = [](double value, double min, double max) {
		if (value < min)
			value = min;
		else if (value > max)
			value = max;
		return value; };
	ob_metatable["Clamp01"] = [](double value) {
		if (value < 0.0)
			return 0.0;
		else if (value > 1.0)
			return 1.0;
		else
			return value; };
	ob_metatable["Lerp"] = [](double start, double end, double t) {
		if (t > 1.0)
			t = 1.0;
		else if (t < 0.0)
			t = 0.0;
		return start + t * (end - start);
	};
	ob_metatable[sol::meta_function::new_index] = readonly_table_error;
	ob_metatable[sol::meta_function::index] = ob_metatable;
	math_table[sol::metatable_key] = ob_metatable;

	

	// Game Object
	//Entity::RegisterLua(lua);


	// components
	//Component::RegisterLua(lua);
	TransformCom::RegisterLua(lua);
	LightCom::RegisterLua(lua);
	LightCom::DirLight::RegisterLua(lua);
	LightCom::PointLight::RegisterLua(lua);
	LightCom::SpotLight::RegisterLua(lua);
	MaterialCom::RegisterLua(lua);
	CameraCom::RegisterLua(lua);
	RigidStaticCom::RegisterLua(lua);
	RigidDynamicCom::RegisterLua(lua);
	EmitterCom::RegisterLua(lua);
	SpriteCom::RegisterLua(lua);
	LuaScriptCom::RegisterLua(lua);
	TextCom::RegisterLua(lua);

	AnimationCom::RegisterLua(lua);
	// bind Input
	Inputs::RegisterLua(lua);
	Inputs::bindInputEnum(input_table, lua);
	Inputs::bindInputControllerEnum(input_controller_table, lua);

	// Register systems
	
	// Debug manager
	//DebugSystem::RegisterLua(lua);
	//SetLuaGlobalVariable("Debug", Engine::GetInstance()->gDebug);
	
	// Scene manager
	//SceneManager::RegisterLua(lua);
	//SetLuaGlobalVariable("SceneManager", Engine::GetInstance()->gScene);
	
	// Time System
	//TimeSystem::RegisterLua(lua);
	//SetLuaGlobalVariable("Time", Engine::GetInstance()->gTime);
	
	// Factory
	//Factory::RegisterLua(lua);
	//SetLuaGlobalVariable("Factory", Engine::GetInstance()->GetSystem<Factory>());

	// Camera System
	CameraSystem::RegisterLua(lua);
	SetLuaGlobalVariable("CameraSystem", Engine::_ecs.getSystem<CameraSystem>().get());
	
	// Audio System
	AudioSystem::RegisterLua(lua);
	SetLuaGlobalVariable("AudioSystem", Engine::_ecs.getSystem<AudioSystem>().get());

	// Physics System
	PhysicsSystem::RegisterLua(lua);
	SetLuaGlobalVariable("PhysicsSystem", Engine::_ecs.getSystem<PhysicsSystem>().get());

	//ECS
	ECS::RegisterLua(lua);
	SetLuaGlobalVariable("ECS", &Engine::_ecs);

	//ScriptSystem
	ScriptingSystem::RegisterLua(lua);
	SetLuaGlobalVariable("ScriptSystem", Engine::_ecs.getSystem<ScriptingSystem>().get());

	//ScriptSystem
	ResourceManager::RegisterLua(lua);
	SetLuaGlobalVariable("ResourceManager", Engine::_ecs.getSystem<ResourceManager>().get());

	//ParticleSystem
	ParticleSystem::RegisterLua(lua);
	SetLuaGlobalVariable("ParticleSystem", Engine::_ecs.getSystem<ParticleSystem>().get());	
	
	//SerializerSystem
	SerializerSystem::RegisterLua(lua);
	SetLuaGlobalVariable("SerializerSystem", Engine::_ecs.getSystem<SerializerSystem>().get());
	
	//RenderSystem
	RenderSystem::RegisterLua(lua);
	SetLuaGlobalVariable("RenderSystem", Engine::_ecs.getSystem<RenderSystem>().get());


	//UISystem
	UISystem::RegisterLua(lua);
	SetLuaGlobalVariable("UISystem", Engine::_ecs.getSystem<UISystem>().get());

	//ContactReportCallback
	ContactReportCallback::RegisterLua(lua);
	SetLuaGlobalVariable("mContactReportCallback", &Engine::_ecs.getSystem<PhysicsSystem>()->getContactCallback());

	std::cout << "[Script] LUA - ENDED Registering User Types" << std::endl;

}

/// <summary>
/// Loading the Scripting System
/// </summary>
void ScriptingSystem::load()
{
	luaL_openlibs(lua);
}

/// <summary>
/// Update the Scripting System
/// </summary>
/// <param name="delta_time">The delta time</param>
void ScriptingSystem::update(float delta_time)
{
	// Run the scripts if they are loaded
	if (mbSceneScriptsLoaded && !Engine::_ecs.getSystem<UISystem>()->_isPaused)
		RunSceneScripts(delta_time);
}

//void ScriptingSystem::draw()
//{
//}

void ScriptingSystem::unload()
{
}

/// <summary>
/// Release the Scripting System
/// </summary>
void ScriptingSystem::release()
{
}

/// <summary>
/// Load all of the scripts in a scene
/// </summary>
void ScriptingSystem::LoadSceneScripts()
{
	// Loads and calls Awake
	for (auto& object : _entities) {
		auto script = Engine::_ecs.getComponent<LuaScriptCom>(object);
		if (script != nullptr)
		{
			script->Load();
			script->Awake();
		}
	}

	// Calls Start
	for (auto& object : _entities) {

		auto script = Engine::_ecs.getComponent<LuaScriptCom>(object);
		if (script != nullptr)
		{
			script->Start();
		}
	}
	
	// Finally, mark that all the scripts in this scene is loaded.
	mbSceneScriptsLoaded = true;
}

/// <summary>
/// Run all the scripts in the scene
/// </summary>
/// <param name="dt">The delta time</param>
void ScriptingSystem::RunSceneScripts(float dt)
{
	// PAUSED, or STEP not pressed, do not run scripts for this frame.
	//if (Engine::GetInstance()->gEditor->mbIsEditorPaused &&
		//!Engine::GetInstance()->gEditor->mbIsEditorStepped)
		//return;

	// References to containers.
	auto& all_objects_list = _entities; //Engine::GetInstance()->GetSystem<Factory>()->GetAllGameObjects();

	//remember to delete to use
	(void)all_objects_list;

	for (auto entity : _entities)
	{
		auto script = Engine::_ecs.getComponent<LuaScriptCom>(entity);

		if (script != nullptr && Engine::_ecs.getEntityManager()->isActive(entity))
		{
			 
			// Don't run bad scripts.
			if (!script->mbIsLoadSuccessful || script->mbHasRuntimeError) continue;
			_luaEntityID = entity;
			if (Engine::_ecs.getSystem<UISystem>()->_playingCutscene)
			{
				if (
					Engine::_ecs.getEntityName(entity).find("Cutscene") !=  std::string::npos
					|| Engine::_ecs.getEntityName(entity).find("Fade") !=  std::string::npos
					)
					script->Update(dt);
			}
			else
			{
				script->Update(dt);
			}

			// for use later when we hav multi scripts per com
			//for (auto& pbhv : script.GetAllBehaviors())
			//{
			//	pbhv->Update(dt);
			//}

			// LateUpdate() only starts getting called after ALL normal updates are called.
			//------------------------------ LateUpdate() ------------------------------
			// Calls LateUpdate()

				if (!Engine::_ecs.getEntityManager()->isActive(entity))
					continue;

				script->LateUpdate(dt);

				// for use later when we hav multi scripts per com
				//for (auto& pbhv : script.GetAllBehaviors()) {
				//	pbhv->LateUpdate(dt);
				//}

		}
		
	}

	//for (auto& object : all_objects_list)
	//{
	//	auto transform = Engine::_ecs.getComponent<TransformCom>(object);

	//	if (!object._isActive || !object.HasComponent<LuaScriptComponent>()) continue;

	//	LuaScriptComponent* script = object.GetComponent<LuaScriptComponent>();

	//	// Don't run bad scripts.
	//	if (!script->mbIsLoadSuccessful || script->mbHasRuntimeError) continue;

	//	for (auto& pbhv : object.GetAllBehaviors())
	//	{
	//		pbhv->Update(dt);
	//	}



	//}// Lua Update()


	// LateUpdate() only starts getting called after ALL normal updates are called.
	//------------------------------ LateUpdate() ------------------------------
	// Calls LateUpdate()
	//for (auto& object : all_objects_list)
	//{
	//	if (!object.mbIsActive)
	//		continue;

	//	for (auto& pbhv : object.GetAllBehaviors()) {
	//		pbhv->LateUpdate(dt);
	//	}
	//}// LateUpdate()
}


/// <summary>
/// Function that returns an error message if user tries to modify read-only table values
/// </summary>
/// <param name="L">The lua state</param>
/// <returns></returns>
int readonly_table_error(lua_State* L) {
	return luaL_error(L, "You are not allowed to modify read only values in table!! - the magical elves in the code");
}

/// <summary>
/// Getter function for _luaEntityID
/// </summary>
/// <returns>Returns the variable _luaEntityID</returns>
EntityID ScriptingSystem::getLuaEntityID()
{
	return _luaEntityID;
}

/// <summary>
/// Setter function for _luaEntityID
/// </summary>
/// <param name="eID">The entity's ID</param>
void ScriptingSystem::setLuaEntityID(EntityID eID)
{
	_luaEntityID = eID;
}

/// <summary>
/// Initializing the console table
/// </summary>
void ScriptingSystem::initConsoleTable()
{
	console_table = lua.create_named_table("Console");
	console_table["WriteLine"] = [&](const std::string& str) {std::cout << str << std::endl; };
	console_table["PrintEntityID"] = [&](const EntityID eID) {std::cout << eID << std::endl; };
	console_table["PrintPrefabID"] = [&](const PrefabID pID) {std::cout << pID << std::endl; };
	console_table["PrintFloat"] = [&](const float num) {std::cout << num << std::endl; };
	console_table["PrintBool"] = [&](const bool var) {std::cout << var << std::endl; };
	console_table["PrintInt"] = [&](const int num) {std::cout << num << std::endl; };
}

/// <summary>
/// Sets the current state of the game
/// </summary>
/// <param name="gameState">The game state</param>
void ScriptingSystem::setGameState(int gameState)
{
	GameStateManager::_curState = gameState;
}

/// <summary>
/// Sets the Next State of the game
/// </summary>
/// <param name="nState">The next state</param>
void ScriptingSystem::setNState(int nState)
{
	GameStateManager::_nState = nState;
}

/// <summary>
/// Gets the current state of the game
/// </summary>
/// <returns>Returns the current state of the game</returns>
int ScriptingSystem::getGameState()
{
	return GameStateManager::_curState;
}