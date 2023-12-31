#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 screenPOS; //matrix to move the drawing

uniform bool playingVideo;
void main()
{
     TexCoords = aTexCoords;
    if(playingVideo)
        TexCoords.y = 1 - aTexCoords.y;
    gl_Position = screenPOS * vec4(aPos.x, aPos.y, 0.0, 1.0); 
}  