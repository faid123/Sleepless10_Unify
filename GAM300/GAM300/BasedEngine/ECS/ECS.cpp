#include "ECS.h"
#include <Graphics/TransformCom.h>
#include <Graphics/LightCom.h>
#include <Camera/CameraCom.h>
#include <Graphics/MaterialCom.h>
#include <Particles/EmitterComponent.h>
#include <Particles/ParticleComponent.h>
#include <Physics/RigidBodyComponent.h>
#include <Physics/PhysicsSystem.h>
#include <Logic/Lua/LuaScriptCom.h>
#include "Animation/AnimationCom.h"
#include "UserIntered/UICom.h"
#include "Graphics/SpriteCom.h"
#include "Audio/AudioSrcCom.h"
#include "Animation/AnimationCom.h"
#include "Graphics/TextCom.h"
#include <Graphics/Window/Time/Time.h>

void ECS::init()
{
	_entityManager = std::make_unique<EntityManager>();
	_componentManager = std::make_unique<ComponentManager>();
	_systemManager = std::make_unique<SystemManager>();
}

std::vector<Entity> ECS::getActiveEntities()
{
	return _entityManager->getActiveEntities();
}

std::vector<Prefab> ECS::getActivePrefabs()
{
	return _entityManager->getActivePrefabs();
}

EntityID ECS::instantiatePrefab(PrefabID prefabID, std::string name, Vector3 position, Quat rotation, 
								bool freshID, bool UIEntity, bool isPrefab)
{
	EntityID newID = _entityManager->newEntity(name, freshID, UIEntity, isPrefab);
	//std::cout << "new entity id: " << std::to_string(newID) << std::endl;
	//std::cout << "new entity name: " << Engine::_ecs.getEntity(newID)._entityName << std::endl;
	//Engine::_ecs.changeEntityName(Engine::_ecs.getEntity(newID)._entityName, name);
	//std::cout << "renamed entity name: " << Engine::_ecs.getEntity(newID)._entityName << std::endl;
	std::set<std::string> prefabTags = getPrefabTag(prefabID);
	for (auto tag : prefabTags)
	{
		addEntityTag(newID, tag);
	}

	if (Engine::_ecs.hasComponentPrefab<TransformCom>(prefabID))
	{
		auto transform = Engine::_ecs.getComponentPrefab<TransformCom>(prefabID);
		TransformCom c = *transform;
		Engine::_ecs.addComponentToEntity(newID, c);
	}
	if (Engine::_ecs.hasComponentPrefab<CameraCom>(prefabID))
	{
		auto camera = Engine::_ecs.getComponentPrefab<CameraCom>(prefabID);
		CameraCom c = *camera;
		Engine::_ecs.addComponentToEntity(newID, c);
	}
	if (Engine::_ecs.hasComponentPrefab<LightCom>(prefabID))
	{
		auto light = Engine::_ecs.getComponentPrefab<LightCom>(prefabID);
		LightCom c = *light;
		Engine::_ecs.addComponentToEntity(newID, c);
	}
	if (Engine::_ecs.hasComponentPrefab<MaterialCom>(prefabID))
	{
		auto material = Engine::_ecs.getComponentPrefab<MaterialCom>(prefabID);
		MaterialCom c = *material;
		Engine::_ecs.addComponentToEntity(newID, c);
	}
	if (Engine::_ecs.hasComponentPrefab<RigidDynamicCom>(prefabID))
	{
		auto rigid = Engine::_ecs.getComponentPrefab<RigidDynamicCom>(prefabID);
		RigidDynamicCom c = *rigid;
		Engine::_ecs.addComponentToEntity(newID, c);
		auto transform = Engine::_ecs.getComponent<TransformCom>(newID);
		//if (transform->state == 2) // Dynamic
		//{
		//	auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(newID);
		//	auto system = Engine::_ecs.getSystem<PhysicsSystem>();
		//	system->CreateRigidDynamicBody(*transform, *dynamicbody);
		//}
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(newID);
		dynamicbody->rigidPos = position;
		dynamicbody->rigidRot = rotation;
		auto system = Engine::_ecs.getSystem<PhysicsSystem>();
		if (dynamicbody)
			system->LoadRigidDynamicBody(newID, *dynamicbody);
	}
	if (Engine::_ecs.hasComponentPrefab<RigidStaticCom>(prefabID))
	{
		auto staticcom = Engine::_ecs.getComponentPrefab<RigidStaticCom>(prefabID);
		RigidStaticCom c = *staticcom;
		Engine::_ecs.addComponentToEntity(newID, c);
		auto transform = Engine::_ecs.getComponent<TransformCom>(newID);
		//if (transform->state == 1) // Static
		//{
		//	auto staticbody = Engine::_ecs.getComponent<RigidStaticCom>(newID);
		//	auto system = Engine::_ecs.getSystem<PhysicsSystem>();
		//	system->CreateRigidStaticBody(*transform, *staticbody);
		//}
		auto staticbody = Engine::_ecs.getComponent<RigidStaticCom>(newID);
		auto system = Engine::_ecs.getSystem<PhysicsSystem>();
		if (staticbody)
			system->LoadRigidStaticBody(newID, *staticbody);

	}
	if (Engine::_ecs.hasComponentPrefab<EmitterCom>(prefabID))
	{
		auto emitter = Engine::_ecs.getComponentPrefab<EmitterCom>(prefabID);
		EmitterCom c = *emitter;
		Engine::_ecs.addComponentToEntity(newID, c);
	}
	if (Engine::_ecs.hasComponentPrefab<LuaScriptCom>(prefabID))
	{
		auto script = Engine::_ecs.getComponentPrefab<LuaScriptCom>(prefabID);
		LuaScriptCom c = *script;
		Engine::_ecs.addComponentToEntity(newID, c);

		auto luaScript = Engine::_ecs.getComponent<LuaScriptCom>(newID);
		if (luaScript != nullptr)
		{
			luaScript->Load();
			luaScript->Awake();
			luaScript->Start();
		}
	}
	if (Engine::_ecs.hasComponentPrefab<UICom>(prefabID))
	{
		auto ui = Engine::_ecs.getComponentPrefab<UICom>(prefabID);
		UICom c = *ui;
		Engine::_ecs.addComponentToEntity(newID, c);
	}
	if (Engine::_ecs.hasComponentPrefab<SpriteCom>(prefabID))
	{
		auto sprite = Engine::_ecs.getComponentPrefab<SpriteCom>(prefabID);
		SpriteCom c = *sprite;
		Engine::_ecs.addComponentToEntity(newID, c);
	}
	if (Engine::_ecs.hasComponentPrefab<AnimationCom>(prefabID))
	{
		auto animation = Engine::_ecs.getComponentPrefab<AnimationCom>(prefabID);
		AnimationCom c = *animation;
		Engine::_ecs.addComponentToEntity(newID, c);
	}
	if (Engine::_ecs.hasComponentPrefab<AudioSrcCom>(prefabID))
	{
		auto audiosrc = Engine::_ecs.getComponentPrefab<AudioSrcCom>(prefabID);
		AudioSrcCom c = *audiosrc;
		Engine::_ecs.addComponentToEntity(newID, c);
	}
	if (Engine::_ecs.hasComponentPrefab<TextCom>(prefabID))
	{
		auto text = Engine::_ecs.getComponentPrefab<TextCom>(prefabID);
		TextCom c = *text;
		Engine::_ecs.addComponentToEntity(newID, c);
	}
	
	return newID;
}

EntityID ECS::createNewEntity(std::string name, bool freshID, bool UIEntity, bool isPrefab)
{
	return _entityManager->newEntity(name, freshID, UIEntity, isPrefab);
}

void ECS::destroyEntity(EntityID entity)
{
	// if entity is a parent, remove it from the parenting heirarchy
	// so child won't be deleted when game is in play
	if (!getChildEntities(entity).empty())
		removeEntityChild(entity);

	auto childs = _entityManager->getAllChildEntities(entity);

	// destroy the parent since childs container does not contain parent
	_componentManager->destroyEntity(entity);
	_systemManager->destroyEntity(entity);

	// entity manager settles all the destruction of child
	_entityManager->destroyEntity(entity);

	// will not remove if there is no reverb zone
	//AUDIOMGR->deleteReverbZone(entity);
	if (!childs.empty())
	{
		// destroy all the parent's children
		for (auto const& child : childs)
		{
			_componentManager->destroyEntity(child);
			_systemManager->destroyEntity(child);
			//// will not remove if there is no reverb zone
			//AUDIOMGR->deleteReverbZone(child);
		}
	}
}

void ECS::entityToDestroyUpdate()
{
	while (_entityDestroyQueue.size())
	{
		destroyEntity(_entityDestroyQueue.front());
		_entityDestroyQueue.pop();
	}
}

void ECS::addEntityToDestroyQueue(EntityID entity)
{
	_entityDestroyQueue.push(entity);

	// get the copy of entities to destroy
	auto childs = _entityManager->getAllChildEntities(entity);
	for (auto const& child : childs)
	{
		// add the children into the queue
		_entityDestroyQueue.push(child);
	}
}

void ECS::addEntityToDestroyQueueWithoutChild(EntityID entity)
{
	_entityDestroyQueue.push(entity);

	// if entity is a parent, remove it from the parenting heirarchy
	// so child won't be deleted when game is in play
	if (!getChildEntities(entity).empty())
		removeEntityChild(entity);

	// get the copy of entities to destroy
	auto childs = _entityManager->getAllChildEntities(entity);
	for (auto const& child : childs)
	{
		// add the children into the queue
		_entityDestroyQueue.push(child);
	}
}

bool ECS::hasEntity(std::string name)
{
	return _entityManager->hasEntity(name);
}

bool ECS::hasEntity(EntityID id)
{
	return _entityManager->hasEntity(id);
}

bool ECS::hasPrefab(PrefabID id)
{
	return _entityManager->hasPrefab(id);
}

EntityID ECS::getEntityID(std::string name)
{
	return _entityManager->getEntityID(name);
}

PrefabID ECS::getPrefabID(std::string name)
{
	return _entityManager->getPrefabID(name);
}

Entity& ECS::getEntity(std::string name)
{
	return _entityManager->getEntity(name);
}

Entity& ECS::getEntity(EntityID entityid)
{
	return _entityManager->getEntity(entityid);
}

Prefab& ECS::getPrefab(PrefabID prefabid)
{
	return _entityManager->getPrefab(prefabid);
}

Prefab& ECS::getPrefab(std::string name)
{
	return _entityManager->getPrefab(name);
}

std::string ECS::getEntityName(EntityID entity)
{
	return _entityManager->getEntityName(entity);
}

std::set<std::string> ECS::getEntityTag(EntityID entity)
{
	return _entityManager->getEntityTag(entity);
}

std::set<std::string> ECS::getPrefabTag(PrefabID prefab)
{
	return _entityManager->getPrefabTag(prefab);
}

bool ECS::hasTagInEntity(EntityID entity, std::string tag)
{
	auto temp = getEntityTag(entity);
	for (auto& tagName : temp)
	{
		if (tagName == tag)
			return true;
	}
	return false;
}

bool ECS::checkEntityHasTag(EntityID id, std::string tag)
{
	auto temp = getEntityTag(id);
	for (auto& name : temp)
	{
		if (name == tag)
			return true;
	}
	return false;
}

std::string ECS::getPrefabName(PrefabID prefab)
{
	return _entityManager->getPrefabName(prefab);
}

void ECS::changeEntityName(std::string oldName, std::string newName)
{
	_entityManager->changeEntityName(oldName, newName);
}

void ECS::changeEntityNameByID(EntityID entityID, std::string newName)
{
	_entityManager->changeEntityNameByID(entityID, newName);
}

bool ECS::checkName(std::string name)
{
	return _entityManager->checkName(name);
}

void ECS::createNewTag(std::string tag_name)
{
	_entityManager->createNewTag(tag_name);
}

void ECS::deleteTag(std::string tag_name)
{
	_entityManager->deleteTag(tag_name);
}

void ECS::addEntityTag(EntityID entity, std::string tag)
{
	_entityManager->addEntityTag(entity, tag);
}

void ECS::addToEntityTagsMap(EntityID entity, std::string tag)
{
	_entityManager->addToEntityTagsMap(entity, tag);
}

void ECS::addToPrefabTagsMap(PrefabID prefab, std::string tag)
{
	_entityManager->addToPrefabTagsMap(prefab, tag);
}

void ECS::removeEntityTag(EntityID entity, std::string tag)
{
	_entityManager->removeEntityTag(entity, tag);
}

std::vector<std::string> ECS::getTagsList()
{
	return _entityManager->getTagsList();
}

std::vector<EntityID> ECS::getEntityTagged(std::string name)
{
	auto tagg = _entityManager->getEntityTagged(name);
	std::vector<EntityID> temp{ tagg.begin(), tagg.end() };
	return temp;
}

void ECS::setPrefabParentToChild(EntityID entityID_parent, std::vector<EntityID> entityID_child)
{
	_entityManager->setPrefabParentToChild(entityID_parent, entityID_child);
}

std::vector<EntityID> ECS::findPrefabParentEntity(EntityID entityID)
{
	return _entityManager->findPrefabParentEntity(entityID);
}

void ECS::setEntityParent(EntityID child, EntityID parent)
{
	_entityManager->setEntityParent(child, parent);
}

void ECS::removeEntityParent(EntityID child)
{
	_entityManager->removeEntityParent(child);
}

void ECS::removeEntityChild(EntityID parent)
{
	_entityManager->removeEntityChild(parent);
}

std::set<EntityID> ECS::getChildEntities(EntityID child)
{
	return _entityManager->getChildEntities(child);
}

std::set<EntityID> ECS::getRootEntities()
{
	return _entityManager->getRootEntities();
}

EntityID ECS::getRoot(EntityID entity)
{
	return _entityManager->getRoot(entity);
}

std::vector<std::string> ECS::getEntityComponentNames(EntityID entity)
{
	std::vector<std::string> temp;
	for (ComponentID i = 0; i < _componentManager->getComponentsRegistered(); ++i)
	{
		if (getEntity(entity)._signature.test(i))
		{
			std::string temp_string = _componentManager->getComponentName(i);
			temp_string.erase(temp_string.begin(), (temp_string.begin() + 7));
			temp.push_back(temp_string);
		}
	}
	return temp;
}

void ECS::getEntityComponentPtrAndName(EntityID entity, std::vector<std::string>& names, std::vector<std::shared_ptr<Component>>& data)
{

	for (ComponentID i = 0; i < _componentManager->getComponentsRegistered(); ++i)
	{
		if (getEntity(entity)._signature.test(i))
		{
			std::string temp_string = _componentManager->getComponentName(i);
			temp_string.erase(temp_string.begin(), (temp_string.begin() + 7));
			names.push_back(temp_string);

			// here may need to do the hardcode
			if (temp_string.find("Trans"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<TransformCom>>
					(getComContainer(i))->getComponentData(entity));
			}
			else if (temp_string.find("Light"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<LightCom>>
					(getComContainer(i))->getComponentData(entity));
			}
			else if (temp_string.find("Camera"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<CameraCom>>
					(getComContainer(i))->getComponentData(entity));
			}
			else if (temp_string.find("Material"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<MaterialCom>>
					(getComContainer(i))->getComponentData(entity));
			}
			else if (temp_string.find("Emitter"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<EmitterCom>>
					(getComContainer(i))->getComponentData(entity));
			}
			else if (temp_string.find("Particle"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<ParticleCom>>
					(getComContainer(i))->getComponentData(entity));
			}
			else if (temp_string.find("RigidDynamic"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<RigidDynamicCom>>
					(getComContainer(i))->getComponentData(entity));
			}
			else if (temp_string.find("RigidStatic"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<RigidStaticCom>>
					(getComContainer(i))->getComponentData(entity));
			}
			else if (temp_string.find("Script"))
			{
				data.push_back(
					std::reinterpret_pointer_cast<ComponentContainer<LuaScriptCom>>(
					std::static_pointer_cast<
					ComponentContainer<LuaScriptCom>>
					(getComContainer(i))->getComponentData(entity)));
			}
			else if (temp_string.find("UI"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<UICom>>
					(getComContainer(i))->getComponentData(entity));
			}
			else if (temp_string.find("Sprite"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<SpriteCom>>
					(getComContainer(i))->getComponentData(entity));
			}
			else if (temp_string.find("Audio"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<AudioSrcCom>>
					(getComContainer(i))->getComponentData(entity));
			}
			else if (temp_string.find("Animation"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<AnimationCom>>
					(getComContainer(i))->getComponentData(entity));
			}
			else if (temp_string.find("Text"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<TextCom>>
					(getComContainer(i))->getComponentData(entity));
			}
			//data.push_back(getComContainer(i));
		}
	}

}

void ECS::getEntityComponentPtrAndNamePrefab(PrefabID prefab, std::vector<std::string>& names, std::vector<std::shared_ptr<Component>>& data)
{
	for (ComponentID i = 0; i < _componentManager->getComponentsRegistered(); ++i)
	{
		if (getPrefab(prefab)._signature.test(i))
		{
			std::string temp_string = _componentManager->getComponentName(i);
			temp_string.erase(temp_string.begin(), (temp_string.begin() + 7));
			names.push_back(temp_string);

			// here may need to do the hardcode
			if (temp_string.find("Trans"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<TransformCom>>
					(getComContainerPrefab(i))->getComponentDataPrefab(prefab));
			}
			else if (temp_string.find("Light"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<LightCom>>
					(getComContainerPrefab(i))->getComponentDataPrefab(prefab));
			}
			else if (temp_string.find("Camera"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<CameraCom>>
					(getComContainerPrefab(i))->getComponentDataPrefab(prefab));
			}
			else if (temp_string.find("Material"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<MaterialCom>>
					(getComContainerPrefab(i))->getComponentDataPrefab(prefab));
			}
			else if (temp_string.find("Emitter"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<EmitterCom>>
					(getComContainerPrefab(i))->getComponentDataPrefab(prefab));
			}
			else if (temp_string.find("Particle"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<ParticleCom>>
					(getComContainerPrefab(i))->getComponentDataPrefab(prefab));
			}
			else if (temp_string.find("RigidDynamic"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<RigidDynamicCom>>
					(getComContainerPrefab(i))->getComponentDataPrefab(prefab));
			}
			else if (temp_string.find("RigidStatic"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<RigidStaticCom>>
					(getComContainerPrefab(i))->getComponentDataPrefab(prefab));
			}
			else if (temp_string.find("nimation"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<AnimationCom>>
					(getComContainerPrefab(i))->getComponentDataPrefab(prefab));
			}

			else if (temp_string.find("UI"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<UICom>>
					(getComContainerPrefab(i))->getComponentDataPrefab(prefab));
			}
			else if (temp_string.find("Sprite"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<SpriteCom>>
					(getComContainerPrefab(i))->getComponentDataPrefab(prefab));
			}
			else if (temp_string.find("AudioSrc"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<AudioSrcCom>>
					(getComContainerPrefab(i))->getComponentDataPrefab(prefab));
			}
			else if (temp_string.find("Text"))
			{
				data.push_back(
					std::static_pointer_cast<
					ComponentContainer<TextCom>>
					(getComContainerPrefab(i))->getComponentDataPrefab(prefab));
			}
			//data.push_back(getComContainer(i));
		}
	}
}

std::vector<std::string> ECS::listAllComponents()
{
	return _componentManager->listAllComponents();
}

std::shared_ptr<Component> ECS::addComponent(EntityID entityID, std::string componentName)
{
	if (componentName == "TransformCom")
	{
		addComponentToEntity<TransformCom>(entityID, TransformCom());
		return getComponent<TransformCom>(entityID);
	}
	else if (componentName == "LightCom")
	{
		addComponentToEntity<LightCom>(entityID, LightCom());
		return getComponent<LightCom>(entityID);
	}
	else if (componentName == "CameraCom")
	{
		addComponentToEntity<CameraCom>(entityID, CameraCom());
		return getComponent<CameraCom>(entityID);
	}
	else if (componentName == "MaterialCom")
	{
		addComponentToEntity<MaterialCom>(entityID, MaterialCom());
		return getComponent<MaterialCom>(entityID);
	}
	else if (componentName == "EmitterCom")
	{
		addComponentToEntity<EmitterCom>(entityID, EmitterCom());
		return getComponent<EmitterCom>(entityID);
	}
	else if (componentName == "ParticleCom")
	{
		addComponentToEntity<ParticleCom>(entityID, ParticleCom());
		return getComponent<ParticleCom>(entityID);
	}
	else if (componentName == "RigidDynamicCom")
	{
		addComponentToEntity<RigidDynamicCom>(entityID, RigidDynamicCom());
		return getComponent<RigidDynamicCom>(entityID);
	}
	else if (componentName == "RigidStaticCom")
	{
		addComponentToEntity<RigidStaticCom>(entityID, RigidStaticCom());
		return getComponent<RigidStaticCom>(entityID);
	}
	else if (componentName == "uaScriptCom")
	{
		addComponentToEntity<LuaScriptCom>(entityID, LuaScriptCom());
		return std::reinterpret_pointer_cast<ComponentContainer<LuaScriptCom>>(getComponent<LuaScriptCom>(entityID));
	}
	else if (componentName == "AnimationCom")
	{
		addComponentToEntity<AnimationCom>(entityID, AnimationCom());
		return getComponent<AnimationCom>(entityID);
	}
	else if (componentName == "UICom")
	{
		addComponentToEntity<UICom>(entityID, UICom());
		return getComponent<UICom>(entityID);
	}
	else if (componentName == "SpriteCom")
	{
		addComponentToEntity<SpriteCom>(entityID, SpriteCom());
		return getComponent<SpriteCom>(entityID);
	}
	else if (componentName == "AudioSrcCom")
	{
		addComponentToEntity<AudioSrcCom>(entityID, AudioSrcCom());
		return getComponent<AudioSrcCom>(entityID);
	}
	else if (componentName == "TextCom")
	{
		addComponentToEntity<TextCom>(entityID, TextCom());
		return getComponent<TextCom>(entityID);
	}
	return nullptr;
}

std::shared_ptr<Component> ECS::addComponentPrefab(PrefabID prefabID, std::string componentName)
{
	if (componentName == "TransformCom")
	{
		addComponentToPrefab<TransformCom>(prefabID, TransformCom());
		return getComponentPrefab<TransformCom>(prefabID);
	}
	else if (componentName == "LightCom")
	{
		addComponentToPrefab<LightCom>(prefabID, LightCom());
		return getComponentPrefab<LightCom>(prefabID);
	}
	else if (componentName == "CameraCom")
	{
		addComponentToPrefab<CameraCom>(prefabID, CameraCom());
		return getComponentPrefab<CameraCom>(prefabID);
	}
	else if (componentName == "MaterialCom")
	{
		addComponentToPrefab<MaterialCom>(prefabID, MaterialCom());
		return getComponentPrefab<MaterialCom>(prefabID);
	}
	else if (componentName == "EmitterCom")
	{
		addComponentToPrefab<EmitterCom>(prefabID, EmitterCom());
		return getComponentPrefab<EmitterCom>(prefabID);
	}
	else if (componentName == "ParticleCom")
	{
		addComponentToPrefab<ParticleCom>(prefabID, ParticleCom());
		return getComponentPrefab<ParticleCom>(prefabID);
	}
	else if (componentName == "RigidDynamicCom")
	{
		addComponentToPrefab<RigidDynamicCom>(prefabID, RigidDynamicCom());
		return getComponentPrefab<RigidDynamicCom>(prefabID);
	}
	else if (componentName == "RigidStaticCom")
	{
		addComponentToPrefab<RigidStaticCom>(prefabID, RigidStaticCom());
		return getComponentPrefab<RigidStaticCom>(prefabID);
	}
	else if (componentName == "uaScriptCom")
	{
		addComponentToPrefab<LuaScriptCom>(prefabID, LuaScriptCom());
		return std::reinterpret_pointer_cast<ComponentContainer<LuaScriptCom>>(getComponentPrefab<LuaScriptCom>(prefabID));
	}
	else if (componentName == "UICom")
	{
		addComponentToPrefab<UICom>(prefabID, UICom());
		return getComponentPrefab<UICom>(prefabID);
	}
	else if (componentName == "SpriteCom")
	{
		addComponentToPrefab<SpriteCom>(prefabID, SpriteCom());
		return getComponentPrefab<SpriteCom>(prefabID);
	}
	else if (componentName == "AnimationCom")
	{
		addComponentToPrefab<AnimationCom>(prefabID, AnimationCom());
		return getComponentPrefab<AnimationCom>(prefabID);
	}
	else if (componentName == "AudioSrcCom")
	{
		addComponentToPrefab<AudioSrcCom>(prefabID, AudioSrcCom());
		return getComponentPrefab<AudioSrcCom>(prefabID);
	}
	else if (componentName == "TextCom")
	{
		addComponentToPrefab<TextCom>(prefabID, TextCom());
		return getComponentPrefab<TextCom>(prefabID);
	}
	return nullptr;
}

void ECS::systemInit()
{
	_systemManager->systemInit();
}

void ECS::systemLoad()
{
	_systemManager->systemLoad();
}

void ECS::systemUpdate(float dt)
{
	auto windowSys = Engine::_ecs.getSystem<WindowSystem>();
	_systemManager->systemUpdate(dt);
	glfwSwapBuffers(windowSys->getWindowPtr());
	if (Time::_currentNumberOfSteps > 0)
	{
		systemFixedUpdate(static_cast<float>(Time::_fixedDeltaTime), Time::_currentNumberOfSteps);
		Time::_accumulatedTime = 0.0;
	}
}

void ECS::systemFixedUpdate(float dt, int dtSteps)
{
	_systemManager->systemFixedUpdate(dt, dtSteps);
}

void ECS::systemUnload()
{
	_systemManager->systemUnload();
}

void ECS::systemRelease()
{
	_systemManager->systemRelease();
}

EntityID ECS::cloneEntity(EntityID entity)
{
	// create a map to know what child clones what component
	std::unordered_map<EntityID, EntityID> _map;
	// create parents first so child can have a parent id afterward that is active
	EntityID e = _entityManager->cloneEntity(entity, _map);

	for (auto const& map : _map)
	{
		// clone all components
		_componentManager->cloneEntityComponents(map.first, getEntity(map.second));
		// update all system of this cloned component
		_systemManager->updateSystemEntity(map.first, _entityManager->getEntitySignature(map.first));
	}

	return e;
}