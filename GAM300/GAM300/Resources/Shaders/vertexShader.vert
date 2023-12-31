#version 430

layout(location = 0) in vec3 pos_from_vtxbuffer;
layout(location = 1) in vec3 clr_from_vtxbuffer;
layout(location = 2) in vec2 uv_from_vtxbuffer;

layout(std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out vec4 position;

smooth out vec3 clr_from_vtxshader;
out vec2 vTexCoord;
uniform mat4 model;
void main()
{

	position = gl_Position = projection * view * model * vec4(pos_from_vtxbuffer, 1.f);


	//position = gl_Position = uMVP * vec4(pos_from_vtxbuffer, 1.0f);
	clr_from_vtxshader = clr_from_vtxbuffer;
	vTexCoord = uv_from_vtxbuffer;
	vTexCoord.y = 1 - vTexCoord.y;
}

