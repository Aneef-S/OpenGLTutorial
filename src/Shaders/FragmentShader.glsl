#version 330 core

struct Material
{
	sampler2D emission;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
	float emissionStrength;

};
//For directional light
struct SpotLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 color;

	float constant;
	float linear;
	float quadratic;

	//For spot Light
	float innerCutOffAngle;
	float outerCutOffAngle;
	vec3 direction;
};


struct DirectionLight 
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 color;
};

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	vec3 color;
};  


#define NR_POINT_LIGHTS 4  

uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirectionLight directionLight;
uniform SpotLight spotLight;
uniform Material material;

uniform vec3 viewPos;

out vec4 FragmentColor;

in vec3 Normal;
in vec3 FragmentPosition;
in vec2 TexCoord;








vec3 CalculateDirectionLight(DirectionLight light,vec3 normal,vec3 viewDirection)
{
	vec3 lightDirection = normalize(-light.direction);

	//Ambient light
	vec3 ambient = light.ambient * texture(material.diffuse,TexCoord).rgb;

	//Calculating the diffuse magnitude
	float diffuseMagnitude = max(dot(normal,lightDirection),0.0);
	vec3 diffuse = 
		diffuseMagnitude * 
		light.diffuse * 
		texture(material.diffuse,TexCoord).rgb *
		light.color
		;

	//Specular light
	vec3 reflectDirection = reflect(-lightDirection,normal);
	float specularMagnitude = pow(max(dot(viewDirection,reflectDirection),0),material.shininess);
	vec3 specular = 
		specularMagnitude *  
		light.specular * 
		texture(material.specular,TexCoord).rgb *
		light.color
		;

	return (ambient + diffuse + specular); 

}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDirection)
{
    vec3 lightDir = normalize(light.position - FragmentPosition);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - FragmentPosition);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoord)) * light.color;
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord)) * light.color;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalculateSpotLight(SpotLight light,vec3 normal,vec3 viewDirection)
{
	float angleFromLight =	dot(
		normalize(light.position - FragmentPosition),
		-normalize(light.direction));
	//Difference between inner and out cutoff;
	float epsilon = light.innerCutOffAngle - light.outerCutOffAngle;
	//Intensity of directional light
	float intensity = clamp((angleFromLight-light.outerCutOffAngle)/epsilon,0.0,1.0);

	if(angleFromLight > light.outerCutOffAngle)
	{
		float distanceFromLight = length(light.position-FragmentPosition);
		float attenuation = 1.0 /
		(
			light.constant +
			light.linear * distanceFromLight +
			light.quadratic * (distanceFromLight * distanceFromLight)
		);

		// Ambient Color
		vec3 ambient = light.ambient * texture( material.diffuse,TexCoord).rgb;
		vec3 emission = 
			texture(material.emission,TexCoord*1.5f).rgb * 
			material.emissionStrength
			;


	
	
		// Diffuse Color
	
		vec3 lightDir = normalize(light.position - FragmentPosition);

		float diff = max(dot(normal, lightDir), 0.0);
		vec3 diffuse = 
			light.diffuse * 
			diff * 
			texture(material.diffuse,TexCoord).rgb * 
			light.color *
			attenuation *
			intensity
			;

		// Specular Color

		vec3 viewDir = normalize(viewPos - FragmentPosition);
		vec3 reflectDir = reflect(-lightDir, normal);

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = 
			light.specular * 
			spec * 
			texture(material.specular,TexCoord).rgb *
			light.color *
			attenuation *
			intensity;
			;


		return(ambient + diffuse + specular + emission) ;
	}
	else
	{
		return (light.ambient * vec3(texture(material.diffuse, TexCoord)));
	}
}




void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDirection = normalize(viewPos-FragmentPosition);

	vec3 result = CalculateDirectionLight(directionLight,normal,viewDirection);

	result += CalculateSpotLight(spotLight,normal,viewDirection);

	for(int i=0;i<NR_POINT_LIGHTS;i++)
	{
		result += CalculatePointLight(pointLights[i],normal,viewDirection);
	}

	FragmentColor = vec4(result,1.0);
	
}
