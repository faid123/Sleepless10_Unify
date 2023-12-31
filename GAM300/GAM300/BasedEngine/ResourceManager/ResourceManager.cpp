#include <ResourceManager/ResourceManager.h>
#include <filesystem>
#include <Engine.h>
#include <Graphics/TransformCom.h>


ResourceManager* RESOURCE = nullptr;

ResourceManager::ResourceManager()
{
	//assert(RESOURCE != NULL);
	RESOURCE = this;
}
	
ResourceManager::~ResourceManager()
{
	_ColorMap.clear();
	_SoundMap.clear();
	//clear other maps
}

void ResourceManager::init()
{
	LoadColorObjects();
	DeserializePrefabs();
	DeserializeWayPoint("A");
	DeserializeWayPoint("B");
	DeserializeWayPoint("C");
	DeserializeWayPoint("D");
	DeserializeWayPoint("E");
	//load other objects
	LoadFolderObjects("Script");
	//LoadFolderObjects("Models");
	//LoadFolderObjects("Textures");
	//LoadFolderObjects("Shaders");
}

void ResourceManager::load()
{
}

void ResourceManager::update(float delta_time)
{
	(void)delta_time;
	if (isRefresh)
	{
		RefreshPrefabsList();
		isRefresh = false;
	}
}

void ResourceManager::unload()
{
}

void ResourceManager::release()
{
}

FMOD::Sound*& ResourceManager::getSound(std::string name)
{
	return _SoundMap[name].mSound;
}

FMOD::Channel*& ResourceManager::getChannel(std::string name)
{
	return _SoundMap[name].mChannel;
}

void ResourceManager::setSoundVolume(std::string name, float vol)
{
	_SoundMap[name].volume = vol;
}

void ResourceManager::setSoundMute(std::string name, bool mute)
{
	_SoundMap[name].mute = mute;
}

void ResourceManager::setSoundLoopcount(std::string name, int loopcount)
{
	_SoundMap[name].loopcount = loopcount;
}

void ResourceManager::setIsPlaying(std::string name, bool b)
{
	_SoundMap[name].isplaying = b;
}

void ResourceManager::InsertAudio(std::pair<std::string, Sound> Audio)
{
	_SoundMap.insert(std::pair<std::string, Sound>(Audio.first, Audio.second));
}

void ResourceManager::InsertWayPoint(std::pair<std::string, std::vector<Vector3>> WayPoint)
{
	_WayPointMap.insert(std::pair<std::string, std::vector<Vector3>>(WayPoint.first, WayPoint.second));
}

void ResourceManager::AddWayPoint(std::string str, Vector3 point)
{
	auto& point_set = _WayPointMap[str];
	point_set.push_back(point);
}

void ResourceManager::RemoveWayPoint(std::string str)
{
	_WayPointMap[str].clear();
	_WayPointMap.erase(str);
}

Vector3 ResourceManager::GetColor(Colors color)
{
	return _ColorMap[color];
}

Prefab ResourceManager::GetPrefab(PrefabID name)
{
	return _prefabMap[name];
}

std::vector<Vector3> ResourceManager::GetWayPoint(std::string name)
{
	return _WayPointMap[name];
}

Vector3& ResourceManager::getWayPointVector3(std::string name, int index)
{
	//std::vector<Vector3> tmp = _WayPointMap[name];
	return _WayPointMap[name][index];
}

void ResourceManager::LoadPrefabs()
{
	//for (auto prefabVec : Engine::_ecs.getActivePrefabs())
	//{
	//	_prefabMap.insert(std::pair<PrefabID, Prefab>(prefabVec._prefabID, prefabVec));
	//}
	//_prefabMap.insert(std::pair<PREFABNAME, Prefab>(PREFABNAME::PLAYER, Engine::_ecs.getPrefab("Prefab_Player")));
	//_prefabMap.insert(std::pair<PREFABNAME, Prefab>(PREFABNAME::FLOOR, Engine::_ecs.getPrefab("Prefab_Floor")));

	// DEBUG: check for the PREFAB
	//auto prefab = Engine::_ecs.getPrefab(0);
	//std::cout << "PREFAB NAME: " + prefab._prefabName + "\n" + "PREFAB ID: " + std::to_string(prefab._prefabID) << std::endl;

	// DEBUG: check for the COMPONENT
	//auto prefab_com = Engine::_ecs.getComponentPrefab<TransformCom>(prefab._prefabID);
	//std::cout << prefab_com->pos.x << ":" << prefab_com->pos.y << ":" << prefab_com->pos.z << std::endl;
}

void ResourceManager::LoadColorObjects()
{
	_ColorMap.insert(std::pair<Colors, Vector3>(Colors::RED, Vector3(1, 0, 0)));
	_ColorMap.insert(std::pair<Colors, Vector3>(Colors::BLUE, Vector3(0, 0, 1)));
	_ColorMap.insert(std::pair<Colors, Vector3>(Colors::GREEN, Vector3(0, 1, 0)));
	_ColorMap.insert(std::pair<Colors, Vector3>(Colors::YELLOW, Vector3(1, 0.92f, 0.016f)));
	_ColorMap.insert(std::pair<Colors, Vector3>(Colors::WHITE, Vector3(1, 1, 1)));
	_ColorMap.insert(std::pair<Colors, Vector3>(Colors::BLACK, Vector3(0, 0, 0)));
	_ColorMap.insert(std::pair<Colors, Vector3>(Colors::CYAN, Vector3(0, 1, 1)));
	_ColorMap.insert(std::pair<Colors, Vector3>(Colors::GRAY, Vector3(0.5f, 0.5f, 0.5f)));
	_ColorMap.insert(std::pair<Colors, Vector3>(Colors::MAGENTA, Vector3(1, 0, 1)));

	std::cout << "loaded resources size: " << sizeof(_ColorMap) << std::endl;
}

// Set Audio
// Get Audio
	
void ResourceManager::LoadAudioObjects()
{
	// insert to AudioMap
	// std::cout << "loaded resources size: " << sizeof(AudioMap) << std::endl;
}

bool ResourceManager::getIsRefresh()
{
	return isRefresh;
}

void ResourceManager::setIsRefresh(bool refresh)
{
	isRefresh = refresh;
}

std::string ResourceManager::getNewPrefab()
{
	return newPrefab;
}

void ResourceManager::setNewPrefab(std::string str)
{
	newPrefab = str;
}

void ResourceManager::insertTextMap(EntityID entityID, int id)
{
	_textMap.insert(std::pair<EntityID, int>(entityID, id));
}

void ResourceManager::setTextMap(EntityID entityID, int id)
{
	_textMap[entityID] = id;
}

void ResourceManager::eraseTextMap(EntityID entityID)
{
	_textMap.erase(entityID);
}

void ResourceManager::clearTextMap()
{
	_textMap.clear();
}

std::map<EntityID, int>& ResourceManager::getTextMap()
{
	// TODO: insert return statement here
	return _textMap;
}

int ResourceManager::getTextID(EntityID entityID)
{
	return _textMap[entityID];
}

std::vector<int>& ResourceManager::getTextToDeleteMap()
{
	// TODO: insert return statement here
	return _textToDelete;
}

void ResourceManager::insertTextToDeleteMap(int id)
{
	_textToDelete.push_back(id);
}

void ResourceManager::clearTextToDeleteMap()
{
	_textToDelete.clear();
}

void ResourceManager::DeserializePrefabs()
{
	LoadFolderObjects("Prefabs");
	//if (DeserializeFromFile("Resources/Prefabs/player.prefab") 
	//	&& DeserializeFromFile("Resources/Prefabs/floor.prefab"))
	//{
	//	std::cout << "PREFAB DESERIALIZE SUCCESS!" << std::endl;
	//	LoadPrefabs(); // debugging
	//}
}

void ResourceManager::LoadFolderObjects(std::string folder)
{
	std::string sub_path = ".//Resources//" + folder;
	for (auto& path : listDirectory(sub_path))
	{
		std::string name = path;
		if (folder == "Script")
		{
			//name = path.erase(0, 21);
			if (!name.empty())
				_ScriptVec.push_back(name);
		}
		else if (folder == "Prefabs")
		{
			if (DeserializeFromFile(path))
				std::cout << "PREFAB DESERIALIZE SUCCESS!" << std::endl;
		}

		std::cout << "loading: " << name << std::endl;
	}
}

void ResourceManager::LoadWayPoints()
{
	//std::string sub_path = ".//Resources//WayPoints";
	//for (auto& path : listDirectory(sub_path))
	//{
	//	std::string name = path;
	//	if (folder == "Script")
	//	{
	//		//name = path.erase(0, 21);
	//		if (!name.empty())
	//			_ScriptVec.push_back(name);
	//	}

	//	std::cout << "loading: " << name << std::endl;
	//}
}

void ResourceManager::RefreshPrefabsList()
{
	// clear all the prefabs map
	DeserializeFromFile(newPrefab);
}

//void ResourceManager::AddAudio(std::pair<std::string, FMOD_SOUND*> Audio)
//{
//	// insert to AudioMap
//}

std::vector<std::string> ResourceManager::listDirectory(std::filesystem::path path)
{
	namespace stdfs = std::experimental::filesystem;

	std::vector<std::string> filenames;

	const std::filesystem::directory_iterator end{};

	for (std::filesystem::directory_iterator iter{ path }; iter != end; ++iter)
	{
		if (std::filesystem::is_regular_file(*iter)) // comment out if all names (names of directories tc.) are required
			filenames.push_back(iter->path().string());
	}

	return filenames;
}