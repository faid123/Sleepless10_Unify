#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 boneIds;
layout (location = 4) in vec4 boneWeights;

uniform mat4 model;
uniform mat4 bone_transforms[50];
uniform bool useBone;


void main()
{
     if (useBone)
    {
	    mat4 boneTransform  =  mat4(0.0);
	    boneTransform   =    bone_transforms[int(boneIds.x)] * boneWeights.x;
	    boneTransform  +=    bone_transforms[int(boneIds.y)] * boneWeights.y;
	    boneTransform  +=    bone_transforms[int(boneIds.z)] * boneWeights.z;
	    boneTransform  +=    bone_transforms[int(boneIds.w)] * boneWeights.w;
	    
        mat4 A2W = model * boneTransform;

        gl_Position =  A2W * vec4(aPos, 1.0);
    }
    else
    {
        gl_Position = model * vec4(aPos, 1.0);
    }
}