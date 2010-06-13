
float4x4 ModelViewProj : register(c0); //our world view projection matrix

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
    float4 normal	  : NORMAL;
    float2 tex0       : TEXCOORD0;
};

//vertex to pixel shader structure
struct v2p
{
    float4 position    : POSITION;
    float4 normal      : TEXCOORD1;
    float2 tex0        : TEXCOORD0;
};

//pixel shader to screen
struct p2f
{
    float4 color    : COLOR0;
}; 

// Shadow Vertex Shader
void vs( in a2v IN, out v2p OUT ) 
{
    //getting to position to object space
    OUT.position = mul(IN.position, ModelViewProj);
 	OUT.normal = IN.normal;
    OUT.tex0.xy = OUT.position.zw;
}

//SHADOW PIXEL SHADER
void ps( in v2p IN, out p2f OUT )
{
    //
    // Depth is z / w
    //
    OUT.color = IN.tex0.x / IN.tex0.y;
}