#version 450 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

const float gamma =2.2;
void main()
{    
    FragColor = texture(skybox, TexCoords);
    
    FragColor.rgb = pow(FragColor.rgb,vec3(1.0/gamma));
}