/******************************************************************************
* \file			EmitterComponent.h
* \brief		Contain all the Camera related function
* \author 		Samuel Chia, 100% Code Contribution
*
* \copyright	Copyright (C) 2022 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/

#pragma once
#include "../ECS/Component/Component.h"
#include "../ECS/Component/ComponentManager.h"
#include "../ECS/Entity/Entity.h"
#include "Engine.h"
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"
#include <Properties.h>


struct EmitterCom : Component, public ISerializer
{
	bool isInitialised;

	EmitterCom() :
		isInitialised{ false }
	{
	}
	
	float timer = 0.0f;
	float timeset = 0.0f;
	int poolIndex = 0;
	int maxParticles = 50;
	//std::vector<EntityID> particles;
	Vector3 position;
	Vector3 velocity, velocityVariation;
	Vector4 colorBegin, colorEnd;
	float sizeBegin = 0.f, sizeEnd = 0.f, sizeVariation = 0.f;
	float lifeTime = 0.5f;
	float rotation = 0.0f;
	bool emit = false;

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

	static void RegisterLua(sol::state& lua);
	property_vtable()
};

//property_begin_name(EmitterCom, "Emitter Component")
property_begin(EmitterCom)
{
	property_var(velocityVariation)
	, property_var(colorBegin)
	, property_var(colorEnd)
	, property_var(sizeBegin)
	, property_var(sizeEnd)
	, property_var(sizeVariation)
	, property_var(lifeTime)
	, property_var(emit)

} property_vend_h(EmitterCom)
