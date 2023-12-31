#version 430

in vec3 clr_from_vtxshader;
//in vec2 uv_from_vtxshader;
in vec2 vTexCoord;
out vec4 clr_from_fragshader;

in vec4 position;

//uniform vec3 ambientCol;
uniform sampler2D uTex2d;

void main()
{

    //ambientCol = vec3(1.0f,1.0f,1.0f);
	//clr_from_fragshader = vec4(clr_from_vtxshader * ambientCol, 1.f);
	//clr_from_fragshader = vec4(clr_from_vtxshader, 1.f);
	
	//clr_from_fragshader = texture(uTex2d, vTexCoord);
	//clr_from_fragshader = vec4(1.0f);
	//clr_from_fragshader = vec4(vTexCoord.xy,0,1.0f);
	
	clr_from_fragshader = vec4(texture(uTex2d,vTexCoord).xyz, 1.0f);
	
	
	
	
	
}