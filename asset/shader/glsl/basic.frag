#version 430

layout (binding = 0) uniform sampler2D texColour;
layout (binding = 1) uniform sampler2D texNormal;
layout (binding = 2) uniform sampler2D texSpec;
layout (binding = 3) uniform sampler2D texEnv;
layout (binding = 4) uniform sampler2D shadowmap;

in vec4 outNormal;	//input: normal
in vec4 lightvec;	//input: light vector
in vec4 viewvec;	//input: view vector
in vec2 outUV;		//input: texcoords
in vec4 outPosInLight;	//input: position in light space

layout (location = 0) out vec4 outFrag;

struct DirectionalLight 
{
	vec3 direction; // Direction of the light rays
	vec3 ambient; // Global ambient lighting
	vec3 diffuse; // Diffuse (color) for the light
	vec3 specular; // Specular shading color for the light
};
layout (location = 7) uniform DirectionalLight directionalLight;

struct SpotLight
{
	vec3 position; // Light position
	vec3 direction; // Light direction
	vec3 color; // Light color
	float coneAngle;
	float coneCosine;
	float linearAttenuation;
};
layout (location = 11) uniform SpotLight spotLight;

struct AreaLight
{
	vec3 position; // Light position
	vec3 color; // Light color
	float constantAttenuation; // Light constant attenuation
	float linearAttenuation; // Light linear attenuation
	float expAttenuation; // Light exponential attenuation
};
layout (location = 17) uniform AreaLight areaLight;

vec4 calcDirectionalLightColor(DirectionalLight light, vec3 normal);
vec4 calcSpotLightColor(SpotLight light, vec3 normal);
vec4 calcAreaLightColor(AreaLight light, vec3 normal);

void main()
{
	vec4 textureColor = texture(texColour, outUV);
	vec4 lightingColor = vec4(0.2f, 0.2f, 0.2f, 1.0f); // set default to global ambient

	vec3 normal = normalize(outNormal.xyz);
	
	//Calculate directional lights color
	lightingColor += calcDirectionalLightColor(directionalLight, normal);

	outFrag = textureColor * lightingColor;
}

vec4 calcDirectionalLightColor(DirectionalLight light, vec3 normal)
{
	vec4 finalColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	float intensity = max(0.0f, dot(normal, -light.direction));
	finalColor = vec4(light.diffuse * intensity, 1.0f);

	return finalColor;
}

vec4 calcSpotLightColor(SpotLight light, vec3 normal)
{
	vec4 finalColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	return finalColor;
}

vec4 calcAreaLightColor(AreaLight light, vec3 normal)
{
	vec4 finalColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	return finalColor;
}