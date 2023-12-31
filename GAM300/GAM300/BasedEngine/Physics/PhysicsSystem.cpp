#include "PhysicsSystem.h"

#include "../ECS/Entity/Entity.h"
#include "../ECS/ECS.h"
#include "../Engine.h"
#include "../Physics/RigidBodyComponent.h"
//#include "../../BasedEngine/MathLib/Precompiled.h"
#include "Input.h"
//#include "Camera/CameraSystem/CameraSystem.h"
//#include "Camera/CameraCom.h"
#include "Graphics/MaterialCom.h"
#include "Graphics/RenderSystem/RenderSystem.h"
#include "Audio/AudioSystem.h"
#include <iostream>
#include<glm/gtx/quaternion.hpp>
#include<glm/gtc/quaternion.hpp>

#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}

using namespace physx;

//PxFilterFlags mContactReportFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
//	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
//	PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
//{
//	PX_UNUSED(attributes0);
//	PX_UNUSED(attributes1);
//	PX_UNUSED(filterData0);
//	PX_UNUSED(filterData1);
//	PX_UNUSED(constantBlockSize);
//	PX_UNUSED(constantBlock);
//
//	// all initial and persisting reports for everything, with per-point data
//	pairFlags = PxPairFlag::eSOLVE_CONTACT | PxPairFlag::eDETECT_DISCRETE_CONTACT
//		| PxPairFlag::eNOTIFY_TOUCH_FOUND
//		| PxPairFlag::eNOTIFY_TOUCH_PERSISTS
//		| PxPairFlag::eNOTIFY_CONTACT_POINTS | PxPairFlag::eNOTIFY_TOUCH_LOST;
//	return PxFilterFlag::eDEFAULT;
//}

PxFilterFlags mContactReportFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0,
	PxFilterObjectAttributes attributes1, PxFilterData filterData1,
	PxPairFlags & pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	(void)constantBlock;
	(void)constantBlockSize;

	//let triggers through
	if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
	{
		pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
		return PxFilterFlag::eDEFAULT;
	}

	// trigger the contact callback for pairs (A,B) where
	// the filtermask of A contains the ID of B and vice versa.
	if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		pairFlags = PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST | PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eDETECT_DISCRETE_CONTACT;
	else
		pairFlags = PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_LOST | PxPairFlag::eDETECT_DISCRETE_CONTACT;

	return PxFilterFlag::eDEFAULT;
}


PhysicsSystem::PhysicsSystem() : mDispatcher{ nullptr }, mFoundation{ nullptr },
mPhysics{ nullptr }, mScene{ nullptr }
{
}

void PhysicsSystem::init()
{
	mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mAllocator, mErrorCallback);
	if (!mFoundation) throw("PxCreateFoundation failed!");

	mPvd = PxCreatePvd(*mFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
	mPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), true, mPvd);

	PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -85.0f, 0.0f);
	mDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = mDispatcher;
	sceneDesc.filterShader = mContactReportFilterShader;
	sceneDesc.simulationEventCallback = &mContactReportCallback;
	//sceneDesc.filterCallback = mSimulationFilterCallback;
	mScene = mPhysics->createScene(sceneDesc);

	//mControllerManager = PxCreateControllerManager(*mScene);

	PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
	if (pvdClient)
	{
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	}
}

void PhysicsSystem::load()
{
	for (const auto& entity : _entities)
	{
		auto transform = Engine::_ecs.getComponent<TransformCom>(entity);
		auto staticbody = Engine::_ecs.getComponent<RigidStaticCom>(entity);
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(entity);

		if (staticbody)
		{
			LoadRigidStaticBody(entity, *staticbody);
		}
			
		else if (dynamicbody)
		{
			LoadRigidDynamicBody(entity, *dynamicbody);
		}
	}
}

void PhysicsSystem::update(float dt)
{
	(void)dt;
	HandleAllCollisions();

	if (Engine::_isPlay && !Engine::_ecs.getSystem<UISystem>()->_isPaused)
	{
		mScene->simulate(1.0f / 60.0f);
		mScene->fetchResults(true);
	}

	for (const auto& entity : _entities)
	{
		auto transform = Engine::_ecs.getComponent<TransformCom>(entity);
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(entity);
		auto staticbody = Engine::_ecs.getComponent<RigidStaticCom>(entity);

		auto playerBody = Engine::_ecs.getComponent<RigidDynamicCom>(Engine::_ecs.getEntityID("Player"));

		if (Engine::_isPlay == false)
		{
			if (dynamicbody)
			{
				dynamicbody->rigidPos = transform->pos;
				dynamicbody->rigidRot = convertEulerToQuat(transform->rotate);
				dynamicbody->body->setGlobalPose(PxTransform(dynamicbody->rigidPos.Vec3toPVex3(),
					dynamicbody->rigidRot.QuattoPQuat()));
			}
			else if (staticbody)
			{
				std::set<EntityID> children = Engine::_ecs.getChildEntities(entity);
				Vector3 oldPos = staticbody->body->getGlobalPose().p;
				Vector3 oldRot = convertQuatToEuler(staticbody->rigidRot);
				staticbody->rigidPos = transform->pos;
				staticbody->rigidRot = convertEulerToQuat(transform->rotate);
				staticbody->body->setGlobalPose(PxTransform(staticbody->rigidPos.Vec3toPVex3(),
					staticbody->rigidRot.QuattoPQuat()));
				Vector3 newPos = staticbody->body->getGlobalPose().p;
				Vector3 newRot = convertQuatToEuler(staticbody->rigidRot);
				Vector3 posOffset = newPos - oldPos;
				Vector3 rotOffset = newRot - oldRot;

				for (auto& child : children)
				{
					auto child_transform = Engine::_ecs.getComponent<TransformCom>(child);
					auto child_staticbody = Engine::_ecs.getComponent<RigidStaticCom>(child);

					child_transform->pos += posOffset;
					child_staticbody->rigidPos = child_transform->pos;

					// only works for Y rotation for now
					//child_transform->rotate.x -= rotOffset.x;
					child_transform->rotate.y -= rotOffset.y;
					//child_transform->rotate.z += rotOffset.z;
					child_staticbody->rigidRot = convertEulerToQuat(child_transform->rotate);
				}
			}
		}
		else if (Engine::_isPlay == true && !Engine::_ecs.getSystem<UISystem>()->_isPaused)
		{
			if (dynamicbody)
			{
				if (entity == 1)
					continue;
				
				// gets body's position and orientation in the world
				Vector3 newPos = dynamicbody->body->getGlobalPose().p;
				Quat newRot = dynamicbody->body->getGlobalPose().q;
				//Quat tempRot = newRot;
				// update scale and friction
				dynamicbody->body->detachShape(*dynamicbody->shape);
				dynamicbody->shape->setGeometry(PxBoxGeometry((dynamicbody->rigidScale / 2.0f).Vec3toPVex3()));
				dynamicbody->material->setStaticFriction(dynamicbody->staticFriction);
				dynamicbody->material->setDynamicFriction(dynamicbody->dynamicFriction);
				dynamicbody->material->setRestitution(dynamicbody->restitution);
				dynamicbody->shape->setMaterials(&dynamicbody->material, 1);
				dynamicbody->body->attachShape(*dynamicbody->shape);

				// update pos
				if (Engine::_ecs.getEntityName(entity) != "Laser" && Engine::_ecs.getEntityName(entity) != "Weapon")
				{
					transform->pos.x = dynamicbody->rigidPos.x = newPos.x;
					transform->pos.y = dynamicbody->rigidPos.y = newPos.y;
					transform->pos.z = dynamicbody->rigidPos.z = newPos.z;

					// update rot
					//transform->rotate = Quat_to_Euler(newRot);
				
					dynamicbody->rigidRot = newRot = Euler_to_Quat(transform->rotate);
				}
				// update mass
				dynamicbody->body->setMass(dynamicbody->mass);

				// update damping
				dynamicbody->body->setLinearDamping(dynamicbody->linearDamp);
				dynamicbody->body->setAngularDamping(dynamicbody->angularDamp);

				mContactReportCallback.jumping = false;
			}

			else if (staticbody)
			{
				// get body's position and orientation in the world
				Vector3 newPos = staticbody->body->getGlobalPose().p;
				Quat newRot = staticbody->body->getGlobalPose().q;
				// update scale and friction
				staticbody->body->detachShape(*staticbody->shape);
				staticbody->shape->setGeometry(PxBoxGeometry((staticbody->rigidScale / 2.0f).Vec3toPVex3()));
				staticbody->material->setStaticFriction(staticbody->staticFriction);
				staticbody->material->setDynamicFriction(staticbody->dynamicFriction);
				staticbody->material->setRestitution(staticbody->restitution);
				staticbody->shape->setMaterials(&staticbody->material, 1);
				staticbody->body->attachShape(*staticbody->shape);

				if (!Engine::_ecs.hasTagInEntity(entity, "Fence") && !Engine::_ecs.hasTagInEntity(entity, "Target")
					&& !Engine::_ecs.hasTagInEntity(entity, "TargetLED") && !Engine::_ecs.hasTagInEntity(entity, "Cage")
					&& !Engine::_ecs.hasTagInEntity(entity, "Wall")	&& !Engine::_ecs.hasTagInEntity(entity, "Barrier") 
					&& !Engine::_ecs.hasTagInEntity(entity, "RiotShield") && !Engine::_ecs.hasTagInEntity(entity, "Boss_Weapon")
					&& !Engine::_ecs.hasTagInEntity(entity, "Trapped"))
				{
					// update pos
					staticbody->rigidPos.x = newPos.x;
					staticbody->rigidPos.y = newPos.y;
					staticbody->rigidPos.z = newPos.z;

					//update rot
					staticbody->rigidRot = newRot;
				}
				else
				{
					// update pos
					staticbody->rigidPos.x = transform->pos.x;
					staticbody->rigidPos.y = transform->pos.y;
					staticbody->rigidPos.z = transform->pos.z;

					//update rot
					staticbody->rigidRot = Euler_to_Quat(transform->rotate);

					staticbody->body->setGlobalPose(PxTransform{ staticbody->rigidPos.Vec3toPVex3(), staticbody->rigidRot.QuattoPQuat() });
				}
			}
		}
	}
}

void PhysicsSystem::unload()
{
	for (auto entity : _entities)
	{
		auto rigidDynamic = Engine::_ecs.getComponent<RigidDynamicCom>(entity);
		auto rigidStatic = Engine::_ecs.getComponent<RigidStaticCom>(entity);

		if (rigidDynamic)
			mScene->removeActor(*rigidDynamic->body);
		else if (rigidStatic)
			mScene->removeActor(*rigidStatic->body);
	}
}

void PhysicsSystem::release()
{
	PX_RELEASE(mScene);
	PX_RELEASE(mDispatcher);
	PX_RELEASE(mPhysics);
	if (mPvd)
	{
		PxPvdTransport* transport = mPvd->getTransport();
		mPvd->release();	mPvd = NULL;
		PX_RELEASE(transport);
	}
	PX_RELEASE(mFoundation);
	mContactReportCallback._entityHitID.clear();
}

// when loading a level
void PhysicsSystem::LoadRigidStaticBody(EntityID eID, RigidStaticCom& staticbody)
{
	PxTransform t{ PxVec3{ staticbody.rigidPos.x, staticbody.rigidPos.y, staticbody.rigidPos.z } };
	Quat q = staticbody.rigidRot;
	t.q = q.QuattoPQuat();
	staticbody.body = mPhysics->createRigidStatic(t);
	staticbody.material = mPhysics->createMaterial(staticbody.staticFriction,
		staticbody.dynamicFriction,
		staticbody.restitution);
	if (staticbody.rigidScale == Vector3{ 0, 0, 0 })
		staticbody.rigidScale = Vector3{ 1.0f, 1.0f, 1.0f };
	staticbody.shape =
		mPhysics->createShape(PxBoxGeometry((staticbody.rigidScale / 2.0f).Vec3toPVex3()),
			*staticbody.material);
	staticbody.body->attachShape(*staticbody.shape);
	
	staticbody.body->userData = ((uint32_t*)(uintptr_t)eID);
	mScene->addActor(*staticbody.body);
}

// when loading a level
void PhysicsSystem::LoadRigidDynamicBody(EntityID eID, RigidDynamicCom& dynamicbody)
{
	//if (Engine::_ecs.hasTagInEntity(eID, "Player"))
	//{
	//	/*PxTransform t{ dynamicbody.rigidPos.Vec3toPVex3() };
	//	Quat q = dynamicbody.rigidRot;
	//	t.q = q.QuattoPQuat();
	//	dynamicbody.body = mPhysics->createRigidDynamic(t);*/

	//	PxBoxControllerDesc desc;
	//	desc.halfSideExtent = dynamicbody.rigidScale.x / 2;
	//	desc.halfHeight = dynamicbody.rigidScale.y / 2;
	//	desc.halfForwardExtent = dynamicbody.rigidScale.z / 2;
	//	desc.material = mPhysics->createMaterial(dynamicbody.staticFriction,
	//		dynamicbody.dynamicFriction,
	//		dynamicbody.restitution);
	//	desc.position.x = dynamicbody.rigidPos.x;
	//	desc.position.y = dynamicbody.rigidPos.y;
	//	desc.position.z = dynamicbody.rigidPos.z;
	//	PxController* player = mControllerManager->createController(desc);

	//	mScene->addActor(*player->getActor());

	//	/*dynamicbody.body->userData = ((uint32_t*)(uintptr_t)eID);
	//	dynamicbody.body->attachShape(*dynamicbody.shape);
	//	dynamicbody.body->setMass(dynamicbody.mass);
	//	mScene->addActor(*dynamicbody.body);*/
	//}

	//else
	//{
		PxTransform t{ dynamicbody.rigidPos.Vec3toPVex3() };
		Quat q = dynamicbody.rigidRot;
		t.q = q.QuattoPQuat();
		dynamicbody.body = mPhysics->createRigidDynamic(t);

		dynamicbody.material = mPhysics->createMaterial(dynamicbody.staticFriction,
			dynamicbody.dynamicFriction,
			dynamicbody.restitution);
		if (dynamicbody.rigidScale == Vector3{ 0, 0, 0 })
			dynamicbody.rigidScale = Vector3{ 1.0f, 1.0f, 1.0f };
		dynamicbody.shape =
			mPhysics->createShape(PxBoxGeometry((dynamicbody.rigidScale / 2.0f).Vec3toPVex3()),
				*dynamicbody.material);


		dynamicbody.body->userData = ((uint32_t*)(uintptr_t)eID);
		dynamicbody.body->attachShape(*dynamicbody.shape);
		dynamicbody.body->setMass(dynamicbody.mass);
		mScene->addActor(*dynamicbody.body);
	//}
	
}

// when adding component/creating new entity
void PhysicsSystem::AddRigidStaticBody(EntityID eID, RigidStaticCom& staticbody, TransformCom& trans)
{
	PxTransform t{ trans.pos.Vec3toPVex3() };
	Quat q = Euler_to_Quat(trans.rotate);
	t.q = q.QuattoPQuat();
	staticbody.body = mPhysics->createRigidStatic(t);
	staticbody.body->userData = ((uint32_t*)(uintptr_t)eID);

	staticbody.material = mPhysics->createMaterial(0, 0, 0);
	staticbody.shape =
		mPhysics->createShape(PxBoxGeometry(trans.scale.Vec3toPVex3() / 2.0f),
			*staticbody.material);

	PxShape* newShape = mPhysics->createShape(PxBoxGeometry(trans.scale.Vec3toPVex3() / 2.0f),
		*staticbody.material);

	staticbody.rigidPos = trans.pos;
	staticbody.rigidRot = q;
	staticbody.rigidScale = trans.scale.Vec3toPVex3();
	staticbody.staticFriction = 0;
	staticbody.dynamicFriction = 0;
	staticbody.restitution = 0;

	staticbody.body->attachShape(*staticbody.shape);
	staticbody.body->attachShape(*newShape);
	mScene->addActor(*staticbody.body);
}

// when adding component/creating new entity
void PhysicsSystem::AddRigidDynamicBody(EntityID eID, RigidDynamicCom& dynamicbody, TransformCom& trans)
{
	PxTransform t{ trans.pos.Vec3toPVex3() };
	Quat q = Euler_to_Quat(trans.rotate);
	t.q = q.QuattoPQuat();
	dynamicbody.body = mPhysics->createRigidDynamic(t);
	dynamicbody.body->userData = ((uint32_t*)(uintptr_t)eID);
	dynamicbody.material = mPhysics->createMaterial(0, 0, 0);
	dynamicbody.shape =
		mPhysics->createShape(PxBoxGeometry(trans.scale.Vec3toPVex3() / 2.0f),
			*dynamicbody.material);

	dynamicbody.rigidPos = trans.pos;
	dynamicbody.rigidRot = q;
	dynamicbody.rigidScale = trans.scale.Vec3toPVex3();
	dynamicbody.mass = 1.0f;
	dynamicbody.staticFriction = 0;
	dynamicbody.dynamicFriction = 0;
	dynamicbody.restitution = 0;
	dynamicbody.linearDamp = 0;
	dynamicbody.angularDamp = 0;

	dynamicbody.body->setLinearDamping(dynamicbody.linearDamp);
	dynamicbody.body->setAngularDamping(dynamicbody.angularDamp);
	dynamicbody.body->setMass(dynamicbody.mass);

	dynamicbody.body->attachShape(*dynamicbody.shape);
	mScene->addActor(*dynamicbody.body);
}

void PhysicsSystem::rotate_vector_by_quaternion(Vector3& v, Quat& q, Vector3& vprime)
{
	Vector3 u(q.x, q.y, q.z);

	// Extract the scalar part of the quaternion
	float s = q.w;

	// Do the math
	vprime = 2.0f * Dot(u, v) * u
		+ (s * s - Dot(u, u)) * v
		+ 2.0f * s * Cross(u, v);

	if (vprime.z == 1.0f)
		vprime.x = 0;

	//vprime /= vprime.Length();
	vprime = vprime.Normalized();
}

void PhysicsSystem::rotate_vector_by_angle(Vector3 v, float angle, Vector3& vprime)
{
	float radianAngle = -angle * ((float)M_PI / 180.0f);

	vprime.x = cosf(radianAngle) * v.x - sinf(radianAngle) * v.z;
	vprime.y = 0;
	vprime.z = sinf(radianAngle) * v.x + cosf(radianAngle) * v.z;

	vprime = vprime.Normalized();
}

void PhysicsSystem::ApplyRaycast(Vector3 origin, Vector3 unitDir, float maxDist, Vector3& endPoint, std::string weaponType, Vector3 bulletSpawnPos, Vector3 camEye)
{
	PxReal maxdist = maxDist;
	PxRaycastBuffer hit;
	//bool status = 
	if (weaponType == "Bullet" || weaponType == "Rocket")
	{
		if (mScene->raycast(origin.Vec3toPVex3(), unitDir.Vec3toPVex3(), maxdist, hit))
		{
			endPoint = hit.block.position;
			float u = Dot((endPoint - bulletSpawnPos), camEye);
			if (u < 0)
				endPoint = origin + maxDist * unitDir;
		}
		else
		{
			endPoint = origin + maxDist * unitDir;
		}
	}
	else
	{
		endPoint = origin + maxDist * unitDir;
	}
	//if (status)
		//applyDamage(hit.block.position, hit.block.normal);
}

EntityID PhysicsSystem::RaycastHitEntityID(Vector3 origin, Vector3 unitDir, float maxDist)
{
	PxReal maxdist = maxDist;
	PxRaycastBuffer hit;
	//bool status = 
	if (mScene->raycast(origin.Vec3toPVex3(), unitDir.Vec3toPVex3(), maxdist, hit))
	{
		if (hit.block.actor->getName())
		{
			for (const auto& entity : _entities)
			{
				auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(entity);
				auto staticbody = Engine::_ecs.getComponent<RigidStaticCom>(entity);

				if (dynamicbody)
				{
					if (dynamicbody->body->getName() == hit.block.actor->getName())
					{
						return entity;
					}
				}

				if (staticbody)
				{
					if (staticbody->body->getName() == hit.block.actor->getName())
					{
						return entity;
					}
				}
			}
		}
		else
		{
			return NULL;
		}
	}

	return NULL;

	//if (status)
		//applyDamage(hit.block.position, hit.block.normal);
}


void PhysicsSystem::RemoveActor(PxActor& actor)
{
	mScene->removeActor(actor);
}

void PhysicsSystem::LuaRemoveActor(EntityID eID)
{
	//std::cout << "want to remove" << std::endl;
	if (Engine::_ecs.hasComponent<RigidDynamicCom>(eID))
	{
		//std::cout << "Removeing actor" << std::endl;
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(eID);
		mScene->removeActor(*dynamicbody->body);
	}
	else if (Engine::_ecs.hasComponent<RigidStaticCom>(eID))
	{
		auto staticbody = Engine::_ecs.getComponent<RigidStaticCom>(eID);
		mScene->removeActor(*staticbody->body);
	}
}

void PhysicsSystem::SetupCollisionFiltering(PxRigidActor* actor, PxShape*& shape, PxU32 filterGroup, PxU32 filterMask)
{
	PxFilterData filterData;
	filterData.word0 = filterGroup; // word0 = own ID
	filterData.word1 = filterMask;  // word1 = ID mask to filter pairs that trigger a contact callback

	actor->detachShape(*shape);
	shape->setSimulationFilterData(filterData);
	actor->attachShape(*shape);
}

void PhysicsSystem::HandleAllCollisions()
{
	for (const auto& entity : _entities)
	{
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(entity);
		auto staticbody = Engine::_ecs.getComponent<RigidStaticCom>(entity);
		if (dynamicbody)
		{
			// player
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "Player"))
				SetupCollisionFiltering(dynamicbody->body, dynamicbody->shape, FilterGroup::ePlayer, FilterGroup::eFloor
					| FilterGroup::eCityObject | FilterGroup::eEnemy | FilterGroup::eWeaponCase
					| FilterGroup::eCar | FilterGroup::eCivilian | FilterGroup::eCage | FilterGroup::eTarget | FilterGroup::eWall | FilterGroup::eRiotShield
					| FilterGroup::eMinionExplosion | FilterGroup::eBarrier
					| FilterGroup::eCar | FilterGroup::eCivilian | FilterGroup::eEnemyRocket | FilterGroup::eTrapped);

			// enemy
			else if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "Enemy"))
				SetupCollisionFiltering(dynamicbody->body, dynamicbody->shape, FilterGroup::eEnemy, FilterGroup::ePlayer
				| FilterGroup::eFloor | FilterGroup::eCityObject | FilterGroup::eEnemy | FilterGroup::eWeaponCase 
				| FilterGroup::eExplosion | FilterGroup::eEnemy | FilterGroup::eBullet | FilterGroup::eCar 
				| FilterGroup::eCivilian | FilterGroup::eCage | FilterGroup::eTarget | FilterGroup::eWall
				| FilterGroup::eBarrier | FilterGroup::eTrapped);


			// bullet
			else if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "Bullet"))
				SetupCollisionFiltering(dynamicbody->body, dynamicbody->shape, FilterGroup::eBullet, FilterGroup::eEnemy 
					| FilterGroup::eCivilian | FilterGroup::eCage | FilterGroup::eTarget | FilterGroup::eWall | FilterGroup::eRiotShield
					| FilterGroup::eBarrier);

			// cube
			else if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "Cube"))
				SetupCollisionFiltering(dynamicbody->body, dynamicbody->shape, FilterGroup::eCube, FilterGroup::eFloor 
				| FilterGroup::eCityObject);

			// explosion
			else if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "Explosion"))
				SetupCollisionFiltering(dynamicbody->body, dynamicbody->shape, FilterGroup::eExplosion, FilterGroup::eEnemy 
					| FilterGroup::eCityObject | FilterGroup::eCage | FilterGroup::eBarrier | FilterGroup::eCivilian);

			// civilian
			else if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "Civilian") && !Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "Trapped"))
				SetupCollisionFiltering(dynamicbody->body, dynamicbody->shape, FilterGroup::eCivilian, FilterGroup::ePlayer 
					| FilterGroup::eEnemy | FilterGroup::eCityObject | FilterGroup::eCivilian | FilterGroup::eCar | FilterGroup::eFloor
					| FilterGroup::eCage | FilterGroup::eExplosion);

			// trapped civilians
			else if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "Civilian") && Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "Trapped"))
				SetupCollisionFiltering(dynamicbody->body, dynamicbody->shape, FilterGroup::eTrapped, FilterGroup::ePlayer
					| FilterGroup::eEnemy | FilterGroup::eCityObject | FilterGroup::eFloor);

			// car
			else if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "Car"))
				SetupCollisionFiltering(dynamicbody->body, dynamicbody->shape, FilterGroup::eCar, FilterGroup::ePlayer
					| FilterGroup::eEnemy | FilterGroup::eCityObject | FilterGroup::eCivilian | FilterGroup::eCar | FilterGroup::eFloor
					| FilterGroup::eCage | FilterGroup::eTarget | FilterGroup::eBarrier);
			//camera 
			else if(Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "Camera"))
				SetupCollisionFiltering(dynamicbody->body, dynamicbody->shape, FilterGroup::eCamera, 
					FilterGroup::eCityObject | FilterGroup::eFloor | FilterGroup::eCage | FilterGroup::eBarrier);

			// Enemy bullet
			else if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "EnemyBullet"))
				SetupCollisionFiltering(dynamicbody->body, dynamicbody->shape, FilterGroup::eEnemyBullet, FilterGroup::ePlayer | FilterGroup::eCage | FilterGroup::eTarget
					| FilterGroup::eWall | FilterGroup::eRiotShield | FilterGroup::eBarrier);

			// explosion
			else if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "MinionExplosion"))
				SetupCollisionFiltering(dynamicbody->body, dynamicbody->shape, FilterGroup::eMinionExplosion, FilterGroup::ePlayer
					| FilterGroup::eCage | FilterGroup::eBarrier);

			// Enemy rocket
			else if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "EnemyRocket"))
				SetupCollisionFiltering(dynamicbody->body, dynamicbody->shape, FilterGroup::eEnemyRocket, FilterGroup::ePlayer | FilterGroup::eFloor
				| FilterGroup::eCityObject);

			// Helicopter
			else if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "Helicopter"))
				SetupCollisionFiltering(dynamicbody->body, dynamicbody->shape, FilterGroup::eHelicopter, FilterGroup::eRocket);

		}

		else if (staticbody)
		{
			// floor
			if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "Floor"))
				SetupCollisionFiltering(staticbody->body, staticbody->shape, FilterGroup::eFloor, FilterGroup::ePlayer
					| FilterGroup::eEnemy | FilterGroup::eCube | FilterGroup::eExplosion | FilterGroup::eCivilian | FilterGroup::eCamera
					| FilterGroup::eCar | FilterGroup::eCage | FilterGroup::eWall | FilterGroup::eBarrier | FilterGroup::eEnemyRocket
					| FilterGroup::eTrapped);

			// city objects
			else if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "City"))
				SetupCollisionFiltering(staticbody->body, staticbody->shape, FilterGroup::eCityObject, FilterGroup::ePlayer
					| FilterGroup::eEnemy | FilterGroup::eCivilian | FilterGroup::eCamera | FilterGroup::eEnemyRocket
					| FilterGroup::eTrapped);

			// cage
			else if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "Cage"))
				SetupCollisionFiltering(staticbody->body, staticbody->shape, FilterGroup::eCage, FilterGroup::ePlayer | FilterGroup::eEnemy
					| FilterGroup::eFloor | FilterGroup::eBullet | FilterGroup::eExplosion | FilterGroup::eLaser | FilterGroup::eEnemyBullet
					| FilterGroup::eCar | FilterGroup::eCamera | FilterGroup::eMinionExplosion | FilterGroup::eCivilian);

			// target
			else if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "Target"))
				SetupCollisionFiltering(staticbody->body, staticbody->shape, FilterGroup::eTarget, FilterGroup::ePlayer | FilterGroup::eEnemy
					| FilterGroup::eFloor | FilterGroup::eBullet | FilterGroup::eEnemyBullet
					| FilterGroup::eCar);

			// wall
			else if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "Wall"))
				SetupCollisionFiltering(staticbody->body, staticbody->shape, FilterGroup::eWall, FilterGroup::ePlayer | FilterGroup::eEnemy
					| FilterGroup::eFloor | FilterGroup::eBullet | FilterGroup::eEnemyBullet);

			// Barrier
			else if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "Barrier"))
				SetupCollisionFiltering(staticbody->body, staticbody->shape, FilterGroup::eBarrier, FilterGroup::ePlayer | FilterGroup::eEnemy
					| FilterGroup::eFloor | FilterGroup::eBullet | FilterGroup::eExplosion | FilterGroup::eLaser | FilterGroup::eEnemyBullet
					| FilterGroup::eCar | FilterGroup::eCamera | FilterGroup::eMinionExplosion);

			// riotshield
			else if (Engine::_ecs.hasTagInEntity(Engine::_ecs.getRoot(entity), "RiotShield"))
				SetupCollisionFiltering(staticbody->body, staticbody->shape, FilterGroup::eRiotShield, FilterGroup::ePlayer
					| FilterGroup::eBullet | FilterGroup::eEnemyBullet);

		}
	}
}



void PhysicsSystem::addForceToBody(EntityID eID, Vector3 force, float scalar, int type)
{
	if (Engine::_ecs.hasComponent<RigidDynamicCom>(eID))
	{
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(eID);

		if (dynamicbody->body)
		{
			switch (type)
			{
			case 0:
				dynamicbody->body->addForce(scalar * force.Vec3toPVex3(), PxForceMode::eFORCE);
				break;
			case 1:
				dynamicbody->body->addForce(scalar * force.Vec3toPVex3(), PxForceMode::eACCELERATION);
				break;
			case 2:
				dynamicbody->body->addForce(scalar * force.Vec3toPVex3(), PxForceMode::eIMPULSE);
				break;
			case 3:
				dynamicbody->body->addForce(scalar * force.Vec3toPVex3(), PxForceMode::eVELOCITY_CHANGE);
				break;
			default:
				dynamicbody->body->addForce(scalar * force.Vec3toPVex3(), PxForceMode::eFORCE);
			}
		}
			
	}
}

void PhysicsSystem::addTorqueToBody(EntityID eID, Vector3 torque, float scalar)
{
	if (Engine::_ecs.hasComponent<RigidDynamicCom>(eID))
	{
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(eID);
		dynamicbody->body->addTorque(scalar * torque.Vec3toPVex3());
	}
}

void PhysicsSystem::clearForce(EntityID eID)
{
	if (Engine::_ecs.hasComponent<RigidDynamicCom>(eID))
	{
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(eID);

		if (dynamicbody->body)
			dynamicbody->body->clearForce();
	}
}

Quat PhysicsSystem::getGlobalPoseRotation(EntityID eID)
{
	Quat newRot;

	if (Engine::_ecs.hasComponent<RigidDynamicCom>(eID))
	{
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(eID);
		return dynamicbody->body->getGlobalPose().q;
	}
	else if (Engine::_ecs.hasComponent<RigidStaticCom>(eID))
	{
		auto staticbody = Engine::_ecs.getComponent<RigidStaticCom>(eID);
		return staticbody->body->getGlobalPose().q;
	}

	return newRot;
}

Vector3 PhysicsSystem::getGlobalPosePosition(EntityID eID)
{
	Vector3 newPos;

	if (Engine::_ecs.hasComponent<RigidDynamicCom>(eID))
	{
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(eID);
		return dynamicbody->body->getGlobalPose().p;
	}
	else if (Engine::_ecs.hasComponent<RigidStaticCom>(eID))
	{
		auto staticbody = Engine::_ecs.getComponent<RigidStaticCom>(eID);
		return staticbody->body->getGlobalPose().p;
	}
	return newPos;
}

void PhysicsSystem::setGlobalPose(EntityID eID, Vector3 pos, Quat rotQuat)
{
	if (Engine::_ecs.hasComponent<RigidDynamicCom>(eID))
	{
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(eID);
		dynamicbody->body->setGlobalPose(PxTransform(pos.Vec3toPVex3(), rotQuat.QuattoPQuat()));
	}
}


void PhysicsSystem::setBodyLinearDamping(EntityID eID)
{
	if (Engine::_ecs.hasComponent<RigidDynamicCom>(eID))
	{
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(eID);
		dynamicbody->body->setLinearDamping(dynamicbody->linearDamp);
	}
}

void PhysicsSystem::setBodyAngularDamping(EntityID eID)
{
	if (Engine::_ecs.hasComponent<RigidDynamicCom>(eID))
	{
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(eID);
		dynamicbody->body->setAngularDamping(dynamicbody->angularDamp);
	}
}

void PhysicsSystem::setBodyMass(EntityID eID)
{
	if (Engine::_ecs.hasComponent<RigidDynamicCom>(eID))
	{
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(eID);
		dynamicbody->body->setMass(dynamicbody->mass);
	}
}

Vector3 PhysicsSystem::getBodyLinearVelocity(EntityID eID)
{
	Vector3 linearVel;

	if (Engine::_ecs.hasComponent<RigidDynamicCom>(eID))
	{
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(eID);
		return dynamicbody->body->getLinearVelocity();
	}
	return linearVel;
}

void PhysicsSystem::setBodyLinearVelocity(EntityID eID, Vector3 playerVel, float speedCap)
{
	if (Engine::_ecs.hasComponent<RigidDynamicCom>(eID))
	{
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(eID);

		if (playerVel.x >= speedCap)
			dynamicbody->body->setLinearVelocity(PxVec3{ speedCap, playerVel.y, playerVel.z });

		if (playerVel.x <= -speedCap)
			dynamicbody->body->setLinearVelocity(PxVec3{ -speedCap, playerVel.y, playerVel.z });

		if (playerVel.z >= speedCap)
			dynamicbody->body->setLinearVelocity(PxVec3{ playerVel.x, playerVel.y, speedCap });

		if (playerVel.z <= -speedCap)
			dynamicbody->body->setLinearVelocity(PxVec3{ playerVel.x, playerVel.y, -speedCap });
	}
}

Vector3 PhysicsSystem::getAngularVelocity(EntityID eID)
{
	Vector3 angularVel;

	if (Engine::_ecs.hasComponent<RigidDynamicCom>(eID))
	{
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(eID);
		return dynamicbody->body->getAngularVelocity();
	}
	return angularVel;
}

void PhysicsSystem::setAngularVelocity(EntityID eID, Vector3 playerVel, float speedCap)
{
	if (Engine::_ecs.hasComponent<RigidDynamicCom>(eID))
	{
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(eID);

		if (playerVel.x >= speedCap)
			dynamicbody->body->setAngularVelocity(PxVec3{ speedCap, playerVel.y, playerVel.z });

		if (playerVel.x <= -speedCap)
			dynamicbody->body->setAngularVelocity(PxVec3{ -speedCap, playerVel.y, playerVel.z });

		if (playerVel.z >= speedCap)
			dynamicbody->body->setAngularVelocity(PxVec3{ playerVel.x, playerVel.y, speedCap });

		if (playerVel.z <= -speedCap)
			dynamicbody->body->setAngularVelocity(PxVec3{ playerVel.x, playerVel.y, -speedCap });
	}
}

Quat PhysicsSystem::convertEulerToQuat(Vector3& euler)
{
	return Euler_to_Quat(euler);
}

Vector3 PhysicsSystem::convertQuatToEuler(Quat& quat)
{
	return Quat_to_Euler(quat);
}

ContactReportCallback& PhysicsSystem::getContactCallback()
{
	return mContactReportCallback;
}

void PhysicsSystem::setGravityFlag(EntityID eID, bool flag)
{
	if (Engine::_ecs.hasComponent<RigidDynamicCom>(eID))
	{
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(eID);
		if (dynamicbody->body)
			dynamicbody->body->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, flag);
	}
}

void PhysicsSystem::setKinematicFlag(EntityID eID, bool flag)
{
	if (Engine::_ecs.hasComponent<RigidDynamicCom>(eID))
	{
		auto dynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(eID);
		dynamicbody->body->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, flag);
	}
}

void PhysicsSystem::setKinematicTarget(EntityID eID, EntityID pID)
{
	if ((Engine::_ecs.hasComponent<RigidDynamicCom>(eID)) && (Engine::_ecs.hasComponent<RigidDynamicCom>(pID)))
	{
		auto edynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(eID);
		auto pdynamicbody = Engine::_ecs.getComponent<RigidDynamicCom>(pID);

		edynamicbody->body->setKinematicTarget(pdynamicbody->body->getGlobalPose());
	}
}

Quat PhysicsSystem::lookat(Vector3 origin, Vector3 target, Vector3 up)
{
	Matrix4 m{};
	Quat q;

	m = glm::lookAt(origin.Vec3toGlmVec3(), target.Vec3toGlmVec3(), up.Vec3toGlmVec3());

	if ((1.0f + m[0][0] + m[1][1] + m[2][2]) > 0.0f)
		q.w = sqrtf((1.0f + m[0][0] + m[1][1] + m[2][2])) / 2.0f;
	else
		q.w = 0.0f;

	if ((1.0f + m[0][0] - m[1][1] - m[2][2]) > 0.0f)
		q.x = sqrtf((1.0f + m[0][0] - m[1][1] - m[2][2])) / 2.0f;
	else
		q.x = 0.0f;

	if ((1.0f - m[0][0] + m[1][1] - m[2][2]) > 0.0f)
		q.y = sqrtf((1.0f - m[0][0] + m[1][1] - m[2][2])) / 2.0f;
	else
		q.y = 0.0f;

	if ((1.0f - m[0][0] - m[1][1] + m[2][2]) > 0.0f)
		q.z = sqrtf((1.0f - m[0][0] - m[1][1] + m[2][2])) / 2.0f;
	else
		q.z = 0.0f;

	if ((q.x * (m[2][1] - m[1][2])) < 0.0f)
		q.x *= -1.0f;

	if ((q.y * (m[0][2] - m[2][0])) < 0.0f)
		q.y *= -1.0f;

	if ((q.z * (m[1][0] - m[0][1])) < 0.0f)
		q.z *= -1.0f;

	return q;
}

