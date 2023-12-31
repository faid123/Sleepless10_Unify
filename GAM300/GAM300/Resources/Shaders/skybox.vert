#version 450 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

layout(std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

//uniform mat4 projection;
//uniform mat4 view;

void main()
{
    mat4 newView = mat4(mat3(view)); // remove translation from the view matrix
    TexCoords = aPos;
    vec4 pos = projection * newView * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  