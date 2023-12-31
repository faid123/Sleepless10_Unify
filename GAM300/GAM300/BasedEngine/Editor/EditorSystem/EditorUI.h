/**********************************************************************************
* \file EditorUI.h
* \brief File contains the function definitions for all functions that are used
		 to provide the editor utility.
* \author Shaikh Faid Bin Omar, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include <imgui.h>
#include <ImGuizmo.h>
#include "../../lib/imgui-docking/ImGuiFileBrowser.h"
#include <list>
#include <Graphics/TransformCom.h>
#include <Physics/RigidBodyComponent.h>
#include <Camera/CameraCom.h>
#include "Physics/PhysicsSystem.h"
#include <Camera/CameraSystem/CameraSystem.h>
#include <Graphics/LightCom.h>
#include <Graphics/MaterialCom.h>
#include "Particles/EmitterComponent.h"
#include "Particles/ParticleComponent.h"
#include "Logic/Lua/LuaScriptCom.h"
#include "Logic/Lua/ScriptingSystem.h"
#include "Animation/AnimationCom.h"
#include "Audio/AudioSrcCom.h"
#include "Animation/AnimationCom.h"

#define PROPERTY_EDITOR
#ifndef _PROPERTY_H
#include "Properties.h"
#endif

#ifndef EDITORUI_H
#define EDITORUI_H

class EditorUI
{
public:
	void showTopMenu();
	void showFps();
	void showEntity();
	void showPrefab();
	void showProperty();
	void showParenting();
	void showWayPoint();
	void updateParentChildProperty(std::string str, property::data Data, std::string second);
	void updatePrefabsProperty();
	void showPropertyPrefab();
	void showTags();
	void showComponents();
	void showConsole();
	void showViewport();
	void showDebug(float delta_time, std::list<std::pair<std::string, double>>& _TimerList);
	//void setMouseRay(float screenWidth, float screenHeight);
	//Vector3 rayCast(Vector3 ray_start, Vector3 ray_direction, float t);
	//void showDebug(float delta_time, std::list<std::pair<std::string, double>> _TimerList, bool &bWireFrame);
	void showCreateEntity();
	std::string newName(std::string name);
	void setTextureName(std::string name);
	std::string getTextureName();
	void ShowHelpMarker(const char* desc);

	void ShowAudioInspector();

	//Gizmoss
	void EditTransform(const std::shared_ptr<CameraCom>& camera, std::shared_ptr<TransformCom>& trans);
	int _gizmoType = -1;



	template <typename T2>
	T2 getComponentType(std::string comp_name)
	{
		if (comp_name == "LightCom")
			return LightCom;
		else if (comp_name == "TransformCom")
			return TransformCom;
		else if (comp_name == "CameraCom")
			return CameraCom;
		else if (comp_name == "RigidStaticCom")
			return RigidStaticCom;
		else if (comp_name == "RigidDynamicCom")
			return RigidDynamicCom;
		else if (comp_name == "uaScriptCom")
			return LuaScriptCom;
		else if (comp_name == "AudioSrcCom")
			return AudioSrcCom;
		else if (comp_name == "AnimationCom")
			return AnimationCom;
		else
			return NULL;
	}
	using example_entry = std::pair<const property::table&, void*>;
	template< typename T >
	std::pair<const char*, example_entry> CreateInstance()
	{
		static T Instance;
		Instance.DefaultValues();
		return { property::getTable(Instance).m_pName, { property::getTable(Instance), &Instance } };
	}
	template< typename... T_ARGS >
	struct examples
	{
		examples(T_ARGS&&... args)
			: m_Tables{ args.second... }
			, m_Names{ args.first ... }
		{}

		std::array<example_entry, sizeof...(T_ARGS) >   m_Tables;
		std::array<const char*, sizeof...(T_ARGS) >     m_Names;
	};
	std::array<std::string, 2>   Inspector{ "Examples", "Settings" };
	ImVec2 _viewportPanalSize{};

private:
	// FOR FILE EXPLORER
	imgui_addons::ImGuiFileBrowser file_dialog;
	bool open_explorer;
	std::string texture_name;

	Vector2 _viewportBounds[2];
	int  _mousePosition_x, _mousePosition_y;
	Vector3 _rayDirWorld;
	Vector3 _rayCast;

	std::stringstream console_buffer;
	//redirects cout buffer to the stringstream.
	std::streambuf* old_console_buffer = std::cout.rdbuf(console_buffer.rdbuf());
	ImGuiTextBuffer	bufGI;
};
#endif // !EDITORUI_H