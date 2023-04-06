#version 460 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D tex0;


void main()
{
	FragColor = texture(tex0, TexCoord);
}