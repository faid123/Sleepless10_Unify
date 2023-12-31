#version 430

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform float Alpha;
uniform float brightVal;
uniform vec3 spriteColor;
uniform bool UseSprite;
const float gamma = 1.375;
uniform float gammaCorrection;
void main()
{    
    //color = vec4(spriteColor, 1.0) * texture(image, TexCoords);
    //color =  texture(image, TexCoords);
    vec4 texcolor;
    if(UseSprite)
    {
        texcolor = texture(image, TexCoords);
        texcolor.rgb = texcolor.rgb * spriteColor;
        texcolor.rgb = texcolor.rgb * brightVal;
	    texcolor.a = texcolor.a * Alpha;
	    color = texcolor;
        color.rgb = pow(color.rgb,vec3(1.0/(gamma + gammaCorrection)));
    }
    else
    {
        //texcolor.rgb = spriteColor;
        //texcolor.a = 1.0f;

        discard;
    }

    
}