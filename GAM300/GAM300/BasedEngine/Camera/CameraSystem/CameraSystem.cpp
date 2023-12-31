#include "CameraSystem.h"
#include "Engine.h"
#include <Graphics/TransformCom.h>
#include <Physics/RigidBodyComponent.h>
#include <Editor/EditorSystem/EditorSystem.h>
#include <Graphics/Window/Time/Time.h>
#include <algorithm>

void CameraSystem::init()
{

}

void CameraSystem::load()
{
	_editorCamera = Engine::_ecs.getComponent<CameraCom>(
		Engine::_ecs.getEntityID("Camera"));
	_playerCamera1 = Engine::_ecs.getComponent<CameraCom>(
		Engine::_ecs.getEntityID("PlayerCamera1"));
	_playerCamera2 = Engine::_ecs.getComponent<CameraCom>(
		Engine::_ecs.getEntityID("PlayerCamera2"));
	
	_playerCamera = _playerCamera1;
}

void CameraSystem::update(float dt)
{
#ifdef SHOW_IMGUI
	auto& imguiVeiwPort = Engine::_ecs.getSystem<EditorSystem>()->editorWindow._viewportPanalSize;
	if (imguiVeiwPort.x && imguiVeiwPort.y)
#else
	if (Engine::_ecs.getSystem<WindowSystem>()->getWindowWidth() && Engine::_ecs.getSystem<WindowSystem>()->getWindowHeight())
#endif
	{
		resetClickPoint();
		if (_camType == CameraType::EDITOR)
		{
			editorCamUpdate(dt);
		}
		else if (_camType == CameraType::PLAYER)
		{
			playerCamUpdate(dt);
			glm::mat4 view = Matrix4(_playerCamera->_view_mtx).Mtx4toGlmMat4();
			glm::vec3 camEye = glm::vec3(-view[0][2], -view[1][2], -view[2][2]);
			_playerCamera->_camEye = glm::normalize(camEye);
		}
	}

}

void CameraSystem::unload()
{
}

void CameraSystem::release()
{
}

void CameraSystem::resetClickPoint()
{
	_prevClickPoint = _clickPoint;
	_clickPoint = Inputs::get_MosPos();
}

void CameraSystem::editorCamUpdate(float dt)
{
	editorCamMovement(dt);
	// set the cam rotation
	Vector3 convertedRotate = _editorCamera->_rotation * ((float)M_PI / 180.0f);
	glm::mat4
		xRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.x,glm::vec3(1.0f,0.0f,0.0f)) },
		yRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.y,glm::vec3(0.0f,1.0f,0.0f)) },
		zRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.z,glm::vec3(0.0f,0.0f,1.0f)) };

	Matrix4 oRotate = xRotate * yRotate * zRotate;
	dt *= _editorCamera->_movementSpeed;
	Vector3 zDir = oRotate.getZdir() * _forward * dt;
	auto xDir = oRotate.getXdir() * _right * dt;
	auto yDir = oRotate.getYdir() * _up * dt;

	_editorCamera->_up = oRotate.getYdir();

	if (_up)
	{
		_editorCamera->_pos.y += yDir.y;
	}
	else if (_forward)
	{
		_editorCamera->_pos.x += xDir.x - zDir.x - yDir.x;
		if (oRotate.getZdir().z > 0.0f)
			_editorCamera->_pos.y += yDir.y - xDir.y - zDir.y;
		else
			_editorCamera->_pos.y -= yDir.y - xDir.y - zDir.y;
		_editorCamera->_pos.z += zDir.z - xDir.z - yDir.z;
	}
	else if (_right)
	{
		_editorCamera->_pos.x += xDir.x - zDir.x - yDir.x;
		_editorCamera->_pos.z += zDir.z - xDir.z - yDir.z;
	}
	glm::mat4 oTranslate(glm::translate(glm::identity<glm::mat4>(), _editorCamera->_pos.Vec3toGlmVec3()));

	glm::mat4 world_cam = oTranslate * oRotate.Mtx4toGlmMat4();
	auto& imguiVeiwPort = Engine::_ecs.getSystem<EditorSystem>()->editorWindow._viewportPanalSize;
	_editorCamera->_proj_mtx = glm::perspective(
		glm::radians(_editorCamera->_m_fov),
		imguiVeiwPort.x / imguiVeiwPort.y,
		_editorCamera->_m_near,
		_editorCamera->_m_far);

	

	_editorCamera->_view_mtx = glm::inverse(world_cam);
}

void CameraSystem::editorCamMovement(float dt)
{
	(void)dt;
	auto diff = (_clickPoint - _prevClickPoint) * dt * _editorCamera->_movementSpeed;
	// get the rotation
	if (Inputs::get_Mos(1) && _clickPoint != _prevClickPoint)
	{
		_editorCamera->_rotation.x += diff.y;
		_editorCamera->_rotation.y += diff.x;
	}
	// set movement
	//z
	if (Inputs::get_Mos(1) && Inputs::get_Key(GLFW_KEY_S))
	{
		//m_rot_x += 3.14f*m_delta_time;
		_forward = 1.0f;
	}
	else if (Inputs::get_Mos(1) && Inputs::get_Key(GLFW_KEY_W)/* || (Inputs::get_MosScroll_Trigger() == -1.0 && Inputs::get_Key(GLFW_KEY_LEFT_CONTROL))*/)
	{
		//m_rot_x -= 3.14f*m_delta_time;
		_forward = -1.0f;
	}
	else
	{
		_forward = 0.0f;
	}
	// x
	if (Inputs::get_Mos(1) && Inputs::get_Key(GLFW_KEY_A))
	{

		_right = -1.0f;
	}
	else if (Inputs::get_Mos(1) && Inputs::get_Key(GLFW_KEY_D))
	{
		//m_rot_y -= 3.14f*m_delta_time;
		_right = 1.0f;
	}
	else
	{
		_right = 0.0f;
	}
	// y

	if (Inputs::get_Mos(1) && Inputs::get_Key(GLFW_KEY_Q))
	{

		_up = -1.0f;
	}
	else if (Inputs::get_Mos(1) && Inputs::get_Key(GLFW_KEY_E))
	{
		//m_rot_y -= 3.14f*m_delta_time;
		_up = 1.0f;
	}
	else
	{
		_up = 0.0f;
	}
}

void CameraSystem::playerCamUpdate(float dt)
{
	playerCamMovement(dt);
	/*std::cout << "x: " << _playerCamera->_pos.x
		<< "y: " << _playerCamera->_pos.y
		<< "z: " << _playerCamera->_pos.z << std::endl;*/
	Vector3 convertedRotate = _playerCamera->_rotation * ((float)M_PI / 180.0f);
	glm::mat4
		xRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.x,glm::vec3(1.0f,0.0f,0.0f)) },
		yRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.y,glm::vec3(0.0f,1.0f,0.0f)) },
		zRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.z,glm::vec3(0.0f,0.0f,1.0f)) };

	Matrix4 oRotate = yRotate * xRotate;
	auto zDir = oRotate.getZdir() * _playerCamera->_displaceFor;

	auto tranformCom = Engine::_ecs.getComponent<TransformCom>(
		Engine::_ecs.getEntityID("Player"));

	glm::vec3 camRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), zDir.Vec3toGlmVec3()));
	glm::vec3 camUp = glm::normalize(glm::cross(zDir.Vec3toGlmVec3(), camRight));
	glm::vec3 shake = cameraShake(camRight, camUp, dt);

	glm::vec3 displacementForThirdPer = camRight * _playerCamera->_displaceRight + camUp * _playerCamera->_displaceUp;
	if (_screenShake)
		_playerCamera->_pos = tranformCom->pos + zDir + displacementForThirdPer + shake;
	else
		_playerCamera->_pos = tranformCom->pos + zDir + displacementForThirdPer;

	glm::mat4 oTranslate(glm::translate(glm::identity<glm::mat4>(), _playerCamera->_pos.Vec3toGlmVec3()));

	glm::mat4 world_cam = oTranslate * oRotate.Mtx4toGlmMat4();

#ifdef SHOW_IMGUI	
	if (EditorSystem::_PlayInFullScreen)
	{
		_playerCamera->_proj_mtx = glm::perspective(
			glm::radians(_playerCamera->_m_fov),
			((float)Engine::_ecs.getSystem<WindowSystem>()->getWindowWidth() /
				(float)Engine::_ecs.getSystem<WindowSystem>()->getWindowHeight()),
			_playerCamera->_m_near,
			_playerCamera->_m_far);
	}
	else
	{
		auto& imguiVeiwPort = Engine::_ecs.getSystem<EditorSystem>()->editorWindow._viewportPanalSize;
		_playerCamera->_proj_mtx = glm::perspective(
			glm::radians(_playerCamera->_m_fov),
			imguiVeiwPort.x / imguiVeiwPort.y,
			_playerCamera->_m_near,
			_playerCamera->_m_far);
	}
#else
	_playerCamera->_proj_mtx = glm::perspective(
		glm::radians(_playerCamera->_m_fov),
		((float)Engine::_ecs.getSystem<WindowSystem>()->getWindowWidth() /
			(float)Engine::_ecs.getSystem<WindowSystem>()->getWindowHeight()),
		_playerCamera->_m_near,
		_playerCamera->_m_far);
#endif

	_playerCamera->_up = camUp;
	if (_screenShake)
		_playerCamera->_view_mtx = glm::lookAt(
			_playerCamera->_pos.Vec3toGlmVec3(),
			tranformCom->pos.Vec3toGlmVec3() + displacementForThirdPer + shake,
			camUp);
	else
		_playerCamera->_view_mtx = glm::lookAt(
			_playerCamera->_pos.Vec3toGlmVec3(),
			tranformCom->pos.Vec3toGlmVec3() + displacementForThirdPer,
			camUp);
}

void CameraSystem::playerCamMovement(float dt)
{
	(void)dt;
	Vector2 diff = (_clickPoint - _prevClickPoint) * dt * _playerCamera->_movementSpeed;

	//logitech
	if (Inputs::JoystickPresent == 1)
	{
		Vector2 temp{0.0f, 0.0f};
		if (Inputs::get_GamePatButton_RightJoystick_Xaxis() > 0.5f || Inputs::get_GamePatButton_RightJoystick_Xaxis() < -0.5f)
		{
			//std::cout << "test" << std::endl;
			temp.x = Inputs::get_GamePatButton_RightJoystick_Xaxis() * _playerCamera->_movementSpeed;
		}
		if (Inputs::get_GamePatButton_RightJoystick_Yaxis() > 0.5f || Inputs::get_GamePatButton_RightJoystick_Yaxis() < -0.5f)
		{
			//std::cout << "test" << std::endl;
			temp.y = Inputs::get_GamePatButton_RightJoystick_Yaxis() * _playerCamera->_movementSpeed;
		}
		if (temp.x != 0.0f || temp.y != 0.0f)
		{
			diff = temp;
		}
		/*std::cout << "L x: " << Inputs::get_GamePatButton_LeftJoystick_Xaxis() << std::endl;
		std::cout << "L y: " << Inputs::get_GamePatButton_LeftJoystick_Yaxis() << std::endl;
		std::cout << "R x: " << diff.x << std::endl;
		std::cout << "R y: " << diff.y << std::endl << std::endl;*/
	}

	// get the rotation
	_playerCamera->_rotation.x -= diff.y;
	if (_playerCamera->_rotation.x > 85.0f)
		_playerCamera->_rotation.x = 85.0f;
	else if (_playerCamera->_rotation.x < -85.0f)
		_playerCamera->_rotation.x = -85.0f;
	_playerCamera->_rotation.y -= diff.x;
	if (_playerCamera->_rotation.y > 360.0f)
		_playerCamera->_rotation.y -= 360.0f;
	else if (_playerCamera->_rotation.y < 0.0f)
		_playerCamera->_rotation.y += 360.0f;
}

glm::vec3 CameraSystem::cameraShake(glm::vec3& right, glm::vec3& up, float /*dt*/)
{
	if (Time::APP_TIME - _updatedTime >= Time::_fixedDeltaTime)
	{
		_updatedTime = Time::APP_TIME;
		++_counter;
	}
	int temp = _counter % 4;
	switch (temp)
	{
	case 0:
		return _playerCamera->_aggressive * right;
	case 1:
		return -_playerCamera->_aggressive * right;
	case 2:
		return _playerCamera->_aggressive * up;
	case 3:
		return -_playerCamera->_aggressive * up;
	default:
		break;
	}
	return glm::vec3();
}

void CameraSystem::updateCollisionCamera(float dt)
{
	Vector3 convertedRotate = _playerCamera2->_rotation * ((float)M_PI / 180.0f);
	glm::mat4
		xRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.x,glm::vec3(1.0f,0.0f,0.0f)) },
		yRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.y,glm::vec3(0.0f,1.0f,0.0f)) },
		zRotate{ glm::rotate(glm::identity<glm::mat4>(),convertedRotate.z,glm::vec3(0.0f,0.0f,1.0f)) };

	Matrix4 oRotate = yRotate * xRotate;
	auto zDir = oRotate.getZdir() * _playerCamera2->_displaceFor;

	auto tranformCom = Engine::_ecs.getComponent<TransformCom>(
		Engine::_ecs.getEntityID("Player"));

	glm::vec3 camRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), zDir.Vec3toGlmVec3()));
	glm::vec3 camUp = glm::normalize(glm::cross(zDir.Vec3toGlmVec3(), camRight));
	glm::vec3 shake = cameraShake(camRight, camUp, dt);

	glm::vec3 displacementForThirdPer = camRight * _playerCamera2->_displaceRight + camUp * _playerCamera2->_displaceUp;
	if (_screenShake)
		_playerCamera2->_pos = tranformCom->pos + zDir + displacementForThirdPer + shake;
	else
		_playerCamera2->_pos = tranformCom->pos + zDir + displacementForThirdPer;
}

std::shared_ptr<CameraCom> CameraSystem::getPlayerCamera()
{
	return _playerCamera;
}

float CameraSystem::getAppTime()
{
	return Time::APP_TIME;
}