uniform sampler2D tex;

varying vec4 color;
varying vec2 texCoords;

void main()
{
	gl_FragColor = color * texture2D(tex, texCoords);
}
