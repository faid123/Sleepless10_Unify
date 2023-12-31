#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

//uniform vec3 saturation;

vec3 rgb2hsv(vec3 c);
vec3 hsv2rgb(vec3 c);

void main()
{
	if(texture(screenTexture, TexCoords).a < 0.1)
		discard;
    vec3 col = texture(screenTexture, TexCoords).rgb;



	    //vec3 vHSV    = saturation;
        //vec3 fragHSV = rgb2hsv(col).xyz;
        //
        //fragHSV.x   += vHSV.x / 360.0;
        //fragHSV.yz  *= vHSV.yz;
        //fragHSV.x    = mod(fragHSV.x,1.0);
        //fragHSV.yz   = clamp(fragHSV.yz,-100.0,100.0);
        //col = hsv2rgb(fragHSV);

    FragColor = vec4(col, 1.0);
} 

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}