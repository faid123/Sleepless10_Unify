#include "Entity.h"
#include <iostream>
#include <sstream>

bool EntityManager::checkName(std::string name)
{
	if (_entityNameToID.find(name) != _entityNameToID.end())
	{
		return true;
	}
	return false;
}

void EntityManager::renameEntity(EntityID entityID, std::string name)
{
	_entities[entityID]._entityName = name;
}

bool EntityManager::hasEntity(std::string name)
{
	return (_entityNameToID.find(name) != _entityNameToID.end());
}

bool EntityManager::hasEntity(EntityID id)
{
	return (_entityIDToName.find(id) != _entityIDToName.end());
}

bool EntityManager::hasPrefab(PrefabID id)
{
	return (_prefabIDToName.find(id) != _prefabIDToName.end());
}

Entity& EntityManager::getEntity(std::string name)
{
	MY_ASSERT(_entityNameToID.find(name) != _entityNameToID.end());
	return _entities[getEntityID(name)];
}

Entity& EntityManager::getEntity(EntityID entityID)
{
	MY_ASSERT(_entityIDToName.find(entityID) != _entityIDToName.end());
	return _entities[entityID];
}

Prefab& EntityManager::getPrefab(PrefabID prefabID)
{
	MY_ASSERT(_prefabIDToName.find(prefabID) != _prefabIDToName.end());
	return _prefabs[prefabID];
}

Prefab& EntityManager::getPrefab(std::string name)
{
	MY_ASSERT(_prefabNameToID.find(name) != _prefabNameToID.end());
	return _prefabs[getPrefabID(name)];
}

std::string EntityManager::getEntityName(EntityID entityID)
{
	MY_ASSERT(_entityIDToName.find(entityID) != _entityIDToName.end());
	return _entities[entityID]._entityName;
}

std::string EntityManager::getPrefabName(PrefabID prefabID)
{
	MY_ASSERT(_prefabIDToName.find(prefabID) != _prefabIDToName.end());
	return _prefabs[prefabID]._prefabName;
}

EntityID EntityManager::getEntityID(std::string name)
{
	MY_ASSERT(_entityNameToID.find(name) != _entityNameToID.end());
	return _entityNameToID[name];
}

PrefabID EntityManager::getPrefabID(std::string name)
{
	MY_ASSERT(_prefabNameToID.find(name) != _prefabNameToID.end());
	return _prefabNameToID[name];
}

std::set<std::string> EntityManager::getEntityTag(EntityID entityID)
{
	return _entities[entityID]._tags;
}

std::set<std::string> EntityManager::getPrefabTag(EntityID prefabID)
{
	return _prefabs[prefabID]._tags;
}

void EntityManager::setEntityParent(EntityID child, EntityID parent)
{
	if (parent != ROOTID)
	{
		// if there is existing parents, remove it first
		if (_entities[child]._parentID != ROOTID)
		{
			removeEntityParent(child);
		}
		// set the child parentid parameter to parent id
		_entities[child]._parentID = parent;
		// since the parent now has child, add the ID to parent-to-child container
		_parentToChildID[parent].insert(child);
		// remove child from the rootID container
		_rootID.erase(child);
	}
}

EntityID EntityManager::getRoot(EntityID child)
{
	MY_ASSERT(_entityIDToName.find(child) != _entityIDToName.end());
	EntityID tempid = _entities[child]._parentID;
	// loop through the parent id until it reaches the rootID (where there is no more parents entity)
	while (tempid != ROOTID)
	{
		child = tempid;
		tempid = _entities[child]._parentID;
	}
	return child;
}

std::set<EntityID> EntityManager::getRootEntities()
{
	return _rootID;
}

std::set<EntityID> EntityManager::getAllChildEntities(EntityID parent)
{
	std::set<EntityID> childEntities;
	// those stored in this map are active entities
	for (auto const& entities : _entityIDToName)
	{
		if (entities.first == parent)
			continue;

		EntityID parentID = _entities[entities.first]._parentID;
		// check this entity parents all the way till the root, if it reaches the root and 
		// have not found the parent id to be matched, it is not of the same family tree
		while (parentID != ROOTID)
		{
			// if parent is one of the child's further parents insert it and end this loop
			if (parentID == parent)
			{
				childEntities.insert(_entities[entities.first]._entityID);
				break;
			}
			// if it is not the given parentid parameter and not ROOT id, continue looping
			parentID = _entities[parentID]._parentID;
		}
	}
	return childEntities;
}

std::set<EntityID> EntityManager::getChildEntities(EntityID parent)
{
	// need to check if not map will create another key with default c-tor value of set
	if (_parentToChildID.find(parent) != _parentToChildID.end())
		return _parentToChildID[parent];

	// return empty container if no such parents in the container
	std::set<EntityID> _empty;
	return _empty;
}

void EntityManager::removeEntityParent(EntityID child)
{
	EntityID parentID = _entities[child]._parentID;
	// check if there it is available in the container since if it has a child the parents should exist inside
	// if the parentID is not the default rootID
	if ((parentID != ROOTID) && (_parentToChildID.find(parentID) != _parentToChildID.end()))
	{
		// erase the child from the parent set of child
		_parentToChildID[_entities[child]._parentID].erase(child);
		// set the child parent id to be root since it no longer has a parent
		_entities[child]._parentID = ROOTID;
		// add this entity to the rootID container
		_rootID.insert(child);

		// clear the key in the map if value is empty
		if (_parentToChildID[parentID].empty())
		{
			_parentToChildID.erase(parentID);
		}
	}
}

void EntityManager::removeEntityChild(EntityID parent)
{
	// check if parent has childs
	std::set<EntityID> childIDs = getChildEntities(parent);
	if (!childIDs.empty())
	{
		if (_parentToChildID.find(parent) != _parentToChildID.end())
		{
			// remove parent relation
			// loop through childIDs container and set .parentID = -1
			for (auto child : childIDs)
			{
				// erase the child from the parent set of child
				_parentToChildID[_entities[child]._parentID].erase(child);
				// set the child parent id to be root since it no longer has a parent
				_entities[child]._parentID = ROOTID;
				// add this entity to the rootID container
				_rootID.insert(child);
			}
			// remove the parent key map from _parentToChildID that keeps track of parenting
			// clear the key in the map if value is empty
			if (_parentToChildID[parent].empty())
			{
				_parentToChildID.erase(parent);
			}
		}
	}
}

void EntityManager::setEntitySignature(EntityID entityID, ComponentID componentID, bool set)
{
	MY_ASSERT(_entityIDToName.find(entityID) != _entityIDToName.end());
	_entities[entityID]._signature.set(componentID, set);
}

void EntityManager::setPrefabSignature(PrefabID prefabID, ComponentID componentID, bool set)
{
	MY_ASSERT(_prefabIDToName.find(prefabID) != _prefabIDToName.end());
	_prefabs[prefabID]._signature.set(componentID, set);
}

Signature EntityManager::getEntitySignature(EntityID entityID)
{
	MY_ASSERT(_entityIDToName.find(entityID) != _entityIDToName.end());
	return _entities[entityID]._signature;
}

Signature EntityManager::getPrefabSignature(PrefabID prefabID)
{
	MY_ASSERT(_prefabIDToName.find(prefabID) != _prefabIDToName.end());
	return _prefabs[prefabID]._signature;
}

std::vector<Entity> EntityManager::getActiveEntities()
{
	std::vector<Entity> activeEntity;
	//since entityNameToID variable has all the active entities, loop through it and push back all the active entities
	for (auto const& entities : _entityIDToName)
	{
		activeEntity.push_back(_entities[entities.first]);
	}
	return activeEntity;
}

std::vector<Prefab> EntityManager::getActivePrefabs()
{
	std::vector<Prefab> activePrefab;
	//since entityNameToID variable has all the active entities, loop through it and push back all the active entities
	for (auto const& prefabs : _prefabIDToName)
	{
		activePrefab.push_back(_prefabs[prefabs.first]);
	}
	return activePrefab;
}

void EntityManager::setPrefabParentToChild(EntityID entityID_parent, std::vector<EntityID> entityID_vec)
{
	_prefabParentToChild.insert(std::pair<EntityID, std::vector<EntityID>>(entityID_parent, entityID_vec));
}

std::vector<EntityID> EntityManager::findPrefabParentEntity(EntityID entityID)
{
	std::vector<EntityID> entity_vec;
	auto it = _prefabParentToChild.begin();
	bool found = false;
	for (; it != _prefabParentToChild.end(); ++it)
	{
		EntityID id_parent = it->first;
		for (auto vec : it->second)
		{
			EntityID id_child = vec;
			if (entityID == id_parent || entityID == id_child)
			{
				entity_vec.push_back(id_child);
				found = true;
			}
		}
		if (found)
			entity_vec.push_back(id_parent);
	}
	return entity_vec;
}

void EntityManager::createNewTag(std::string tagName)
{
	_entityTags.insert({ tagName, {} });
}

void EntityManager::deleteTag(std::string tag)
{
	// if there is such a tag
	if (_entityTags.find(tag) != _entityTags.end())
	{
		for (auto const& t : _entityTags[tag])
		{
			// iterate through all ID with this existing tag and erase it from the set
			_entities[t]._tags.erase(tag);
		}
		// after erasing from all ids, remove this tag from map
		_entityTags.erase(tag);
	}
}

void EntityManager::renameTag(std::string oldTag, std::string newTag)
{
	if ((_entityTags.find(oldTag) != _entityTags.end()) && (_entityTags.find(newTag) == _entityTags.end()))
	{
		for (auto const& t : _entityTags[oldTag])
		{
			// iterate through all ID with this existing tag and erase old tag and add new tag
			_entities[t]._tags.erase(oldTag);
			_entities[t]._tags.insert(newTag);
		}
		// change tag name
		auto node = _entityTags.extract(oldTag);
		node.key() = newTag;
		_entityTags.insert(std::move(node));
	}
}

std::vector<std::string> EntityManager::getTagsList()
{
	std::vector<std::string> tempTags;
	// store the names of all different tags inside a vector and return it.
	for (auto const& tags : _entityTags)
	{
		tempTags.push_back(tags.first);
	}
	return tempTags;
}

void EntityManager::addEntityTag(EntityID entityID, std::string tag)
{
	MY_ASSERT(_entityIDToName.find(entityID) != _entityIDToName.end());

	// add the name of the tag to the entity and insert this entity id to the entityTag map
	_entities[entityID]._tags.insert(tag);
	_entityTags[tag].insert(entityID);
}

void EntityManager::addToEntityTagsMap(EntityID entityID, std::string tag)
{
	_entityTags[tag].insert(entityID);
}

void EntityManager::addToPrefabTagsMap(PrefabID prefabID, std::string tag)
{
	_prefabTags[tag].insert(prefabID);
}

void EntityManager::removeEntityTag(EntityID entityID, std::string tag)
{
	MY_ASSERT(_entityIDToName.find(entityID) != _entityIDToName.end());
	// find if there is such a tag in the entity before any deletion
	if (_entities[entityID]._tags.find(tag) != _entities[entityID]._tags.end())
	{
		// erase the tag from the entity std::set
		_entities[entityID]._tags.erase(tag);
		// erase the entity from the list of those that have that tag
		_entityTags[tag].erase(_entityTags[tag].find(entityID));
	}
}

void EntityManager::removeAllEntityTag(EntityID entityID)
{
	for (auto const& tag : _entities[entityID]._tags)
	{
		// erase this entity id from the set of every entity tags
		_entityTags[tag].erase(entityID);
	}
	_entities[entityID]._tags = std::set<std::string>{};
}

std::set<EntityID> EntityManager::getEntityTagged(std::string tag)
{
	if (_entityTags.find(tag) == _entityTags.end())
	{
		std::set<EntityID> empty{};
		return empty;
	}
	return _entityTags[tag];
}

void EntityManager::cloneEntityTag(EntityID cloneTo, EntityID cloneFrom)
{
	std::set<std::string> tags = getEntity(cloneFrom)._tags;

	//loop through and copy
	for (auto const& tag : tags)
	{
		addEntityTag(cloneTo, tag);
	}
}

bool EntityManager::isActive(EntityID entityID)
{
	return _entities[entityID]._isActive;
}

void EntityManager::checkDuplicateName(std::string& name)
{
	int number = 1;
	std::stringstream ss;
	ss << name;
	while (_entityNameToID.find(ss.str()) != _entityNameToID.end())
	{
		// clear the stringstream
		ss.str(std::string());
		ss << name << " (" << number++ << ")";
	}
	name = ss.str();
}

void EntityManager::changeEntityName(std::string oldName, std::string newName)
{
	EntityID temp_id = getEntityID(oldName);
	checkDuplicateName(newName);
	// update data member in Entity 
	getEntity(oldName)._entityName = newName;
	// update name in entity to id map
	_entityIDToName.at(getEntityID(oldName)) = newName;
	// delete old entry in entitynametoID
	_entityNameToID.erase(oldName);
	// create new entry with new name but with same id
	_entityNameToID.insert({ newName, temp_id });
}

void EntityManager::changeEntityNameByID(EntityID entityID, std::string newName)
{
	std::string oldName = getEntityName(entityID);
	checkDuplicateName(newName);
	// update data member in Entity 
	getEntity(entityID)._entityName = newName;
	// update name in entity to id map
	_entityIDToName.at(getEntityID(oldName)) = newName;
	// delete old entry in entitynametoID
	_entityNameToID.erase(oldName);
	// create new entry with new name but with same id
	_entityNameToID.insert({ newName, entityID });
}

EntityID EntityManager::newEntity(std::string name, bool freshID, bool UIEntity, bool isPrefab)
{
	// check if reached limit of entities
	bool entityLimit;
	bool prefabLimit;
	if (UIEntity)
	{
		if (freshID)
			//MY_ASSERT(_entityUIIDNumber < ENTITIES_UI);
			entityLimit = (_entityUIIDNumber < MAX_ENTITIES);
		else
			//MY_ASSERT((!_entityQueueUI.empty() || _entityUIIDNumber < ENTITIES_UI));
			entityLimit = (!_entityQueueUI.empty() || _entityUIIDNumber < MAX_ENTITIES);
	}
	else if (isPrefab)
	{
		if (freshID)
			//MY_ASSERT(_entityIDNumber < ENTITIES);
			prefabLimit = (_prefabIDNumber < MAX_PREFABS);
		else
			//MY_ASSERT((!_entityQueue.empty() || _entityIDNumber < ENTITIES));
			prefabLimit = (!_prefabQueue.empty() || _prefabIDNumber < MAX_PREFABS);
	}
	else
	{
		if (freshID)
			//MY_ASSERT(_entityIDNumber < ENTITIES);
			entityLimit = (_entityIDNumber < ENTITIES);
		else
			//MY_ASSERT((!_entityQueue.empty() || _entityIDNumber < ENTITIES));
			entityLimit = (!_entityQueue.empty() || _entityIDNumber < ENTITIES);
	}
	//MY_ASSERT(entityLimit);
	//MY_ASSERT(prefabLimit);

	// check for duplicate name if not add a number behind
	if (!isPrefab)
		checkDuplicateName(name);
	// get un-used entity id
	uint32_t newID{};
	uint32_t newPrefabID{};

	// creating normal entity
	if (!UIEntity && !isPrefab)
	{
		if (!freshID && !_entityQueue.empty())
		{
			newID = _entityQueue.front();
			// remove un-used entity id from queue so it wont be used again
			_entityQueue.erase(_entityQueue.begin());
		}
		else
			newID = _entityIDNumber++;
	}
	else if (!UIEntity && isPrefab)
	{
		if (!freshID && !_prefabQueue.empty())
		{
			newPrefabID = _prefabQueue.front();
			// remove un-used entity id from queue so it wont be used again
			_prefabQueue.erase(_prefabQueue.begin());
		}
		else
			newPrefabID = _prefabIDNumber++;
	}

	// creating ui entity, different set of numbers
	else
	{
		if (!freshID && !_entityQueueUI.empty())
		{
			newID = _entityQueueUI.front();

			// remove un-used entity id from queue so it wont be used again
			_entityQueue.erase(_entityQueue.begin());
		}
		else
			newID = _entityUIIDNumber++;
	}

	if (isPrefab)
	{
		_prefabs[newPrefabID]._prefabID = newPrefabID;
		_prefabs[newPrefabID]._prefabName = name;

		// add this prefab to the rootprefabid since it has no parent
		_rootPrefabID.insert(newPrefabID);

		// tie active prefab name to it's id
		_prefabNameToID[name] = newPrefabID;
		_prefabIDToName[newPrefabID] = name;

		return newPrefabID;
	}
	else
	{
		// create the entity details
		_entities[newID]._entityID = newID;
		_entities[newID]._isActive = true;
		_entities[newID]._entityName = name;
		_entities[newID]._parentID = ROOTID;

		// add this entity to the rootid since it has no parent
		_rootID.insert(newID);

		// tie active entity name to it's id
		_entityNameToID[name] = newID;
		_entityIDToName[newID] = name;
		
		return newID;
	}
}

void EntityManager::destroyEntity(EntityID entity)
{
	//if there is such an entity
	if (_entityIDToName.find(entity) != _entityIDToName.end())
	{
		// check if it has child
		// if it has, recurr into the child and delete the child first
		if (_parentToChildID.find(entity) != _parentToChildID.end())
		{
			// get a copy so when deleting from container will not invalid the for each loop
			std::set<EntityID> _copy = _parentToChildID[entity];

			for (auto const& ent : _copy)
			{
				destroyEntity(ent);
			}
		}
		// remove entity from entityNameToID container as it is no longer active
		_entityNameToID.erase(_entities[entity]._entityName);
		_entityIDToName.erase(entity);
		// reset entity parameters
		_entities[entity]._isActive = false;
		_entities[entity]._entityID = 0;
		_entities[entity]._entityName.clear();
		_entities[entity]._signature.reset();
		removeAllEntityTag(entity);

		// remove entity from all tags that has it

		if (entity >= ENTITIES)
			_entityQueueUI.push(entity);
		else
			_entityQueue.push_back(entity);
		std::sort(_entityQueue.begin(), _entityQueue.end());
		// remove from the parent if there is any, function will not do anything if it is not a child
		removeEntityParent(entity);
		// remove from rootID, if there is no such entity, there will not be error thrown
		_rootID.erase(entity);
	}
}

EntityID EntityManager::cloneEntity(EntityID e2, std::unordered_map<EntityID, EntityID>& map)
{
	MY_ASSERT(_entityIDToName.find(e2) != _entityIDToName.end());

	// create a container that holds entityid to be cloned to for key and entityid to clone from value
	// to be used for the components cloning
	std::unordered_map<EntityID, EntityID> ToFrom;

	// check if it is a UI to clone
	bool UI = false;
	if (e2 >= ENTITIES)
		UI = true;

	// create the entity itself first
	EntityID e = newEntity(getEntityName(e2), true, UI);
	Entity& en1 = getEntity(e);
	Entity& en2 = getEntity(e2);
	cloneEntityTag(e, e2);
	en1._signature = en2._signature;

	// add the "entity to clone to" = "entity to copy from"
	map[e] = e2;

	// recur here till the final child and do the clone
	// if it is a parent with child
	if (_parentToChildID.find(e2) != _parentToChildID.end())
	{
		// get a copy so when deleting from container will not invalid the for each loop
		std::set<EntityID> _copy = _parentToChildID[e2];

		for (auto const& entity : _copy)
		{

			// if function enters here means the new clone has a parents which is the newly created 
			// entity on top
			EntityID ce1 = cloneEntity(entity, map);
			setEntityParent(ce1, e);
		}
	}

	// return 
	return e;
}

void EntityManager::resetEntityCount(bool clearUI)
{
	// clear queue and count
	_entityQueue.clear();
	_entityIDNumber = 0;
	// reset UI ID queue and count
	if (clearUI)
	{
		while (_entityQueueUI.size())
		{
			_entityQueueUI.pop();
		}
		_entityUIIDNumber = ENTITIES;
	}
}
