/**********************************************************************************
* \file Skinned_Mesh.h
* \brief File contains the function definitions for all functions that are used
		 to 3d animation.
* \author Zhang Xinyu, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <unordered_map>
#include "MathLib/Precompiled.h"


// vertex of an animated model
struct AnimVertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec4 boneIds = glm::vec4(0);
	glm::vec4 boneWeights = glm::vec4(0.0f);
	void addBoneData(int bone_id, float weight);
};

// structure to hold bone tree (skeleton)
struct Bone {
	int id = 0; // position of the bone in final upload array
	std::string name = "";
	aiMatrix4x4 transform;
	std::vector<Bone> children = {};
};

// sturction representing an animation track
struct BoneTransformTrack {
	std::vector<float> positionTimestamps = {};
	std::vector<float> rotationTimestamps = {};
	std::vector<float> scaleTimestamps = {};

	std::vector<aiVector3D> positions = {};
	std::vector<aiQuaternion> rotations = {};
	std::vector<aiVector3D> scales = {};
};

struct BoneMatrix
{
	aiMatrix4x4 offset_matrix;
	aiMatrix4x4 final_world_transform;
};

// structure containing animation information
struct Animation {
	float duration = 0.0f;
	float ticksPerSecond = 1.0f;
	std::unordered_map<std::string, BoneTransformTrack> boneTransforms = {};
};

void readSkeleton(Bone& boneOutput, aiNode* node);
void loadAnimation(const aiScene* scene, std::vector<Animation>& animations);
bool findbone(Animation p_animation, const std::string p_node_name, BoneTransformTrack& tmp);
uint findPosition(float p_animation_time, BoneTransformTrack& btt);
uint findRotation(float p_animation_time, BoneTransformTrack& btt);
uint findScaling(float p_animation_time, BoneTransformTrack& btt);
aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend);






