/******************************************************************************
* \file			ParticleComponent.h
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
#include <Properties.h>

struct ParticleCom : Component, public ISerializer
{
	Vector3 position;
	Vector3 velocity, velocityVariation;
	Vector4 colorBegin, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float lifeTime = 1.0f;
	float lifeRemaining = 0.0f;
	float rotation = 0.0f;
	bool isActive = false;

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
	property_vtable()
};

//property_begin_name(ParticleCom, "Particle Component")
property_begin(ParticleCom)
{
	
} property_vend_h(ParticleCom)
