#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform float opacity;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	FragColor = mix(texture(texture1,TexCoord),texture(texture2,TexCoord),opacity);
	//FragColor = texture(texture2, TexCoord) * vec4(ourColor,1.0f) * opacity;
}