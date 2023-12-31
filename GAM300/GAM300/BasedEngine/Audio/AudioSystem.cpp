#include "AudioSystem.h"
#include "Serializer/ISerializer.h"
#include <iostream>
#include <fstream>
#include <Graphics/TransformCom.h>
#include <ResourceManager/ResourceManager.h>
#include <Audio/AudioSrcCom.h>
#include "Engine.h"
#include "Physics/RigidBodyComponent.h"
#include "Input.h"
#include "../GameState.h"
#include "../UserIntered/UI_System.h"

bool testsoundplayed = false;

//static FMOD_RESULT F_CALLBACK EndOfSound(FMOD_CHANNELCONTROL* channelControl, FMOD_CHANNELCONTROL_TYPE controlType, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbackType,
//	void* commanData1, void* commanData2)
//{
//	(void)channelControl;
//	(void)controlType;
//	(void)callbackType;
//	(void)commanData1;
//	(void)commanData2;
//
//	//if (callbackType == FMOD_CHANNELCONTROL_CALLBACK_END)
//		// sound ends
//		//FMOD::Channel* channel = (FMOD::Channel*)channelControl;
//
//	return FMOD_OK;
//}

AudioSystem::AudioSystem() : _mSystem{ nullptr }, _mResult{}, _BGMGroup{ nullptr },
_SFXGroup{ nullptr }, _UIGroup{ nullptr }, _MasterGroup{ nullptr }, _listenerpos{ FMOD_VECTOR{0, 0, 0} }
{

}

void AudioSystem::init()
{
	_mResult = FMOD::System_Create(&_mSystem);      // Create the main system object.

	_mResult = _mSystem->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.

	InitChannelGroups();
	LoadSounds();
	_mResult = _mSystem->set3DSettings(1.0f, 1.0f, 1.0f);

	// test
	//PlaySFX("drumloop", Vector3{ 0, 0, -15.0f }, Vector3{ 0, 0, 0 });
}

void AudioSystem::load()
{
	for (const auto& entity : _entities)
	{
		auto audiosrc = Engine::_ecs.getComponent<AudioSrcCom>(entity);
		if (audiosrc)
		{
			CreateChannelGroup(Engine::_ecs.getEntityName(entity).c_str(), *audiosrc);
		}
	}
}

void AudioSystem::update(float dt)
{
	/*if (!testsoundplayed)
	{
		PlaySFXFromEntity("drumloop", Engine::_ecs.getEntityID("Enemy"));
		testsoundplayed = true;
	}*/

	if (Engine::_isPlay)
	{
		auto playerTrans = Engine::_ecs.getComponent<TransformCom>(Engine::_ecs.getEntityID("Player"));
		UpdateListener(playerTrans->pos, dt);

		for (const auto& entity : _entities)
		{
			auto audiosrc = Engine::_ecs.getComponent<AudioSrcCom>(entity);

			if (audiosrc)
			{
				auto transform = Engine::_ecs.getComponent<TransformCom>(entity);
				auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(entity);

				FMOD_VECTOR position = { transform->pos.x, transform->pos.y, transform->pos.z };
				FMOD_VECTOR velocity;

				if (dynamicbody)
					velocity = { dynamicbody->body->getLinearVelocity().x,
					dynamicbody->body->getLinearVelocity().y,
					dynamicbody->body->getLinearVelocity().z };
				else
					velocity = { 0, 0, 0 };

				audiosrc->mChannelGroup->setVolume(audiosrc->volume);
				audiosrc->mChannelGroup->setMute(audiosrc->mute);
				audiosrc->mChannelGroup->set3DAttributes(&position, &velocity);
			}
		}
	}

	for (auto& sound : RESOURCE->GetSoundMap())
	{
		if (sound.second.isplaying)
		{
			//if (sound.second.mType != SoundType::BGM)
			sound.second.mChannel->setVolume(sound.second.volume);
			sound.second.mChannel->setMute(sound.second.mute);
			sound.second.mChannel->setLoopCount(sound.second.loopcount);
		}
	}

	_MasterGroup->setVolume(masterVol);

	if (Engine::_ecs.getSystem<UISystem>()->_isPaused)
	{
		PauseAllSounds();
	}
	else
	{
		UnpauseAllSounds();
	}

	_mResult = _mSystem->update();
}

void AudioSystem::unload()
{
	//StopAllSounds();
	if (Engine::_isGameMode)
	{

		switch ((GameState)GameStateManager::_preState)
		{
		case GameState::LV1:
			StopBGM("BG_Level1");
			break;
		case GameState::LV2:
			StopBGM("BG_Level2");
			break;
		case GameState::LV3:
			StopBGM("BG_Level3");
			break;
		case GameState::NONE:
			break;
		default:
			break;
		}
	}
}

void AudioSystem::release()
{
	for (auto& sound : RESOURCE->GetSoundMap())
		sound.second.mSound->release();

	_mResult = _mSystem->release();
}

FMOD::ChannelGroup*& AudioSystem::GetBGMChannelGroup()
{
	return _BGMGroup;
}

FMOD::ChannelGroup*& AudioSystem::GetSFXChannelGroup()
{
	return _SFXGroup;
}

FMOD::ChannelGroup*& AudioSystem::GetMasterChannelGroup()
{
	return _MasterGroup;
}

float& AudioSystem::getMasterVolume()
{
	return masterVol;
}

void AudioSystem::setMasterVolume(float vol)
{
	masterVol = vol;
}

void AudioSystem::increaseMasterVolume()
{
	masterVol += 0.5f;

	if (masterVol >= 5.f)
		masterVol = 5.f;
}

void AudioSystem::decreaseMasterVolume()
{
	masterVol -= 0.5f;

	if (masterVol <= 0.0f)
		masterVol = 0.0f;
}

std::vector<std::string> AudioSystem::getSoundNames()
{
	return _SoundNames;
}

void AudioSystem::InitChannelGroups()
{
	_mResult = _mSystem->createChannelGroup("BGM_Group", &_BGMGroup);
	_mResult = _mSystem->createChannelGroup("SFX_Group", &_SFXGroup);
	_mResult = _mSystem->createChannelGroup("UI_Group", &_UIGroup);
	_mResult = _mSystem->getMasterChannelGroup(&_MasterGroup);
	_mResult = _MasterGroup->addGroup(_BGMGroup);
	_mResult = _MasterGroup->addGroup(_SFXGroup);
	_mResult = _MasterGroup->addGroup(_UIGroup);
}

void AudioSystem::CreateChannelGroup(const char* name, AudioSrcCom& audiosrc)
{
	_mResult = _mSystem->createChannelGroup(name, &audiosrc.mChannelGroup);
}

void AudioSystem::CreateBGM(Sound& pSound, const char* FileName, std::string name)
{
	(void)name;
	_mResult = _mSystem->createSound(FileName, FMOD_LOOP_NORMAL, 0, &pSound.mSound);

}

void AudioSystem::PlayBGM(std::string name)
{
	if (!RESOURCE->GetSoundMap()[name].isplaying)
	{
		_mResult = _mSystem->playSound(RESOURCE->getSound(name), _BGMGroup, false,
			&RESOURCE->getChannel(name));

		RESOURCE->getChannel(name)->setVolumeRamp(true);

		RESOURCE->getChannel(name)->setVolume(RESOURCE->GetSoundMap()[name].volume);
		RESOURCE->getChannel(name)->setMute(RESOURCE->GetSoundMap()[name].mute);
		RESOURCE->getChannel(name)->setLoopCount(RESOURCE->GetSoundMap()[name].loopcount);
		//RESOURCE->getSound(name)->setLoopCount(RESOURCE->GetSoundMap()[name].loopcount);
		RESOURCE->setIsPlaying(name, true);
	}
}

void AudioSystem::PauseBGM(std::string name)
{
	if (RESOURCE->GetSoundMap()[name].isplaying)
	{
		RESOURCE->GetSoundMap()[name].mChannel->setPaused(true);
		//RESOURCE->setIsPlaying(name, false);
	}
}

void AudioSystem::UnpauseBGM(std::string name)
{
	RESOURCE->GetSoundMap()[name].mChannel->setPaused(false);
	//RESOURCE->setIsPlaying(name, true);
}

void AudioSystem::StopBGM(std::string name)
{
	if (RESOURCE->GetSoundMap()[name].isplaying)
	{
		RESOURCE->GetSoundMap()[name].mChannel->stop();
		RESOURCE->setIsPlaying(name, false);
	}
}

void AudioSystem::CreateSFX(Sound& pSound, const char* FileName)
{
	_mResult = _mSystem->createSound(FileName, FMOD_DEFAULT | FMOD_LOOP_NORMAL, 0, &pSound.mSound);
	//mResult = TestSound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 50.0f * DISTANCEFACTOR);
}

void AudioSystem::PlaySFX(std::string name, EntityID entity)
{
	auto audiosrc = Engine::_ecs.getComponent<AudioSrcCom>(entity);
	if (audiosrc)
		_mResult = _mSystem->playSound(RESOURCE->getSound(name), _SFXGroup, false,
			&RESOURCE->getChannel(name));
		
	//RESOURCE->getSound(name)->setLoopCount(RESOURCE->GetSoundMap()[name].loopcount);
	RESOURCE->getChannel(name)->setLoopCount(RESOURCE->GetSoundMap()[name].loopcount);
	RESOURCE->getChannel(name)->setVolume(RESOURCE->GetSoundMap()[name].volume);
	RESOURCE->getChannel(name)->setMute(RESOURCE->GetSoundMap()[name].mute);
	RESOURCE->setIsPlaying(name, true);

}

void AudioSystem::Create3DSFX(Sound& pSound, const char* FileName)
{
	_mResult = _mSystem->createSound(FileName, FMOD_3D | FMOD_LOOP_NORMAL, 0, &pSound.mSound);
}

void AudioSystem::Play3DSFX(std::string name, EntityID entity)
{
	auto transform = Engine::_ecs.getComponent<TransformCom>(entity);
	auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(entity);
	auto audiosrc = Engine::_ecs.getComponent<AudioSrcCom>(entity);

	FMOD_VECTOR position;
	if (transform)
		position = { transform->pos.x, transform->pos.y, transform->pos.z };

	FMOD_VECTOR velocity;
	if (dynamicbody)
		velocity = { dynamicbody->body->getLinearVelocity().x,
		dynamicbody->body->getLinearVelocity().y,
		dynamicbody->body->getLinearVelocity().z };
	else
		velocity = { 0, 0, 0 };

	if (audiosrc)
		_mResult = _mSystem->playSound(RESOURCE->getSound(name),
			_SFXGroup, false, &RESOURCE->getChannel(name));

	RESOURCE->getChannel(name)->set3DAttributes(&position, &velocity);

	RESOURCE->getChannel(name)->set3DMinMaxDistance(5.0f, 1000.0f);

	RESOURCE->getChannel(name)->set3DDopplerLevel(0);

	RESOURCE->getChannel(name)->setVolume(RESOURCE->GetSoundMap()[name].volume);
	RESOURCE->getChannel(name)->setMute(RESOURCE->GetSoundMap()[name].mute);
	RESOURCE->getChannel(name)->setLoopCount(RESOURCE->GetSoundMap()[name].loopcount);
	RESOURCE->getSound(name)->setLoopCount(RESOURCE->GetSoundMap()[name].loopcount);

	RESOURCE->setIsPlaying(name, true);
}

void AudioSystem::Stop3DSFX(std::string name, EntityID entity)
{
	auto audiosrc = Engine::_ecs.getComponent<AudioSrcCom>(entity);
	//audiosrc->mChannelGroup->stop();
	RESOURCE->GetSoundMap()[name].mChannel->stop();
	RESOURCE->setIsPlaying(name, false);
}

void AudioSystem::MuteSound(std::string name)
{
	RESOURCE->getChannel(name)->setMute(true);
}

void AudioSystem::UnmuteSound(std::string name)
{
	RESOURCE->getChannel(name)->setMute(false);
}

void AudioSystem::CreateUISound(Sound& pSound, const char* FileName)
{
	_mResult = _mSystem->createSound(FileName, FMOD_LOOP_NORMAL, 0, &pSound.mSound);
}

void AudioSystem::PlayUISound(std::string name)
{
	_mResult = _mSystem->playSound(RESOURCE->getSound(name), _UIGroup, false,
		&RESOURCE->getChannel(name));

	RESOURCE->getChannel(name)->setVolume(RESOURCE->GetSoundMap()[name].volume);
	RESOURCE->getChannel(name)->setMute(RESOURCE->GetSoundMap()[name].mute);
	RESOURCE->getChannel(name)->setLoopCount(RESOURCE->GetSoundMap()[name].loopcount);
	//RESOURCE->getSound(name)->setLoopCount(RESOURCE->GetSoundMap()[name].loopcount);
	RESOURCE->setIsPlaying(name, true);
}

void AudioSystem::PauseUISound(std::string name)
{
	RESOURCE->getChannel(name)->setPaused(true);
}

void AudioSystem::UnpauseUISound(std::string name)
{
	RESOURCE->getChannel(name)->setPaused(false);
}

void AudioSystem::StopUISound(std::string name)
{
	RESOURCE->GetSoundMap()[name].mChannel->stop();
	RESOURCE->setIsPlaying(name, false);
}

void AudioSystem::PauseAllSounds()
{
	/*for (auto& sound : RESOURCE->GetSoundMap())
	{
		if (sound.second.mType != SoundType::UI)
		{
			sound.second.mChannel->setPaused(true);
		}
	}*/

	_BGMGroup->setPaused(true);
	_SFXGroup->setPaused(true);
}

void AudioSystem::UnpauseAllSounds()
{
	/*for (auto& sound : RESOURCE->GetSoundMap())
	{
		if (sound.second.isplaying)
		{
			sound.second.mChannel->setPaused(false);
		}
	}*/

	_BGMGroup->setPaused(false);
	_SFXGroup->setPaused(false);
}

void AudioSystem::StopAllSounds()
{
	for (auto& sound : RESOURCE->GetSoundMap())
	{
		if (sound.second.isplaying)
		{
			//sound.second.mChannel->stop();
			RESOURCE->setIsPlaying(sound.first, false);
		}
	}
	_MasterGroup->stop();
}

void AudioSystem::StopAllSoundsWhenDead()
{
	for (auto& sound : RESOURCE->GetSoundMap())
	{
		if (sound.second.isplaying)
		{
			if (sound.first != "SFX_GameOver")
			{
				sound.second.mChannel->stop();
				RESOURCE->setIsPlaying(sound.first, false);
			}
				
		}
	}
}

void AudioSystem::MuteAllChannels()
{
	for (auto& sound : RESOURCE->GetSoundMap())
	{
		if (sound.second.isplaying && sound.second.mType != SoundType::UI)
		{
			sound.second.mChannel->setMute(true);
		}
	}
}

void AudioSystem::UnmuteAllChannels()
{
	for (auto& sound : RESOURCE->GetSoundMap())
	{
		if (sound.second.isplaying && sound.second.mType != SoundType::UI)
		{
			sound.second.mChannel->setMute(false);
		}
	}
}

void AudioSystem::StopAllSFX()
{
	_SFXGroup->stop();
	for (auto& sound : RESOURCE->GetSoundMap())
	{
		if (sound.second.isplaying && (sound.second.mType == SoundType::SFX || sound.second.mType == SoundType::SFX_3D))
		{
			RESOURCE->setIsPlaying(sound.first, false);
		}
	}
}

void AudioSystem::StopAllBGM()
{
	_BGMGroup->stop();
	for (auto& sound : RESOURCE->GetSoundMap())
	{
		if (sound.second.isplaying && sound.second.mType == SoundType::BGM)
		{
			RESOURCE->setIsPlaying(sound.first, false);
		}
	}
}

void AudioSystem::UpdateListener(Vector3 pos, float dt)
{
	_listenerpos.x = pos.x;
	_listenerpos.y = pos.y;
	_listenerpos.z = pos.z;

	static float t = 0;
	static FMOD_VECTOR lastpos = { 0.0f, 0.0f, 0.0f };
	FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
	FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
	FMOD_VECTOR vel;
	FMOD_VECTOR listenerPos = { _listenerpos.x, _listenerpos.y, _listenerpos.z };

	//listenerpos.x = (float)sin(t * 0.05f) * 24.0f * DISTANCEFACTOR; // left right pingpong

	// ********* NOTE ******* READ NEXT COMMENT!!!!!
	// vel = how far we moved last FRAME (m/f), then time compensate it to SECONDS (m/s).
	vel.x = (_listenerpos.x - lastpos.x) * dt;
	vel.y = (_listenerpos.y - lastpos.y) * dt;
	vel.z = (_listenerpos.z - lastpos.z) * dt;

	// store pos for next time
	lastpos = listenerPos;

	_mResult = _mSystem->set3DListenerAttributes(0, &listenerPos, &vel,
		&forward, &up);

	t += (30 * dt);    // t is just a time value .. it increments in 30m/s steps in this example
}

void AudioSystem::ReadSoundsFromJson(const std::string JsonFile)
{
	using namespace rapidjson;
	std::ifstream fp(JsonFile);

	if (!fp.is_open())
		std::cout << "AudioFiles Not Open" << std::endl;

	JsonReader reader;
	reader.StartObject(fp);
	IStreamWrapper isw(fp);
	if (!reader.IsError(&isw))
	{
		reader.DeserializeComponent<AudioSystem>(*Engine::_ecs.getSystem<AudioSystem>(), "0");
		fp.close();
	}
}

void AudioSystem::SaveSoundsToJson(const std::string JsonFile)
{
	using namespace rapidjson;
	std::ofstream fp(JsonFile);

	if (!fp.is_open())
		std::cout << "AudioFiles Not Open" << std::endl;

	JsonWriter writer;
	writer.StartObject();
	writer.SerializeComponent<AudioSystem>(*Engine::_ecs.getSystem<AudioSystem>());
	writer.EndObject();
	std::ofstream ofs(JsonFile);
	ofs << writer.GetString();
	ofs.close();
}

void AudioSystem::InsertSounds(const std::string Name, float vol, bool mute, int loop, std::string type)
{
	Sound sound;

	std::string filepath = "./Resources/AudioFiles/";
	filepath.append(Name);
	filepath.append(".mp3");

	sound.volume = vol;
	sound.mute = mute;
	sound.loopcount = loop;

	if (type == "BGM")
	{
		sound.mType = SoundType::BGM;
		CreateBGM(sound, filepath.c_str(), Name);
		sound.mChannel->setChannelGroup(_BGMGroup);
		sound.mChannel->setChannelGroup(_MasterGroup);
	}

	else if (type == "SFX")
	{
		sound.mType = SoundType::SFX;
		CreateSFX(sound, filepath.c_str());
		sound.mChannel->setChannelGroup(_SFXGroup);
		sound.mChannel->setChannelGroup(_MasterGroup);
	}

	else if (type == "SFX_3D")
	{
		sound.mType = SoundType::SFX_3D;
		Create3DSFX(sound, filepath.c_str());
		sound.mChannel->setChannelGroup(_SFXGroup);
		sound.mChannel->setChannelGroup(_MasterGroup);
	}

	else if (type == "UI")
	{
		sound.mType = SoundType::UI;
		CreateUISound(sound, filepath.c_str());
		sound.mChannel->setChannelGroup(_SFXGroup);
		sound.mChannel->setChannelGroup(_MasterGroup);
	}

	RESOURCE->InsertAudio(std::pair<std::string, Sound>(Name, sound));
	_SoundNames.push_back(Name);
}

void AudioSystem::LoadSounds()
{
	std::string jsonPath = "./Resources/AudioFiles/AudioFiles.json";
	ReadSoundsFromJson(jsonPath);
}

void AudioSystem::Deserialize(const rapidjson::Document& doc, std::string id)
{
	if (doc.HasMember("SoundsContainer") && doc["SoundsContainer"].IsArray())
	{
		for (rapidjson::SizeType i = 0; i < doc["SoundsContainer"].Size(); ++i)
		{
			std::string soundname;
			float volume;
			bool mute;
			int loopcount;
			std::string type;

			soundname = doc["SoundsContainer"][i]["SoundName"].GetString();
			volume = (float)doc["SoundsContainer"][i]["Volume"].GetDouble();
			mute = doc["SoundsContainer"][i]["Muted"].GetBool();
			loopcount = doc["SoundsContainer"][i]["Loop"].GetInt();
			type = doc["SoundsContainer"][i]["Type"].GetString();

			InsertSounds(soundname, volume, mute, loopcount, type);
		}
	}
}

void AudioSystem::Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer)
{
	writer->Key("SoundsContainer");
	writer->StartArray();

	size_t noOfSounds = _SoundNames.size();

	for (size_t i = 0; i < noOfSounds; i++)
	{
		writer->StartObject();

		std::string soundName = _SoundNames[i];
		float volume = RESOURCE->GetSoundMap()[soundName].volume;
		bool mute = RESOURCE->GetSoundMap()[soundName].mute;
		int loopcount = RESOURCE->GetSoundMap()[soundName].loopcount;
		std::string type;

		if (RESOURCE->GetSoundMap()[soundName].mType == SoundType::BGM)
			type = "BGM";
		else if (RESOURCE->GetSoundMap()[soundName].mType == SoundType::SFX)
			type = "SFX";
		else if (RESOURCE->GetSoundMap()[soundName].mType == SoundType::SFX_3D)
			type = "SFX_3D";
		else
			type = "UI";

		writer->String("SoundName");
		writer->String(soundName.c_str());
		writer->String("Volume");
		writer->Double((double)volume);
		writer->String("Muted");
		writer->Bool(mute);
		writer->String("Loop");
		writer->Int(loopcount);
		writer->String("Type");
		writer->String(type.c_str());

		writer->EndObject();
	}
	writer->EndArray();
}
