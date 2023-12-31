#include "Skinned_Mesh.h"
#include <Graphics/DDSLoader/ModelLoader.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

void AnimVertex::addBoneData(int bone_id, float weight)
{
	for (int i = 0; i < 4; i++)
	{
		if (boneWeights[i] == 0.0)
		{
			boneIds[i] = (float)bone_id;
			boneWeights[i] = weight;
			return;
		}
	}
}

void readSkeleton(Bone& boneOutput, aiNode* node)
{
	// if node is actually a bone
	boneOutput.name = node->mName.data;
	boneOutput.transform = node->mTransformation;

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		Bone tmp;

		readSkeleton(tmp, node->mChildren[i]);

		boneOutput.children.push_back(tmp);
	}
}

void loadAnimation(const aiScene* scene, std::vector<Animation>& animations)
{
	//loading  first Animation
	for (unsigned int n = 0; n < scene->mNumAnimations; ++n)
	{
		aiAnimation* anim = scene->mAnimations[n];
		Animation animation;

		if (anim->mTicksPerSecond != 0.0f)
			animation.ticksPerSecond = static_cast<float>(anim->mTicksPerSecond);
		else
			animation.ticksPerSecond = 25;


		animation.duration = static_cast<float>(anim->mDuration);  //animation.ticksPerSecond *
		animation.boneTransforms = {};

		//load positions rotations and scales for each bone
		// each channel represents each bone
		for (unsigned int i = 0; i < anim->mNumChannels; i++) {
			aiNodeAnim* channel = anim->mChannels[i];
			BoneTransformTrack track;
			for (unsigned int j = 0; j < channel->mNumPositionKeys; j++) {
				track.positionTimestamps.push_back(static_cast<float>(channel->mPositionKeys[j].mTime));
				track.positions.push_back((channel->mPositionKeys[j].mValue));
			}
			for (unsigned int j = 0; j < channel->mNumRotationKeys; j++) {
				track.rotationTimestamps.push_back(static_cast<float>(channel->mRotationKeys[j].mTime));
				track.rotations.push_back((channel->mRotationKeys[j].mValue));

			}
			for (unsigned int j = 0; j < channel->mNumScalingKeys; j++) {
				track.scaleTimestamps.push_back(static_cast<float>(channel->mScalingKeys[j].mTime));
				track.scales.push_back((channel->mScalingKeys[j].mValue));
			}
			animation.boneTransforms[channel->mNodeName.C_Str()] = track;
		}
		animations.push_back(animation);
	}
}

bool findbone(Animation p_animation, const std::string p_node_name, BoneTransformTrack& tmp)
{
	if (p_animation.boneTransforms.find(p_node_name) != p_animation.boneTransforms.end())
	{
		tmp = p_animation.boneTransforms[p_node_name];
		return true;
	}
	else
		return false;
}

aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend)
{
	//cout << a.w + a.x + a.y + a.z << endl;
	a.Normalize();
	b.Normalize();

	aiQuaternion result;
	float dot_product = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	float one_minus_blend = 1.0f - blend;

	if (dot_product < 0.0f)
	{
		result.x = a.x * one_minus_blend + blend * -b.x;
		result.y = a.y * one_minus_blend + blend * -b.y;
		result.z = a.z * one_minus_blend + blend * -b.z;
		result.w = a.w * one_minus_blend + blend * -b.w;
	}
	else
	{
		result.x = a.x * one_minus_blend + blend * b.x;
		result.y = a.y * one_minus_blend + blend * b.y;
		result.z = a.z * one_minus_blend + blend * b.z;
		result.w = a.w * one_minus_blend + blend * b.w;
	}

	return result.Normalize();
}

uint findPosition(float p_animation_time, BoneTransformTrack& btt)
{
	for (uint i = 0; i < btt.positions.size() - 1; i++)
	{
		if (p_animation_time < (float)btt.positionTimestamps[i + 1])
		{
			return i;
		}
	}

	assert(0);
	return 0;
}

uint findRotation(float p_animation_time, BoneTransformTrack& btt)
{
	for (uint i = 0; i < btt.rotations.size() - 1; i++)
	{
		if (p_animation_time < (float)btt.rotationTimestamps[i + 1])
		{
			return i;
		}
	}

	assert(0);
	return 0;
}

uint findScaling(float p_animation_time, BoneTransformTrack& btt)
{
	for (uint i = 0; i < btt.scales.size() - 1; i++)
	{
		if (p_animation_time < (float)btt.scaleTimestamps[i + 1])
		{
			return i;
		}
	}
	assert(0);
	return 0;
}

