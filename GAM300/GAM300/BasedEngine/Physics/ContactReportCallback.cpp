#include "../Physics/ContactReportCallback.h"
#include "../Engine.h"
#include "../Physics/RigidBodyComponent.h"

/*///////////////////////////////////////// COLLISION CHECKS ////////////////////////////////////*/

void ContactReportCallback::onConstraintBreak(PxConstraintInfo* /*constraints*/, PxU32 /*count*/)
{
}

void ContactReportCallback::onWake(PxActor** /*actors*/, PxU32 /*count*/)
{
}

void ContactReportCallback::onSleep(PxActor** /*actors*/, PxU32 /*count*/)
{
}

void ContactReportCallback::onTrigger(PxTriggerPair* /*pairs*/, PxU32 /*count*/)
{
}

void ContactReportCallback::onAdvance(const PxRigidBody* const*, const PxTransform*, const PxU32)
{
}

void ContactReportCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs)
{
	(void)pairHeader;
	std::vector<PxContactPairPoint> contactPoints;

	for (PxU32 i = 0; i < nbPairs; i++)
	{
		PxU32 contactCount = pairs[i].contactCount;
		if (contactCount)
		{
			contactPoints.resize(contactCount);
			pairs[i].extractContacts(&contactPoints[0], contactCount);

			for (PxU32 j = 0; j < contactCount; j++)
			{
				mContactPositions.push_back(contactPoints[j].position);
				mContactImpulses.push_back(contactPoints[j].impulse);
			}
		}
	}

	for (PxU32 i = 0; i < nbPairs; i++)
	{
		const PxContactPair& cp = pairs[i];
		if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			EntityID e1 = static_cast<EntityID>((uintptr_t)(uint32_t*)(pairHeader.actors[0]->userData));
			EntityID e2 = static_cast<EntityID>((uintptr_t)(uint32_t*)(pairHeader.actors[1]->userData));

			UpdateOnCollisionBehaviours(e1, e2);
		}

		else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			EntityID e1 = static_cast<EntityID>((uintptr_t)(uint32_t*)(pairHeader.actors[0]->userData));
			EntityID e2 = static_cast<EntityID>((uintptr_t)(uint32_t*)(pairHeader.actors[1]->userData));

			UpdateOffCollisionBehaviours(e1, e2);
		}
	}
}

void ContactReportCallback::UpdateOnCollisionBehaviours(EntityID e1, EntityID e2)
{
	if (Engine::_ecs.hasEntity(e1) && Engine::_ecs.hasEntity(e2))
	{
		// collision behaviours for player
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Player"))
		{
			// player hit floor
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Floor"))
				if (!jumping)
					playerOnFloor = true;

			// player hit enemy
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Enemy"))
			{
				_collidedEnemy = e2;
				enemyHitPlayer = true;
				OnGetHit(e1, e2);
			}

			// player hit trigger
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Trigger"))
				OnGetHit(e1, e2);

			// player hit enemy bullet
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "EnemyBullet"))
			{
				enemyBulletHitPlayer = true;
				OnGetHit(e1, e2);
			}

			// player hit enemy rocket
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "EnemyRocket"))
			{
				OnGetHit(e1, e2);
			}

			// player hit car
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Car"))
			{
				carHitPlayer = true;
				OnGetHit(e1, e2);
			}

			// player hit boss laser
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Boss_Laser"))
			{
				bossLaserHitPlayer = true;
			}

			// player hit boss rocket
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Boss_Rocket"))
			{
				bossRocketHitPlayer = true;
				OnGetHit(e1, e2);
			}


			// player hit minion explosion
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "MinionExplosion"))
			{
				minionExplosionHitPlayer = true;
				OnGetHit(e1, e2);
			}




			// explosion hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "EnemyExplosion"))
			{
				explosionHitPlayer = true;
				OnGetHit(e1, e2);
			}
		}

		// enemy
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Enemy"))
		{
			// enemy hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Player"))
			{
				_collidedEnemy = e1;
				enemyHitPlayer = true;
				OnGetHit(e1, e2);
			}

			// enemy hit bullet
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Bullet"))
			{
				_detectedBullet = e2;
				_detectedEnemy = e1;
				bulletHitEnemy = true;
				OnGetHit(e1, e2);
			}


			// enemy hit laser
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Laser"))
			{
				_detectedLaser = e2;
				_detectedEnemy = e1;
				// If ID is not in the map then insert it
				if (std::find(_entityHitID[{"Enemy", "Laser"}].begin(), _entityHitID[{"Enemy", "Laser"}].end(), e1) == _entityHitID[{"Enemy", "Laser"}].end())
					_entityHitID[{"Enemy", "Laser"}].push_back(e1);
				laserHitEnemy = true;
				OnGetHit(e1, e2);
			}

			// enemy hit rocket
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Rocket"))
				OnGetHit(e1, e2);

			// enemy hit explosion
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Explosion"))
			{
				// If ID is not in the map then insert it
				if (std::find(_entityHitID[{"Enemy", "Explosion"}].begin(), _entityHitID[{"Enemy", "Explosion"}].end(), e1) == _entityHitID[{"Enemy", "Explosion"}].end())
					_entityHitID[{"Enemy", "Explosion"}].push_back(e1);

				_detectedEnemy = e1;
				explosionHitEnemy = true;
				OnGetHit(e1, e2);
			}
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "City"))
			{
				ememyHitBuilding = true;
				OnGetHit(e1, e2);
			}
			
		}

		// civilian
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Civilian"))
		{
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "City"))
			{
				civilianHitBuilding = true;
				OnGetHit(e1, e2);
			}

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Bullet"))
			{
				_detectedCivilian = e1;
				bulletHitCivilian = true;
				OnGetHit(e1, e2);
			}


			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Laser"))
			{
				// If ID is not in the map then insert it
				if (std::find(_entityHitID[{"Civilian", "Laser"}].begin(), _entityHitID[{"Civilian", "Laser"}].end(), e1) == _entityHitID[{"Civilian", "Laser"}].end())
					_entityHitID[{"Civilian", "Laser"}].push_back(e1);

				_detectedLaser = e2;
				_detectedCivilian = e1;
				laserHitCivilian = true;
				OnGetHit(e1, e2);
			}

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Rocket"))
				OnGetHit(e1, e2);

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Explosion"))
			{
				// If ID is not in the map then insert it
				if (std::find(_entityHitID[{"Civilian", "Explosion"}].begin(), _entityHitID[{"Civilian", "Explosion"}].end(), e1) == _entityHitID[{"Civilian", "Explosion"}].end())
					_entityHitID[{"Civilian", "Explosion"}].push_back(e1);

				_detectedCivilian = e1;
				explosionHitCivilian = true;
				OnGetHit(e1, e2);
			}
		}

		// bullet
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Bullet"))
		{
			// bullet hit building
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Building"))
			{
				_detectedBuilding = e2;
				bulletHitBuilding = true;
				OnGetHit(e1, e2);
			}

			// bullet hit enemy
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Enemy"))
			{
				_detectedBullet = e1;
				_detectedEnemy = e2;
				bulletHitEnemy = true;
				OnGetHit(e1, e2);
			}

			// bullet hit floor
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Floor"))
			{
				_detectedBullet = e1;
				_detectedFloor = e2;
				bulletHitFloor = true;
				OnGetHit(e1, e2);
			}

			// bullet hit civilian
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Civilian"))
			{
				_detectedCivilian = e2;
				bulletHitCivilian = true;
				OnGetHit(e1, e2);
			}

			// bullet hit generator
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Generator"))
			{
				_detectedBullet = e1;
				OnGetHit(e1, e2);
			}

			// bullet hit anything in the city
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "City"))
			{
				_detectedBullet = e1;
				OnGetHit(e1, e2);
			}

			// bullet hit boss
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Boss"))
			{
				bulletHitBoss = true;
				OnGetHit(e1, e2);
			}

			// bullet hit heli
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Helicopter"))
			{
				bulletHitHelicopter = true;
				OnGetHit(e1, e2);
			}


			// bullet hit cage
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Cage"))
			{
				OnGetHit(e1, e2);
			}

			// bullet hit target
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Target"))
			{
				_detectedTarget = e2;
				bulletHitTarget = true;
				OnGetHit(e1, e2);
			}

			// bullet hit wall
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Wall"))
			{
				OnGetHit(e1, e2);
			}

			// bullet hit riotshield
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "RiotShield"))
			{
				OnGetHit(e1, e2);
			}

			// bullet hit barrier
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Barrier"))
			{
				OnGetHit(e1, e2);
			}

		}

		// rocket
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Rocket"))
		{
			// rocket hit enemy
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Enemy"))
				OnGetHit(e1, e2);

			// rocket hit civilian
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Civilian"))
				OnGetHit(e1, e2);

			// rocket hit building
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Building"))
				OnGetHit(e1, e2);

			// rocket hit generator
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Generator"))
			{
				_detectedBullet = e1;
				OnGetHit(e1, e2);
			}
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Floor"))
			{
				_detectedBullet = e1;
				OnGetHit(e1, e2);
			}

			// rocket hit cage
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Cage"))
				OnGetHit(e1, e2);

			// rocket hit target
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Target"))
				OnGetHit(e1, e2);

			// rocket hit wall
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Wall"))
				OnGetHit(e1, e2);

			// rocket hit riotshield
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "RiotShield"))
				OnGetHit(e1, e2);

			// rocket hit barrier
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Barrier"))
				OnGetHit(e1, e2);
			// rocket hit boss
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Boss"))
			{
				OnGetHit(e1, e2);
				rocketHitBoss = true;
			}

			// rocket hit heli
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Helicopter"))
				OnGetHit(e1, e2);
		}

		// laser
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Laser"))
		{
			// laser hit building
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Building"))
			{
				// If ID is not in the map then insert it
				if (std::find(_entityHitID[{"Building", "Laser"}].begin(), _entityHitID[{"Building", "Laser"}].end(), e2) == _entityHitID[{"Building", "Laser"}].end())
					_entityHitID[{"Building", "Laser"}].push_back(e2);

				_detectedLaser = e1;
				_detectedBuilding = e2;
				laserHitBuilding = true;
				OnGetHit(e1, e2);
			}

			// laser hit enemy rocket
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Boss_Rocket"))
			{
				laserHitEnemyRocket = true;
				OnGetHit(e1, e2);
			}

			// laser hit enemy
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Enemy"))
			{
				_detectedLaser = e1;
				_detectedEnemy = e2;
				// If ID is not in the map then insert it
				if (std::find(_entityHitID[{"Enemy", "Laser"}].begin(), _entityHitID[{"Enemy", "Laser"}].end(), e2) == _entityHitID[{"Enemy", "Laser"}].end())
					_entityHitID[{"Enemy", "Laser"}].push_back(e2);
				laserHitEnemy = true;
				OnGetHit(e1, e2);
			}

			// laser hit civilian
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Civilian"))
			{
				// If ID is not in the map then insert it
				if (std::find(_entityHitID[{"Civilian", "Laser"}].begin(), _entityHitID[{"Civilian", "Laser"}].end(), e2) == _entityHitID[{"Civilian", "Laser"}].end())
					_entityHitID[{"Civilian", "Laser"}].push_back(e2);

				_detectedLaser = e1;
				_detectedCivilian = e2;
				laserHitCivilian = true;
				OnGetHit(e1, e2);
			}

			// laser hit generator
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Generator"))
			{
				_detectedBullet = e1;
				OnGetHit(e1, e2);
			}
			// lazer hit boss
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Boss"))
			{
				_detectedLaser = e1;
				lazerHitBoss = true;
				OnGetHit(e1, e2);
			}

			// laser hit heli
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Helicopter"))
			{
				laserHitHelicopter = true;
				OnGetHit(e1, e2);
			}
			// laser hit target
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Target"))
			{
				_detectedLaser = e1;
				laserHitTarget = true;
				OnGetHit(e1, e2);
			}

			// laser hit Cage
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Cage"))
			{
				_detectedCage = e2;
				laserHitCage = true;
				OnGetHit(e1, e2);
			}

			// laser hit Barrier
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Barrier"))
			{
				_detectedBarrier = e2;
				laserHitBarrier = true;
				OnGetHit(e1, e2);
			}
		}

		// building
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Building"))
		{
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Bullet"))
			{
				_detectedBullet = e2;
				_detectedBuilding = e1;
				bulletHitBuilding = true;
				OnGetHit(e1, e2);
			}

			// laser hit building
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Laser"))
			{
				// If ID is not in the map then insert it
				if (std::find(_entityHitID[{"Building", "Laser"}].begin(), _entityHitID[{"Building", "Laser"}].end(), e1) == _entityHitID[{"Building", "Laser"}].end())
					_entityHitID[{"Building", "Laser"}].push_back(e1);

				_detectedLaser = e2;
				_detectedBuilding = e1;
				laserHitBuilding = true;
				OnGetHit(e1, e2);
			}

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Explosion"))
			{
				// If ID is not in the map then insert it
				if (std::find(_entityHitID[{"Building", "Explosion"}].begin(), _entityHitID[{"Building", "Explosion"}].end(), e1) == _entityHitID[{"Building", "Explosion"}].end())
					_entityHitID[{"Building", "Explosion"}].push_back(e1);

				_detectedBuilding = e1;
				explosionHitBuilding = true;
				OnGetHit(e1, e2);
			}
		}

		// explosion
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Explosion"))
		{
			// explosion hit building
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Building"))
			{
				// If ID is not in the map then insert it
				if (std::find(_entityHitID[{"Building", "Explosion"}].begin(), _entityHitID[{"Building", "Explosion"}].end(), e2) == _entityHitID[{"Building", "Explosion"}].end())
					_entityHitID[{"Building", "Explosion"}].push_back(e2);

				_detectedExplosion = e1;
				_detectedBuilding = e2;
				explosionHitBuilding = true;
				OnGetHit(e1, e2);
			}

			// explosion hit enemy
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Enemy"))
			{
				// If ID is not in the map then insert it
				if (std::find(_entityHitID[{"Enemy", "Explosion"}].begin(), _entityHitID[{"Enemy", "Explosion"}].end(), e2) == _entityHitID[{"Enemy", "Explosion"}].end())
					_entityHitID[{"Enemy", "Explosion"}].push_back(e2);

				_detectedExplosion = e1;
				_detectedEnemy = e2;
				explosionHitEnemy = true;
				OnGetHit(e1, e2);
			}

			// explosion hit civilian
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Civilian"))
			{
				// If ID is not in the map then insert it
				if (std::find(_entityHitID[{"Civilian", "Explosion"}].begin(), _entityHitID[{"Civilian", "Explosion"}].end(), e2) == _entityHitID[{"Civilian", "Explosion"}].end())
					_entityHitID[{"Civilian", "Explosion"}].push_back(e2);

				_detectedExplosion = e1;
				_detectedCivilian = e2;
				explosionHitCivilian = true;
				OnGetHit(e1, e2);
			}

			// explosion hit heli
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Helicopter"))
			{
				explosionHitHelicopter = true;
				OnGetHit(e1, e2);
			}


			// explosion hit cage
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Cage"))
				OnGetHit(e1, e2);

			// explosion hit target
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Target"))
			{
				explosionHitTarget = true;
				OnGetHit(e1, e2);
			}

			// explosion hit wall
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Wall"))
				OnGetHit(e1, e2);

			// explosion hit riotshield
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "RiotShield"))
				OnGetHit(e1, e2);

			// explosion hit Barrier
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Barrier"))
				OnGetHit(e1, e2);

			// explosion hit Barrier
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Boss"))
			{
				explosionHitBoss = true;
				OnGetHit(e1, e2);
			}
		}

		// cube
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Cube"))
		{
			// cube hit floor
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Floor"))
				cubeOnFloor = true;

			// cube hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Player"))
			{
				cubeHitPlayer = true;
				OnGetHit(e1, e2);
			}
		}

		// boss rocket shower/homing
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Boss_Rocket"))
		{
			// boss rocket hit floor
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Floor"))
				OnGetHit(e1, e2);

			// boss rocket hit city
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "City"))
				OnGetHit(e1, e2);

			// boss rocket hit building
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Building"))
			{
				_detectedBuilding = e2;
				bossRocketHitBuilding = true;
				OnGetHit(e1, e2);
			}
		}

		// car
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Car"))
		{
			// car hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Player"))
			{
				carHitPlayer = true;
				OnGetHit(e1, e2);
			}

		}

		// camera 
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Camera"))
		{
			// cube hit floor
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Floor"))
			{
				if (e1 == Engine::_ecs.getEntityID("PlayerCamera1"))
					cameraCollision1 = true;
				else if (e1 == Engine::_ecs.getEntityID("PlayerCamera2"))
					cameraCollision2 = true;
			}

			// cube hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "City"))
			{
				if (e1 == Engine::_ecs.getEntityID("PlayerCamera1"))
					cameraCollision1 = true;
				else if (e1 == Engine::_ecs.getEntityID("PlayerCamera2"))
					cameraCollision2 = true;
			}
		}

		// enemy bullet
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "EnemyBullet"))
		{
			// enemy bullet hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Player"))
			{
				enemyBulletHitPlayer = true;
				OnGetHit(e1, e2);
			}

			// enemy bullet hit wall
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Wall"))
			{
				OnGetHit(e1, e2);
			}

			// enemy bullet hit Cage
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Cage"))
			{
				OnGetHit(e1, e2);
			}

			// enemy bullet hit target
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Target"))
			{
				OnGetHit(e1, e2);
			}

			// enemy bullet hit Barrier
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Barrier"))
			{
				OnGetHit(e1, e2);
			}

		}

		// enemy explosion
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "EnemyExplosion"))
		{
			// enemy explosion hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Player"))
			{
				explosionHitPlayer = true;
				OnGetHit(e1, e2);
			}
		}

		// helicopter
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Helicopter"))
		{
			// bullet hit helicopter
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Bullet"))
			{
				bulletHitHelicopter = true;
				OnGetHit(e1, e2);
			}

			// laser hit helicopter
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Laser"))
			{
				laserHitHelicopter = true;
				OnGetHit(e1, e2);
			}

			// explosion hit helicopter
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Explosion"))
			{
				explosionHitHelicopter = true;
				OnGetHit(e1, e2);
			}

			// rocket hit heli
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Rocket"))
				OnGetHit(e1, e2);
		}

		// Boss
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Boss_Laser"))
		{
			// enemy bullet hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Player"))
			{
				bossLaserHitPlayer = true;
				OnGetHit(e1, e2);
			}
		}

		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Boss_Rocket"))
		{
			// boss rocket hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Player"))
			{
				bossRocketHitPlayer = true;
				OnGetHit(e1, e2);
			}
		}

		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Boss"))
		{
			//  bullet hit boss
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Bullet"))
			{
				bulletHitBoss = true;
				OnGetHit(e1, e2);
			}

			// Laser hit boss
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Laser"))
			{
				_detectedLaser = e2;
				lazerHitBoss = true;
				OnGetHit(e1, e2);
			}

			// Explosion hit boss
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Explosion"))
			{
				explosionHitBoss = true;
				OnGetHit(e1, e2);
			}

			// Rocket hit boss
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Rocket"))
			{
				rocketHitBoss = true;
				OnGetHit(e1, e2);
			}
		}

		// enemy rocket
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "EnemyRocket"))
		{
			// enemy rocket hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Player"))
				OnGetHit(e1, e2);

			// enemy rocket hit floor
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Floor"))
				OnGetHit(e1, e2);

			// enemy rocket hit city obj
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "City"))
				OnGetHit(e1, e2);
		}

		//cage
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Cage"))
		{
			// Cage hit by bullet
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Bullet"))
			{
				OnGetHit(e1, e2);
			}

			// Cage hit by Enemy Bullet
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "EnemyBullet"))
			{
				OnGetHit(e1, e2);
			}

			// Cage hit by Explosion
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Explosion"))
			{
				OnGetHit(e1, e2);
			}

			// Cage hit by Rocket
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Rocket"))
			{
				OnGetHit(e1, e2);
			}

			// Cage hit by Laser
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Laser"))
			{
				_detectedCage = e1;
				laserHitCage = true;
				OnGetHit(e1, e2);
			}
		}

		//target
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Target"))
		{
			// Target hit by laser
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Laser"))
			{
				_detectedLaser = e2;
				laserHitTarget = true;
				OnGetHit(e1, e2);
			}

			// Target hit by Explosion
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Explosion"))
			{
				explosionHitTarget = true;
				OnGetHit(e1, e2);
			}

			// Target hit by Rocket
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Rocket"))
			{
				OnGetHit(e1, e2);
			}

			// Target hit by Bullet
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Bullet"))
			{
				_detectedTarget = e1;
				bulletHitTarget = true;
				OnGetHit(e1, e2);
			}

			// Target hit by EnemyBullet
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "EnemyBullet"))
			{
				OnGetHit(e1, e2);
			}
		}

		//Wall
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Wall"))
		{
			// Wall hit by bullet
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Bullet"))
			{
				OnGetHit(e1, e2);
			}

			// Wall hit by Enemy Bullet
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "EnemyBullet"))
			{
				OnGetHit(e1, e2);
			}

			// Wall hit by Explosion
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Explosion"))
			{
				OnGetHit(e1, e2);
			}

			// Wall hit by Rocket
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Rocket"))
			{
				OnGetHit(e1, e2);
			}
		}

		//RiotShield
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "RiotShield"))
		{
			// Wall hit by bullet
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Bullet"))
			{
				OnGetHit(e1, e2);
			}

			// Wall hit by Explosion
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Explosion"))
			{
				OnGetHit(e1, e2);
			}

			// Wall hit by Rocket
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Rocket"))
			{
				OnGetHit(e1, e2);
			}
		}

		// Minion explosion
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "MinionExplosion"))
		{
			// Minion explosion hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Player"))
			{
				minionExplosionHitPlayer = true;
				OnGetHit(e1, e2);
			}
		}

		//Barrier
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Barrier"))
		{
			// Barrier hit by bullet
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Bullet"))
			{
				OnGetHit(e1, e2);
			}

			// Barrier hit by Enemy Bullet
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "EnemyBullet"))
			{
				OnGetHit(e1, e2);
			}

			// Barrier hit by Explosion
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Explosion"))
			{
				OnGetHit(e1, e2);
			}

			// Barrier hit by Rocket
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Rocket"))
			{
				OnGetHit(e1, e2);
			}

			// Barrier hit by Laser
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Laser"))
			{
				_detectedBarrier = e1;
				laserHitBarrier = true;
				OnGetHit(e1, e2);
			}
		}
	}
}

void ContactReportCallback::UpdateOffCollisionBehaviours(EntityID e1, EntityID e2)
{
	if (Engine::_ecs.hasEntity(e1) && Engine::_ecs.hasEntity(e2))
	{
		// collision behaviours for player
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Player"))
		{
			// player hit floor
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Floor"))
				playerOnFloor = false;

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "EnemyBullet"))
				enemyBulletHitPlayer = false;

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Boss_Rocket"))
				bossRocketHitPlayer = false;

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Boss_Laser"))
				bossLaserHitPlayer = false;

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Enemy"))
				enemyHitPlayer = false;

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "EnemyExplosion"))
				explosionHitPlayer = false;

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "MinionExplosion"))
			{
				minionExplosionHitPlayer = false;
				OffGetHit(e1, e2);
			}

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "EnemyRocket"))
				OffGetHit(e1, e2);
		}

		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Floor"))
		{
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "EnemyRocket"))
				OffGetHit(e1, e2);
		}

		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "City"))
		{
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "EnemyRocket"))
				OffGetHit(e1, e2);
		}

		// enemy
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Enemy"))
		{
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Building"))
				ememyHitBuilding = false;

			// enemy hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Player"))
				enemyHitPlayer = false;

			// enemy hit laser
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Laser"))
			{
				std::vector<EntityID>::iterator position = std::find(_entityHitID[{"Enemy", "Laser"}].begin(), _entityHitID[{"Enemy", "Laser"}].end(), e1);
				if (position != _entityHitID[{"Enemy", "Laser"}].end())
					_entityHitID[{"Enemy", "Laser"}].erase(position);
				laserHitEnemy = false;
			}

			// enemy hit explosion
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Explosion"))
			{
				std::vector<EntityID>::iterator position = std::find(_entityHitID[{"Enemy", "Explosion"}].begin(), _entityHitID[{"Enemy", "Explosion"}].end(), e1);
				if (position != _entityHitID[{"Enemy", "Explosion"}].end())
					_entityHitID[{"Enemy", "Explosion"}].erase(position);
				explosionHitEnemy = false;
			}

		}

		// Civilian
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Civilian"))
		{
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Building"))
				civilianHitBuilding = false;

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Explosion"))
			{
				std::vector<EntityID>::iterator position = std::find(_entityHitID[{"Civilian", "Explosion"}].begin(), _entityHitID[{"Civilian", "Explosion"}].end(), e1);
				if (position != _entityHitID[{"Civilian", "Explosion"}].end())
					_entityHitID[{"Civilian", "Explosion"}].erase(position);

				explosionHitCivilian = false;
			}

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Laser"))
			{
				std::vector<EntityID>::iterator position = std::find(_entityHitID[{"Civilian", "Laser"}].begin(), _entityHitID[{"Civilian", "Laser"}].end(), e1);
				if (position != _entityHitID[{"Civilian", "Laser"}].end())
					_entityHitID[{"Civilian", "Laser"}].erase(position);
				laserHitEnemy = false;
			}


		}

		// bullet
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Bullet"))
		{
			// bullet hit building
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Building"))
				bulletHitBuilding = false;


			// bullet hit enemy
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Enemy"))
				bulletHitEnemy = false;

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Civilian"))
				bulletHitCivilian = false;

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Target"))
			{
				bulletHitTarget = false;
			}

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Boss"))
			{
				bulletHitBoss = false;
			}

			// heli hit bullet
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Helicopter"))
			{
				bulletHitHelicopter = false;
			}
		}

		// rocket
		/*if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Rocket"))
		{
		}*/

		// laser
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Laser"))
		{
			// laser hit building
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Building"))
			{
				std::vector<EntityID>::iterator position = std::find(_entityHitID[{"Building", "Laser"}].begin(), _entityHitID[{"Building", "Laser"}].end(), e2);
				if (position != _entityHitID[{"Building", "Laser"}].end())
					_entityHitID[{"Building", "Laser"}].erase(position);

				laserHitBuilding = false;
			}

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Boss_Rocket"))
				laserHitEnemyRocket = false;

			// laser hit enemy
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Enemy"))
			{
				std::vector<EntityID>::iterator position = std::find(_entityHitID[{"Enemy", "Laser"}].begin(), _entityHitID[{"Enemy", "Laser"}].end(), e2);
				if (position != _entityHitID[{"Enemy", "Laser"}].end())
					_entityHitID[{"Enemy", "Laser"}].erase(position);
				laserHitEnemy = false;
			}
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Civilian"))
			{
				std::vector<EntityID>::iterator position = std::find(_entityHitID[{"Civilian", "Laser"}].begin(), _entityHitID[{"Civilian", "Laser"}].end(), e2);
				if (position != _entityHitID[{"Civilian", "Laser"}].end())
					_entityHitID[{"Civilian", "Laser"}].erase(position);

				laserHitCivilian = false;
			}

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Target"))
			{
				laserHitTarget = false;
				OffGetHit(e1, e2);
			}

			// laser hit cage
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Cage"))
				laserHitCage = false;

			// laser hit barrier
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Barrier"))
				laserHitBarrier = false;

			// laser hit helicopter
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Helicopter"))
			{
				laserHitHelicopter = false;
			}

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Boss"))
			{
				lazerHitBoss = false;
			}

		}

		// explosion
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Explosion"))
		{
			// explosion hit building
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Building"))
			{
				std::vector<EntityID>::iterator position = std::find(_entityHitID[{"Building", "Explosion"}].begin(), _entityHitID[{"Building", "Explosion"}].end(), e2);
				if (position != _entityHitID[{"Building", "Explosion"}].end())
					_entityHitID[{"Building", "Explosion"}].erase(position);

				explosionHitBuilding = false;
			}

			// explosion hit enemy
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Enemy"))
			{
				std::vector<EntityID>::iterator position = std::find(_entityHitID[{"Enemy", "Explosion"}].begin(), _entityHitID[{"Enemy", "Explosion"}].end(), e2);
				if (position != _entityHitID[{"Enemy", "Explosion"}].end())
					_entityHitID[{"Enemy","Explosion"}].erase(position);

				explosionHitEnemy = false;
			}

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Civilian"))
			{
				std::vector<EntityID>::iterator position = std::find(_entityHitID[{"Civilian", "Explosion"}].begin(), _entityHitID[{"Civilian", "Explosion"}].end(), e2);
				if (position != _entityHitID[{"Civilian", "Explosion"}].end())
					_entityHitID[{"Civilian", "Explosion"}].erase(position);

				explosionHitCivilian = false;
			}


			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Target"))
			{
				explosionHitTarget = false;
				OffGetHit(e1, e2);
			}

			// heli hit explosion
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Helicopter"))
			{
				explosionHitHelicopter = false;
			}

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Boss"))
			{
				explosionHitBoss = false;
			}
		}

		// building
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Building"))
		{
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Bullet"))
			{
				bulletHitBuilding = false;
				OffGetHit(e1, e2);
			}

			// laser hit building
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Laser"))
			{
				std::vector<EntityID>::iterator position = std::find(_entityHitID[{"Building", "Laser"}].begin(), _entityHitID[{"Building", "Laser"}].end(), e1);
				if (position != _entityHitID[{"Building", "Laser"}].end())
					_entityHitID[{"Building", "Laser"}].erase(position);

				laserHitBuilding = false;
				OffGetHit(e1, e2);
			}

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Explosion"))
			{
				std::vector<EntityID>::iterator position = std::find(_entityHitID[{"Building", "Explosion"}].begin(), _entityHitID[{"Building", "Explosion"}].end(), e1);
				if (position != _entityHitID[{"Building", "Explosion"}].end())
					_entityHitID[{"Building", "Explosion"}].erase(position);

				explosionHitBuilding = false;
				OffGetHit(e1, e2);
			}
		}

		// cube
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Cube"))
		{
			// cube hit floor
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Floor"))
				cubeOnFloor = false;

			// cube hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Player"))
				cubeHitPlayer = true;
		}

		// enemy bullet
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "EnemyBullet"))
		{
			// enemy bullet hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Player"))
				enemyBulletHitPlayer = false;
		}

		// target
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Target"))
		{
			// Laser hit target
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Laser"))
			{
				laserHitTarget = false;
				OffGetHit(e1, e2);
			}

			// Explosion hit target
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Explosion"))
			{
				explosionHitTarget = false;
				OffGetHit(e1, e2);
			}

			// Bullet hit target
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Bullet"))
			{
				bulletHitTarget = false;
				OffGetHit(e1, e2);
			}
		}

		// riotshield
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "RiotShield"))
		{
			// Explosion hit riotshield
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Explosion"))
			{
				OffGetHit(e1, e2);
			}

			// Bullet hit riotshield
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Bullet"))
			{
				OffGetHit(e1, e2);
			}

			// Rocket hit riotshield
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Rocket"))
			{
				OffGetHit(e1, e2);
			}
		}

		// minion explosion
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "MinionExplosion"))
		{
			// minion explosion hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Player"))
			{
				minionExplosionHitPlayer = false;
				OffGetHit(e1, e2);
			}
		}

		// enemy explosion
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "EnemyExplosion"))
		{
			// enemy explosion hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Player"))
				explosionHitPlayer = false;
		}
		// Boss Rocket
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Boss_Rocket"))
		{
			// boss rocket hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Player"))
				bossRocketHitPlayer = false;

			//// boss rocket hit floor
			//if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Floor"))
			//	;

			//// boss rocket hit city
			//if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "City"))
			//	;

			// boss rocket hit building
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Building"))
				bossRocketHitBuilding = false;
		}

		// Boss Laser
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Boss_Laser"))
		{
			// boss laser hit player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Player"))
				bossLaserHitPlayer = false;
		}

		// cage
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Cage"))
		{
			// cage hit laser
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Laser"))
			{
				laserHitCage = false;
			}
		}

		// barrier
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Barrier"))
		{
			// cage hit laser
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Laser"))
			{
				laserHitBarrier = false;
			}
		}

		// helicopter
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Helicopter"))
		{
			// heli hit laser
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Laser"))
			{
				laserHitHelicopter = false;
			}

			// heli hit explosion
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Explosion"))
			{
				explosionHitHelicopter = false;
			}

			// Heli rocket hit itself
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "EnemyRocket"))
				enemyRocketHitHeli = false;

			// Heli hit bullet
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Bullet"))
				bulletHitHelicopter = false;
		}

		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "EnemyRocket"))
		{
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Floor"))
				OffGetHit(e1, e2);

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Player"))
				OffGetHit(e1, e2);

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "City"))
				OffGetHit(e1, e2);

			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Helicopter"))
				enemyRocketHitHeli = false;
		}

		// Boss
		if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e1), "Boss"))
		{
			// heli hit laser
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Laser"))
			{
				lazerHitBoss = false;
			}

			// heli hit explosion
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Explosion"))
			{
				explosionHitBoss = false;
			}

			// Heli rocket hit itself
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(e2), "Bullet"))
				bulletHitBoss = false;
		}
	}
}

void ContactReportCallback::OnGetHit(EntityID e1, EntityID e2)
{
	auto dynamicbody1 = Engine::_ecs.getComponent<RigidDynamicCom>(Engine::_ecs.getRoot(e1));
	auto staticbody1 = Engine::_ecs.getComponent<RigidStaticCom>(Engine::_ecs.getRoot(e1));
	if (dynamicbody1)
		dynamicbody1->getHit = true;
	else if (staticbody1)
		staticbody1->getHit = true;


	auto dynamicbody2 = Engine::_ecs.getComponent<RigidDynamicCom>(Engine::_ecs.getRoot(e2));
	auto staticbody2 = Engine::_ecs.getComponent<RigidStaticCom>(Engine::_ecs.getRoot(e2));
	if (dynamicbody2)
		dynamicbody2->getHit = true;
	else if (staticbody2)
		staticbody2->getHit = true;
}

void ContactReportCallback::OffGetHit(EntityID e1, EntityID e2)
{
	auto dynamicbody1 = Engine::_ecs.getComponent<RigidDynamicCom>(Engine::_ecs.getRoot(e1));
	auto staticbody1 = Engine::_ecs.getComponent<RigidStaticCom>(Engine::_ecs.getRoot(e1));
	if (dynamicbody1)
		dynamicbody1->getHit = false;
	else if (staticbody1)
		staticbody1->getHit = false;


	auto dynamicbody2 = Engine::_ecs.getComponent<RigidDynamicCom>(Engine::_ecs.getRoot(e2));
	auto staticbody2 = Engine::_ecs.getComponent<RigidStaticCom>(Engine::_ecs.getRoot(e2));
	if (dynamicbody2)
		dynamicbody2->getHit = false;
	else if (staticbody2)
		staticbody2->getHit = false;
}

std::vector<EntityID> ContactReportCallback::getEntityHitID(std::string entityType, std::string weaponType)
{
	return _entityHitID[{entityType, weaponType}];
}