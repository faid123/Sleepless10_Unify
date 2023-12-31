/******************************************************************************
* \file			CameraSystem.h
* \brief		Contain all the Camera related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2021 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#pragma once
#include "System/System.h"
#include "Camera/CameraCom.h"
#include "Input.h"
#include "Graphics/Window/Window/Window.h"
#include "Vector2.h"
#include "Quat.h"
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"
/// <summary>
/// how camera operate in the system
/// </summary>
class CameraSystem : public ISystem
{
public:
	/// <summary>
	/// init for camera system
	/// </summary>
	void init() override;
	/// <summary>
	/// load for camera system
	/// </summary>
	void load() override;
	/// <summary>
	/// update for camera system
	/// </summary>
	/// <param name="dt"> delta time</param>
	void update(float dt) override;
	/// <summary>
	/// unload for camera system
	/// </summary>
	void unload() override;
	/// <summary>
	/// release for camera system
	/// </summary>
	void release() override;
	/// <summary>
	/// Camera type that we have
	/// </summary>
	enum class CameraType
	{
		PLAYER,
		EDITOR
	};
	/// <summary>
	/// re-setting or refreshing click point as we save the prev mos pos (should be place in input)
	/// </summary>
	void resetClickPoint();
	/// <summary>
	/// updates editor camera when it is determined its the editor view
	/// </summary>
	/// <param name="dt">delta time</param>
	void editorCamUpdate(float dt);
	/// <summary>
	/// how editor camera move 
	/// </summary>
	/// <param name="dt">delta time</param>
	void editorCamMovement(float dt);
	/// <summary>
	/// updates player camera when it is determined its the player view
	/// </summary>
	/// <param name="dt">delta time</param>
	void playerCamUpdate(float dt);
	/// <summary>
	/// how player camera move 
	/// </summary>
	/// <param name="dt">delta time</param>
	void playerCamMovement(float dt);
	/// <summary>
	/// fuction that return the amount of movement and making it look it is 
	/// shaking
	/// </summary>
	/// <param name="right">right vector of the cam</param>
	/// <param name="up">up vector of the cam</param>
	/// <param name="dt">delta time</param>
	/// <returns>value it needs to move</returns>
	glm::vec3 cameraShake(glm::vec3& right, glm::vec3& up, float dt);
	/// <summary>
	/// update collision camera position 
	/// </summary>
	/// <param name="dt">delta time</param>
	void updateCollisionCamera(float dt);
	/// <summary>
	/// geting game time
	/// </summary>
	/// <returns>game time</returns>
	float getAppTime();
	/// <summary>
	/// register for lua to use
	/// </summary>
	static void RegisterLua(sol::state& lua);
	bool _screenShake = false;
	CameraType	_camType{ CameraType::EDITOR };
	std::shared_ptr<CameraCom> _editorCamera{ nullptr };
	/// <summary>
	/// getter function for player camera
	/// </summary>
	/// <returns>ptr to player camera</returns>
	std::shared_ptr<CameraCom> getPlayerCamera();
	std::shared_ptr<CameraCom> _playerCamera{ nullptr };
private:
	
	std::shared_ptr<CameraCom> _playerCamera1{ nullptr };
	std::shared_ptr<CameraCom> _playerCamera2{ nullptr };

	
	Vector2		_clickPoint{};
	Vector2		_prevClickPoint{};

	float		_forward{ 0.0f };
	float		_right{ 0.0f };
	float		_up{ 0.0f };
	int			_counter{ 0 };
	float		_updatedTime{ 0.0f };
};