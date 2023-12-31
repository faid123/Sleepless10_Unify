#pragma once
#ifndef CONVERTER_H_
#define CONVERTER_H_
#include "PxPhysics.h"
#include "Vector2.h"
#include "Matrix4.h"

using namespace physx;
using namespace glm;
physx::PxVec3 glmVec3ToPxVec3(const glm::vec3& vec3);
glm::vec3 PxVec3ToglmVec3(const physx::PxVec3& vec3);
PxQuat glmQuatToPxQuat(const glm::quat& quat);
glm::quat PxQuatToglmQuat(const PxQuat& quat);
void PxMat4ToglmMat4(const PxMat44& mat4, glm::mat4& newMat);
glm::mat4 assimpToGlmMatrix(aiMatrix4x4 mat);
glm::vec3 assimpToGlmVec3(aiVector3D vec);
glm::quat assimpToGlmQuat(aiQuaternion quat);
//PxMat44 glmMat4ToPxMat4(const glm::mat4& mat4);
#endif