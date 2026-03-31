#version 330 core
out vec4 FragColor;


in vec3 Normal;
in vec3 FragmentPosition;
in vec2 TexCoord;


struct DirectionLight 
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 color;
};

uniform DirectionLight directionLight[1];
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 viewPos;
uniform samplerCube skybox;

vec4 CalculateDirectionLight(DirectionLight light,vec3 normal,vec3 viewDirection)
{
	vec3 lightDirection = normalize(-light.direction);

	//Ambient light
	vec4 ambient = vec4(light.ambient, 1.0) * texture(texture_diffuse1,TexCoord);

	//Calculating the diffuse magnitude
	float diffuseMagnitude = max(dot(normal,lightDirection),0.0);
	vec4 diffuse = 
		diffuseMagnitude * 
		vec4(light.diffuse, 1.0) * 
		texture(texture_diffuse1,TexCoord) *
		vec4(light.color, 1.0)
		;

	//Specular light
	vec3 reflectDirection = reflect(-lightDirection,normal);
	float specularMagnitude = pow(max(dot(viewDirection,reflectDirection),0),32);
	vec4 specular = 
		specularMagnitude *  
		vec4(light.specular, 1.0) * 
		texture(texture_specular1,TexCoord) *
		vec4(light.color, 1.0)
		;

	return (ambient + diffuse + specular); 

}





void main()
{    
    // vec3 normal = normalize(Normal);
    // vec3 viewDirection = normalize(viewPos-FragmentPosition);
    // vec4 result = CalculateDirectionLight(directionLight[0],normal,viewDirection);
	// FragColor = result;

	vec3 I = normalize(FragmentPosition - viewPos);
    vec3 R = reflect(I, normalize(Normal));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
   

}