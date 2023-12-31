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

#include "../Entity/Entity.h"
#include <unordered_map>
#include <iostream>
#include <Properties.h>

class Component : public property::base
{
public:
	virtual ~Component() = default;
	virtual void destroyEntity(EntityID entity) = 0;
	virtual void cloneComponent(EntityID entity1, EntityID entity2) = 0;
	property_vtable()
};

property_begin(Component)
{
} property_vend_h(Component)


template<typename T>
class ComponentContainer : public Component
{
public:
	ComponentContainer() :
		_componentSize{ 0 },
		_prefabSize{ 0 }
	{
	}
	~ComponentContainer() = default;
	/// <summary>
	/// adding component data into the specific container
	/// </summary>
	/// <param name="entity">the number in the container</param>
	/// <param name="component">a newly created component to store</param>
	void addComponentData(EntityID entity, std::shared_ptr<T> component)
	{
		MY_ASSERT(_entityToInstance.find(entity) == _entityToInstance.end());
		_entityToInstance[entity] = _componentSize;
		_instanceToEntity[_componentSize] = entity;
		_componentContainer[_componentSize++] = component;
	}
	/// <summary>
	/// saving prefab component data
	/// </summary>
	/// <param name="prefab">the prefab id</param>
	/// <param name="component">data that is created</param>
	void addComponentDataPrefab(PrefabID prefab, std::shared_ptr<T> component)
	{
		MY_ASSERT(_prefabToInstance.find(prefab) == _prefabToInstance.end());
		_prefabToInstance[prefab] = _prefabSize;
		_instanceToPrefab[_prefabSize] = prefab;
		_componentContainerPrefab[_prefabSize++] = component;
	}
	/// <summary>
	/// deleting the entity from the component list
	/// </summary>
	/// <param name="entity">the entity id</param>
	void removeComponentData(EntityID entity)
	{
		if (_entityToInstance.find(entity) != _entityToInstance.end())
		{
			InstanceID removedInstanceID = _entityToInstance[entity];
			EntityID lastEntityID = _instanceToEntity[_componentSize - 1];

			_entityToInstance[lastEntityID] = removedInstanceID;
			_instanceToEntity[removedInstanceID] = lastEntityID;

			_componentContainer[removedInstanceID] = _componentContainer[_componentSize - 1];

			_entityToInstance.erase(entity);
			_instanceToEntity.erase(--_componentSize);
		}
	}
	/// <summary>
	/// getting the component data for the respective entity when the type is 
	/// define
	/// </summary>
	/// <param name="entity">the entity id</param>
	/// <returns>data that was requested</returns>
	std::shared_ptr<T> getComponentData(EntityID entity)
	{
		if (_entityToInstance.find(entity) != _entityToInstance.end())
			return _componentContainer[_entityToInstance[entity]];
		return nullptr;
	}
	/// <summary>
	/// getting the prefab component data
	/// </summary>
	/// <param name="prefab">the prefab id</param>
	/// <returns>data that was requested</returns>
	std::shared_ptr<T> getComponentDataPrefab(PrefabID prefab)
	{
		if (_prefabToInstance.find(prefab) != _prefabToInstance.end())
			return _componentContainerPrefab[_prefabToInstance[prefab]];
		return nullptr;
	}
	/// <summary>
	/// removing the entity from the component data
	/// </summary>
	/// <param name="entity">the entity id</param>
	void destroyEntity(EntityID entity) override
	{
		removeComponentData(entity);
	}
	/// <summary>
	/// making a copy from 2 to 1 
	/// </summary>
	/// <param name="entity1">entity copy to</param>
	/// <param name="entity2">entity copy from</param>
	void cloneComponent(EntityID entity1, EntityID entity2) override
	{
		addComponentData(entity1, clone<T>(entity2));
	}
	/// <summary>
	/// making the copy 
	/// </summary>
	/// <typeparam name="T">the type that was specified</typeparam>
	/// <param name="entity2">entity to make the data from</param>
	/// <returns>the copy of the requested entity data</returns>
	template<typename T>
	std::shared_ptr<T> clone(EntityID entity2)
	{
		return std::make_shared<T>(*getComponentData(entity2));
	}
	/// <summary>
	/// getting the entity that is active in the component
	/// </summary>
	/// <returns>active entity list</returns>
	std::set<EntityID> getEntityTypeIDs()
	{
		std::set<EntityID> e;
		// since it is all active entities in this container can fill up the set
		for (auto const& ent : _entityToInstance)
		{
			e.insert(ent.first);
		}
		return e;
	}
private:
	ComponentID _componentSize;
	std::array<std::shared_ptr<T>, MAX_ENTITIES> _componentContainer{};
	std::unordered_map <InstanceID, EntityID> _instanceToEntity{};
	std::unordered_map <EntityID, InstanceID> _entityToInstance{};

	// for prefab
	PrefabID _prefabSize;
	std::array<std::shared_ptr<T>, MAX_PREFABS> _componentContainerPrefab{};
	std::unordered_map <PrefabInstanceID, PrefabID> _instanceToPrefab{};
	std::unordered_map <PrefabID, PrefabInstanceID> _prefabToInstance{};
};

