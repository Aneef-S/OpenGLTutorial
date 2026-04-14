#version 330 core
out vec4 FragColor;


uniform sampler2D texture_diffuse1;


in GS_OUT {
	vec2 TexCoords;
} fs_in;



void main()
{    
   FragColor = texture(texture_diffuse1, fs_in.TexCoords);
}