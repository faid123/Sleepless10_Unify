#version 460 core
layout(location = 0) out vec4 FragColor;
layout(location = 1) out int EntityID;

layout(location = 0) uniform sampler2D      uTex2d;
layout(location = 1) uniform sampler2DArray shadowMap;

struct Material {
    //sampler2D diffuse;
    //sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;   
    vec3 position;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
         
};





//MAX AMOUNT OF LIGHT PER TYPE
#define NR_MAX_LIGHTS 4
//UNIFORMS OF AMT OF LIGHT COMING IN
uniform int numDir;
uniform int numPoint;
uniform int numSpot;

in vec3 FragPos;
in vec3 Normal;
in vec3 Clr;
in vec2 TexCoords;
in vec4 ShadowPos;
//in vec4 bw;

uniform bool useTexture;

layout(std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

//layout(std140 binding = 1) uniform Lights
//{
//    int number;
//    DirLight dlights[NR_MAX_LIGHTS];
//}

uniform vec3 viewPos;
uniform DirLight dirLights[NR_MAX_LIGHTS];

uniform PointLight pointLights[NR_MAX_LIGHTS];
uniform SpotLight spotLights[NR_MAX_LIGHTS];
uniform Material material;

uniform vec3 saturation;
uniform float gammaCorrection;
uniform int entityID;

// function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 rgb2hsv(vec3 c);
vec3 hsv2rgb(vec3 c);

const float gamma =2.2;
vec3 texClr;

vec3 wcPosition = FragPos;  // Fragment's 3D position in world space.
vec3 ecPosition = viewPos;  // Fragment's 3D position in eye space.
vec3 ecNormal = Normal;    // Fragment's normal vector in eye space.
vec2 texCoord = TexCoords;    // Fragment's texture coordinates.


//   shadow stuffs
//layout (binding = 1) uniform sampler2DShadow shadowMap;  // Shadow map. 0
//uniform mat4 lightSpaceMatrices[5];
layout (std140) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[5];
};

float shadowFact;
uniform float cascadePlaneDistances[16];
uniform int cascadeCount;   // number of frusta - 1
uniform float farPlane;
//shadow function prototypes
float ShadowCalculation( );

uniform float Alpha;

void main()
{    
	float sum = 0.0;
	ivec2 bias = ivec2(-1,1);
    EntityID = entityID;
	

    //check alpha
    if(Alpha <= 0.0001f)
        discard;

    // calculate shadow
    shadowFact = ShadowCalculation();  
	
    // properties
    vec3 norm    = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    if( useTexture )
    {
        //test texture alpha
        if(texture2D(uTex2d, TexCoords).a < 0.1)
            discard;

        vec3 vHSV    = saturation;
        vec3 fragRGB = texture2D(uTex2d, TexCoords).rgb;
        vec3 fragHSV = rgb2hsv(fragRGB).xyz;

        fragHSV.x   += vHSV.x / 360.0;
        fragHSV.yz  *= vHSV.yz;
        fragHSV.x    = mod(fragHSV.x,1.0);
        fragHSV.yz   = clamp(fragHSV.yz,-100.0,100.0);
        texClr = hsv2rgb(fragHSV);


        //texClr = texture(uTex2d, TexCoords).rgb;

    }
        
    else
        texClr = Clr;

    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    vec3 result = vec3(0.0f);
    
    //result = result + texture(ShadowMap, projCoords.xy).xyz;
    // phase 1: directional lighting
    for(int i = 0; i < numDir; i++)
        result += CalcDirLight(dirLights[i], norm, viewDir);

    //result *= shadowFact;
    // phase 2: point lights
    for(int i = 0; i < numPoint; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    

    // phase 3: spot light
	for(int i = 0; i < numSpot; i++)
		result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);    

    //result = Clr;
    //FragColor = vec4(result, 1.0);

             
    FragColor     = vec4(result, Alpha);
    FragColor.rgb = pow(FragColor.rgb,vec3(1.0/(gammaCorrection + gamma)));
}

//--------------------------------------------------------------------

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
// calculates the color when using a directional light.
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir )
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 ambient  = light.ambient;
    vec3 diffuse  = light.diffuse  * diff;
    vec3 specular = light.specular * spec;
     
    vec3 lighting = (ambient + (1.0 - shadowFact) * (diffuse + specular)) * texClr;   

    return lighting;
;
}

// calculates the color when using a point light.

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    
    vec3 ambient = light.ambient * texClr;
    vec3 diffuse = light.diffuse * diff * texClr;
    vec3 specular = light.specular * spec * texClr;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
	return ((diffuse + specular) + ambient );
    //return (shadowFact*(diffuse + specular) + ambient );
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results

    
    vec3 ambient = light.ambient * texClr;
    vec3 diffuse = light.diffuse * diff * texClr;
    vec3 specular = light.specular * spec * texClr;
    
    /*
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    */


    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    //return (shadowFact*(diffuse + specular) + ambient );
	return ((diffuse + specular) + ambient );
}

float ShadowCalculation()
{
    // select cascade layer
    vec4 fragPosViewSpace = view * vec4(FragPos, 1.0);
    float depthValue = abs(fragPosViewSpace.z);

    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (depthValue < cascadePlaneDistances[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }

    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(FragPos, 1.0);

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if (currentDepth > 1.0)
    {
        return 0.0;
    }

    // calculate bias (based on depth map resolution and slope)
    vec3        normal       = normalize(Normal);
    vec3        dirLight     = normalize(dirLights[0].direction);
    float       bias         = max(0.05 * (1.0 - dot(normal, dirLight)), 0.005);
    const float biasModifier = 0.5f;
    if (layer == cascadeCount)
    {
        bias *= 1 / (farPlane * biasModifier);
    }
    else
    {
        bias *= 1 / (cascadePlaneDistances[layer] * biasModifier);
    }
    
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
        
    return shadow;
}