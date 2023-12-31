#version 430

//layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 _projection;
uniform vec4 texOffsetScale;
uniform bool spriteAnim;

float _z;
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
      gl_Position = _projection * model * vec4(aPos.xyz, 1.0);
}