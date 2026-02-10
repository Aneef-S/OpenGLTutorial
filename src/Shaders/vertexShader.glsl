#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoordinate;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 vertexTextureCoordinate;

void main()
{
  gl_Position = projection * view * model * vec4(position,1.0);
  vertexTextureCoordinate = vec2(textureCoordinate.x,textureCoordinate.y);
}
