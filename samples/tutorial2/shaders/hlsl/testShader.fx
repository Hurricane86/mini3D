float4x4 ModelViewProj			: register(c0); //our world view projection matrix
float4x4 WindowModelViewProj	: register(c4); //The world view projection for the window
float4 CameraPosition			: register(c8); //Position of the camera in world coordinates
 
sampler2D texSampler : register(s0);


//VERTEX SHADER
void vs(	in float4 iPosition			: POSITION,
			in float4 iColor			: COLOR,
			in float2 iTex				: TEXCOORD0,
			out float4 oPosition		: POSITION,
			out float2 tex0				: TEXCOORD0,
			out float4 color			: COLOR) 
{
    //getting to position to object space
    oPosition = mul(iPosition, ModelViewProj);
 	color = iColor;
	tex0 = iTex;
}
 
//PIXEL SHADER
void ps(	in float4 color			: COLOR,
			in float2 tex0			: TEXCOORD0,
			out float4 oColor		: COLOR)
{
    //get the color from the texture
	oColor =  color;
}

// Window VERTEX SHADER
void windowvs(	in float4 iPosition			: POSITION,
				in float4 iColor			: COLOR,
				in float2 iTex				: TEXCOORD0,
				out float4 oPosition		: POSITION,
				out float4 color			: COLOR,
				out float2 tex0				: TEXCOORD0,
				out float4 position			: TEXCOORD1,
				out float4 cameraPosition	: TEXCOORD2) 
{
    //getting to position to object space
    oPosition = mul(iPosition, ModelViewProj);
 	color = 0.0; //iColor;
    tex0 = iTex;
	position = oPosition;
}
 
//PIXEL SHADER
void windowps(	in float4 color				: COLOR,
				in float2 tex0				: TEXCOORD0,
				in float4 position			: TEXCOORD1,
				in float4 cameraPosition	: TEXCOORD2,
				out float4 oColor			: COLOR)
{
    //get the color from the texture
	
	float3 viewDirection = normalize(cameraPosition.xyz - position.xyz);
	float xangle = dot(viewDirection, float3(1,0,0));
	float yangle = dot(viewDirection, float3(0,1,0));
	
	float2 screenCoordinates = position.xy / position.w * float2(0.5, -0.5);
	float2 textureCoordinates = tex0 + screenCoordinates + 0.5;

	// Give it a blue tint
	oColor =  float4(0.9,1.2,1.5,1) * tex2D(texSampler, textureCoordinates);
}
  