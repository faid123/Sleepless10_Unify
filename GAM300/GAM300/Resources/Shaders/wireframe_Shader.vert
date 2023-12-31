#version 450 core

layout (location=0) in vec3 vVertexPosition;

layout (location=0) out vec3 vClrCoord;

layout(std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

//uniform mat4 viewProj;
uniform mat4 model;
uniform vec3 colour_modifier;

void main() 
{
	//gl_Position =  viewProj* model * vec4(vVertexPosition, 1.f);
	gl_Position =  projection * view * model * vec4(vVertexPosition, 1.f);
	vClrCoord =  colour_modifier;
}
