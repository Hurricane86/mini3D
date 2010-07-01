float4x4 g_LightViewProjectionMatrix : register(c4); //our world view projection matrix

// SHADOW VERTEX SHADER
void vertexShaderShadow ( in float4 iPos		: POSITION,
						  in float3 iTangent	: NORMAL, 
						  in float3 iBiTangent	: NORMAL, 
						  in float3 iNormal		: NORMAL, 
						  in float2 iTex0		: TEXCOORD0, 
						  out float4 oPos		: POSITION, 
						  out float2 vTex0		: TEXCOORD0)
{
	// calculate the position by multiplying with light view projection matrix
    oPos = mul(iPos, g_LightViewProjectionMatrix);

	// send the depth coordinates to the pixel shader 
    vTex0.xy = oPos.zw;
}

//SHADOW PIXEL SHADER
void pixelShaderShadow( in float4 tex0			: TEXCOORD0,
						in float4 tex1			: TEXCOORD1,
						out float4 oColor		: COLOR)
{
    // calculate the normalized depth (between 0.0 and 1.0) and store this as the pixel value in the render target (shadow map)
	oColor = tex0.x / tex0.y;
}