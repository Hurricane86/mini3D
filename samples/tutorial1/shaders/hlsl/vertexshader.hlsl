float4x4 ModelViewProj : register(c0); //our world view projection matrix
texture texture0;

//DATA STRUCTURE FOR IN AND OUT ARGUMENTS
struct data
{ 
    float4 position   : POSITION;
    float4 color	  : COLOR;
    float2 tex0       : TEXCOORD0;
};

//VERTEX SHADER
void main( in data IN, out data OUT ) 
{
    //getting to position to object space
    OUT.position = mul(IN.position, ModelViewProj);
 	OUT.color = IN.color;
    OUT.tex0 = IN.tex0;
}
