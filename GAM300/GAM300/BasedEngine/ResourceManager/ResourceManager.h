/**********************************************************************************
* \file ResourceManager.h
* \brief File contains the function definitions for all functions that are used
		 to manage the resources.
* \author Shaikh Faid Bin Omar, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include <System/System.h>
#include <Audio/Sound.h>

#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

enum class Colors // add more
{
	RED,
	BLUE,
	GREEN,
	YELLOW,
	WHITE,
	BLACK,
	CYAN,
	GRAY,
	MAGENTA
};

enum class Entities // add more
{
	E_PLAYER,
	E_ENEMY,
	E_WEAPON,
	E_PLATFORM,
	E_LIGHT
};

//enum class PREFABNAME
//{
//	PLAYER = 0,
//	FLOOR
//};

typedef std::map<PrefabID, Prefab> PREFABMAP;
typedef std::map<std::string, std::vector<Vector3>> WAYPOINTMAP;


class ResourceManager : public ISystem
{
public:
	ResourceManager();
	~ResourceManager();

	void init();
	void load();
	void update(float delta_time);
	void unload();
	void release();

	// get entities?

	// get audio
	//FMOD_SOUND* GetAudio(SOUNDTYPE);
	//void SetAudio(SOUNDTYPE, FMOD_SOUND*);
	std::map<std::string, Sound> GetSoundMap() { return _SoundMap; }
	FMOD::Sound*& getSound(std::string name);
	FMOD::Channel*& getChannel(std::string name);
	void setSoundVolume(std::string name, float vol);
	void setSoundMute(std::string name, bool mute);
	void setSoundLoopcount(std::string name, int loopcount);
	void setIsPlaying(std::string name, bool b);
	void InsertAudio(std::pair<std::string, Sound> Audio);
	void InsertWayPoint(std::pair<std::string, std::vector<Vector3>> WayPoint);
	void AddWayPoint(std::string str, Vector3 point);
	void RemoveWayPoint(std::string str);
	static void RegisterLua(sol::state& lua);
	// get colors
	Vector3 GetColor(Colors);
	std::map<Colors, Vector3> GetColorsMap() { return _ColorMap; }

	// get script
	std::vector<std::string> GetScriptVec() { return _ScriptVec; }

	Prefab GetPrefab(PrefabID);
	PREFABMAP* GetPrefabMap() { return &_prefabMap; }

	std::vector<Vector3> GetWayPoint(std::string);
	Vector3& getWayPointVector3(std::string,int index);
	WAYPOINTMAP* GetWayPointMap() { return &_WayPointMap; }
	
	bool getIsRefresh();
	void setIsRefresh(bool);

	std::string getNewPrefab();
	void setNewPrefab(std::string);

	void insertTextMap(EntityID entityID, int id);
	void setTextMap(EntityID entityID, int id);
	void eraseTextMap(EntityID entityID);
	void clearTextMap();
	std::map<EntityID, int>& getTextMap();
	int getTextID(EntityID entityID);

	std::vector<int>& getTextToDeleteMap();
	void insertTextToDeleteMap(int id);
	void clearTextToDeleteMap();

	bool isAddText = false;
	int textID = 0;
private:
	std::map<Colors, Vector3> _ColorMap;
	std::map<std::string, Sound> _SoundMap;
	std::vector<std::string> _ScriptVec;
	std::map<PrefabID, Prefab> _prefabMap;
	std::map<EntityID, int> _textMap;
	std::vector<int> _textToDelete;

	WAYPOINTMAP _WayPointMap;

	//std::map<std::string, std::string> _ModelMap;
	//std::map<std::string, std::string> _TextureMap;
	//std::vector<std::string> _ShaderVec;

	bool isRefresh = false;
	std::string newPrefab;
	
	// Entities/Game object map?
	// Image map?
	// texture map
	// font map
	// mesh map
	void DeserializePrefabs();
	
	void LoadPrefabs();
	void LoadColorObjects();
	void LoadAudioObjects();
	void LoadFolderObjects(std::string);
	void LoadWayPoints();

	void RefreshPrefabsList();

	std::vector<std::string> listDirectory(std::filesystem::path path);
	

};
	
extern ResourceManager* RESOURCE;