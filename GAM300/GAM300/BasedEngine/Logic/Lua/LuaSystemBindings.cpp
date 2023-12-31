#include "Audio/AudioSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Camera/CameraSystem/CameraSystem.h"
#include "ScriptingSystem.h"
#include "Particles/ParticleSystem.h"
#include "Serializer/SerializerSystem.h"
#include "GameState.h"
#include "UserIntered/UI_System.h"
#include "Graphics/RenderSystem/RenderSystem.h"
#include "LuaScriptCom.h"
#include "ResourceManager/ResourceManager.h"

//Definition for registering systems

/// <summary>
/// Binding AudioSystem member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void AudioSystem::RegisterLua(sol::state& lua)
{
	lua.new_usertype<AudioSystem>("AudioSystem",
		//---------- Constructors ----------
		sol::constructors<
		AudioSystem()
		>(),

		//---------- Data ----------
		"_mResult", &AudioSystem::_mResult,
		"_mSystem", &AudioSystem::_mSystem,
		"_BGMGroup", &AudioSystem::_BGMGroup,
		"_SFXGroup", &AudioSystem::_SFXGroup,
		"_MasterGroup", &AudioSystem::_MasterGroup,
		"_listenerpos", &AudioSystem::_listenerpos,
		"_SoundNames", &AudioSystem::_SoundNames,

		//---------- Member Function ----------
		"init", &AudioSystem::init,
		"load", &AudioSystem::load,
		"update", &AudioSystem::update,
		"unload", &AudioSystem::unload,
		"release", &AudioSystem::release,
		"GetBGMChannelGroup", &AudioSystem::GetBGMChannelGroup,
		"GetSFXChannelGroup", &AudioSystem::GetSFXChannelGroup,
		"GetMasterChannelGroup", &AudioSystem::GetMasterChannelGroup,
		"InitChannelGroups", &AudioSystem::InitChannelGroups,
		"CreateBGM", &AudioSystem::CreateBGM,
		"PlayBGM", &AudioSystem::PlayBGM,
		"StopBGM", &AudioSystem::StopBGM,
		"CreateSFX", &AudioSystem::CreateSFX,
		"PlaySFX", &AudioSystem::PlaySFX,
		"Play3DSFX", &AudioSystem::Play3DSFX,
		"Stop3DSFX", &AudioSystem::Stop3DSFX,
		"PlayUISound", &AudioSystem::PlayUISound,
		"UpdateListener", &AudioSystem::UpdateListener,
		"ReadSoundsFromJson", &AudioSystem::ReadSoundsFromJson,
		"InsertSounds", &AudioSystem::InsertSounds,
		"LoadSounds", &AudioSystem::LoadSounds,
		"Deserialize", &AudioSystem::Deserialize,
		"increaseMasterVolume", &AudioSystem::increaseMasterVolume,
		"decreaseMasterVolume", &AudioSystem::decreaseMasterVolume,
		"getMasterVolume", &AudioSystem::getMasterVolume,
		"PauseBGM", &AudioSystem::PauseBGM,
		"StopUISound", &AudioSystem::StopUISound,
		"UnpauseBGM", &AudioSystem::UnpauseBGM,
		"MuteSound", &AudioSystem::MuteSound,
		"UnmuteSound", &AudioSystem::UnmuteSound
		); //END
}

/// <summary>
/// Binding CameraSystem member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void CameraSystem::RegisterLua(sol::state& lua)
{
	lua.new_usertype<CameraSystem>("CameraSystem",
		//---------- Constructors ----------
		sol::constructors<
		>(),

		//---------- Data ----------
		"_clickPoint", &CameraSystem::_clickPoint,
		"_prevClickPoint", &CameraSystem::_prevClickPoint,
		"_camType", &CameraSystem::_camType,
		"_editorCamera", &CameraSystem::_editorCamera,
		"_forward", &CameraSystem::_forward,
		"_right", &CameraSystem::_right,
		"_up", &CameraSystem::_up,
		"_screenShake", &CameraSystem::_screenShake,
		"_playerCamera", & CameraSystem::_playerCamera,
		//---------- Member Function ----------
		"init", &CameraSystem::init,
		"load", &CameraSystem::load,
		"update", &CameraSystem::update,
		"unload", &CameraSystem::unload,
		"release", &CameraSystem::release,
		"resetClickPoint", &CameraSystem::resetClickPoint,
		"editorCamUpdate", &CameraSystem::editorCamUpdate,
		"editorCamMovement", &CameraSystem::editorCamMovement,
		"playerCamUpdate", &CameraSystem::playerCamUpdate,
		"playerCamMovement", &CameraSystem::playerCamMovement,
		"updateCollisionCamera", &CameraSystem::updateCollisionCamera,
		"getPlayerCamera", &CameraSystem::getPlayerCamera,
		"getAppTime", &CameraSystem::getAppTime
		); //END
}



/// <summary>
/// Binding PhysicSystem member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void PhysicsSystem::RegisterLua(sol::state& lua)
{
	lua.new_usertype<PhysicsSystem>("PhysicsSystem",
		//---------- Constructors ----------
		sol::constructors<
		PhysicsSystem()
		>(),

		//---------- Data ----------
		"mAllocator", &PhysicsSystem::mAllocator,
		"mErrorCallback", &PhysicsSystem::mErrorCallback,
		"mToleranceScale", &PhysicsSystem::mToleranceScale,
		"mDispatcher", &PhysicsSystem::mDispatcher,
		"mFoundation", &PhysicsSystem::mFoundation,
		"mPhysics", &PhysicsSystem::mPhysics,
		"mScene", &PhysicsSystem::mScene,
		"mPvd", &PhysicsSystem::mPvd,

		//---------- Member Function ----------
		"init", &PhysicsSystem::init,
		"load", &PhysicsSystem::load,
		"update", &PhysicsSystem::update,
		"unload", &PhysicsSystem::unload,
		"release", &PhysicsSystem::release,
		"CreateRigidStaticBody", &PhysicsSystem::LoadRigidStaticBody,
		"CreateRigidDynamicBody", &PhysicsSystem::LoadRigidDynamicBody,
		"rotate_vector_by_quaternion", &PhysicsSystem::rotate_vector_by_quaternion,
		"rotate_vector_by_angle", &PhysicsSystem::rotate_vector_by_angle,
		"addForceToBody", &PhysicsSystem::addForceToBody,
		"addTorqueToBody", &PhysicsSystem::addTorqueToBody,
		"clearForce", &PhysicsSystem::clearForce,
		"getGlobalPoseRotation", &PhysicsSystem::getGlobalPoseRotation,
		"getGlobalPosePosition", &PhysicsSystem::getGlobalPosePosition,
		"setGlobalPose", &PhysicsSystem::setGlobalPose,
		"setBodyLinearDamping", &PhysicsSystem::setBodyLinearDamping,
		"setBodyAngularDamping", &PhysicsSystem::setBodyAngularDamping,
		"setBodyMass", &PhysicsSystem::setBodyMass,
		"getBodyLinearVelocity", &PhysicsSystem::getBodyLinearVelocity,
		"setBodyLinearVelocity", &PhysicsSystem::setBodyLinearVelocity,
		"getAngularVelocity", &PhysicsSystem::getAngularVelocity,
		"setAngularVelocity", &PhysicsSystem::setAngularVelocity,
		"ApplyRaycast", &PhysicsSystem::ApplyRaycast,
		"convertEulerToQuat", &PhysicsSystem::convertEulerToQuat,
		"convertQuatToEuler", &PhysicsSystem::convertQuatToEuler,
		"setGravityFlag", &PhysicsSystem::setGravityFlag,
		"setKinematicFlag", &PhysicsSystem::setKinematicFlag,
		"setKinematicTarget", &PhysicsSystem::setKinematicTarget,
		"LuaRemoveActor", &PhysicsSystem::LuaRemoveActor,
		"lookat", &PhysicsSystem::lookat,
		"RaycastHitEntityID", &PhysicsSystem::RaycastHitEntityID
		); //END
}

/// <summary>
/// Binding ScriptingSystem member functions and variable to use in LUA
/// </summary>
/// <param name="_lua">The sol state</param>
void ScriptingSystem::RegisterLua(sol::state& _lua)
{
	_lua.new_usertype<ScriptingSystem>("ScriptSystem",

		"_luaEntityID", &ScriptingSystem::_luaEntityID,
		"getLuaEntityID", &ScriptingSystem::getLuaEntityID,
		"getGameState", &ScriptingSystem::getGameState,
		"setGameState", &ScriptingSystem::setGameState,
		"setNState", & ScriptingSystem::setNState
		);
}

/// <summary>
/// Binding ParticleSystem member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void ParticleSystem::RegisterLua(sol::state& lua)
{
	lua.new_usertype<ParticleSystem>("ParticleSystem",
		//---------- Constructors ----------
		sol::constructors<
		ParticleSystem()
		>(),

		//---------- Member Functions ----------
		"init", &ParticleSystem::init,
		"load", &ParticleSystem::load,
		"update", &ParticleSystem::update,
		"unload", &ParticleSystem::unload,
		"release", &ParticleSystem::release,
		"Emit", &ParticleSystem::Emit
		);
}

/// <summary>
/// Binding UISystem member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void UISystem::RegisterLua(sol::state& lua)
{
	lua.new_usertype<UISystem>("UISystem",
		//---------- Constructors ----------
		sol::constructors<
		UISystem()
		>(),

		//---------- Data ----------
		"_threatPercent", & UISystem::_threatPercent,

		//---------- Member Functions ----------
		"increaseHealth", & UISystem::increaseHealth,
		"decreaseHealth", & UISystem::decreaseHealth,
		"increasePower", & UISystem::increasePower,
		"decreasePower", & UISystem::decreasePower,
		"increaseThreat", & UISystem::increaseThreat,
		"decreaseThreat", & UISystem::decreaseThreat,
		"setHealth", & UISystem::setHealth,
		"setPower", & UISystem::setPower,
		"setThreat", & UISystem::setThreat,
		

		"increasePistolHealth", & UISystem::increasePistolHealth,
		"decreasePistolHealth", & UISystem::decreasePistolHealth,
		"increaseLaserHealth", & UISystem::increaseLaserHealth,
		"decreaseLaserHealth", & UISystem::decreaseLaserHealth,
		"increaseRocketHealth", & UISystem::increaseRocketHealth,
		"decreaseRocketHealth", & UISystem::decreaseRocketHealth,
		"swapToWeapon", & UISystem::swapToWeapon,
		"getGameState", & UISystem::getGameState,
		"toggleCutscene", & UISystem::toggleCutscene,
		"setGameState", & UISystem::setGameState,
		"setPrevState", & UISystem::setPrevState,
		"goToPrevState", & UISystem::goToPrevState,
		"getPrevState", & UISystem::getPrevState,
		"setFullscreen", & UISystem::setFullscreen,
		"getFullscreenState", & UISystem::getFullscreenState,
		"ResetLevel", & UISystem::ResetLevel
		,"setGameState_wF2B", & UISystem::setGameState_wF2B
		,"getNextState", & UISystem::getNextState
		,"getChangingSceneState", & UISystem::getChangingSceneState
		,"setChangingSceneState", & UISystem::setChangingSceneState
		);
}


/// <summary>
/// Binding RenderSystem member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void RenderSystem::RegisterLua(sol::state& lua)
{
	lua.new_usertype<RenderSystem>("RenderSystem",
		//---------- Constructors ----------
		sol::constructors<
		RenderSystem()
		>(),

		//---------- Member Functions ----------
		"toggle_renderFramebuffer", & RenderSystem::toggle_renderFramebuffer
		, "toggle_renderFramebuffer", & RenderSystem::toggle_renderFramebuffer
		, "toggle_renderLightbox", & RenderSystem::toggle_renderLightbox
		, "toggleMinimap", & RenderSystem::toggleMinimap
		, "getMinimap", & RenderSystem::getMinimap
		, "ChangeSkybox", & RenderSystem::ChangeSkybox
		, "toggleCutscene", & RenderSystem::toggleCutscene
		, "getCutscene", & RenderSystem::getCutscene
		, "addGamma", & RenderSystem::addGamma
		, "getGammaVal", & RenderSystem::getGammaVal
		, "setGammaVal", & RenderSystem::setGammaVal

		, "setVideoName", & RenderSystem::setVideoName
		, "getVideoName", & RenderSystem::getVideoName
		, "hasVideoName", & RenderSystem::hasVideoName
		, "setVideoEndTime", & RenderSystem::setVideoEndTime
		, "setVideoStart", & RenderSystem::setVideoStart
		, "setPlayVideo", & RenderSystem::setPlayVideo
		, "getPlayVideo", & RenderSystem::getPlayVideo
		, "closeVideo", & RenderSystem::closeVideo
		, "getStartVideo", & RenderSystem::getStartVideo
		, "getFadetoBlack", & RenderSystem::getFadetoBlack
		, "getVideoMinimapAfter", & RenderSystem::getVideoMinimapAfter
		, "setVideoMinimapAfter", & RenderSystem::setVideoMinimapAfter
		);
}

/// <summary>
/// Binding ContactReport member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void ContactReportCallback::RegisterLua(sol::state& lua)
{
	lua.new_usertype<ContactReportCallback>("ContactReportCallback",

		//---------- Data Member ----------
		"playerOnFloor", &ContactReportCallback::playerOnFloor,
		"cubeOnFloor", &ContactReportCallback::cubeOnFloor,
		"cubeHitPlayer", &ContactReportCallback::cubeHitPlayer,
		"bulletHitBuilding", &ContactReportCallback::bulletHitBuilding,
		"bulletHitEnemy", & ContactReportCallback::bulletHitEnemy,
		"bulletHitCivilian", & ContactReportCallback::bulletHitCivilian,
		"ememyHitBuilding",&ContactReportCallback::ememyHitBuilding,
		"detectedBullet", & ContactReportCallback::_detectedBullet,
		"explosionHitEnemy", & ContactReportCallback::explosionHitEnemy,
		"explosionHitBuilding", & ContactReportCallback::explosionHitBuilding,
		"laserHitEnemy", & ContactReportCallback::laserHitEnemy,
		"laserHitCivilian", & ContactReportCallback::laserHitCivilian,
		"laserHitBuilding", & ContactReportCallback::laserHitBuilding,
		"laserHitEnemyRocket", & ContactReportCallback::laserHitEnemyRocket,
		"enemyHitPlayer", & ContactReportCallback::enemyHitPlayer,
		"collidedEnemy", & ContactReportCallback::_collidedEnemy,
		"cameraCollision1", & ContactReportCallback::cameraCollision1,
		"cameraCollision2", & ContactReportCallback::cameraCollision2,
		"bulletHitBoss", & ContactReportCallback::bulletHitBoss,
		"lazerHitBoss", & ContactReportCallback::lazerHitBoss,
		"rocketHitBoss", & ContactReportCallback::rocketHitBoss,
		"explosionHitBoss", & ContactReportCallback::explosionHitBoss,
		"playerHitBoss", & ContactReportCallback::playerHitBoss,
		"bossLaserHitPlayer", & ContactReportCallback::bossLaserHitPlayer,
		"bossRocketHitPlayer", &ContactReportCallback::bossRocketHitPlayer,
		"bossRocketHitBuilding", & ContactReportCallback::bossRocketHitBuilding,
		"_detectedExplosion", & ContactReportCallback::_detectedExplosion,
		"move", & ContactReportCallback::move,
		"enemyBulletHitPlayer", & ContactReportCallback::enemyBulletHitPlayer,
		"_detectedBuilding", & ContactReportCallback::_detectedBuilding,
		"_detectedCivilian", & ContactReportCallback::_detectedCivilian,
		"_detectedEnemy", & ContactReportCallback::_detectedEnemy,
		"laserHitTarget", & ContactReportCallback::laserHitTarget,
		"explosionHitTarget", & ContactReportCallback::explosionHitTarget,
		"bulletHitTarget", & ContactReportCallback::bulletHitTarget,
		"_detectedTarget", & ContactReportCallback::_detectedTarget,
		"minionExplosionHitPlayer", & ContactReportCallback::minionExplosionHitPlayer,
		"laserHitCage", & ContactReportCallback::laserHitCage,
		"laserHitBarrier", & ContactReportCallback::laserHitBarrier,
		"explosionHitPlayer", & ContactReportCallback::explosionHitPlayer,
		"bulletHitHelicopter", & ContactReportCallback::bulletHitHelicopter,
		"laserHitHelicopter", & ContactReportCallback::laserHitHelicopter,
		"explosionHitHelicopter", & ContactReportCallback::explosionHitHelicopter,
		"explosionHitCivilian", & ContactReportCallback::explosionHitCivilian,
		"enemyRocketHitHeli", & ContactReportCallback::enemyRocketHitHeli,

		"carHitPlayer", & ContactReportCallback::carHitPlayer,
		"_detectedBarrier", & ContactReportCallback::_detectedBarrier,
		"_detectedCage", & ContactReportCallback::_detectedCage,
		"_detectedLaser", & ContactReportCallback::_detectedLaser,

		//---------- Member Function ----------
		"onConstraintBreak", &ContactReportCallback::onConstraintBreak,
		"onWake", &ContactReportCallback::onWake,
		"onSleep", &ContactReportCallback::onSleep,
		"onTrigger", &ContactReportCallback::onTrigger,
		"onAdvance", &ContactReportCallback::onAdvance,
		"onContact", &ContactReportCallback::onContact,
		"getEntityHitID", &ContactReportCallback::getEntityHitID
		);
}

/// <summary>
/// Binding SerializerSystem member functions and variable to use in LUA
/// </summary>
/// <param name="lua">The sol state</param>
void SerializerSystem::RegisterLua(sol::state& lua)
{
	lua.new_usertype<SerializerSystem>("SerializerSystem",

		"setFileName", &SerializerSystem::setFileName,
		"setIsLoaded", &SerializerSystem::setIsLoaded
	);
}

void ResourceManager::RegisterLua(sol::state& lua)
{
	lua.new_usertype<ResourceManager>("ResourceManager",
		sol::constructors<
		>(),

		"GetWayPointMap", & ResourceManager::GetWayPointMap,
		"getWayPointVector3", &ResourceManager::getWayPointVector3,
		"GetWayPoint", & ResourceManager::GetWayPoint
		);
}