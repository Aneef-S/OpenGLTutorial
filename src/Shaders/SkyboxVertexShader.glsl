#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

layout (std140) uniform Matrices
{
    mat4 view;
    mat4 projection;
};

void main()
{
    TexCoords = aPos;
    mat4 viewNoTranslation = mat4(mat3(view));
    vec4 pos = projection * viewNoTranslation * vec4(aPos, 1.0);
    gl_Position = pos.xyww; // Set w component to the depth value, so that the skybox is always rendered at the farthest depth
}