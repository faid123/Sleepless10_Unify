#version 430

//layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
out vec2 TexCoords;

layout(std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform mat4 model;
uniform vec4 texOffsetScale;
uniform bool spriteAnim;

void main()
{
    if(spriteAnim)
	{
 		TexCoords = vec2(aTexCoords.x / texOffsetScale.z + texOffsetScale.x,
                       aTexCoords.y / texOffsetScale.w + texOffsetScale.y);
		TexCoords.y = 1.0 - TexCoords.y;
	}
	else
	{
		TexCoords = aTexCoords;
      	TexCoords.y = 1.0 - TexCoords.y;
	}


    vec3 particleScale_worldSpace = vec3( model * vec4(1.0,1.0,1.0,0.0));
    vec3 particleCenter_worldSpace = vec3( model[3][0], model[3][1], model[3][2]);

    vec3 camRight_worldspace = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 camUp_worldspace = vec3(view[0][1], view[1][1], view[2][1]);

    vec3 vertexPosition_worldspace =
        particleCenter_worldSpace
        + camRight_worldspace * aPos.x * particleScale_worldSpace.x
        + camUp_worldspace * aPos.y * particleScale_worldSpace.y;
    
    gl_Position = projection * view * vec4(vertexPosition_worldspace,1.0f);


}