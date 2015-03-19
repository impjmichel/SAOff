#version 410 compatibility

uniform sampler2D texture;
in vec2 textureCoord;

out vec4 outputColor;

void main()
{
	outputColor = texture2D(texture, textureCoord);
}