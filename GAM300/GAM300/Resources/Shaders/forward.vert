#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 boneIds;
layout (location = 4) in vec4 boneWeights;

out vec3 FragPos;
out vec3 Normal;
out vec3 Clr;
out vec2 TexCoords;
out vec4 ShadowPos;
out vec4 bw;   

layout(std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;
uniform vec3 vertex_Clr;
uniform mat4 ShadowMatrix;
uniform mat4 bone_transforms[50];
uniform mat4 normals_matrix;
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

        gl_Position = projection * view * A2W * vec4(aPos, 1.0);
	    FragPos     = ( A2W * vec4(aPos, 1.0) ).xyz;
	    Normal      = normalize( (A2W * vec4(aNormal, 0.0)).xyz );
        TexCoords   = aTexCoords;
        //TexCoords.y  = 1 - aTexCoords.y;
        Clr         = vertex_Clr;
    }
    else
    {
        gl_Position  = projection * view * model * vec4(aPos, 1.0);
        FragPos      = vec3(model * vec4(aPos, 1.0));
        Normal       = normalize( (model * vec4( aNormal, 0 )).xyz );  
        TexCoords    = aTexCoords;
        TexCoords.y  = 1 - aTexCoords.y;
        Clr          = vertex_Clr;
    }
}