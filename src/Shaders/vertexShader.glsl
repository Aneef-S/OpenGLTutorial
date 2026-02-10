#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 textureCoordinate;

uniform mat4 transform;

out vec2 vertexTextureCoordinate;

void main()
{
  gl_Position = transform * vec4(position,1.0);
  vertexTextureCoordinate = vec2(textureCoordinate.x,textureCoordinate.y);
}
