#pragma once
#include "Converter.h"
#include <glm/gtc/quaternion.hpp>
#include <foundation/PxMat44.h>

physx::PxVec3 glmVec3ToPxVec3(const glm::vec3& vec3)
{
    return physx::PxVec3(vec3.x, vec3.y, vec3.z);
}

glm::vec3 PxVec3ToglmVec3(const physx::PxVec3& vec3)
{
    return glm::vec3(vec3.x, vec3.y, vec3.z);
}

PxQuat glmQuatToPxQuat(const glm::quat& quat)
{
    return PxQuat(quat.x, quat.y, quat.z, quat.w);
}

glm::quat PxQuatToglmQuat(const PxQuat& quat)
{
    return glm::quat{ quat.w, quat.x, quat.y, quat.z };
}

void PxMat4ToglmMat4(const PxMat44& mat4, glm::mat4& newMat)
{

    newMat[0][0] = mat4[0][0];
    newMat[0][1] = mat4[0][1];
    newMat[0][2] = mat4[0][2];
    newMat[0][3] = mat4[0][3];

    newMat[1][0] = mat4[1][0];
    newMat[1][1] = mat4[1][1];
    newMat[1][2] = mat4[1][2];
    newMat[1][3] = mat4[1][3];

    newMat[2][0] = mat4[2][0];
    newMat[2][1] = mat4[2][1];
    newMat[2][2] = mat4[2][2];
    newMat[2][3] = mat4[2][3];

    newMat[3][0] = mat4[3][0];
    newMat[3][1] = mat4[3][1];
    newMat[3][2] = mat4[3][2];
    newMat[3][3] = mat4[3][3];
}
glm::mat4 assimpToGlmMatrix(aiMatrix4x4 mat) {
    glm::mat4 m;
    for (int y = 0; y < 4; y++)
    {
        for (int x = 0; x < 4; x++)
        {
            m[x][y] = mat[y][x];
        }
    }
    return m;
}
glm::vec3 assimpToGlmVec3(aiVector3D vec) {
    return glm::vec3(vec.x, vec.y, vec.z);
}

glm::quat assimpToGlmQuat(aiQuaternion quat) {
    glm::quat q;
    q.x = quat.x;
    q.y = quat.y;
    q.z = quat.z;
    q.w = quat.w;

    return q;
}