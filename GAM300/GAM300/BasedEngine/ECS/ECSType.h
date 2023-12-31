/******************************************************************************
* \file			ECSType.h
* \brief		Contain all the ECSType related function
* \author 		Ang, Pheng Siang , 100% Code Contribution
*
* \copyright	Copyright (C) 2021 DigiPen Institute of Technology.
				Reproduction or disclosure of this file or its contents without the
				prior written consent of DigiPen Institute of Technology is prohibited.
 /******************************************************************************/
#pragma once

#include <cstdlib>
#include <bitset>
#include <string>

#define ROOTID 4294967295

using EntityID = uint32_t;
using InstanceID = uint32_t;
using ComponentID = uint32_t;
using EntityName = std::string;

const EntityID ENTITIES = 3000;
const EntityID ENTITIES_UI = 500;
const EntityID MAX_ENTITIES = ENTITIES + ENTITIES_UI;

const ComponentID MAX_COMPONENTS = 50;
using Signature = std::bitset< MAX_COMPONENTS>;

using PrefabName = std::string;
using PrefabID = uint32_t;
using PrefabInstanceID = uint32_t;

const PrefabID MAX_PREFABS = 300;   // 200 initial


