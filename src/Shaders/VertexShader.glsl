#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 Normal;
out vec3 FragmentPosition;
out vec2 TexCoord;

void main()
{
	gl_Position = projection * view * model * vec4(vertexPosition,1.0f);
	FragmentPosition = vec3(model * vec4(vertexPosition, 1.0f));
	Normal = vertexNormal;
	TexCoord = vertexTexCoord;

}
