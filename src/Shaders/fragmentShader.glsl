#version 330 core
out vec4 FragmentColor;
in vec4 vertexColor;
void main()
{
  FragmentColor = vertexColor;
}
