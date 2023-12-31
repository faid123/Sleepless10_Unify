/**********************************************************************************
* \file PrefabsManager.h
* \brief File contains the function definitions for all functions that are used
		 to serialize & deserialize prefabs.
* \author Shaikh Faid Bin Omar, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include <System/System.h>
#include <Engine.h>
#include <array>
#include <map>
#include <unordered_map>
#include <string>
#include "Assert/Assert.h"
#include "ECSType.h"
#include "Serializer/ISerializer.h"







//class PrefabsManager
//{
//	int prefabsCount{ 0 };
//
//public:
//	PrefabsManager();
//	~PrefabsManager() = default;
//
//	void init();
//	// Create the prefab to be used (deserialize prefabs .json file e.g. player.json)
//	PrefabID createPrefab(PrefabName prefabName);
//	// Create prefab instance using prefab name
//	EntityID createPrefabInstance(PrefabName prefabName);
//	// Clear all prefabs
//	void clearAllPrefab();
//
//	// Function to register prefabs to PrefabsMap (to be used during init of PrefabManager)
//	void registerPrefabToMap(PrefabName prefabName, std::string fp);
//	void registerPrefabToPrefabEntityMap(PrefabName prefabName);
//
//	Prefab& getPrefab(PrefabName prefabName);
//	Prefab& getPrefab(PrefabID prefabID);
//	std::string getPrefabName(PrefabID prefabID);
//	PrefabID getPrefabID(PrefabName prefabName);
//
//private:
//	// array of serialized prefabs
//	std::array<Prefab, MAX_PREFABS> _prefabs{};
//	std::map<PrefabID, PrefabID> _prefabIDToName{};
//	std::unordered_map<PrefabName, PrefabID> _prefabNameToID{};
//
//	// Map of prefabs with prefab name as key and filepath as value
//	std::map<PrefabName, std::string> prefabMap;
//
//	// Second map of prefab entities that hold the name as key and a vector of entities with the same key name
//	std::map<std::string, std::vector<EntityID>> prefabEntities;
//
//};
//
//extern PrefabsManager* PREFAB;