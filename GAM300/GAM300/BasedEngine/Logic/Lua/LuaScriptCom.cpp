
#include "LuaScriptCom.h"


/// <summary>
/// Function gets the variables in the script
/// </summary>
/// <param name="var_name">The variable name to get</param>
/// <returns>Returns the sol object instance</returns>
sol::object LuaScriptCom::GetScriptVariable(const std::string& var_name)
{
	sol::object result = mEnvironment.get<sol::object>(var_name);
	if (result.valid())
	{
		return result;
	}
	else {
		std::cout << "[Script][ERROR] GetScriptVariable(" << var_name << ") Variable Name Not Found !!" << std::endl;
		return sol::object{};
	}
}

/// <summary>
/// Function binds LUA functions to solution
/// </summary>
void LuaScriptCom::Bind()
{
	spfAwake = mEnvironment["Awake"];
	spfStart = mEnvironment["Start"];
	spfUpdate = mEnvironment["Update"];
	spfPausedUpdate = mEnvironment["PausedUpdate"];
	spfLateUpdate = mEnvironment["LateUpdate"];

	spfOnCollisionEnter2DFunc = mEnvironment["onCollisionEnter3D"];// param = (gameobj, dt, manifold)
	spfOnCollisionStay2DFunc = mEnvironment["onCollisionStay3D"];// param = (gameobj, dt, manifold)
	spfOnCollisionExit2DFunc = mEnvironment["onCollisionExit3D"];// param = (gameobj, dt, manifold)
	spfOnTriggerEnter2DFunc = mEnvironment["onTriggerEnter3D"];// param = (gameobj, dt, manifold)
	spfOnTriggerStay2DFunc = mEnvironment["onTriggerStay3D"];// param = (gameobj, dt, manifold)
	spfOnTriggerExit2DFunc = mEnvironment["onTriggerExit3D"];// param = (gameobj, dt)

	spfOnMouseEnter = mEnvironment["OnMouseEnter"];
	spfOnMouseExit = mEnvironment["OnMouseExit"];
	spfOnMouseOver = mEnvironment["OnMouseOver"];
	spfOnMouseDown = mEnvironment["OnMouseDown"];
	spfOnMouseUp = mEnvironment["OnMouseUp"];
	spfOnMouseDrag = mEnvironment["OnMouseDrag"];
	
	// Default Reserved Variables.
	//mEnvironment["gameObject"] = GetOwnerObject();	// Refers to the Game Object who owns this script.
	mEnvironment["this"] = this;
}

/// <summary>
/// Function loads all scripts
/// </summary>
void LuaScriptCom::Load()
{
	// If no script specified for the object, skip loading.
	if (mScriptPath.empty()) {
		std::cout << "[Lua Script] Empty Path, not Loading!" << std::endl;
		return;
	}

	// Loads the script. Only Loading no execution.
	sol::load_result lr = ScriptingSystem::lua.load_file(mScriptPath);
	// Reset status flags
	mbIsLoadAttempted = true;
	mbIsLoadSuccessful = false;

	// FIRST PASS - Check if there are any syntax errors.
	if (!lr.valid()) {
		// There is a syntax error such that loading failed.
		// This means that the script is not successfully loaded.
		sol::error err = lr;
		std::string what = err.what();
		//std::cout << "[Lua Script] Script Loading Failed! GameObject : " << GetOwnerObject()->GetName() << std::endl;
		std::cout << "\tFile : " << mScriptPath << std::endl;
		std::cout << "\tError Message : " << what << std::endl;
		//log.LogToFile("Loading Script Failed!");
		//log.LogToFile(what);
		mErrorMessages.push_back(std::make_pair<>("First Pass Loading Failed", err.what()));
		return;
	}
	else {
		// The script is successfully loaded.

		// Create an environemnt for this script to execute.
		// The upvalue will be the global environment.
		mEnvironment = sol::environment{ ScriptingSystem::lua, sol::create, ScriptingSystem::lua.globals() };

		// Set the script execution onto the environment. As the script is only loaded, not executed yet.
		sol::protected_function script_func = lr.get<sol::protected_function>();
		sol::set_environment(mEnvironment, script_func);

		// Proceed to execute script in the environment
		sol::protected_function_result result = script_func();

		// SECOND PASS - Check if there are any execution errors.
		if (!result.valid()) {
			// Script Execution Failed

			sol::error err = result;
			sol::call_status status = result.status();
			std::cout << "[Lua Script] Loading Success, but environment execution failed!" << std::endl;
			std::cout << "\tCall Status : " << sol::to_string(status) << std::endl;
			std::cout << "\tError Message : " << err.what() << std::endl;
			mErrorMessages.push_back(std::make_pair<>(sol::to_string(status), err.what()));
		}
		else {
			// FIRST & SECOND PASS SUCCESS

			// Proceed to bind built in event functions.
			Bind();

			mbIsLoadSuccessful = true;
		}
	}
}

/// <summary>
/// Function runs the awake function in scripts
/// </summary>
void LuaScriptCom::Awake()
{
	if (!mbIsLoadSuccessful || mbHasRuntimeError) return;

	if (spfAwake) {
		//std::cout << "[Script] Found Awake() function! for script : " << obj.m_Script.scriptPath << std::endl;
		sol::protected_function_result result;
		result = spfAwake();
		BasicFunctionErrorHandler(result);
	}
}

/// <summary>
/// Function runs the start function in scripts
/// </summary>
void LuaScriptCom::Start()
{
	if (!mbIsLoadSuccessful || mbHasRuntimeError) return;

	if (spfStart) {
		//std::cout << "[Script] Found Start() function! for script : " << obj.m_Script.scriptPath << std::endl;
		sol::protected_function_result result;
		result = spfStart();
		BasicFunctionErrorHandler(result);
	}
}

/// <summary>
/// Function runs the pause update function in scripts
/// </summary>
/// <param name="dt">The delta time</param>
void LuaScriptCom::PausedUpdate(float dt)
{
	if (!mbIsLoadSuccessful || mbHasRuntimeError) return;

	if (spfPausedUpdate) {
		sol::protected_function_result result;
		result = spfPausedUpdate(dt);
		BasicFunctionErrorHandler(result);
	}
}

/// <summary>
/// Function runs the update function in scripts
/// </summary>
/// <param name="dt">The delta time</param>
void LuaScriptCom::Update(float dt)
{
	if (!mbIsLoadSuccessful || mbHasRuntimeError) return;

	if (spfUpdate) {
		sol::protected_function_result result = spfUpdate(dt);
		BasicFunctionErrorHandler(result);
	}
}

/// <summary>
/// Function runs the late update function in scripts
/// </summary>
/// <param name="dt">The delta time</param>
void LuaScriptCom::LateUpdate(float dt)
{
	if (!mbIsLoadSuccessful || mbHasRuntimeError) return;

	if (spfLateUpdate) {
		sol::protected_function_result result = spfLateUpdate(dt);
		BasicFunctionErrorHandler(result);
	}
}

/// <summary>
/// Display LuaScript Com on ImGui
/// </summary>
/// <returns>Return success or failure</returns>
bool LuaScriptCom::OnImGuiDisplay()
{
	ImGui::PushID(this);

	if (!mbIsLoadSuccessful && !mbEditorLoaded) {
		Load();
		mbEditorLoaded = true;
	}

	bool result = false;
	ImGui::SetNextTreeNodeOpen(true, ImGuiCond_FirstUseEver);
	if (ImGui::TreeNodeEx(sDisplayName, ImGuiTreeNodeFlags_Framed))
	{
		// Special case for scripts.
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Remove Component (Cannot Undo!)")) {
				// TODO :: remove this com
			}
			ImGui::EndPopup();
		}//ImGui::BeginPopupContextItem()

		
		//------------------------------------------------------------
		bool anyErrors = !mErrorMessages.empty();

		if (anyErrors)
		{
			ImGui::TextColored(ImVec4{ 1.0f,0.0f,0.0f,1.0f }, "SCRIPT ERROR !!");
		}

		ImGui::Text("Script: \"%s\"", mScriptPath.c_str());

		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseDoubleClicked(0))
			{
				//TODO :: link to editor opening of notepad++
				//std::cout << "Lua Script in ImGui Double Clicked! Opening Notepad++!" << std::endl;
				//if (!Engine::_ecs.->launcher.OpenTextFile(mScriptPath))
					//std::cout << "[Launcher] Notepad Error" << std::endl;
			}

			ImGui::BeginTooltip();
			ImGui::Text("Path: %s", mScriptPath.c_str());
			ImGui::Text("Double click to open script in Notepad/Notepad++!");
			ImGui::EndTooltip();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FileSystem: .lua"))
			{
				// TODO :: get path from asset browser == returns true if valid path
				//const Path* p = *(const Path**)payload->Data;
				//Engine::GetInstance()->gEditor->GetCommandSystem().EditDataCommand(&mScriptPath, p->GetRelativePath());
				result = true;
			}
			ImGui::EndDragDropTarget();
		}



		// -------------------- Script Status --------------------
		ImGui::BulletText("Script Status : ");
		ImGui::SameLine();
		if (mbIsLoadAttempted)
		{
			if (mbIsLoadSuccessful)
				ImGui::TextColored({ 0.f,1.f,0.f,1.f }, "\tLoad Success!");
			else
				ImGui::TextColored({ 1.f,0.f,0.f,1.f }, "\tLoad Failed!");
		}
		else if (mbHasRuntimeError) {
			ImGui::TextColored({ 1.f,0.f,0.f,1.f }, "RUNTIME ERROR!");
		}
		else {
			ImGui::TextDisabled("\tNot Loaded!");
		}

		// -------------------- Error Messages --------------------
		


		// advanced functionality try later get the file path first

		//static ImVec4 IM_COL_CYAN{ 0.0f,1.0f,1.0f,0.75f };
		//static ImVec4 IM_COL_GREEN{ 0.0f,1.0f,0.0f,0.75f };

		//int recur = 0;

		//int counter = 0;
		//auto fx = [&](auto& fun, auto& table) -> void {
		//	UNREFERENCED_PARAMETER(fun);
		//	std::vector<std::pair<sol::object, sol::object>> vec_fn;
		//	std::vector<std::pair<sol::object, sol::object>> vec_tb;
		//	if (++recur > 3)
		//		return;

		//	for (auto kvp : table)
		//	{
		//		const sol::object& key = kvp.first;
		//		const sol::object& value = kvp.second;
		//		sol::type t = value.get_type();
		//		ImGui::PushID(++counter);
		//		switch (t)
		//		{
		//		case sol::type::boolean:
		//		{
		//			bool val = value.as<bool>();
		//			if (key.is<std::string>()) {
		//				std::string kk = key.as<std::string>();
		//				ImGui::TextColored(IM_COL_CYAN, "%s : ", kk.c_str());
		//				ImGui::SameLine();
		//				if (val) {
		//					if (ImGui::SmallButton(("true##" + kk).c_str())) { table[kk.c_str()] = false; }
		//				}
		//				else {
		//					if (ImGui::SmallButton(("false##" + kk).c_str())) { table[kk.c_str()] = true; }
		//				}
		//			}
		//			else if (key.is<int>()) {
		//				int kki = key.as<int>();
		//				ImGui::TextColored(IM_COL_CYAN, "[%d] : ", kki);
		//				ImGui::SameLine();
		//				if (val) {
		//					if (ImGui::SmallButton("true##")) { table[kki] = false; }
		//				}
		//				else {
		//					if (ImGui::SmallButton("false##")) { table[kki] = true; }
		//				}
		//			}
		//			break;
		//		}
		//		case sol::type::string:
		//		{
		//			std::string val = value.as<std::string>();
		//			if (key.is<std::string>()) {
		//				std::string kk = key.as<std::string>();
		//				ImGui::TextColored(IM_COL_CYAN, "%s : ", kk.c_str());
		//				ImGui::SameLine();
		//				ImGui::Text("%s", val.c_str());
		//			}
		//			else if (key.is<int>()) {
		//				int kki = key.as<int>();
		//				ImGui::TextColored(IM_COL_CYAN, "[%d] : ", kki);
		//				ImGui::SameLine();
		//				ImGui::Text("%s", val.c_str());
		//			}
		//			break;
		//		}
		//		case sol::type::number:
		//		{
		//			float val = value.as<float>();
		//			if (key.is<std::string>()) {
		//				std::string kk = key.as<std::string>();
		//				ImGui::TextColored(IM_COL_CYAN, "%s : ", kk.c_str());
		//				//ImGui::SameLine();
		//				//ImGui::Text("%f", val);

		//				ImGui::SameLine();
		//				ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
		//				if (ImGui::DragFloat(("##" + kk).c_str(), &val)) {
		//					table[kk.c_str()] = val;
		//				}
		//				ImGui::PopItemWidth();
		//			}
		//			else if (key.is<int>()) {
		//				int kki = key.as<int>();
		//				ImGui::TextColored(IM_COL_CYAN, "[%d] : ", kki);
		//				//ImGui::SameLine();
		//				//ImGui::Text("%f", val);
		//				ImGui::SameLine();
		//				ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
		//				if (ImGui::DragFloat("##", &val)) {
		//					table[kki] = val;
		//				}
		//				ImGui::PopItemWidth();
		//			}
		//			break;
		//		}
		//		case sol::type::thread:
		//		{
		//			if (key.is<std::string>()) {
		//				std::string kk = key.as<std::string>();
		//				ImGui::TextColored(IM_COL_GREEN, "(Thread) : ");
		//				ImGui::SameLine();
		//				ImGui::Text("%s", kk.c_str());
		//			}
		//			else if (key.is<int>()) {
		//				int kki = key.as<int>();
		//				ImGui::TextColored(IM_COL_GREEN, "(Thread) : ");
		//				ImGui::SameLine();
		//				ImGui::Text("[%d]", kki);
		//			}
		//			break;
		//		}
		//		case sol::type::userdata:
		//		{
		//			sol::optional<Vector3&> maybe_vector = value.as<sol::optional<Vector3&>>();
		//			if (maybe_vector) {
		//				Vector3& the_thing = maybe_vector.value();
		//				if (key.is<std::string>()) {
		//					std::string kk = key.as<std::string>();
		//					ImGui::TextColored(IM_COL_GREEN, "(Vector3) : ");
		//					ImGui::SameLine();
		//					ImGui::Text("%s", kk.c_str());
		//				}
		//				else if (key.is<int>()) {
		//					int kki = key.as<int>();
		//					ImGui::TextColored(IM_COL_GREEN, "(Vector3) : ");
		//					ImGui::SameLine();
		//					ImGui::Text("[%d]", kki);
		//				}
		//				ImGui::Indent();
		//				ImGui::TextColored({ 0.0f,1.0f,0.0,1.0f }, " (x:%.2f , y:%.2f , z:%.2f)", the_thing.x, the_thing.y, the_thing.z);
		//				ImGui::Unindent();
		//				break;
		//			}



		//			sol::optional<GameObject&> maybe_gameobject = value.as<sol::optional<GameObject&>>();
		//			if (maybe_gameobject) {
		//				GameObject& the_thing = maybe_gameobject.value();
		//				if (key.is<std::string>()) {
		//					std::string kk = key.as<std::string>();
		//					ImGui::TextColored(IM_COL_GREEN, "(GameObject) : ");
		//					ImGui::SameLine();
		//					ImGui::Text("%s", kk.c_str());
		//				}
		//				else if (key.is<int>()) {
		//					int kki = key.as<int>();
		//					ImGui::TextColored(IM_COL_GREEN, "(GameObject) : ");
		//					ImGui::SameLine();
		//					ImGui::Text("[%d]", kki);
		//				}
		//				ImGui::Indent();
		//				ImGui::TextColored({ 0.0f,1.0f,0.0,1.0f }, "Reference : %s", the_thing.GetName().c_str());
		//				ImGui::Unindent();
		//				break;
		//			}

		//			sol::optional<TransformCom&> maybe_transform = value.as<sol::optional<TransformCom&>>();
		//			if (maybe_transform) {
		//				TransformComponent& the_thing = maybe_transform.value();
		//				if (key.is<std::string>()) {
		//					std::string kk = key.as<std::string>();
		//					ImGui::TextColored(IM_COL_GREEN, "(Transform) : ");
		//					ImGui::SameLine();
		//					ImGui::Text("%s", kk.c_str());
		//				}
		//				else if (key.is<int>()) {
		//					int kki = key.as<int>();
		//					ImGui::TextColored(IM_COL_GREEN, "(Transform) : ");
		//					ImGui::SameLine();
		//					ImGui::Text("[%d]", kki);
		//				}
		//				ImGui::Indent();
		//				GameObject* owner = the_thing.GetOwnerObject();
		//				if (owner)
		//					ImGui::TextColored({ 0.0f,1.0f,0.0,1.0f }, "Reference : %s", the_thing.GetOwnerObject()->GetName().c_str());
		//				ImGui::Unindent();
		//				break;
		//			}

		//			sol::optional<LuaScriptCom&> maybe_luascript = value.as<sol::optional<LuaScriptCom&>>();
		//			if (maybe_luascript) {
		//				LuaScriptComponent& the_thing = maybe_luascript.value();
		//				if (key.is<std::string>()) {
		//					std::string kk = key.as<std::string>();
		//					ImGui::TextColored(IM_COL_GREEN, "(Lua Script) : ");
		//					ImGui::SameLine();
		//					ImGui::Text("%s", kk.c_str());
		//				}
		//				else if (key.is<int>()) {
		//					int kki = key.as<int>();
		//					ImGui::TextColored(IM_COL_GREEN, "(Lua Script) : ");
		//					ImGui::SameLine();
		//					ImGui::Text("[%d]", kki);
		//				}
		//				ImGui::Indent();
		//				GameObject* owner = the_thing.GetOwnerObject();
		//				if (owner)
		//					ImGui::TextColored({ 0.0f,1.0f,0.0,1.0f }, "Reference : %s", the_thing.GetOwnerObject()->GetName().c_str());
		//				ImGui::Unindent();
		//				break;
		//			}

		//			sol::optional<Timer&> maybe_timer = value.as<sol::optional<Timer&>>();
		//			if (maybe_timer) {
		//				//Timer& the_thing = maybe_timer.value();
		//				if (key.is<std::string>()) {
		//					std::string kk = key.as<std::string>();
		//					ImGui::TextColored(IM_COL_GREEN, "(Timer) : ");
		//					ImGui::SameLine();
		//					ImGui::Text("%s", kk.c_str());
		//				}
		//				else if (key.is<int>()) {
		//					int kki = key.as<int>();
		//					ImGui::TextColored(IM_COL_GREEN, "(Timer) : ");
		//					ImGui::SameLine();
		//					ImGui::Text("[%d]", kki);
		//				}
		//				break;
		//			}

		//			if (key.is<std::string>()) {
		//				std::string kk = key.as<std::string>();
		//				ImGui::TextColored(IM_COL_GREEN, "(Object) : ");
		//				ImGui::SameLine();
		//				ImGui::Text("%s", kk.c_str());
		//			}
		//			else if (key.is<int>()) {
		//				int kki = key.as<int>();
		//				ImGui::TextColored(IM_COL_GREEN, "(Object) : ");
		//				ImGui::SameLine();
		//				ImGui::Text("[%d]", kki);
		//			}
		//			break;
		//		}
		//		case sol::type::function:
		//		{
		//			vec_fn.emplace_back(kvp);
		//			break;
		//		}
		//		case sol::type::table:
		//		{
		//			vec_tb.emplace_back(kvp);
		//			break;
		//		}
		//		}//END switch
		//		ImGui::PopID();
		//	}//END LOOP

		//	for (auto key_value_pair : vec_fn)
		//	{
		//		const sol::object& key = key_value_pair.first;
		//		const sol::object& value = key_value_pair.second;
		//		sol::type t = value.get_type();
		//		UNREFERENCED_PARAMETER(value);
		//		UNREFERENCED_PARAMETER(t);
		//		if (key.is<std::string>()) {
		//			std::string  kk = key.as<std::string>();
		//			ImGui::TextColored(IM_COL_GREEN, "(Function) : ");
		//			ImGui::SameLine();
		//			ImGui::Text("%s", kk.c_str());
		//		}
		//		else if (key.is<int>()) {
		//			int kki = key.as<int>();
		//			ImGui::TextColored(IM_COL_GREEN, "(Function) : ");
		//			ImGui::SameLine();
		//			ImGui::Text("[%d]", kki);
		//		}
		//	}

		//	for (auto key_value_pair : vec_tb)
		//	{
		//		const sol::object& key = key_value_pair.first;
		//		const sol::object& value = key_value_pair.second;
		//		if (key.is<std::string>()) {
		//			std::string  kk = key.as<std::string>();
		//			ImGui::TextColored(ImVec4{ 0.0f,1.0f,0.0f,0.75f }, "(Table) : ");
		//			ImGui::SameLine();
		//			ImGui::Text("%s", kk.c_str());
		//		}
		//		else if (key.is<int>()) {
		//			int kki = key.as<int>();
		//			ImGui::TextColored(ImVec4{ 0.0f,1.0f,0.0f,0.75f }, "(Table) : ");
		//			ImGui::SameLine();
		//			ImGui::Text("[%d]", kki);
		//		}
		//		sol::table inner = value.as<sol::table>();
		//		ImGui::Indent();
		//		//fun(fun, inner);
		//		ImGui::Unindent();

		//	}
		//};

		//if (ImGui::TreeNode("Lua Introspection"))
		//{
		//	if (mbIsLoadSuccessful) {
		//		fx(fx, mEnvironment);
		//	}

		//	ImGui::TreePop();
		//}//ImGui::TreeNode

		ImGui::TreePop();
	}

	ImGui::PopID();

	if (result)
		mbEditorLoaded = false;

	return result;
}

/// <summary>
/// Deserializer function for LuaScript
/// </summary>
/// <param name="doc">json file</param>
/// <param name="id">file name</param>
void LuaScriptCom::Deserialize(const rapidjson::Document& doc, std::string id)
{
	(void)doc;
	(void)id;
}

/// <summary>
/// Serializer function for LuaScript
/// </summary>
/// <param name="writer">PrettyWriter object</param>
void LuaScriptCom::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{
	(void)writer;
}

/// <summary>
/// Function prints the error that occurs in LUA to console
/// </summary>
/// <param name="result">Type of error</param>
inline void LuaScriptCom::BasicFunctionErrorHandler(const sol::protected_function_result& result)
{
	// Safety Check and Error Handling
	if (!result.valid()) {
		sol::error err = result;
		sol::call_status status = result.status();
		std::cout << "SCRIPT: " << sol::to_string(status) << " error" << "\n\t" << err.what() << std::endl;
		mErrorMessages.push_back(std::make_pair<>(sol::to_string(status), err.what()));
		mbHasRuntimeError = true;// Set Runtime Error Flag
	}
}