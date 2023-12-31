/******************************************************************************
* \file			System.h
* \brief		Contain all the System related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2021 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#pragma once
#include "System/System.h"
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"

class ECS
{
public:
	void init();
	///////////////////////////////////////////////////////////////////
	// Entity
	///////////////////////////////////////////////////////////////////
	std::vector<Entity> getActiveEntities();
	std::vector<Prefab> getActivePrefabs();
	EntityID instantiatePrefab(PrefabID prefabID, std::string name, Vector3 postition = Vector3{ 0,0,0 }, Quat rotation = Quat{ 0,0,0,1 },
								bool freshID = false, bool UIEntity = false, bool isPrefab = false);
	EntityID createNewEntity(std::string name, bool freshID = false, bool UIEntity = false, bool isPrefab = false);
	void destroyEntity(EntityID entity);
	void entityToDestroyUpdate();
	void addEntityToDestroyQueue(EntityID entity);
	void addEntityToDestroyQueueWithoutChild(EntityID entity);
	EntityID cloneEntity(EntityID entity);
	//void cloneEntity(EntityID entity, float x, float y);
	//EntityID createEmpty();

	bool hasEntity(std::string name);
	bool hasEntity(EntityID id);
	bool hasPrefab(PrefabID id);
	EntityID getEntityID(std::string name);
	PrefabID getPrefabID(std::string name);
	Entity& getEntity(std::string name);
	Entity& getEntity(EntityID entityid);
	Prefab& getPrefab(PrefabID prefabid);
	Prefab& getPrefab(std::string name);
	std::string getEntityName(EntityID entity);
	
	std::string getPrefabName(PrefabID prefab);
	void changeEntityName(std::string oldName, std::string newName);
	void changeEntityNameByID(EntityID entityID, std::string newName);
	bool checkName(std::string name);

	void createNewTag(std::string tag_name);
	void deleteTag(std::string tag_name);
	void addEntityTag(EntityID entity, std::string tag);
	void addToEntityTagsMap(EntityID entity, std::string tag);
	void addToPrefabTagsMap(PrefabID prefab, std::string tag);
	void removeEntityTag(EntityID entity, std::string tag);
	std::vector<std::string> getTagsList();
	std::vector<EntityID> getEntityTagged(std::string name);
	std::set<std::string> getEntityTag(EntityID entity);
	std::set<std::string> getPrefabTag(PrefabID prefab);
	bool hasTagInEntity(EntityID entity, std::string tag);
	bool checkEntityHasTag(EntityID id, std::string tag);

	void setPrefabParentToChild(EntityID entityID_parent, std::vector<EntityID> entityID_child);
	std::vector<EntityID> findPrefabParentEntity(EntityID entityID);

	void setEntityParent(EntityID child, EntityID parent);
	void removeEntityParent(EntityID child);
	void removeEntityChild(EntityID parent);

	std::set<EntityID> getChildEntities(EntityID child);
	std::set<EntityID> getRootEntities();
	EntityID getRoot(EntityID entity);

	std::vector<std::string> getEntityComponentNames(EntityID entity);
	void getEntityComponentPtrAndName(EntityID entity, std::vector<std::string>& names, std::vector<std::shared_ptr<Component>>& data);
	void getEntityComponentPtrAndNamePrefab(PrefabID prefab, std::vector<std::string>& names, std::vector<std::shared_ptr<Component>>& data);
	///////////////////////////////////////////////////////////////////
	// Component
	///////////////////////////////////////////////////////////////////
	std::vector<std::string> listAllComponents();
	template <typename T>
	void registerNewComponent()
	{
		_componentManager->registerNewComponent<T>();
	}

	// get component given template T
	template <typename T1>
	std::shared_ptr<T1> getComponent(EntityID entity)
	{
		return _componentManager->getComponent<T1>(entity);
	}

	// get component given template T
	template <typename T1>
	std::shared_ptr<T1> getComponentPrefab(PrefabID prefab)
	{
		return _componentManager->getComponentPrefab<T1>(prefab);
	}

	// get component ID given template T
	template <typename T>
	ComponentID getComponentID()
	{
		return _componentManager->getComponentID<T>();
	}

	template <typename T>
	bool hasComponent(EntityID entity)
	{
		return getEntity(entity)._signature.test(getComponentID<T>());
	}

	template <typename T>
	bool hasComponentPrefab(PrefabID prefabID)
	{
		return getPrefab(prefabID)._signature.test(getComponentID<T>());
	}

	// tie a component to the entity given template T
	template <typename T>
	void addComponentToEntity(EntityID entity, T component)
	{
		// add registered component to the entity
		_componentManager->addComponent<T>(entity, component);
		// set the corresponding bit of the entity's signature to 1
		_entityManager->setEntitySignature(entity, _componentManager->getComponentID<T>());
		// update the system to check if the entity should still remain in the system
		_systemManager->updateSystemEntity(entity, _entityManager->getEntitySignature(entity));
	}

	template <typename T>
	void addComponentToPrefab(PrefabID prefab, T component)
	{
		// add registered component to the entity
		_componentManager->addComponentPrefab<T>(prefab, component);
		// set the corresponding bit of the entity's signature to 1
		_entityManager->setPrefabSignature(prefab, _componentManager->getComponentID<T>());
		// update the system to check if the entity should still remain in the system
		//_systemManager->updateSystemEntity(entity, _entityManager->getEntitySignature(entity));
	}

	//// tie a component to the entity given template T1 & T2
	//template <typename T1, typename T2>
	//void addComponentToEntity(EntityID entity, T2 component)
	//{
	//	// add registered component to the entity
	//	_componentManager->addComponent<T1>(entity, component);
	//	// set the corresponding bit of the entity's signature to 1
	//	_entityManager->setEntitySignature(entity, _componentManager->getComponentID<T1>());
	//	// update the system to check if the entity should still remain in the system
	//	_systemManager->updateSystemEntity(entity, _entityManager->getEntitySignature(entity));
	//}

	template <typename T, typename ... Args>
	void luaAddComponent(EntityID entity, T component)
	{
		if constexpr (sizeof ...(Args) > 0)
		{
			// add registered component to the entity
			_componentManager->addComponent<Args...>(entity, component);

			// set the corresponding bit of the entity's signature to 1
			_entityManager->setEntitySignature(entity, _componentManager->getComponentID<Args...>());

		}
		else
		{
			// add registered component to the entity
			_componentManager->addComponent<T>(entity, component);

			// set the corresponding bit of the entity's signature to 1
			_entityManager->setEntitySignature(entity, _componentManager->getComponentID<T>());
		}
		// update the system to check if the entity should still remain in the system
		_systemManager->updateSystemEntity(entity, _entityManager->getEntitySignature(entity));

	}

	// remove acomponent from entity given template T 
	template <typename T>
	void removeComponentFromEntity(EntityID entity)
	{
		// remove registered component to the entity
		_componentManager->removeComponent<T>(entity);
		// set the corresponding bit of the entity's signature to 0
		_entityManager->setEntitySignature(entity, _componentManager->getComponentID<T>(), false);
		// update the system to check if the entity should still remain in the system
		_systemManager->updateSystemEntity(entity, _entityManager->getEntitySignature(entity));
	}

	template <typename T1>
	std::set<EntityID> getEntityTypeIDs()
	{
		return _componentManager->getEntityTypeIDs<T1>();
	}

	std::shared_ptr<Component> getComContainer(ComponentID id)
	{
		return _componentManager->getComContainer(id);
	}

	std::shared_ptr<Component> getComContainerPrefab(ComponentID id)
	{
		return _componentManager->getComContainerPrefab(id);
	}

	std::shared_ptr<Component> addComponent(EntityID entityID, std::string componentName);
	std::shared_ptr<Component> addComponentPrefab(PrefabID prefabID, std::string componentName);
	///////////////////////////////////////////////////////////////////
	// System
	///////////////////////////////////////////////////////////////////
	template <typename T>
	std::shared_ptr<T> registerSystem() 
	{
		return _systemManager->registerNewSystem<T>();
	}

	template<typename T>
	void setSystemSignature(ComponentID componentID, bool set = true)
	{
		_systemManager->setSystemSignature<T>(componentID, set);
	}

	template<typename T>
	std::shared_ptr<T> getSystem()
	{
		return _systemManager->getSystem<T>();
	}
	std::unique_ptr<EntityManager>& getEntityManager()
	{
		return _entityManager;
	}
	std::unique_ptr<ComponentManager>& getComponentManager()
	{
		return _componentManager;
	}

	void systemInit();
	void systemLoad();
	void systemUpdate(float dt);
	void systemFixedUpdate(float dt, int dtSteps);
	void systemUnload();
	void systemRelease();
	static void RegisterLua(sol::state& lua);
private:
	std::unique_ptr<EntityManager>		_entityManager;
	std::unique_ptr<ComponentManager>	_componentManager;
	std::unique_ptr<SystemManager>		_systemManager;
	std::queue<EntityID> _entityDestroyQueue;
};


