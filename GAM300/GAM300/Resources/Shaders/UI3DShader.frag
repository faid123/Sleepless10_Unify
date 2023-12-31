#version 430

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform float brightVal;
uniform float Alpha;
uniform bool UseSprite;
uniform vec3 ColourMod;
void main()
{    
    //color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
    //color =  texture(image, TexCoords);
    vec4 texcolor;
    if(UseSprite)
    {
        texcolor = texture(image, TexCoords);
        texcolor.rgb = texcolor.rgb * ColourMod;
    }
    else
    {
        //texcolor = texture(image, TexCoords);
        //texcolor.rgb = ColourMod;
        discard;
    }
    if(texcolor.a < 0.1)
        discard;
    
	texcolor.a = texcolor.a * Alpha;
    texcolor.rgb = texcolor.rgb * brightVal;
	color = texcolor;
}