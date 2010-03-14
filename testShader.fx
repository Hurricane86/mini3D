float4x4 ModelViewProj : register(c0); //our world view projection matrix
texture texture0;
 
sampler2D texSampler : register(s0) = sampler_state
{
    Texture	  = <texture0>;
    MIPFILTER = LINEAR;
    MAGFILTER = LINEAR;
    MINFILTER = LINEAR;
};

//application to vertex structure
struct a2v
{ 
    float4 position   : POSITION;
    int1   color	    : COLOR;
    float2 tex0       : TEXCOORD0;
};

//vertex to pixel shader structure
struct v2p
{
    float4 position    : POSITION;
    int1   color       : COLOR;
    float2 tex0        : TEXCOORD0;
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
    OUT.position    = mul(float4(IN.position.xyz, 1.0), ModelViewProj);
 	OUT.color = IN.color;
    OUT.tex0 = IN.tex0;
}
 
//PIXEL SHADER
void ps( in v2p IN, out p2f OUT )
{
    //get the color from the texture
	OUT.color =  IN.color * tex2D(texSampler, IN.tex0);
	
}
 
technique water
{
    pass p0
    {
        vertexshader = compile vs_1_1 vs();
        pixelshader = compile ps_2_0 ps();
    }
} 