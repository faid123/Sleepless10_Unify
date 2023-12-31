#include "Audio/AudioSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Camera/CameraSystem/CameraSystem.h"
#include "ScriptingSystem.h"
#include "Particles/ParticleSystem.h"
#include "Graphics/SpriteCom.h"
#include "LuaScriptCom.h"
#include <Graphics/TextCom.h>

/// <summary>
/// Binding the ECS's member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void ECS::RegisterLua(sol::state& lua)
{
	lua.new_usertype<ECS>("ECS",
		//---------- Constructors ----------
		sol::constructors<
		>(),

		//---------- Member Function ----------
		"init", &ECS::init,
		"getActiveEntities", &ECS::getActiveEntities,
		"createNewEntity", &ECS::createNewEntity,
		"destroyEntity", &ECS::destroyEntity,
		"entityToDestroyUpdate", &ECS::entityToDestroyUpdate,
		"addEntityToDestroyQueue", &ECS::addEntityToDestroyQueue,
		"cloneEntity", &ECS::cloneEntity,
		"hasEntity",
		sol::overload(
			sol::resolve<bool(std::string)>(&ECS::hasEntity),
			sol::resolve<bool(EntityID)>(&ECS::hasEntity)
		),
		"getEntityID", &ECS::getEntityID,
		"getEntity",
		sol::overload(
			sol::resolve<Entity& (std::string)>(&ECS::getEntity),
			sol::resolve<Entity& (EntityID)>(&ECS::getEntity)
		),
		"getEntityName", &ECS::getEntityName,
		"changeEntityName", &ECS::changeEntityName,
		"changeEntityNameByID", &ECS::changeEntityNameByID,
		"checkName", &ECS::checkName,
		"createNewTag", &ECS::createNewTag,
		"deleteTag", &ECS::deleteTag,
		"addEntityTag", &ECS::addEntityTag,
		"removeEntityTag", &ECS::removeEntityTag,
		"getTagsList", &ECS::getTagsList,
		"getEntityTagged", &ECS::getEntityTagged,
		"hasTagInEntity", & ECS::hasTagInEntity,
		"getEntityComponentNames", &ECS::getEntityComponentNames,
		"listAllComponents", &ECS::listAllComponents,
		"getComponent_Transform", &ECS::getComponent<TransformCom>,
		"getComponent_Camera", &ECS::getComponent<CameraCom>,
		"getComponent_RigidStatic", &ECS::getComponent<RigidStaticCom>,
		"getComponent_RigidDynamic", &ECS::getComponent<RigidDynamicCom>,
		"getComponent_Light", &ECS::getComponent<LightCom>,
		"getComponent_Material", &ECS::getComponent<MaterialCom>,
		"getComponent_Particle", &ECS::getComponent<ParticleCom>,
		"getComponent_Emitter", &ECS::getComponent<EmitterCom>,
		"getComponent_LuaScriptCom", &ECS::getComponent<LuaScriptCom>,
		"getComponent_SpriteCom", &ECS::getComponent<SpriteCom>,
		"getComponent_TextCom", & ECS::getComponent<TextCom>,
		"getComponent_AnimationCom", & ECS::getComponent<AnimationCom>,
		"hasComponent_Transform", &ECS::hasComponent<TransformCom>,
		"hasComponent_Camera", &ECS::hasComponent<CameraCom>,
		"hasComponent_RigidStatic", &ECS::hasComponent<RigidStaticCom>,
		"hasComponent_RigidDynamic", &ECS::hasComponent<RigidDynamicCom>,
		"hasComponent_Light", &ECS::hasComponent<LightCom>,
		"hasComponent_Material", &ECS::hasComponent<MaterialCom>,
		"hasComponent_Particle", &ECS::hasComponent<ParticleCom>,
		"hasComponent_Emitter", &ECS::hasComponent<EmitterCom>,
		"hasComponent_LuaScriptCom", &ECS::hasComponent<LuaScriptCom>,
		"hasComponent_SpriteCom", &ECS::hasComponent<SpriteCom>,
		"hasComponent_TextCom", & ECS::hasComponent<TextCom>,
		"hasComponent_AnimationCom", &ECS::hasComponent<AnimationCom>,
		"removeComponentFromEntity_Transform", &ECS::removeComponentFromEntity<TransformCom>,
		"removeComponentFromEntity_Camera", &ECS::removeComponentFromEntity<CameraCom>,
		"removeComponentFromEntity_RigidStatic", &ECS::removeComponentFromEntity<RigidStaticCom>,
		"removeComponentFromEntity_RigidDynamic", &ECS::removeComponentFromEntity<RigidDynamicCom>,
		"removeComponentFromEntity_Light", &ECS::removeComponentFromEntity<LightCom>,
		"removeComponentFromEntity_Material", &ECS::removeComponentFromEntity<MaterialCom>,
		"removeComponentFromEntity_Particle", &ECS::removeComponentFromEntity<ParticleCom>,
		"removeComponentFromEntity_Emitter", &ECS::removeComponentFromEntity<EmitterCom>,
		"removeComponentFromEntity_LuaScriptCom", &ECS::removeComponentFromEntity<LuaScriptCom>,
		"removeComponentFromEntity_AnimationCom", &ECS::removeComponentFromEntity<AnimationCom>,
		"getActivePrefabs", &ECS::getActivePrefabs,
		"instantiatePrefab", &ECS::instantiatePrefab,
		"hasPrefab", &ECS::hasPrefab,
		"getPrefab",
		sol::overload(
			sol::resolve<Prefab& (std::string)>(&ECS::getPrefab),
			sol::resolve<Prefab& (PrefabID)>(&ECS::getPrefab)
		),
		"getPrefabName", &ECS::getPrefabName,
		"getComponentPrefab_Transform", &ECS::getComponentPrefab<TransformCom>,
		"getComponentPrefab_Camera", &ECS::getComponentPrefab<CameraCom>,
		"getComponentPrefab_RigidStatic", &ECS::getComponentPrefab<RigidStaticCom>,
		"getComponentPrefab_RigidDynamic", &ECS::getComponentPrefab<RigidDynamicCom>,
		"getComponentPrefab_Light", &ECS::getComponentPrefab<LightCom>,
		"getComponentPrefab_Material", &ECS::getComponentPrefab<MaterialCom>,
		"getComponentPrefab_Particle", &ECS::getComponentPrefab<ParticleCom>,
		"getComponentPrefab_Emitter", &ECS::getComponentPrefab<EmitterCom>,
		"getComponentPrefab_LuaScriptCom", &ECS::getComponentPrefab<LuaScriptCom>,
		"getComponentPrefab_AnimationCom", &ECS::getComponentPrefab<AnimationCom>,
		"hasComponentPrefab_Transform", &ECS::hasComponentPrefab<TransformCom>,
		"hasComponentPrefab_Camera", &ECS::hasComponentPrefab<CameraCom>,
		"hasComponentPrefab_RigidStatic", &ECS::hasComponentPrefab<RigidStaticCom>,
		"hasComponentPrefab_RigidDynamic", &ECS::hasComponentPrefab<RigidDynamicCom>,
		"hasComponentPrefab_Light", &ECS::hasComponentPrefab<LightCom>,
		"hasComponentPrefab_Material", &ECS::hasComponentPrefab<MaterialCom>,
		"hasComponentPrefab_Particle", &ECS::hasComponentPrefab<ParticleCom>,
		"hasComponentPrefab_Emitter", &ECS::hasComponentPrefab<EmitterCom>,
		"hasComponentPrefab_LuaScriptCom", &ECS::hasComponentPrefab<LuaScriptCom>,
		"hasComponentPrefab_AnimationCom", &ECS::hasComponentPrefab<AnimationCom>,
		"getPrefabID", & ECS::getPrefabID,
		"checkEntityHasTag", & ECS::checkEntityHasTag
		); //END1
}
