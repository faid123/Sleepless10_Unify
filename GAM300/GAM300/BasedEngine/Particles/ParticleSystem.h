/******************************************************************************
* \file			ParticleSystem.h
* \brief		Contain all the Camera related function
* \author 		Samuel Chia, 100% Code Contribution
*
* \copyright	Copyright (C) 2022 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#pragma once
#include "ParticleComponent.h"
#include "../ECS/System/System.h"
#include "../ECS/Component/Component.h"
#include "Engine.h"
#include "Graphics/TransformCom.h"
#include "EmitterComponent.h"
#include "Graphics/Graphics/Graphics.h"
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"
#include <map>

struct ParticleData
{
	ParticleCom _particleCom{};
	TransformCom _transformCom{ Vector3(0.0f,0.0f,-0.0f),
								Vector3(0.0f),
								Vector3(0.0f) };
	SpriteCom _spriteCom{ "UI_White",1.0f,true,true };
};

class ParticleSystem : public ISystem
{
public:
	ParticleSystem();
	void init() override;
	void load() override;
	void update(float dt) override;
	void unload() override;
	void release() override;

	void Emit(const EmitterCom& emitter,  ParticleCom& particleCom);
	static std::map<EntityID, std::vector<ParticleData>> _particleData;
	static std::map<EntityID, EntityID> _emitterList;
	static void RegisterLua(sol::state& lua);
};