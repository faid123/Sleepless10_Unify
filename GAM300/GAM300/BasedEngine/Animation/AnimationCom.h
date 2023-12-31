/**********************************************************************************
* \file AnimationCom.h
* \brief File contains the function definitions for all functions that are used
		 to animation.
* \author Zhang Xinyu, 100% Code Contribution
*
* \copyright Copyright (C) 2022 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include "Skinned_Mesh.h"
#include <Component/Component.h>
#include <Properties.h>
#include "Serializer/ISerializer.h"
#define SOL_CHECK_ARGUMENTS 1
#include "sol/sol.hpp"
struct AnimationCom:Component //,public ISerializer
{
	int currentAnim;
	bool play; //animated or static model
	bool pause; //pause a animated model 
	bool restart = false;
	float speed;
	
	AnimationCom() :currentAnim{ 0 }, play{true},
		pause{ false }, speed{ 10.0f } {}

	void destroyEntity(EntityID entity) override
	{
		(void)entity;
	}
	void cloneComponent(EntityID entity1, EntityID entity2) override
	{
		(void)entity1;
		(void)entity2;
	}
	/*void Deserialize(const rapidjson::Document& doc, std::string id) {}
	void Serialize(rapidjson::PrettyWriter<rapidjson::StringBuffer>* writer) {}*/
	static void RegisterLua(sol::state& lua);
	property_vtable()
};

property_begin(AnimationCom)
{
	property_var(currentAnim)
		, property_var(speed)
} property_vend_h(AnimationCom)

class Animator
{
public:
	Animator() {  }
	Animator(std::string file, std::string texture_, std::string model_, float speed_ = 7.0f);
	~Animator() {}

	void update(float dt);
	//void setAnimation(int c) { animation = animationContainer[c]; }
	int getCurrentAnimation() { return comp.currentAnim; }

	void loadModel(const aiScene* scene, Bone& skeletonOutput);
	void readNodeHierarchy(float p_animation_time, Bone& bone, const aiMatrix4x4 parent_transform);
	void boneTransform(float time_in_sec, std::vector<glm::mat4>& transforms);

	EntityID owner = EntityID(0);
	AnimationCom comp;
	Animation animation;

	Bone skeleton;
	int displayedAnimation=0;

	std::map<std::string, uint> m_bone_mapping; // maps a bone name and their index
	uint m_num_bones = 0;
	std::vector<BoneMatrix> m_bone_matrices;

	std::string texture;
	std::string model;
	float elapsedTime;
	int totalAnimations = 0;

	glm::mat4 modelMatrix{ 1.0f };
	glm::mat4 viewMatrix;
	glm::mat4 viewProjectionMatrix;
	glm::mat4 projectionMatrix;

	//for animation 
	glm::mat4 identity{ 1.0 };
	aiMatrix4x4 globalInverseTransform;
	std::vector<glm::mat4> currentPose = {};
	std::vector<Animation> animationContainer = {};
};