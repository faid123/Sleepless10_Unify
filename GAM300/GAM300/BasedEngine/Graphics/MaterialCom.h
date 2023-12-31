/**********************************************************************************
* \file MaterialCom.h
* \brief File contains the function definitions for all functions that are used
		 in Material component.
* \author Willy Lau, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "Graphics/Graphics.h"
#include <glm/gtc/type_ptr.hpp>
//#include "Serializer/ISerializer.h"
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

#include "Serializer/ISerializer.h"
#include <Properties.h>


struct MaterialCom : Component , public ISerializer
{
	//===serialise these values===//
	std::string m_model{ "default" };
	std::string m_shader{ "forward_shader" };
	std::string m_texture{ "none" };

	float _alpha{ 1.0f };
	Vector3 _saturation{ 1.0f};
	//=========================//
	bool isInitialised;

	using model_map = std::map<std::string, GLModel>;

	using model_iterator = std::map<std::string, GLModel>::iterator;
	using shader_iterator = std::map<std::string, GLSLShader>::iterator;
	using tex_iterator = std::map<std::string, Texture>::iterator;

	model_iterator model_ref;
	shader_iterator shader_ref;
	tex_iterator tex_ref;

	model_map models;

	int entityID;
	//For minimap, currently there is only player and enemy
	std::string entityTag{};

	//if no texture, this is the default colour
	Vector3 colour{ 1.0f };

	MaterialCom() :
		isInitialised{false}
	{
	}
	
	void DefaultValues(void) noexcept
	{
		isInitialised = false;
		_alpha = 1.0f;
	}
	MaterialCom(const std::string& mdl, const std::string& shd, const std::string& tex, const float& alpha = 1.0f) :
		isInitialised{ false },
		m_model{ mdl },
		m_shader{ shd },
		m_texture{ tex },
		_alpha{alpha}
	{}

	void SetModel(const std::string& mod_name)
	{
		m_model = mod_name;
		m_texture = mod_name;
		isInitialised = false;
	}
	void SetShader(const std::string& shd_name)
	{
		m_shader = shd_name;
		isInitialised = false;
	}
	void SetTexture(const std::string& tex_name)
	{
		m_texture = tex_name;
		isInitialised = false;
	}

	void destroyEntity(EntityID entity) override
	{
		(void)entity;
	}
	void cloneComponent(EntityID entity1, EntityID entity2) override
	{
		(void)entity1;
		(void)entity2;
	}
	void ResetSaturation()
	{
		_saturation = glm::vec3(1.0f);
	}


	void Deserialize(const rapidjson::Document& doc, std::string id);
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);

	static void RegisterLua(sol::state& lua);
	property_vtable()
};
//property_begin_name(MaterialCom, "Material Component")
property_begin(MaterialCom)
{
	property_var(_alpha)
	, property_var(colour)
	, property_var(m_model)
	, property_var(m_shader)
	, property_var(m_texture)
	, property_var(_saturation).Name("HSV")

} property_vend_h(MaterialCom)
