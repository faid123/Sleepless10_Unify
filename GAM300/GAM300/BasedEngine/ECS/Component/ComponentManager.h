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
#include "Component.h"
#include "Serializer/ISerializer.h"

class ComponentManager:public ISerializer
{
public:
	ComponentManager() :
		_componentID{ 0 }
	{

	}
	~ComponentManager() = default;
	/// <summary>
	/// registering component list into the engine
	/// </summary>
	/// <typeparam name="T">the type of the component list</typeparam>
	template<typename T>
	void registerNewComponent()
	{
		std::string componentName = typeid(T).name();
		MY_ASSERT(_componentTypes.find(componentName) == _componentTypes.end());
		// set the component to a unique componentID and increase count for next component
		_componentNames[_componentID] = componentName;
		_componentTypes[componentName] = _componentID;
		//link componentID to the same component container to be able to see which entities has this component
		_componentContainersPrefab[_componentID] = std::make_shared<ComponentContainer<T>>();
		_componentContainers[_componentID++] = std::make_shared<ComponentContainer<T>>();
	}
	/// <summary>
	/// getting the component id 
	/// </summary>
	/// <typeparam name="T">type of the component</typeparam>
	/// <returns>the id that was saved in the engine</returns>
	template<typename T>
	ComponentID getComponentID()
	{
		std::string componentName = typeid(T).name();
		MY_ASSERT(_componentTypes.find(componentName) != _componentTypes.end());
		// return the component id
		return _componentTypes[componentName];
	}
	/// <summary>
	/// gettting component data under the entity id
	/// </summary>
	/// <typeparam name="T">type for the compoent</typeparam>
	/// <param name="entity">entity id</param>
	/// <returns>the data that was requested</returns>
	template <typename T>
	std::shared_ptr<T> getComponent(EntityID entity)
	{
		//return the component container's data of the entity given
		return getComponentContainer<T>()->getComponentData(entity);
	}
	/// <summary>
	/// getting component data under the prefab id
	/// </summary>
	/// <typeparam name="T">type for the component</typeparam>
	/// <param name="prefab"></param>
	/// <returns>the data that was requested</returns>
	template <typename T>
	std::shared_ptr<T> getComponentPrefab(PrefabID prefab)
	{
		//return the component container's data of the entity given
		return getComponentContainerPrefab<T>()->getComponentDataPrefab(prefab);
	}
	/// <summary>
	/// adding the component data into component list under the 
	/// entity id
	/// </summary>
	/// <typeparam name="T1">type for the list</typeparam>
	/// <typeparam name="T2">type for the component</typeparam>
	/// <param name="entity">entity id</param>
	/// <param name="component">component data</param>
	template <typename T1, typename T2>
	void addComponent(EntityID entity, T2 component)
	{
		// add the component data that is is tied with the entity
		getComponentContainer<T1>()->addComponentData(entity, std::make_shared<T2>(component));
	}
	/// <summary>
	/// adding prefab component data into component list
	/// </summary>
	/// <typeparam name="T3">type for the list</typeparam>
	/// <typeparam name="T4">type for the component</typeparam>
	/// <param name="prefab">prefab id</param>
	/// <param name="component">component data</param>
	template <typename T3, typename T4>
	void addComponentPrefab(PrefabID prefab, T4 component)
	{
		// add the component data that is is tied with the entity
		getComponentContainerPrefab<T3>()->addComponentDataPrefab(prefab, std::make_shared<T4>(component));
	}
	/// <summary>
	/// removing component data under the enitity 
	/// </summary>
	/// <typeparam name="T">component list type</typeparam>
	/// <param name="entity">entity id</param>
	template<typename T>
	void removeComponent(EntityID entity)
	{
		// remove the component data that consist of given entity
		getComponentContainer<T>()->removeComponentData(entity);
	}
	/// <summary>
	/// deleting entity from the component. where not component list will
	/// have this entity 
	/// </summary>
	/// <param name="entity">entity id</param>
	void destroyEntity(EntityID entity)
	{
		//iterate through all the containres and destroy the data that consist of given entity
		for (auto const& container : _componentContainers)
		{
			container.second->destroyEntity(entity);
		}
	}
	/// <summary>
	/// taking the last registered component id
	/// </summary>
	/// <returns>component id in the manager</returns>
	ComponentID getComponentsRegistered()
	{
		return _componentID;
	}
	/// <summary>
	/// getting the component name from component id
	/// </summary>
	/// <param name="id">component id</param>
	/// <returns>the name of the id</returns>
	std::string getComponentName(ComponentID id)
	{
		MY_ASSERT(id < _componentID);
		return _componentNames[id];
	}
	/// <summary>
	/// getting all registered component name
	/// </summary>
	/// <returns>a list component name</returns>
	std::vector<std::string> listAllComponents()
	{
		std::vector<std::string> temp_container{};
		for (const auto& componentTypes : _componentTypes)
		{
			std::string temp_name{};
			// remove the struct 
			temp_name.assign((componentTypes.first.begin() + 7), componentTypes.first.end());
			temp_container.push_back(temp_name);
		}
		return temp_container;
	}
	/// <summary>
	/// getting entity list that is link to the component
	/// </summary>
	/// <typeparam name="T1">component type</typeparam>
	/// <returns>entity list link to the component</returns>
	template <typename T1>
	std::set<EntityID> getEntityTypeIDs()
	{
		return getComponentContainer<T1>()->getEntityTypeIDs();
	}
	/// <summary>
	/// cloning component form entity
	/// </summary>
	/// <param name="entity1">copy to </param>
	/// <param name="entity2">copy from, and also uses the entire entity</param>
	void cloneEntityComponents(EntityID entity1, Entity entity2)
	{
		for (auto const& components : _componentContainers)
		{
			// check if the signature is true, if not do not bother going to the containerarray to serialize
			if (entity2._signature.test(components.first))
			{
				components.second->cloneComponent(entity1, entity2._entityID);
			}
		}
	}
	/// <summary>
	/// getting component list
	/// </summary>
	/// <param name="id">compoent id</param>
	/// <returns>the entire component list</returns>
	std::shared_ptr<Component> getComContainer(ComponentID id)
	{
		return _componentContainers[id];
	}
	/// <summary>
	/// getting prefab component list
	/// </summary>
	/// <param name="id">prefab id</param>
	/// <returns>the entire prefab component list</returns>
	std::shared_ptr<Component> getComContainerPrefab(ComponentID id)
	{
		return _componentContainersPrefab[id];
	}
	/// <summary>
	/// get all component list that have been registered
	/// </summary>
	/// <returns>a list of component list</returns>
	std::unordered_map <ComponentID, std::shared_ptr<Component>> getAllComponentMap()
	{
		return _componentContainers;
	}

	void Deserialize(const rapidjson::Document& doc, std::string id);
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer);
	void SerializePrefab(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer, std::string prefabName);
private:
	ComponentID _componentID;

	// stores name of component types
	std::unordered_map <std::string, ComponentID> _componentTypes{};
	std::unordered_map <ComponentID, std::string> _componentNames{};

	//store the pointer to the component types
	std::unordered_map <ComponentID, std::shared_ptr<Component> >_componentContainers{};
	std::unordered_map <ComponentID, std::shared_ptr<Component> >_componentContainersPrefab{};

	// Convenience function to get the statically casted pointer to the ComponentArray of type T.
	template<typename T>
	std::shared_ptr<ComponentContainer<T>> getComponentContainer()
	{
		std::string componentName = typeid(T).name();
		MY_ASSERT(_componentTypes.find(componentName) != _componentTypes.end());
		// get the container to the component
		return std::static_pointer_cast<ComponentContainer<T>>(_componentContainers[_componentTypes[componentName]]);
	}

	template<typename T>
	std::shared_ptr<ComponentContainer<T>> getComponentContainerPrefab()
	{
		std::string componentName = typeid(T).name();
		MY_ASSERT(_componentTypes.find(componentName) != _componentTypes.end());
		// get the container to the component
		return std::static_pointer_cast<ComponentContainer<T>>(_componentContainersPrefab[_componentTypes[componentName]]);
	}
};

