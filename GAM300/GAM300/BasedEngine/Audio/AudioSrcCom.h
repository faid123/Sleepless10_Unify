/**********************************************************************************
* \file AudioSrcCom.h
* \brief File contains the function definitions for all functions that are used
		 in Audio Source Component.
* \author Kenneth Yeo, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "MathLib/Precompiled.h"
#include "Audio/Sound.h"

struct AudioSrcCom : Component, public ISerializer
{
	FMOD::ChannelGroup* mChannelGroup;
	float volume{ 1.0f };
	bool mute{ false };

	void DefaultValues(void) noexcept
	{
		volume = 1.0f;
		mute = false;
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

	void Deserialize(const rapidjson::Document& doc, std::string id);
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);
	static void RegisterLua(sol::state& lua);

	property_vtable()
};

property_begin(AudioSrcCom)
{
	property_var(volume)
	, property_var(mute)

} property_vend_h(AudioSrcCom)