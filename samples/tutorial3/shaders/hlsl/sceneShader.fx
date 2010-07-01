
// Global variables (set by the application)
float4x4 g_ModelViewProj		: register(c0); //our world view projection matrix
float4x4 g_LightModelViewProj	: register(c4); // the transform from model space to light view
float3 g_lightPosition			: register(c8); //Normal direction of light source
float3 g_camPosition			: register(c9); // Positon of the camera.

#define AMBIENT_INTENSITY 0.5f

sampler2D texSampler : register(s0);
sampler2D normalSampler : register(s2);
sampler2D shadowTextureSampler : register(s1);



// ----- SCENE VERTEX SHADER --------------------------------------------------

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
    //Transform the position to view space
    oPos = mul(iPosition, g_ModelViewProj);
	
	// Pass through texture coordinates
	tex0 = iTex0;

	// calculate shadow map texture coordinates
	shadowMapCoordinates = mul(iPosition, g_LightModelViewProj);

	// Pass through other variables
 	pos = iPosition.xyz;
	cameraPos = g_camPosition;
	lightPos = g_lightPosition;

	normal = iNormal;
	tangent = iTangent;
	bitangent = iBitangent;

}

 
// ----- SCENE PIXEL SHADER ---------------------------------------------------

void pixelShaderScene(	
						in float2 tex0							: TEXCOORD0,
						in float4 shadowMapCoordinates			: TEXCOORD1,
						
						in float3 lightPos						: TEXCOORD2,
						in float3 pos					 		: TEXCOORD3,
						in float3 cameraPos						: TEXCOORD4,

						in float3 tangent						: TEXCOORD5, 
						in float3 bitangent						: TEXCOORD6, 
						in float3 normal						: TEXCOORD7,

						out float4 oColor						: COLOR)
{
	// Find the light normal at the current pixel world coordinates
	float3 localLightNormal = normalize(lightPos - pos);
	
	// Calculate the normal from the normal map	
	float3 dot3Color = tex2D(normalSampler, tex0).rgb;
	dot3Color.rg = (dot3Color.rg - 0.5f ) * float2(1.0f, -1.0f);
	float3 dot3Normal = dot3Color.r * tangent + dot3Color.g * bitangent + dot3Color.b * normal; 
	
	// Diffuse light intensity
	float diffuseIntensity = dot(dot3Normal, localLightNormal);

	// Specular light intensity
	float3 reflectionVector = 2 * diffuseIntensity * dot3Normal - localLightNormal;
	float3 viewVector = normalize(cameraPos - pos);
	float specularIntensity = dot(reflectionVector, viewVector);

	// Shadow intensity
	shadowMapCoordinates /= shadowMapCoordinates.w;
	shadowMapCoordinates.xy = shadowMapCoordinates.xy * float2(0.5f, -0.5f) + 0.5f;
	float shadowIntensity = shadowMapCoordinates.z - 0.00005f > tex2D(shadowTextureSampler, shadowMapCoordinates.xy).x ? 0.0f : 1.0f;

	// Clamp light levels
	specularIntensity = pow(specularIntensity, 5.0f);
	specularIntensity = clamp(specularIntensity, 0.0f, 1.0f);
	diffuseIntensity = clamp(diffuseIntensity, 0.0f, 1.0f);
	shadowIntensity = clamp(shadowIntensity, 0.0f, 1.0f);

	// Set output color
	oColor = tex2D(texSampler, tex0) * (AMBIENT_INTENSITY + shadowIntensity * (0.5 * specularIntensity + 0.5 * diffuseIntensity));
	oColor.a = 1;
}


// ----- SHADOW VERTEX SHADER -------------------------------------------------

void vertexShaderShadow ( in float4 iPos		: POSITION,
						  in float3 iTangent	: NORMAL, 
						  in float3 iBiTangent	: NORMAL, 
						  in float3 iNormal		: NORMAL, 
						  in float2 iTex0		: TEXCOORD0, 
						  out float4 oPos		: POSITION, 
						  out float2 vTex0		: TEXCOORD0)
{
	// calculate the position by multiplying with light view projection matrix
    oPos = mul(iPos, g_LightModelViewProj);

	// send the depth coordinates to the pixel shader 
    vTex0.xy = oPos.zw;
}


// ----- SHADOW PIXEL SHADER --------------------------------------------------

void pixelShaderShadow( in float2 tex0			: TEXCOORD0,
						out float4 oColor		: COLOR)
{
    // calculate the normalized depth (between 0.0 and 1.0) and store this as the pixel value in the render target (shadow map)
	oColor = tex0.x / tex0.y;
}