

float4x4 ModelViewProj : register(c0);//our world view projection matrix
float4x4 LightModelViewProj : register(c4); // the transform from model space to light view
float4 lightNormal : register(c8); //Normal direction of light source

sampler2D texSampler : register(s0) = sampler_state
{
    Texture	  = <apa>;
    MIPFILTER = LINEAR;
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
};

sampler2D shadowTexSampler : register(s1) = sampler_state
{
    Texture	  = <banan>;
    MIPFILTER = LINEAR;
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
};

//application to vertex structure
struct a2v
{ 
    float4 position   : POSITION;
    float4 normal	  : NORMAL;
    float2 tex0       : TEXCOORD0;
};

//vertex to pixel shader structure
struct v2p
{
    float4 position    : POSITION;
    float2 tex0        : TEXCOORD0;
    float4 tex1        : TEXCOORD1;
    float4 normal      : TEXCOORD2;
    float4 lNormal     : TEXCOORD3;
};
 
//pixel shader to screen
struct p2f
{
    float4 color    : COLOR0;
};
 
//VERTEX SHADER
void vs( in a2v IN, out v2p OUT ) 
{
    //getting to position to object space
    OUT.position = mul(IN.position, ModelViewProj);
 	OUT.normal = IN.normal;
    OUT.tex0 = IN.tex0;
	OUT.tex1 = mul(IN.position, LightModelViewProj);
	OUT.lNormal = lightNormal;
}
 
//PIXEL SHADER
void ps( in v2p IN, out p2f OUT )
{
	if (dot(IN.normal.xyz, IN.lNormal.xyz) > 0.0)
	{
		float3 localNormal = normalize(IN.normal.xyz + (tex2D(texSampler, IN.tex0).rgb - 0.5) * 0.5);
		float lightIntensity = dot(localNormal, IN.lNormal.xyz);

		//transform from RT space to texture space.
		float2 ShadowTexC = 0.5 * IN.tex1.xy / IN.tex1.w + float2( 0.5, 0.5 );
		ShadowTexC.y = 1.0 - ShadowTexC.y;

		float shadowColor = tex2D(shadowTexSampler, ShadowTexC) + 0.00005 <  IN.tex1.z / IN.tex1.w ? 0.0 : 1.0;

		//get the color from the texture
		OUT.color = tex2D(texSampler, IN.tex0);
		OUT.color.rgb *= (shadowColor * lightIntensity * 0.5) + 0.5;  // (shadowColor * lightIntensity)
	}
	else
	{
		OUT.color = tex2D(texSampler, IN.tex0);
		OUT.color.rgb *= 0.5;
	}
}
 
technique water
{
    pass p0
    {
        vertexshader = compile vs_2_0 vs();
        pixelshader = compile ps_2_0 ps();
    }
} 