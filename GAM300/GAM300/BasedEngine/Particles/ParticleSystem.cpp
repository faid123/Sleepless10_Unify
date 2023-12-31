#include "ParticleSystem.h"
#include "MathLib/Random.h"

std::map<EntityID, std::vector<ParticleData>> ParticleSystem::_particleData{};
std::map<EntityID, EntityID> ParticleSystem::_emitterList{};
ParticleSystem::ParticleSystem()
{

}

void ParticleSystem::init()
{
}

void ParticleSystem::load()
{
	_emitterList.clear();
	for (auto const& entity : _entities)
	{
		auto emitter = Engine::_ecs.getComponent<EmitterCom>(entity);
		if (emitter != nullptr)
		{
			emitter->isInitialised = false;
			//emitter->particles.clear();
		}
	}
}

void ParticleSystem::update(float dt)
{
	_emitterList.clear();
	for (auto const& entity : _entities)
	{
		auto emitter = Engine::_ecs.getComponent<EmitterCom>(entity);
		if (emitter != nullptr)
		{
			_emitterList[entity] = entity;
			auto transformEmit = Engine::_ecs.getComponent<TransformCom>(entity);
			if (!transformEmit)
				continue;


			emitter->position = transformEmit->pos;

			if (emitter->isInitialised == false)
			{
				//emitter->particles.resize(emitter->maxParticles);
				_particleData[entity].clear();
				_particleData[entity].resize(emitter->maxParticles);
				/*for (int i = 0; i < emitter->maxParticles; ++i)
				{
					emitter->particles[i] = Engine::_ecs.createNewEntity("Particles" + std::to_string(i));
					Engine::_ecs.addComponentToEntity(
						emitter->particles[i],
						TransformCom
						{
								Vector3(0.0f,0.0f,-0.0f),
								Vector3(0.0f),
								Vector3(0.0f)
						});
					Engine::_ecs.addComponentToEntity(
						emitter->particles[i],
						SpriteCom{ "UI_White",1.0f,true,true}
					);
					Engine::_ecs.addComponentToEntity(
						emitter->particles[i],
						ParticleCom{}
					);

					Engine::_ecs.addEntityTag(emitter->particles[i], "Particles");
					Engine::_ecs.setEntityParent(emitter->particles[i], entity);
				}*/
				emitter->isInitialised = true;
			}


			for (auto& particles : _particleData[entity])
			{
				auto particleCom = &particles._particleCom;
				//Emit(emitter, particleCom);
				/*if (particleCom)
				{*/
					if (particleCom->isActive)
					{
						if (particleCom->lifeRemaining <= 0.0f)
						{
							particleCom->isActive = false;
							continue;
						}

						particleCom->lifeRemaining -= dt;
						particleCom->position += particleCom->velocity * (float)dt;
						particleCom->rotation += 0.01f * dt;

						auto transform = &particles._transformCom;
						//auto material = Engine::_ecs.getComponent<MaterialCom>(particles);
						auto sprite = &particles._spriteCom;
						float life = particleCom->lifeRemaining / particleCom->lifeTime;
						float size = lerp(particleCom->sizeEnd, particleCom->sizeBegin, life);
						transform->scale.x = transform->scale.y = transform->scale.z = size;
						transform->pos = particleCom->position;
						auto pos = particleCom->position;
						/*material->colour.x = particleCom->colorBegin.x;
						material->colour.y = particleCom->colorBegin.y;
						material->colour.z = particleCom->colorBegin.z;*/
						float color_x = lerp(particleCom->colorEnd.x, particleCom->colorBegin.x, life);
						float color_y = lerp(particleCom->colorEnd.y, particleCom->colorBegin.y, life);
						float color_z = lerp(particleCom->colorEnd.z, particleCom->colorBegin.z, life);
						sprite->_colour.x = color_x;
						sprite->_colour.y = color_y;
						sprite->_colour.z = color_z;
					}
				//}

			}
			if (emitter->emit)
			{
				if (emitter->timer <= 0.0f)
				{
					//int particleperframe = 1;
					/*for (int i = 0; i < particleperframe; ++i)
					{*/
						//auto particleCom = Engine::_ecs.getComponent<ParticleCom>(emitter->particles[emitter->poolIndex]);
						auto particleCom = &_particleData[entity][emitter->poolIndex]._particleCom;
						if (!particleCom->isActive)
							Emit(*emitter, *particleCom);
						///*else
						//	--i;*/

						emitter->poolIndex++;
						if (emitter->poolIndex >= emitter->maxParticles)
							emitter->poolIndex = 0;
						/*for (auto& particleData : _particleData[entity])
						{
							if (!particleData._particleCom.isActive)
								Emit(*emitter, particleData._particleCom);
						}*/
					//}
					emitter->timer = emitter->timeset;
				}
				else
				{
					emitter->timer -= dt;
				}

			}
		}
	}
}

void ParticleSystem::unload()
{
}

void ParticleSystem::release()
{
}

void ParticleSystem::Emit(const EmitterCom& emitter, ParticleCom& particleCom)
{

	particleCom.isActive = true;
	particleCom.position = emitter.position;
	particleCom.rotation = emitter.rotation;

	// Velocity
	particleCom.velocity = emitter.velocity;
	particleCom.velocity.x += emitter.velocityVariation.x * (Random::Float() - 0.5f);
	particleCom.velocity.y += emitter.velocityVariation.y * (Random::Float() - 0.5f);
	particleCom.velocity.z += emitter.velocityVariation.z * (Random::Float() - 0.5f);

	// Color
	particleCom.colorBegin = emitter.colorBegin;
	particleCom.colorEnd = emitter.colorEnd;

	particleCom.lifeTime = emitter.lifeTime;
	particleCom.lifeRemaining = emitter.lifeTime;
	particleCom.sizeBegin = emitter.sizeBegin + emitter.sizeVariation * (Random::Float() - 0.5f);
	particleCom.sizeEnd = emitter.sizeEnd;

}


