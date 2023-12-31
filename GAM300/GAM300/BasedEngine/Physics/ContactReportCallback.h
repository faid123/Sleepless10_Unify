/**********************************************************************************
* \file ContactReportCallback.h
* \brief File contains the function definitions for all functions that are used
		 in Contact Report Callback.
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
#include <vector>

class ContactReportCallback : public PxSimulationEventCallback
{
	std::vector<PxVec3> mContactPositions;
	std::vector<PxVec3> mContactImpulses;

	void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count);
	void onWake(PxActor** actors, PxU32 count);
	void onSleep(PxActor** actors, PxU32 count);
	void onTrigger(PxTriggerPair* pairs, PxU32 count);
	void onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32);
	void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs);

public:
	bool cubeOnFloor = false;
	bool cubeHitPlayer = false;

	bool bulletHitBuilding = false;
	bool bulletHitCivilian = false;
	bool bulletHitEnemy = false;
	bool bulletHitFloor = false;

	bool enemyHitPlayer = false;
	bool enemyBulletHitPlayer = false;
	bool minionExplosionHitPlayer = false;

	bool explosionHitEnemy = false;
	bool explosionHitBuilding = false;

	bool explosionHitCivilian = false;
	bool laserHitEnemy = false;
	bool laserHitCivilian = false;
	bool laserHitBuilding = false;
	bool laserHitCage = false;
	bool laserHitBarrier = false;
	bool laserHitEnemyRocket = false;

	bool carHitPlayer = false;

	bool civilianHitBuilding = false;
	bool ememyHitBuilding = false;
	bool playerOnFloor{ false };
	bool jumping{ false };

	bool cameraCollision1 = false;
	bool cameraCollision2 = false;
	bool move = false;

	bool rocketHitBoss = false;
	bool playerHitBoss = false;
	bool lazerHitBoss  = false;
	bool explosionHitBoss = false;
	bool bulletHitBoss = false;
	bool bossLaserHitPlayer = false;
	bool bossRocketHitPlayer = false;
	bool bossRocketHitBuilding = false;

	bool explosionHitPlayer = false;

	bool bulletHitHelicopter = false;
	bool laserHitHelicopter = false;
	bool explosionHitHelicopter = false;
	bool laserHitTarget = false;
	bool bulletHitTarget = false;
	bool explosionHitTarget = false;

	bool enemyRocketHitHeli = false;


	static void RegisterLua(sol::state& lua);

	EntityID _detectedBullet;
	EntityID _collidedEnemy;
	EntityID _detectedBuilding;
	EntityID _detectedCivilian;
	EntityID _detectedEnemy;
	EntityID _detectedTarget;
	EntityID _detectedCage;
	EntityID _detectedBarrier;
	EntityID _detectedLaser;
	EntityID _detectedFloor;

	EntityID _detectedExplosion;

	std::map<std::pair<std::string, std::string>, std::vector<EntityID>> _entityHitID;

	//std::map <EntityID, EntityID> _enemyHitID;
	//EntityID getEnemyHitID(EntityID ID);

	void UpdateOnCollisionBehaviours(EntityID e1, EntityID e2);
	void UpdateOffCollisionBehaviours(EntityID e1, EntityID e2);
	void OnGetHit(EntityID e1, EntityID e2);
	void OffGetHit(EntityID e1, EntityID e2);

	std::vector<EntityID> getEntityHitID(std::string weaponType, std::string entityType);
};