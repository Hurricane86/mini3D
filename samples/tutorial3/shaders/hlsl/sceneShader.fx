
// Global variables (set by the application)
float4x4 g_ModelViewProj		: register(c0); //our world view projection matrix
float4x4 g_LightModelViewProj	: register(c4); // the transform from model space to light view
float4 g_lightNormal			: register(c8); //Normal direction of light source
float4 g_camPosition			: register(c9); // Posiiton of the camera.

#define AMBIENT_INTENSITY 0.5f

sampler2D texSampler : register(s0) = sampler_state
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
void vertexShaderScene( in float4 iPosition						: POSITION,
						in float4 iSurfaceNormal				: NORMAL,
						in float2 iTex0							: TEXCOORD0,

						out float4 oPos							: POSITION,
						out float2 tex0							: TEXCOORD0,
						out float4 shadowMapCoordinates			: TEXCOORD1,
						out float4 surfaceNormal				: TEXCOORD2,
						out float4 lightNormal					: TEXCOORD3,
						out float4 vPos							: TEXCOORD4,
						out float4 vCameraPos					: TEXCOORD5)
{
    //getting to position to object space
    oPos = mul(iPosition, g_ModelViewProj);
	surfaceNormal = iSurfaceNormal;
    tex0 = iTex0;
 	vPos = iPosition;
	vCameraPos = g_camPosition;

	// calculate shadow map texture coordinates
	shadowMapCoordinates = mul(iPosition, g_LightModelViewProj);

	lightNormal = g_lightNormal;
}
 
//PIXEL SHADER
void pixelShaderScene(	in float2 tex0							: TEXCOORD0,
						in float4 shadowMapCoordinates			: TEXCOORD1,
						in float4 surfaceNormal					: TEXCOORD2,
						in float4 lightNormal					: TEXCOORD3,
						in float4 pos							: TEXCOORD4,
						in float4 cameraPos						: TEXCOORD5,

						out float4 oColor						: COLOR)

{
	float3 localLightNormal = normalize(lightNormal.xyz - pos.xyz);
	float diffuseIntensity = dot(surfaceNormal.xyz, localLightNormal.xyz);
	
	float3 reflectionVector = 2 * diffuseIntensity * surfaceNormal.xyz - localLightNormal.xyz;
	float3 viewVector = normalize(cameraPos.xyz - pos.xyz);
	float specularIntensity = dot(reflectionVector.xyz, viewVector.xyz);
	
	if (diffuseIntensity < 0)
		diffuseIntensity = 0;

	if (specularIntensity < 0)
		specularIntensity = 0;
	else
		specularIntensity = pow(specularIntensity, 10.0f);

//	if (diffuseIntensity >= 0.0f)
//	{
		//float4 localNormal = normalize(surfaceNormal + (tex2D(texSampler, tex0) - 0.5f) * 0.5f);
		//diffuseIntensity = dot(localNormal, lightNormal);

		float4 shadowMapTextureCoordinates = shadowMapCoordinates / shadowMapCoordinates.w;
		shadowMapTextureCoordinates.xy = shadowMapTextureCoordinates.xy * 0.5f + 0.5f;
		shadowMapTextureCoordinates.y = 1.0f - shadowMapTextureCoordinates.y;

		//Get the shadow intensity from the shadow map using the shadow map texture coordinates from the vertex shader
		float shadowIntensity = tex2D(shadowTexSampler, shadowMapTextureCoordinates.xy) + 0.00005f < shadowMapTextureCoordinates.z ? 0.0f : 1.0f;

		//get the color from the texture
		//float totalIntensity = AMBIENT_INTENSITY + (0.4f * diffuseIntensity + specularIntensity);
		oColor.a = 1.0;
		oColor.rgb = tex2D(texSampler, tex0).rgb * (AMBIENT_INTENSITY + 0.4f * diffuseIntensity * shadowIntensity) + 0.5f * specularIntensity * shadowIntensity; // * shadowIntensity;  

//	}
//	else
//	{
		// get the texture color and multiply with ambient level
//		oColor = tex2D(texSampler, tex0) * float4(AMBIENT_INTENSITY, AMBIENT_INTENSITY, AMBIENT_INTENSITY, 1.0f);
//	}
}