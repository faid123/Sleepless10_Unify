/**********************************************************************************
* \file PhysicsSystem.h
* \brief File contains the function definitions for all functions that are used
		 in Physics System.
* \author Kenneth Yeo, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "../ECS/System/System.h"
#include "../ECS/Component/Component.h"

#include <PxPhysics.h>
#include <PxPhysicsAPI.h>
//#include "MathLib/Precompiled.h"
#include "Graphics/TransformCom.h"
#include "Physics/RigidBodyComponent.h"
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"
#include "../Physics/ContactReportCallback.h"

using namespace physx;

struct FilterGroup
{
	enum Enum
	{
		ePlayer = (1 << 0),
		eFloor = (1 << 1),
		eBullet = (1 << 2),
		eEnemy = (1 << 3),
		eCube = (1 << 4),
		eTriggerVolume = (1 << 5),
		eRocket = (1 << 6),
		eLaser = (1 << 7),
		eGenerator = (1 << 8),
		eExplosion = (1 << 9),
		eWeaponCase = (1 << 10),
		eCar = (1 << 11),
		eCivilian = (1 << 12),
		eCityObject = (1 << 13),
		eCamera = (1 << 14),
		eEnemyBullet = (1 << 15),
		eCage = (1 << 16),
		eTarget = (1 << 17),
		eWall = (1 << 18),
		eRiotShield = (1 << 19),
		eMinionExplosion = (1 << 20),
		eBarrier = (1 << 21),
		eEnemyRocket = (1 << 22),
		eHelicopter = (1 << 23),
		eTrapped = (1 << 24)
	};
};



class PhysicsSystem : public ISystem
{
	PxDefaultAllocator mAllocator;
	PxDefaultErrorCallback mErrorCallback;
	PxTolerancesScale mToleranceScale;
	PxDefaultCpuDispatcher* mDispatcher;

	PxFoundation* mFoundation;
	PxPhysics* mPhysics;
	PxScene* mScene;

	PxPvd* mPvd;
	ContactReportCallback mContactReportCallback;

	//PxControllerManager* mControllerManager;
public:

	PhysicsSystem();
	void init() override;
	void load() override;
	void update(float dt) override;
	void unload() override;
	void release() override;

	void LoadRigidStaticBody(EntityID eID, RigidStaticCom& staticbody);
	void LoadRigidDynamicBody(EntityID eID, RigidDynamicCom& dynamicbody);

	void AddRigidStaticBody(EntityID eID, RigidStaticCom& staticbody, TransformCom& trans);
	void AddRigidDynamicBody(EntityID eID, RigidDynamicCom& dynamicbody, TransformCom& trans);

	void rotate_vector_by_quaternion(Vector3& v, Quat& q, Vector3& vprime);
	void rotate_vector_by_angle(Vector3 v, float angle, Vector3& vprime);	

	void ApplyRaycast(Vector3 origin, Vector3 unitDir, float maxDist, Vector3& endPoint, std::string weaponType, Vector3 bulletSpawnPos, Vector3 camEye);
	EntityID RaycastHitEntityID(Vector3 origin, Vector3 unitDir, float maxDist);

	void addForceToBody(EntityID eID, Vector3 force, float scalar = 1.0f, int type = 0);
	void addTorqueToBody(EntityID eID, Vector3 torque, float scalar = 1.0f);

	void clearForce(EntityID eID);

	Quat getGlobalPoseRotation(EntityID eID);
	Vector3 getGlobalPosePosition(EntityID eID);
	void setGlobalPose(EntityID eID, Vector3 pos, Quat rotQuat);

	void setBodyLinearDamping(EntityID eID);
	void setBodyAngularDamping(EntityID eID);

	void setBodyMass(EntityID eID);

	Vector3 getBodyLinearVelocity(EntityID eID);
	void setBodyLinearVelocity(EntityID eID, Vector3 playerVel, float speedCap);

	Vector3 getAngularVelocity(EntityID eID);
	void setAngularVelocity(EntityID eID, Vector3 playerVel, float speedCap);
	Quat convertEulerToQuat(Vector3& euler);
	Vector3 convertQuatToEuler(Quat& quat);

	void setGravityFlag(EntityID eID, bool flag);

	void setKinematicFlag(EntityID eID, bool flag);
	void setKinematicTarget(EntityID eID, EntityID pID);

	ContactReportCallback& getContactCallback();

	static void RegisterLua(sol::state& lua);
	void RemoveActor(PxActor& actor);

	void LuaRemoveActor(EntityID eID);

	void SetupCollisionFiltering(PxRigidActor* actor, PxShape*& shape, PxU32 filterGroup, PxU32 filterMask);

	void HandleAllCollisions();

	Quat lookat(Vector3 origin, Vector3 target, Vector3 up);
};
