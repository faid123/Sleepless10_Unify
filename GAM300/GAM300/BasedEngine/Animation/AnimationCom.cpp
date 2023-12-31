#include "AnimationCom.h"
#include <sys/utime.h>
#include <sys/types.h>
#include "Graphics/Window/Window/Window.h"
#include <glm/gtc/constants.hpp >
#include "../Graphics/RenderSystem/RenderSystem.h"
#include "Camera/CameraSystem/CameraSystem.h"


void Animator::loadModel(const aiScene* scene, Bone& skeletonOutput) {

    // load bones
    for (uint j = 0; j < scene->mNumMeshes; ++j)
    {
        for (uint i = 0; i < scene->mMeshes[j]->mNumBones; i++)
        {
            uint bone_index = 0;
            std::string bone_name(scene->mMeshes[j]->mBones[i]->mName.data);

            if (m_bone_mapping.find(bone_name) == m_bone_mapping.end())
            {
                // Allocate an index for a new bone
                bone_index = m_num_bones;
                m_num_bones++;
                BoneMatrix bi;
                m_bone_matrices.push_back(bi);
                m_bone_matrices[bone_index].offset_matrix = scene->mMeshes[j]->mBones[i]->mOffsetMatrix;
                m_bone_mapping[bone_name] = bone_index;
            }
            else
            {
                bone_index = m_bone_mapping[bone_name];
            }
        }
    }
    readSkeleton(skeletonOutput, scene->mRootNode);
}
Animator::Animator(std::string file,std::string texture_,std::string model_,float speed_)
{
     Assimp::Importer importer;
     const char* filePath = file.c_str();
     const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    }
    //aiMesh* mesh = scene->mMeshes[0];

    //as the name suggests just inverse the global transform
    globalInverseTransform = (scene->mRootNode->mTransformation);
    globalInverseTransform.Inverse();

    loadModel( scene,skeleton);
    loadAnimation(scene, animationContainer);

    //set default animation 
    animation = animationContainer[0];
    totalAnimations = static_cast<int>(animationContainer.size());

    //currentPose is held in this vector and uploaded to gpu as a matrix array uniform
    elapsedTime = 0.5;
    boneTransform(elapsedTime, currentPose); // use this for no animation

    texture = texture_;
    model = model_;
    comp.speed = speed_;
}

void Animator::update(float delta_time)
{

    if (comp.pause)
    {
        elapsedTime = elapsedTime;
        if (comp.restart && model == "BossArm1Bone")
        {
            elapsedTime = 12;
          //  std::cout << "Reset Time" << std::endl;
        }
            
    }   
    else
        elapsedTime += comp.speed * delta_time;

    if (!comp.play)
    {
       elapsedTime = 0.5;
    }

    if (elapsedTime >= animation.duration)
        elapsedTime = 0.1f;
    boneTransform(elapsedTime, currentPose);
}

void Animator::readNodeHierarchy(float p_animation_time, Bone& bone, const aiMatrix4x4 parent_transform)
{
    aiMatrix4x4 node_transform = bone.transform;
    BoneTransformTrack btt;
    bool check = findbone(animation, bone.name, btt);

    if (check)
    {
        //scaling
        aiVector3D scaling_vector;
        if (btt.scales.size() == 1) 
        {
            scaling_vector = btt.scales[0];
        }

        uint scaling_index = findScaling(p_animation_time, btt);
        uint next_scaling_index = scaling_index + 1;
        float delta_time = (float)(btt.scaleTimestamps[next_scaling_index] - btt.scaleTimestamps[scaling_index]);
        float  factor = (p_animation_time - (float)btt.scaleTimestamps[scaling_index]) / delta_time;
        assert(factor >= 0.0f && factor <= 1.0f);
        aiVector3D start = btt.scales[scaling_index];
        aiVector3D end = btt.scales[next_scaling_index];
        aiVector3D delta = end - start;
        scaling_vector = start + factor * delta;
        aiMatrix4x4 scaling_matr;
        aiMatrix4x4::Scaling(scaling_vector, scaling_matr);

        //rotation
        aiQuaternion rotate_quat;
        if (btt.rotations.size() == 1) 
        {
            rotate_quat = btt.rotations[0];
        }

        uint rotation_index = findRotation(p_animation_time, btt);
        uint next_rotation_index = rotation_index + 1;
        delta_time = (float)(btt.rotationTimestamps[next_rotation_index] - btt.rotationTimestamps[rotation_index]);
        factor = (p_animation_time - (float)btt.rotationTimestamps[rotation_index]) / delta_time;
        assert(factor >= 0.0f && factor <= 1.0f);
        aiQuaternion start_quat = btt.rotations[rotation_index];
        aiQuaternion end_quat = btt.rotations[next_rotation_index];
        rotate_quat = nlerp(start_quat, end_quat, factor);
        aiMatrix4x4 rotate_matr = aiMatrix4x4(rotate_quat.GetMatrix());

        //translation
        aiVector3D translate_vector;
        if (btt.positions.size() == 1) 
        {
            translate_vector = btt.positions[0];
        }

        uint position_index = findPosition(p_animation_time, btt);
        uint next_position_index = position_index + 1;
        delta_time = (float)(btt.positionTimestamps[next_position_index] - btt.positionTimestamps[position_index]);
        factor = (p_animation_time - (float)btt.positionTimestamps[position_index]) / delta_time;
        assert(factor >= 0.0f && factor <= 1.0f);
        start = btt.positions[position_index];
        end = btt.positions[next_position_index];
        delta = end - start;
        translate_vector = start + factor * delta;
        aiMatrix4x4 translate_matr;
        aiMatrix4x4::Translation(translate_vector, translate_matr);

        {
            node_transform = translate_matr * rotate_matr * scaling_matr;
        }

    }

    aiMatrix4x4 global_transform = parent_transform * node_transform;

    if (m_bone_mapping.find(bone.name) != m_bone_mapping.end()) // true if node_name exist in bone_mapping
    {
        uint bone_index = m_bone_mapping[bone.name];
        m_bone_matrices[bone_index].final_world_transform = globalInverseTransform * global_transform *m_bone_matrices[bone_index].offset_matrix;
    }

    for (uint i = 0; i < bone.children.size(); i++)
    {
        readNodeHierarchy(p_animation_time, bone.children[i], global_transform);
    }


}
glm::mat4 aiToGlm(aiMatrix4x4 ai_matr)
{
    glm::mat4 result;
    result[0].x = ai_matr.a1; result[0].y = ai_matr.b1; result[0].z = ai_matr.c1; result[0].w = ai_matr.d1;
    result[1].x = ai_matr.a2; result[1].y = ai_matr.b2; result[1].z = ai_matr.c2; result[1].w = ai_matr.d2;
    result[2].x = ai_matr.a3; result[2].y = ai_matr.b3; result[2].z = ai_matr.c3; result[2].w = ai_matr.d3;
    result[3].x = ai_matr.a4; result[3].y = ai_matr.b4; result[3].z = ai_matr.c4; result[3].w = ai_matr.d4;
    return result;
}
void Animator::boneTransform(float time_in_sec, std::vector<glm::mat4>& transforms)
{
    aiMatrix4x4 identity_matrix; 
    float animation_time = fmod(time_in_sec, animation.duration);

    readNodeHierarchy(animation_time, skeleton, identity_matrix);

    transforms.resize(m_num_bones);

    for (uint i = 0; i < m_num_bones; i++)
    {
        transforms[i] = aiToGlm( m_bone_matrices[i].final_world_transform);
    }
}
