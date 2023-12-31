#ifndef TEXTCOM_H
#define TEXTCOM_H

#include <GL/glew.h>
#include <string.h>
#include "MathLib/Precompiled.h"

#include "Serializer/ISerializer.h"
#include <Properties.h>
#include "Component/Component.h"

#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

struct TextCom : Component, public ISerializer
{
	std::string TextName;
	std::string _fontType;
	Vector3 _pos{ 0.0f };
	Vector4 _colour{ 1.0f }; //a modifer to modify colour, does not work on some!
	float _size{ 40.0f };
	//float _alphaVal{ 1 }; //alpha value/transparency, does not work in 3d!
	//bool _drawSprite; //simple bool to render or not
	//float _brightVal{ 1 }; // bool to modify brightness values
	//bool _drawIn3D{ 0 }; //bool to render as 2d screen or 3d object in world
	//bool _useSprite{ 1 }; //bool to render sprite or color modifer (shape stll follows sprite!)
	//bool _isBar{ 0 }; // bool for if the sprite is a bar// gives it slider propertes

	TextCom()
	{
	}

	TextCom(std::string name) :
		TextName(name)
	{
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

	void Deserialize(const rapidjson::Document& doc, std::string id)
	{
		(void)doc;
		(void)id;
	}
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
	{
		(void)writer;
	}

	void setName(std::string name)
	{
		TextName = name;
	}

	static void RegisterLua(sol::state& lua);
	property_vtable()
};

property_begin(TextCom)
{
	/*property_var(_pos)
		, property_var(_scale)
		, property_var(_rotate)*/

	property_var(TextName)
		, property_var(_colour).Name("Colour")
		, property_var(_size).Name("Size")
		, property_var(_fontType).Name("Font")
		, property_var(_pos).Name("Pos")

} property_vend_h(TextCom)
#endif // !TEXTCOM_H
