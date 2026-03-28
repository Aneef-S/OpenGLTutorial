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

vec3 CalculateDirectionLight(DirectionLight light,vec3 normal,vec3 viewDirection)
{
	vec3 lightDirection = normalize(-light.direction);

	//Ambient light
	vec3 ambient = light.ambient * texture(texture_diffuse1,TexCoord).rgb;

	//Calculating the diffuse magnitude
	float diffuseMagnitude = max(dot(normal,lightDirection),0.0);
	vec3 diffuse = 
		diffuseMagnitude * 
		light.diffuse * 
		texture(texture_diffuse1,TexCoord).rgb *
		light.color
		;

	//Specular light
	vec3 reflectDirection = reflect(-lightDirection,normal);
	float specularMagnitude = pow(max(dot(viewDirection,reflectDirection),0),32);
	vec3 specular = 
		specularMagnitude *  
		light.specular * 
		texture(texture_specular1,TexCoord).rgb *
		light.color
		;

	return (ambient + diffuse + specular); 

}



uniform vec3 viewPos;


float near = 0.1f;
float far = 100.0f;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // Back to NDC 
	return (2.0 * near * far) / (far + near - z * (far - near));	
}


void main()
{    
    vec3 normal = normalize(Normal);
    vec3 viewDirection = normalize(viewPos-FragmentPosition);
    vec3 result = CalculateDirectionLight(directionLight[0],normal,viewDirection);
    FragColor = vec4(result, 1.0);
	// float depthValue = LinearizeDepth(gl_FragCoord.z) / far; // Divide by far for demonstration
	// FragColor = vec4(vec3(depthValue), 1.0);
}