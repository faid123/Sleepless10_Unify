#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <map>

#include "Serializer/ISerializer.h"
#include <Properties.h>

#include "Graphics/TransformCom.h"
#include "Graphics/SpriteCom.h"

enum class UI_State
{
	Default,
	Off,
	Pause
};
enum Type_UI
{
	Health = 0,
	Ammo,
	ColourBar
};
enum WeaponType_UI
{
	Pistol =0,
	Laser,
	Rocket
};

struct UICom : Component, public ISerializer, public std::enable_shared_from_this<UICom>
{
	int _UI_State;
	std::shared_ptr<TransformCom>	_tComp;
	std::shared_ptr<SpriteCom>		_sComp;

	int _UI_Type{(int)Type_UI::Health};
	bool _IsUIActive;

	UICom() : _UI_State{ (int)UI_State::Off }
	{
		//registerToSystem();
	}

	UICom(int state) : 
		_UI_State{ state }
	{
		//registerToSystem();
	}

	void destroyEntity(EntityID entity)
	{
		(void)entity;
	}
	void cloneComponent(EntityID entity1, EntityID entity2)
	{
		(void)entity1;
		(void)entity2;
	}

	void update();
	
	//Based on type
	//health
	int _healthPercent = 100;
	void increaseHealth();
	void decreaseHealth();
	void findHealth();
	//ammo
	int _currentAmmoType = WeaponType_UI::Pistol;
	void swapToWeapon(WeaponType_UI weapType);
	void findWeapons();
	std::map<WeaponType_UI, std::shared_ptr<TransformCom>> weaponList;

	//unfinished. Don't use
	void registerToSystem();


	
	void Deserialize(const rapidjson::Document& doc, std::string id)
	{
		(void)doc;
		(void)id;
	}
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
	{
		(void)writer;
	}

	property_vtable()
};

property_begin(UICom)
{
	property_var(_UI_State)
	,property_var(_UI_Type)
	,property_var(_IsUIActive)
		

} property_vend_h(UICom)