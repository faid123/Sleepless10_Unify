/**********************************************************************************
* \file AudioSystem.h
* \brief File contains the function definitions for all functions that are used
		 in Audio System.
* \author Kenneth Yeo, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#ifndef _AUDIOSYSTEM_H_
#define _AUDIOSYSTEM_H_

#include "../ECS/System/System.h"
#include <fmod.hpp>
#include "Audio/Sound.h"
#include <map>
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"
#include "Audio/AudioSrcCom.h"

class AudioSystem : public ISystem
{
	FMOD_RESULT _mResult;
	FMOD::System* _mSystem;

	FMOD::ChannelGroup* _BGMGroup;
	FMOD::ChannelGroup* _SFXGroup;
	FMOD::ChannelGroup* _UIGroup;
	FMOD::ChannelGroup* _MasterGroup;

	float masterVol = 2.0f;

	Vector3 _listenerpos;

	float startTimer = 0.0f;

	std::vector<std::string> _SoundNames;
public:
	AudioSystem();
	void init() override;
	void load() override;
	void update(float dt) override;
	void unload() override;
	void release() override;

	FMOD::ChannelGroup*& GetBGMChannelGroup();
	FMOD::ChannelGroup*& GetSFXChannelGroup();
	FMOD::ChannelGroup*& GetMasterChannelGroup();

	float& getMasterVolume();
	void setMasterVolume(float vol);

	void increaseMasterVolume();
	void decreaseMasterVolume();

	std::vector<std::string> getSoundNames();

	void InitChannelGroups();
	void CreateChannelGroup(const char* name, AudioSrcCom& audiosrc);

	void CreateBGM(Sound& pSound, const char* FileName, std::string name);
	void PlayBGM(std::string name);
	void PauseBGM(std::string name);
	void UnpauseBGM(std::string name);
	void StopBGM(std::string name);

	void CreateSFX(Sound& pSound, const char* FileName);
	void PlaySFX(std::string name, EntityID entity);

	void Create3DSFX(Sound& pSound, const char* FileName);
	void Play3DSFX(std::string name, EntityID entity);
	void Stop3DSFX(std::string name, EntityID entity);
	void MuteSound(std::string name);
	void UnmuteSound(std::string name);

	void CreateUISound(Sound& pSound, const char* FileName);
	void PlayUISound(std::string name);
	void PauseUISound(std::string name);
	void UnpauseUISound(std::string name);
	void StopUISound(std::string name);

	void PauseAllSounds();
	void UnpauseAllSounds();
	void StopAllSounds();
	void StopAllSoundsWhenDead();
	void MuteAllChannels();
	void UnmuteAllChannels();
	void StopAllSFX();
	void StopAllBGM();

	void UpdateListener(Vector3 pos, float dt);

	void ReadSoundsFromJson(const std::string JsonFile);
	void SaveSoundsToJson(const std::string JsonFile);

	void InsertSounds(const std::string Name, float vol, bool mute, int loop, std::string type);
	void LoadSounds();

	void Deserialize(const rapidjson::Document& doc, std::string id);
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);

	static void RegisterLua(sol::state& lua);
};

#endif