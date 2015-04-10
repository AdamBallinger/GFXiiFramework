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
in vec4 vposition;
in vec4 eposition;
in vec4 outPosInLight;	//input: position in light space

layout (location = 0) out vec4 outFrag;

struct BaseLight
{
	vec3 position;
	vec3 direction;
	vec3 color;
	float intensity;
};

struct DirectionalLight 
{
	BaseLight base;
};

struct SpotLight
{
	BaseLight base;
	float exponent;
	float cutOff;
};

struct AreaLight
{
	BaseLight base;
	float constantAttenuation;
	float linearAttenuation; 
	float expAttenuation;
};

layout (location = 7) uniform DirectionalLight directionalLight;
layout (location = 11) uniform SpotLight spotLight;
layout (location = 17) uniform AreaLight areaLight;

vec3 calcBumpedNormal();
vec4 calcLightColor(BaseLight light, vec3 lightDir, vec3 normal, float shadowFactor);
vec4 calcDirectionalLightColor(DirectionalLight light, vec3 normal);
vec4 calcSpotLightColor(SpotLight light, vec3 normal);
vec4 calcAreaLightColor(AreaLight light, vec3 normal);

float calcShadoFactor(vec4 lightSpacePosition);

void main()
{
	vec4 finalColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 texDiffuse = texture(texColour, outUV);
	float depth = texture(shadowmap, outUV).x;
	depth = 1.0 - (1.0 - depth) * 25.0;

	vec4 lightingColor = vec4(0.2f, 0.2f, 0.2f, 1.0f); // set default lighting to global ambient

	vec3 normal = calcBumpedNormal();
	
	//Calculate lighting color for light types
	lightingColor += calcDirectionalLightColor(directionalLight, normal);
	//lightingColor += calcSpotLightColor(spotLight, normal);
	lightingColor += calcAreaLightColor(areaLight, normal);

	finalColor = texDiffuse * lightingColor;
	outFrag = finalColor;
	//outFrag = vec4(normal, 1.0f);
	//outFrag = vec4(depth); 
}

float calcShadowFactor(vec4 lightSpacePosition)
{
	vec3 project = lightSpacePosition.xyz / lightSpacePosition.w;
	vec2 uv;
	uv.x = 0.5f * project.x + 0.5f;
	uv.y = 0.5f * project.y + 0.5f;
	float z = 0.5f * project.z + 0.5f;
	float depth = texture(shadowmap, uv).x;
	if(depth < (z + 0.00001f))
	{
		return 0.5f;
	}
	else
	{
		return 1.0f;
	}
}

vec3 calcBumpedNormal()
{
	vec3 normal = normalize(outNormal.xyz);

	vec3 tangent = vec3(0.0f);
	vec3 c1 = cross(normal, vec3(0.0f, 0.0f, 1.0f));
	vec3 c2 = cross(normal, vec3(0.0f, 1.0f, 0.0f));
	if(length(c1) > length(c2))
	{
		tangent = c1;
	}
	else
	{
		tangent = c2;
	}

	tangent = normalize(tangent);
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(tangent, normal);
	vec3 bumpedNormal = texture(texNormal, outUV).xyz;
	bumpedNormal = 2.0f * bumpedNormal - vec3(1.0f, 1.0f, 1.0f);
	vec3 finalNormal;
	mat3 TBN = mat3(tangent, bitangent, normal);
	finalNormal = TBN * bumpedNormal;
	finalNormal = normalize(finalNormal);
	return finalNormal;
}

vec4 calcLightColor(BaseLight light, vec3 lightDir, vec3 normal, float shadowFactor)
{
	vec4 finalColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 diffuseColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 specMap = texture(texSpec, outUV);

	float diffuseFactor = max(0.0f, dot(-lightDir, normal));

	if(diffuseFactor > 0.0f)
	{
		diffuseColor = vec4(light.color * diffuseFactor, 1.0f) * light.intensity;

		vec3 reflection = reflect(lightDir, normal);

		float specularFactor = pow(max(0.0f, dot(viewvec.xyz, reflection)), 25.0f);

		specularColor = specMap * specularFactor;
	}

	finalColor = (shadowFactor * (diffuseColor + specularColor));
	return finalColor;
}

vec4 calcDirectionalLightColor(DirectionalLight light, vec3 normal)
{
	vec4 finalColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	finalColor = calcLightColor(light.base, light.base.direction, normal, 1.0f);

	return finalColor;
}

//vec4 calcSpotLightColor(SpotLight light, vec3 normal)
//{
//	vec4 finalColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

//	vec4 surfaceToLight = normalize(vec4(light.base.position, 0.0f) - outPosInLight);
//	float angle = acos(dot(-surfaceToLight, normalize(vec4(light.base.direction, 0.0f))));
//	float cutOff = radians(clamp(light.cutOff, 0.0f, 90.0));
	
//	if(angle < cutOff)
//	{
//		finalColor = vec4(light.base.color, 1.0f);
//		return finalColor * light.base.intensity;
//	}
//	else
//	{
//		return finalColor;
//	}
//}

vec4 calcAreaLightColor(AreaLight light, vec3 normal)
{
	vec4 finalColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	vec4 surfaceToLight = vposition - vec4(light.base.position, 0.0f);
	float lightDistance = length(surfaceToLight);
	surfaceToLight = normalize(surfaceToLight);
	float shadowFactor = calcShadowFactor(outPosInLight);

	finalColor = calcLightColor(light.base, surfaceToLight.xyz, normal, shadowFactor);

	float attenuation = (light.constantAttenuation) + (light.linearAttenuation * lightDistance) + (light.expAttenuation * pow(lightDistance, 2));

	return finalColor / attenuation;
}