/**********************************************************************************
* \file Settig.h
* \brief File contains the function definitions for all header files that are used
		 for serializer.
* \author Zhang Xinyu, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "../../lib/rapidjson/document.h"
#include "../../lib/rapidjson/prettywriter.h"
#include "../../lib/rapidjson/stringbuffer.h"
#include "../../lib/rapidjson/istreamwrapper.h"
#include "../MathLib/Precompiled.h"
#include <glm/glm.hpp>
#include <PxPhysics.h>

using Vec2D = Vector2;
using Vec3D = Vector3;
using Vec4D = Vector4;
using PVec2D = physx::PxVec2;
using PVec3D = physx::PxVec3;
using PVec4D = physx::PxVec4;
using GVec2D = glm::vec2;
using GVec3D = glm::vec3;
using GVec4D = glm::vec4;
using PQuat = physx::PxQuat;
using GQuat = glm::quat;


