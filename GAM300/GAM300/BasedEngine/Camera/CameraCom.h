/******************************************************************************
* \file			CameraCom.h
* \brief		Contain all the Camera Component related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2021 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "Vector3.h"
#include "Matrix4.h"
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"
#include <Properties.h>
#include <Component/Component.h>
#include "Serializer/ISerializer.h"

struct CameraCom : Component, public ISerializer
{
	Vector3		_pos{ 0.0f,0.0f,0.0f };
	Vector3		_up{ 0.0f,0.0f,0.0f };
	Vector3		_rotation{ 0.0f,0.0f,0.0f };
	Vector3		_camEye;

	float		_displaceRight{ 1.0f };
	float		_displaceUp{ 1.0f };
	float		_displaceFor{ 1.0f };

	float		_m_fov{ 0.0f };
	float		_m_near{ 0.0f };
	float		_m_far{ 0.0f };

	float		_movementSpeed{ 1.0f };
	float		_aggressive{ 10.0f };

	Matrix4		_proj_mtx{};
	Matrix4		_view_mtx{};

	CameraCom() = default;
	CameraCom(glm::vec3 pos, float mFov, float mNear, float mFar, glm::mat4 proj, glm::mat4 view) :
		_pos{ pos },
		_m_fov{ mFov },
		_m_near{ mNear },
		_m_far{ mFar },
		_proj_mtx{ proj },
		_view_mtx{ view }
	{}
	CameraCom(const CameraCom& rhs) :
		_pos{ rhs._pos },
		_up{ rhs._up },
		_rotation{ rhs._rotation },
		_displaceRight{ rhs._displaceRight },
		_displaceUp{ rhs._displaceUp },
		_displaceFor{ rhs._displaceFor },
		_m_fov{ rhs._m_fov },
		_m_near{ rhs._m_near },
		_m_far{ rhs._m_far },
		_movementSpeed{ rhs._movementSpeed },
		_proj_mtx{ rhs._proj_mtx },
		_view_mtx{ rhs._view_mtx },
		_aggressive{rhs._aggressive}
	{}
	CameraCom& operator=(const CameraCom& rhs)
	{
		if (this != &rhs)
		{
			_pos = { rhs._pos };
			_up = { rhs._up };
			_rotation = { rhs._rotation };

			_displaceRight = { rhs._displaceRight };
			_displaceUp = { rhs._displaceUp };
			_displaceFor = { rhs._displaceFor };

			_m_fov = { rhs._m_fov };
			_m_near = { rhs._m_near };
			_m_far = { rhs._m_far };

			_movementSpeed = { rhs._movementSpeed };
			_aggressive = { rhs._aggressive };

			_proj_mtx = { rhs._proj_mtx };
			_view_mtx = { rhs._view_mtx };
		}
		return *this;
	}

	void Deserialize(const rapidjson::Document& doc, std::string id);
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);

	void destroyEntity(EntityID entity) override
	{
		(void)entity;
	}
	void cloneComponent(EntityID entity1, EntityID entity2) override
	{
		(void)entity1;
		(void)entity2;
	}

	static void RegisterLua(sol::state& lua);
	property_vtable()
};

//property_begin_name(CameraCom, "Camera Component")
property_begin(CameraCom)
{
	property_var(_pos)
		, property_var(_up)
		, property_var(_displaceRight)
		, property_var(_displaceUp)
		, property_var(_displaceFor)
		, property_var(_movementSpeed)
		, property_var(_rotation)
		, property_var(_m_fov)
		, property_var(_m_near)
		, property_var(_m_far)
		, property_var(_aggressive)

} property_vend_h(CameraCom)
