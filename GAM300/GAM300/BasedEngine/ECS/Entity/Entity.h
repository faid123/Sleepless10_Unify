/******************************************************************************
* \file			Component.h
* \brief		Contain all the Component related function
* \author 		Ang, Pheng Siang , 90% Code Contribution
*
* \copyright	Copyright (C) 2021 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#pragma once
#include <cstdint>
#include <bitset>
#include <queue>
#include <array>
#include <map>
#include <unordered_map>
#include <string>
#include "Assert/Assert.h"
#include "ECSType.h"
#include <set>
#include "Serializer/ISerializer.h"
 /// <summary>
 /// Entity class contain data for identifying each of them easily
 /// </summary>
struct Entity : public ISerializer
{
	Signature _signature{};
	bool _isActive{ false };
	EntityID _entityID{};
	EntityName _entityName;
	EntityID _parentID{};
	std::set<std::string> _tags{};

	void Deserialize(const rapidjson::Value& obj);
	void SerializePrefab(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);
	void Deserialize(const rapidjson::Document& doc, std::string id) { (void)id; (void)doc; }
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;
};
/// <summary>
/// Prefab class contain data for identifying each of them easily
/// </summary>
struct Prefab : public ISerializer
{
	Signature _signature{};
	PrefabID _prefabID{};
	PrefabName _prefabName;
	std::set<std::string> _tags{};

	//Reading from file to initialize all prefab, componenets and systems
	void Deserialize(const rapidjson::Value& obj);
	void Deserialize(const rapidjson::Document& doc, std::string id) { (void)id; (void)doc; }
	//Saving the File into the json string then into the file
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) override;
};
/// <summary>
/// entity manager handle all entity created
/// </summary>
class EntityManager
{
public:
	EntityManager() {};
	///////////////////////////////////////////////////////////////////
	// Checks entity 
	///////////////////////////////////////////////////////////////////
	bool hasEntity(std::string name);
	bool hasEntity(EntityID id);
	bool hasPrefab(PrefabID id);
	bool isActive(EntityID entityID);
	bool checkName(std::string name);
	///////////////////////////////////////////////////////////////////
	// Name related
	///////////////////////////////////////////////////////////////////
	void renameEntity(EntityID entityID, std::string name);
	void checkDuplicateName(std::string& name);
	void changeEntityName(std::string oldName, std::string newName);
	void changeEntityNameByID(EntityID entityID, std::string newName);
	///////////////////////////////////////////////////////////////////
	// created/ delete
	///////////////////////////////////////////////////////////////////
	EntityID newEntity(std::string name, bool freshID = false, bool UIEntity = false, bool isPrefabs = false);
	void destroyEntity(EntityID entity);
	EntityID cloneEntity(EntityID e2, std::unordered_map<EntityID, EntityID>& map);
	///////////////////////////////////////////////////////////////////
	// Getter function
	///////////////////////////////////////////////////////////////////
	Entity& getEntity(std::string name);
	Entity& getEntity(EntityID entityID);
	Prefab& getPrefab(PrefabID prefabID);
	Prefab& getPrefab(std::string name);
	std::string getEntityName(EntityID entityID);
	std::string getPrefabName(PrefabID prefabID);
	EntityID getEntityID(std::string name);
	PrefabID getPrefabID(std::string name);
	std::vector <Entity> getActiveEntities();
	std::vector<Prefab> getActivePrefabs();
	///////////////////////////////////////////////////////////////////
	// Root / parenting
	///////////////////////////////////////////////////////////////////
	EntityID getRoot(EntityID child);
	std::set<EntityID> getRootEntities();
	std::set<EntityID> getAllChildEntities(EntityID parent);
	std::set<EntityID> getChildEntities(EntityID parent);
	void setEntityParent(EntityID child, EntityID parent);
	void removeEntityParent(EntityID child);
	void removeEntityChild(EntityID parent);
	void setPrefabParentToChild(EntityID entityID_parent, std::vector<EntityID> entityID_child);
	std::map<EntityID, std::vector<EntityID>>* getPrefabParentToChild() { return &_prefabParentToChild; }
	std::vector<EntityID> findPrefabParentEntity(EntityID entityID);
	///////////////////////////////////////////////////////////////////
	// Signature
	///////////////////////////////////////////////////////////////////
	void setEntitySignature(EntityID  entityID, ComponentID componentID, bool set = true);
	void setPrefabSignature(PrefabID prefabID, ComponentID componentID, bool set = true);
	Signature getEntitySignature(EntityID entityID);
	Signature getPrefabSignature(PrefabID prefabID);
	///////////////////////////////////////////////////////////////////
	// Tag
	///////////////////////////////////////////////////////////////////
	void createNewTag(std::string tagName);
	void deleteTag(std::string tag);
	void renameTag(std::string oldTag, std::string newTag);
	std::vector<std::string> getTagsList();
	void addEntityTag(EntityID entityID, std::string tag);
	void addToEntityTagsMap(EntityID entityID, std::string tag);
	void addToPrefabTagsMap(PrefabID prefabID, std::string tag);
	void removeEntityTag(EntityID entityID, std::string tag);
	void removeAllEntityTag(EntityID entityID);
	std::set<EntityID> getEntityTagged(std::string tag);
	void cloneEntityTag(EntityID cloneTo, EntityID cloneFrom);
	std::set<std::string> getEntityTag(EntityID entityID);
	std::set<std::string> getPrefabTag(EntityID prefabID);

	void resetEntityCount(bool clearUI = false);

	void Deserialize(const rapidjson::Document& doc, std::string id);
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);
private:

	std::set<EntityID> _rootID{};
	std::unordered_map < EntityID, std::set<EntityID>> _parentToChildID{};

	std::unordered_map<std::string, std::set<EntityID>> _entityTags{};
	std::unordered_map<std::string, EntityID> _entityNameToID{};
	std::map<EntityID, std::string> _entityIDToName{};
	std::array<Entity, MAX_ENTITIES> _entities{};

	// START OF PREFAB
	std::set<PrefabID> _rootPrefabID{};
	std::unordered_map<std::string, std::set<PrefabID>> _prefabTags{};
	std::unordered_map<std::string, PrefabID> _prefabNameToID{};
	std::map<PrefabID, std::string> _prefabIDToName{};
	std::array<Prefab, MAX_PREFABS> _prefabs{};

	// used prefabParentToChild map for inheritance of cloning prefab
	// so one if one instance changed, all other follows.
	std::map <EntityID, std::vector<EntityID>> _prefabParentToChild{};

	PrefabID _prefabIDNumber{};
	std::vector<PrefabID> _prefabQueue{};
	// END OF PREFAB

	EntityID _entityIDNumber{};
	std::vector<EntityID> _entityQueue{};

	EntityID _entityUIIDNumber = ENTITIES;
	std::queue<EntityID> _entityQueueUI{};
};