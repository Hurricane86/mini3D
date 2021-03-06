
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


// ---------- BASIC OPENGL VERTEX SHADER --------------------------------------
char* vertexShaderGLSL =
(char*)
"\
uniform mat4 modelViewProj;									\
uniform sampler2D texture0;									\
															\
attribute vec4 IN_position;									\
attribute vec4 IN_color;									\
attribute vec2 IN_texCoords;								\
															\
varying vec4 PS_color;										\
varying vec2 PS_texCoords;									\
															\
void main()													\
{															\
	gl_Position = modelViewProj * IN_position;				\
	PS_color = IN_color;									\
	PS_texCoords = IN_texCoords;							\
}															\
"; 

char* vertexShaderGLSL_v2 = (char*)
"												\
in  vec3 in_Position;							\
in  vec4 in_Color;								\
out vec4 ex_Color;								\
												\
void main(void)									\
{												\
	gl_Position = vec4(in_Position, 1.0);		\
	ex_Color = in_Color;						\
}												\
";


// ---------- BASIC OPENGL PIXEL SHADER ---------------------------------------
char* pixelShaderGLSL = 
(char*)
"\
uniform sampler2D texture0;									\
															\
varying vec4 PS_color;										\
varying vec2 PS_texCoords;									\
															\
void main()													\
{															\
	gl_FragColor = PS_color * texture2D(texture0, PS_texCoords);\
}															\
";

char* pixelShaderGLSL_v2 = 
	(char*)
"															\
in  vec4 ex_Color;											\
out vec4 out_Color;											\
															\
void main(void)												\
{															\
	out_Color = ex_Color;									\
}															\
";


// ---------- BASIC DIRECTX VERTEX SHADER --------------------------------------
char* vertexShaderHLSL =
(char*)
"\
float4x4 modelViewProj : register(c0);						\
texture texture0;											\
															\
struct data													\
{															\
    float4 position   : POSITION;							\
    float4 color	  : COLOR;								\
    float2 tex0       : TEXCOORD0;							\
};															\
															\
void main( in data IN, out data PS )						\
{															\
    PS.position = mul(IN.position, modelViewProj);			\
 	PS.color = IN.color;									\
    PS.tex0 = IN.tex0;										\
}															\
"; 


// ---------- BASIC DIRECTX PIXEL SHADER ---------------------------------------
char* pixelShaderHLSL = 
(char*)
"\
sampler2D texture0 : register(s0);							\
															\
struct v2p													\
{															\
    float4 position    : POSITION;							\
    float4 color       : COLOR;								\
    float2 tex0        : TEXCOORD0;							\
};															\
															\
void main( in v2p PS, out float4 pixel_color : COLOR0 )		\
{															\
	pixel_color =  PS.color * tex2D(texture0, PS.tex0);		\
}															\
";

