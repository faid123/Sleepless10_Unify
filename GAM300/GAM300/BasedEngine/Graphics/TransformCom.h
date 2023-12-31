#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "Serializer/ISerializer.h"
#include <map>
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"
#include <Properties.h>
#include "Component/Component.h"

struct TransformCom : Component , public ISerializer
{
	Vector3 pos{0.0f};
	Vector3 scale{2.0f};
	Vector3 rotate{0.0f};

	TransformCom()
	{

	}

	TransformCom(Vector3 Pos, Vector3 Scale, Vector3 Rot) : pos{ Pos }, scale{ Scale }, rotate{ Rot }
	{

	}

	void DefaultValues(void) noexcept
	{
		pos = { 0.0f,0.0f,0.0f };
		scale = { 2.0f,2.0f,2.0f };
		rotate = { 0.0f,0.0f,1.0f };
	}
	void SanityCheck(void) noexcept
	{
		//assert(_pos == { 0.0f,0.0f,0.0f });
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

	const char* StateToString(int);
	std::map <std::string, int> _stateMap = { {"Static",1},{"Dynamic",2},{"None",0} };
	void Deserialize(const rapidjson::Document& doc, std::string id) override;
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;
	
	static void RegisterLua(sol::state& lua);
	property_vtable()
};
	

//property_begin_name(TransformCom, "Transform Component")
property_begin(TransformCom)
{
	property_var(pos)
	, property_var(scale)
	, property_var(rotate)

} property_vend_h(TransformCom)
