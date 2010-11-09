
// Copyright (c) <2010> Daniel Peterson
// This file is part of mini3d <www.mini3d.org>
// It is distributed under the MIT Software License <www.mini3d.org/license>


// ---------- BASIC OPENGL VERTEX SHADER --------------------------------------
char* vertexShaderGLSL =

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


// ---------- BASIC OPENGL PIXEL SHADER ---------------------------------------
char* pixelShaderGLSL = 

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


// ---------- BASIC DIRECTX VERTEX SHADER --------------------------------------
char* vertexShaderHLSL =

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
	pixel_color =  PS.color * tex2D(texture0, IN.tex0);		\
}															\
";

