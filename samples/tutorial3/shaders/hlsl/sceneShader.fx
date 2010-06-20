
// Global variables (set by the application)
float4x4 g_ModelViewProj		: register(c0); //our world view projection matrix
float4x4 g_LightModelViewProj	: register(c4); // the transform from model space to light view
float3 g_lightPosition			: register(c8); //Normal direction of light source
float3 g_camPosition			: register(c9); // Positon of the camera.

#define AMBIENT_INTENSITY 0.5f

sampler2D texSampler : register(s0) = sampler_state
{
    Texture	  = <apa>;
    MIPFILTER = GAUSSIAN;
    MAGFILTER = GAUSSIAN;
    MINFILTER = GAUSSIAN;
};

sampler2D normalSampler : register(s2) = sampler_state
{
    Texture	  = <apa>;
    MIPFILTER = GAUSSIAN;
    MAGFILTER = GAUSSIAN;
    MINFILTER = GAUSSIAN;
};

sampler2D shadowTexSampler : register(s1) = sampler_state
{
    Texture	  = <banan>;
    MIPFILTER = GAUSSIAN;
    MAGFILTER = GAUSSIAN;
    MINFILTER = GAUSSIAN;
};

//VERTEX SHADER
void vertexShaderScene( 
						in float4 iPosition						: POSITION0,
						in float3 iTangent						: NORMAL0, 
						in float3 iBitangent					: NORMAL1, 
						in float3 iNormal						: NORMAL2, 
						in float2 iTex0							: TEXCOORD0,

						out float2 tex0							: TEXCOORD0,
						out float4 shadowMapCoordinates			: TEXCOORD1,

						out float3 lightPos						: TEXCOORD2,
						out float3 pos							: TEXCOORD3,
						out float3 cameraPos					: TEXCOORD4,
						
						out float3 tangent						: TEXCOORD5, 
						out float3 bitangent					: TEXCOORD6, 
						out float3 normal						: TEXCOORD7,
						
						out float4 oPos							: POSITION)

{
    //getting to position to object space
    oPos = mul(iPosition, g_ModelViewProj);
	
	tex0 = iTex0;

	// calculate shadow map texture coordinates
	shadowMapCoordinates = mul(iPosition, g_LightModelViewProj);

 	pos = iPosition.xyz;
	cameraPos = g_camPosition;
	lightPos = g_lightPosition;

	normal = iNormal;
	tangent = iTangent;
	bitangent = iBitangent;

}
 
//PIXEL SHADER
void pixelShaderScene(	
						in float2 tex0							: TEXCOORD0,
						in float4 shadowMapCoordinates			: TEXCOORD1,
						
						in float3 lightPos						: TEXCOORD2,
						in float3 pos							: TEXCOORD3,
						in float3 cameraPos						: TEXCOORD4,

						in float3 tangent						: TEXCOORD5, 
						in float3 bitangent						: TEXCOORD6, 
						in float3 normal						: TEXCOORD7,

						out float4 oColor						: COLOR)
{
	float3 localLightNormal = normalize(lightPos - pos);
	
	// Calculate the normal from the normal map	
	float3 dot3Color = tex2D(normalSampler, tex0).rgb;
	dot3Color.r = (dot3Color.r - 0.5f ) * 2.0f;
	dot3Color.g = (dot3Color.g - 0.5f ) * 2.0f;
	float3 dot3Normal = dot3Color.r * tangent + dot3Color.g * bitangent + dot3Color.b * normal; 
	
	// Diffuse light intensity
	float diffuseIntensity = dot(dot3Normal, localLightNormal);

	if (diffuseIntensity < 0)
		diffuseIntensity = 0;

	// specular light intensity
	float3 reflectionVector = 2 * diffuseIntensity * dot3Normal - localLightNormal;
	float3 viewVector = normalize(cameraPos - pos);
	float specularIntensity = dot(reflectionVector, viewVector);


	if (specularIntensity < 0)
		specularIntensity = 0;
	else
		specularIntensity = pow(specularIntensity, 2.0f);

	float4 shadowMapTextureCoordinates = shadowMapCoordinates / shadowMapCoordinates.w;
	shadowMapTextureCoordinates.xy = shadowMapTextureCoordinates.xy * 0.5f + 0.5f;
	shadowMapTextureCoordinates.y = 1.0f - shadowMapTextureCoordinates.y;

	//Get the shadow intensity from the shadow map using the shadow map texture coordinates from the vertex shader
	float shadowIntensity = tex2D(shadowTexSampler, shadowMapTextureCoordinates.xy) + 0.00005f < shadowMapTextureCoordinates.z ? 0.0f : 1.0f;

	oColor.argb = 1.0;
	oColor.rgb = tex2D(texSampler, tex0).rgb * (AMBIENT_INTENSITY + 0.4f * diffuseIntensity * shadowIntensity) + 0.5f * specularIntensity * shadowIntensity + 0.5f * specularIntensity * shadowIntensity;

}

