#version 330 core


uniform vec3 color;

out vec4 FragmentColor;

void main()
{
	FragmentColor = vec4(color,1.0f);
}