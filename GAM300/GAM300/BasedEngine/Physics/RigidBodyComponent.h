/**********************************************************************************
* \file RigidBodyComponent.h
* \brief File contains the function definitions for all functions that are used
		 in Rigid Body Component.
* \author Kenneth Yeo, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

#include "../ECS/Component/Component.h"
#include "../ECS/Component/ComponentManager.h"
#include <Properties.h>
#include "Serializer/ISerializer.h"
#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
using namespace physx;

struct iRigid
{
	virtual Vector3 getPos() = 0;
	virtual Vector3 getScale() = 0;
	virtual Quat getRot() = 0;
};

struct RigidStaticCom : Component, public ISerializer, virtual iRigid
{
	Vector3 rigidPos, rigidScale;
	Quat rigidRot{ 0, 0, 0, 1.0f };

	PxRigidStatic* body;
	PxShape* shape;
	PxMaterial* material;

	float staticFriction{ 0.0f };
	float dynamicFriction{ 0.0f };
	float restitution{ 0.0f };

	bool getHit{ false };

	void DefaultValues(void) noexcept
	{
		rigidPos = { 0.0f,0.0f,0.0f };
		rigidScale = { 1.0f, 1.0f, 1.0f };
		rigidRot = { 0.0f, 0.0f, 0.0f, 1.0f };
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
	Vector3 getPos()
	{
		return rigidPos;
	}
	Vector3 getScale()
	{
		return rigidScale;
	}
	Quat getRot()
	{
		return rigidRot;
	}
	void Deserialize(const rapidjson::Document& doc, std::string id);
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);
	static void RegisterLua(sol::state& lua);
	

	PxBounds3 GetAABB()
	{
		PxShapeExt* ShapeExt;
		return ShapeExt->getWorldBounds(*shape, *body);
	}

	void setActorName(const char* Name)
	{
		body->setName(Name);
	}

	const char* getActorName()
	{
		if (body->getName())
			return body->getName();
		return nullptr;
	}
	
	property_vtable()
};

//property_begin_name(RigidStaticCom, "RigidStatic Component")
property_begin(RigidStaticCom)
{
	property_var(rigidPos)
	, property_var(rigidScale)
	, property_var(rigidRot)
	, property_var(staticFriction)
	, property_var(dynamicFriction)
	, property_var(restitution)

} property_vend_h(RigidStaticCom)

struct RigidDynamicCom : Component , public ISerializer, virtual iRigid
{
	int m_Int{ 0 };
	Vector3 rigidPos, rigidScale;
	Quat rigidRot{ 0, 0, 0, 1.0f };
	float mass{ 1.0f };

	PxRigidDynamic* body;
	PxShape* shape;
	PxMaterial* material;
	
	float staticFriction{ 0.0f };
	float dynamicFriction{ 0.0f };
	float restitution{ 0.0f };
	float linearDamp{ 0.0f };
	float angularDamp{ 0.0f };
	bool getHit{ false };

	void DefaultValues(void) noexcept
	{
		rigidPos = { 0.0f,0.0f,0.0f };
		rigidScale = { 1.0f, 1.0f, 1.0f };
		rigidRot = { 0.0f, 0.0f, 0.0f, 1.0f };
		mass = 0.0f;
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
	Vector3 getPos()
	{
		return rigidPos;
	}
	Vector3 getScale()
	{
		return rigidScale;
	}
	Quat getRot()
	{
		return rigidRot;
	}
	void Deserialize(const rapidjson::Document& doc, std::string id);
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);
	
	static void RegisterLua(sol::state& lua);

	PxBounds3 GetAABB()
	{
		PxShapeExt* ShapeExt;
		return ShapeExt->getWorldBounds(*shape, *body);
	}

	void setActorName(const char* Name)
	{
		body->setName(Name);
	}

	const char* getActorName()
	{
		if (body->getName())
			return body->getName();
		return nullptr;
	}

	property_vtable()
};

//property_begin_name(RigidDynamicCom, "RigidDynamic Component")
property_begin(RigidDynamicCom)
{
	property_var(rigidPos)
	, property_var(rigidScale)
	, property_var(rigidRot)
	, property_var(mass)
	, property_var(staticFriction)
	, property_var(dynamicFriction)
	, property_var(restitution)
	, property_var(linearDamp)
	, property_var(angularDamp)

} property_vend_h(RigidDynamicCom)
