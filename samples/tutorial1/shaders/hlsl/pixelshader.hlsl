sampler2D texSampler : register(s0);

//vertex to pixel shader structure
struct v2p
{
    float4 position    : POSITION;
    float4 color       : COLOR;
    float2 tex0        : TEXCOORD0;
};
 
//pixel shader to screen
struct p2f
{
    float4 color    : COLOR0;
};
 
//PIXEL SHADER
void main( in v2p IN, out p2f OUT )
{
    //get the color from the texture
	OUT.color =  IN.color * tex2D(texSampler, IN.tex0);
	
}
 