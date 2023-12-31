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

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>
#include "Entity/Entity.h"
#include "ECS/Component/Component.h"
#include "Serializer/ISerializer.h"
#include "ScriptingSystem.h"
#include <Properties.h>
/// <summary>
/// The LuaScript Component to be attached to entities that requires script behaviours
/// </summary>
class LuaScriptCom : Component, public ISerializer
{
public:
	/// <summary>
	/// The default constructor for the LuaScriptCom class
	/// </summary>
	LuaScriptCom() = default;

	/// <summary>
	/// The copy constructor of the LuaScriptCom class
	/// </summary>
	/// <param name="path">The path name of the script</param>
	LuaScriptCom(std::string path)
		: mScriptPath{ path }
	{}
	
	/// <summary>
	/// The default destructor of the LuaScriptCom class
	/// </summary>
	~LuaScriptCom() = default;

	/// <summary>
	/// Function to register new usertypes for lua
	/// </summary>
	/// <param name="lua">The sol state</param>
	static void RegisterLua(sol::state& lua);

	/// <summary>
	/// The display name
	/// </summary>
	static constexpr const char* sDisplayName{ "Lua Script" };
	//===== General Variables =====

	sol::object GetScriptVariable(const std::string& var_name);

	//decltype(auto) GetScriptTable(std::string script_name, std::string table_name)
	//{
	//	ScriptingSystem::lua.script_file(script_name);
	//	return ScriptingSystem::lua[table_name];
	//}

	/// <summary>
	/// The Environment of the script
	/// </summary>
	sol::environment mEnvironment; 

	/// <summary>
	/// The relative path of the CORE script.
	/// </summary>
	std::string mScriptPath = "../GAM300/Resources/Script/temptest.lua";

	void Bind();
	void Load() ;

	// [LUA] Awake()
	void Awake() ;

	// [LUA] Start()
	void Start() ;

	// [LUA] PausedUpdate( dt)
	void PausedUpdate(float dt) ;

	// [LUA] Update(dt)
	void Update(float dt) ;

	// [LUA] LateUpdate(dt)
	void LateUpdate(float dt) ;

	sol::environment& GetEnvironment() { return mEnvironment; }

	//--------------------------------------------------
	// Trigger
	//void OnTriggerEnter3D(GameObject* owner, float dt, const Collision::Manifold& manifold);
	//void OnTriggerStay3D(GameObject* owner, float dt, const Collision::Manifold& manifold);
	//void OnTriggerExit2D(GameObject* owner, float dt, const Collision::Manifold& manifold);
	// Collision
	//void OnCollisionEnter3D(GameObject* owner, float dt, const Collision::Manifold& manifold);
	//void OnCollisionStay3D(GameObject* owner, float dt, const Collision::Manifold& manifold);
	//void OnCollisionExit3D(GameObject* owner, float dt, const Collision::Manifold& manifold);
	// Flags
	bool mbIsLoadAttempted{ false };// Is the script loaded yet?
	bool mbIsLoadSuccessful{ false };// Is Successfully loaded, and ready for execution?
	bool mbHasRuntimeError{ false };// Has the script encountered any runtime error?

	bool mbEditorLoaded{ false };// Is this loaded in editor mode?

	// Function Bindings for the Game Object.
	// A nullptr means there is no binding.
	// Naming Conventions:
	// C++ : spfNameNameName, spf = sol::protected_function
	// Lua : See Unity Scripting API, MonoBehaviour for some examples.
	//		https://docs.unity3d.com/ScriptReference/MonoBehaviour.html
	// No funny, confusing, messy, verbose, or weird looking conventions allowed.
	// Please look at Unity, and think about it. No exceptional exceptions will be made.

	// Generic Script Functions
	sol::protected_function spfAwake;// Called once upon loading
	sol::protected_function spfStart;// Called once upon enable and before any Update()
	sol::protected_function spfUpdate;// Called every game tick
	sol::protected_function spfLateUpdate;// Called every game tick, after all Update() has been called.
	sol::protected_function spfPausedUpdate;// Called every game tick, Not Fixed, good for pause menus.

	sol::protected_function spfOnEnable;
	sol::protected_function spfOnDisable;
	sol::protected_function spfOnDestroy;

	// Mouse
	sol::protected_function spfOnMouseDown;
	sol::protected_function spfOnMouseUp;
	sol::protected_function spfOnMouseDrag;
	sol::protected_function spfOnMouseEnter;
	sol::protected_function spfOnMouseExit;
	sol::protected_function spfOnMouseOver;


	// Collision
	sol::protected_function spfOnCollisionEnter2DFunc;
	sol::protected_function spfOnCollisionStay2DFunc; // not used
	sol::protected_function spfOnCollisionExit2DFunc; // not used
	// Trigger
	sol::protected_function spfOnTriggerEnter2DFunc;
	sol::protected_function spfOnTriggerStay2DFunc;
	sol::protected_function spfOnTriggerExit2DFunc;

	// Container to store error information...
	std::vector<std::pair<std::string, std::string>> mErrorMessages;
	bool OnImGuiDisplay();

	void destroyEntity(EntityID entity) override
	{
		(void)entity;
	}
	void cloneComponent(EntityID entity1, EntityID entity2) override
	{
		(void)entity1;
		(void)entity2;
	}
	void Deserialize(const rapidjson::Document& doc, std::string id);
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);
	
private:
	inline void BasicFunctionErrorHandler(const sol::protected_function_result& result);

	property_vtable()
};

property_begin(LuaScriptCom)
{
	property_var(mScriptPath)
} property_vend_h(LuaScriptCom)

