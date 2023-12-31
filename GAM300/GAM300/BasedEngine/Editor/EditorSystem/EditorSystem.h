/**********************************************************************************
* \file EditorSystem.h
* \brief File contains the function definitions for all functions that are used
		 for the ImGui editor system.
* \author Shaikh Faid Bin Omar, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include <System/System.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <ImGuizmo.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>
#include <Properties.h>
#include "Camera/CameraCom.h"
#include "../Graphics/TransformCom.h"
#include "EditorUI.h"

class EditorSystem : public ISystem
{
	//Entity ID reference for camera object
	//used during updates/rendering only
	EntityID _camID{};
public:
	void init();
	void load();
	void update(float delta_time);
	void unload();
	void release();

	void getWinPtr(GLFWwindow* wptr);
	~EditorSystem() = default;

	struct SystemTimer
	{
		std::pair<std::string, double> systemTime;
		std::string systemName;
		double startTime;
		EditorSystem* editor;
		SystemTimer() = delete;
		SystemTimer(std::string name, EditorSystem* e) : systemName{ name }, startTime{ glfwGetTime() }, editor{ e }{}

		~SystemTimer()
		{
			systemTime = std::make_pair(systemName, glfwGetTime() - startTime);
			editor->AddSysTime(systemTime);
		}
	};

	std::list<std::pair<std::string, double>> timerList;
	void AddSysTime(std::pair<std::string, double> time);

	GLFWwindow* _winPtr{nullptr};
	EditorUI editorWindow{};
	int imgui_editor_mode{};

	void setCamID(const EntityID&);
	static bool _RenderObjWireframe;
	static bool _RenderObjAABB;
	static bool _RenderLightBox;
	static bool _PlayInFullScreen;
	static bool _RenderUI;
	static EntityID _selectedEntity;
};