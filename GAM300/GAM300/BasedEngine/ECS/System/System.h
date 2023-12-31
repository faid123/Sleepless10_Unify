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
#include "../Component/ComponentManager.h"
#include <set>
#include <string>
#include <memory>
#include <algorithm>
/// <summary>
/// interface class for all system
/// </summary>
class ISystem
{
public:
	/// <summary>
	/// init for the system
	/// </summary>
	virtual void init() = 0;
	/// <summary>
	/// load for the system
	/// </summary>
	virtual void load() = 0;
	/// <summary>
	/// update for the system
	/// </summary>
	/// <param name="delta_time"></param>
	virtual void update(float delta_time) = 0;
	/// <summary>
	/// unload for the system
	/// </summary>
	virtual void unload() = 0;
	/// <summary>
	/// release for the system
	/// </summary>
	virtual void release() = 0;

	~ISystem() = default;
	std::set<EntityID> _entities{};
	bool _isActive{ true };
};
/// <summary>
/// system manager where it manage all registered system
/// </summary>
class SystemManager
{
public:
	/// <summary>
	/// register new system into the engine, the order of the system matter
	/// </summary>
	/// <typeparam name="T">system type</typeparam>
	/// <returns>the pointer for registered system</returns>
	template<typename T>
	std::shared_ptr<T> registerNewSystem()
	{
		std::string name = typeid(T).name();
		MY_ASSERT(_systems.find(name) == _systems.end());
		auto system = std::make_shared<T>();
		_systems.insert(std::pair{ name,system });
		_systemOrder.push_back(name);
		return system;
	}
	/// <summary>
	/// setting the component the system require
	/// </summary>
	/// <typeparam name="T">system type</typeparam>
	/// <param name="componentID">component id</param>
	/// <param name="set">to offet</param>
	template<typename T>
	void setSystemSignature(ComponentID componentID, bool set = true)
	{
		std::string systemName = typeid(T).name();
		MY_ASSERT(_systems.find(systemName) != _systems.end());
		_signatures[systemName].set(componentID, set);
	}
	/// <summary>
	/// deleting entity in the system link to the system
	/// </summary>
	/// <param name="entity">entity id</param>
	void destroyEntity(EntityID entity)
	{
		// remove entity from all systems
		for (auto const& system : _systems)
		{
			system.second->_entities.erase(entity);
		}
	}
	/// <summary>
	/// getter function to get the whole system
	/// </summary>
	/// <typeparam name="T">system type</typeparam>
	/// <returns>pointer to the system</returns>
	template<typename T>
	std::shared_ptr<T> getSystem()
	{
		std::string name = typeid(T).name();
		if (_systems.find(name) != _systems.end())
			return std::static_pointer_cast<T>(_systems[name]);
		else
			return nullptr;
	}
	/// <summary>
	/// update entity into the system as it save
	/// </summary>
	/// <param name="entity">entity id</param>
	/// <param name="signature">bitset to compare data</param>
	void updateSystemEntity(EntityID entity, Signature signature)
	{
		// loop through all systems
		for (auto const& system : _systems)
		{
			// if new component added, e.g 0111 & 0011, bitwise AND signature becomes 0011 which is use to check 
			// whether the entity still is required to be updated in the current system
			Signature bitwiseAndSig = signature & _signatures[system.first];
			// if different signatures means deleted
			if (bitwiseAndSig == _signatures[system.first])
			{
				// since it is using set, non-unique entity will not be added
				system.second->_entities.insert(entity);
			}
			// if there is such entity in the system and the signature of the entity after bitwise AND is not same as
			// system signature, remove it from the system set of entites.
			else if (system.second->_entities.find(entity) != system.second->_entities.end())
			{
				system.second->_entities.erase(entity);
			}
		}
	}
	/// <summary>
	/// calling all registered system init
	/// </summary>
	void systemInit()
	{
		for (auto const& name : _systemOrder)
		{
			_systems[name]->init();
		}
	}
	/// <summary>
	/// calling all registered system load
	/// </summary>
	void systemLoad()
	{
		for (auto const& name : _systemOrder)
		{
			_systems[name]->load();
		}
	}
	/// <summary>
	/// calling all registered system updated
	/// </summary>
	/// <param name="dt">delta time</param>
	void systemUpdate(float dt);
	
	void systemFixedUpdate(float dt, int dtSteps);

	/// <summary>
	/// calling all registered system unload
	/// </summary>
	void systemUnload()
	{
		auto temp = _systemOrder;
		std::reverse(temp.begin(), temp.end());
		for (auto const& name : temp)
		{
			_systems[name]->unload();
		}
	}
	/// <summary>
	/// calling all registered system release
	/// </summary>
	void systemRelease()
	{
		auto temp = _systemOrder;
		std::reverse(temp.begin(), temp.end());
		for (auto const& name : temp)
		{
			_systems[name]->release();
		}
	}

private:
	std::vector<std::string> _systemOrder{};	// unordered map does not have it ordered as insertion so this is the vector holding
	std::unordered_map<std::string, std::shared_ptr<ISystem>> _systems{};
	std::unordered_map<std::string, Signature> _signatures{};
};