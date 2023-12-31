/**********************************************************************************
* \file SpriteCom.h
* \brief File contains the function definitions for all functions that are used
		 in Material component.
* \author Willy Lau, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#ifndef SPRITECOM_H
#define SPRITECOM_H

#include <GL/glew.h>
#include <string.h>
#include "MathLib/Precompiled.h"

#include "Serializer/ISerializer.h"
#include <Properties.h>
#include "Component/Component.h"

#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

struct SpriteCom : Component, public ISerializer
{/*
	Vector2 _pos; 
	Vector2 _scale;
	float _rotate;*/
	//Vector2 _initalpos;

	std::string SpriteName;
	Vector3 _colour{1.0f}; //a modifer to modify colour, does not work on some!
	float _alphaVal{ 1 }; //alpha value/transparency, does not work in 3d!
	bool _drawSprite; //simple bool to render or not
	float _brightVal{ 1 }; // bool to modify brightness values
	bool _drawIn3D{0}; //bool to render as 2d screen or 3d object in world
	bool _useSprite{ 1 }; //bool to render sprite or color modifer (shape stll follows sprite!)
	bool _isBar{ 0 }; // bool for if the sprite is a bar// gives it slider propertes
	int  _fack{ 0 };

	//for spriteSheet
	float _u;
	float _v;
	float _clipSpeed = 10;
	bool _isAnimation = false;
	bool _isPlaying = false;

	//modifer for healthbar
	float barPercentage = 100;
	float barXAmount = 0.0f;
	SpriteCom()
	{
	}

	SpriteCom(std::string name, float alpha, bool isDraw, bool is3D,bool isAnim = false) :
		//_pos(pos),
		//_scale(scale),
		//_rotate(rot),

		_alphaVal(alpha),
		SpriteName(name),
		_drawSprite(isDraw),
		_drawIn3D(is3D)
	{
		_isAnimation = isAnim;
		_u = 0.f;
		_v = 0.f;
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
		SpriteName = name;
	}

	static void RegisterLua(sol::state& lua);
	property_vtable()
};

property_begin(SpriteCom)
{
	/*property_var(_pos)
		, property_var(_scale)
		, property_var(_rotate)*/

		property_var(SpriteName)
		, property_var(_alphaVal).Name("Alpha Value")
		, property_var(_brightVal).Name("Brightness Value")
		, property_var(_colour).Name("Colour Modifer")
		, property_var(_drawSprite).Name("Draw Sprite")
		, property_var(_drawIn3D).Name("Render in 3D")
		, property_var(_useSprite).Name("Use Sprite")
		, property_var(_isBar).Name("Is a bar").Help("Bool for bar behavior")
		, property_var(barPercentage).Name("Bar Percentage").Help("Percentage/health of the bar")
		, property_var(barXAmount).Name("bar offset X").Help("Offset adjust for bar to decrease\nNote: u can go negative for the other way ;)")
		, property_var(_clipSpeed)
	    , property_var(_isAnimation)
		, property_var(_isPlaying)

} property_vend_h(SpriteCom)

//class Sprite
//{
//	GLuint SpriteID, Width, Height;
//
//
//public:
//	Sprite() :
//		Width(0),
//		Height(0)
//	{
//		glGenTextures(1, &SpriteID);
//	}
//
//	void Generate(GLuint width, GLuint height, unsigned char* data)
//	{
//		Width = width;
//		Height = height;
//
//		// create Texture
//		glBindTexture(GL_TEXTURE_2D, SpriteID);
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//
//		// set Texture wrap and filter modes
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		// unbind texture
//		glBindTexture(GL_TEXTURE_2D, 0);
//	}
//
//	void Bind() const
//	{
//		glBindTexture(GL_TEXTURE_2D, SpriteID);
//	}
//
//	void SetSpriteID(GLuint ID)
//	{
//		SpriteID = ID;
//	}
//
//	GLuint GetSpiteID()
//	{
//		return SpriteID;
//	}
//};

#endif // !SPRITECOM_H
