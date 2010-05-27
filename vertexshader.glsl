uniform mat4 modelViewProj;
uniform sampler2D tex;

attribute vec4 inPosition;
attribute vec4 inColor;
attribute vec2 inTexCoords;

varying vec4 color;
varying vec2 texCoords;

void main()
{
    gl_Position = modelViewProj * inPosition;
	
	color = inColor;
	texCoords = inTexCoords;
}
 