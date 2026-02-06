#version 330 core
out vec4 FragmentColor;


in vec2 vertexTextureCoordinate;

uniform sampler2D containerTexture;
uniform sampler2D smilyTexture;

void main()
{
  FragmentColor = mix(
    texture(containerTexture,vertexTextureCoordinate),
    texture(smilyTexture,vertexTextureCoordinate),
    0.2
  );
}
