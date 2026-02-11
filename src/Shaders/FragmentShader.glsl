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
struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec4 color;

	float attenuationConstant;
	float attenuationConstantLinear;
	float attenuationConsantQuadartic;

	//For spot Light
	float cutOffAngle;
	float outerCutOffAngle;
	vec3 direction;
};



out vec4 FragmentColor;

in vec3 Normal;
in vec3 FragmentPosition;
in vec2 TexCoord;



uniform vec3 viewPos;
uniform Material material;
uniform Light light;






void main()
{
	
	float angleFromLight =	dot(
		normalize(light.position - FragmentPosition),
		-normalize(light.direction));
	//Difference between inner and out cutoff;
	float epsilon = light.cutOffAngle - light.outerCutOffAngle;
	//Intensity of directional light
	float intensity = clamp((angleFromLight-light.outerCutOffAngle)/epsilon,0.0,1.0);

	if(angleFromLight > light.outerCutOffAngle)
	{
		float distanceFromLight = length(light.position-FragmentPosition);
		float attenuation = 1.0 /
		(
			light.attenuationConstant + 
			light.attenuationConstantLinear * distanceFromLight + 
			light.attenuationConsantQuadartic * distanceFromLight + distanceFromLight
		);

		// Ambient Color
		vec3 ambient = light.ambient * texture( material.diffuse,TexCoord).rgb;
		vec3 emission = 
			texture(material.emission,TexCoord*1.5f).rgb * 
			material.emissionStrength * 
			attenuation
			;


	
	
		// Diffuse Color
		vec3 norm = normalize(Normal);
		vec3 lightDir = normalize(light.position - FragmentPosition);

		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = 
			light.diffuse * 
			diff * 
			texture(material.diffuse,TexCoord).rgb * 
			light.color.rgb *
			attenuation *
			intensity
			;

		// Specular Color

		vec3 viewDir = normalize(viewPos - FragmentPosition);
		vec3 reflectDir = reflect(-lightDir, norm);

		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = 
			light.specular * 
			spec * 
			texture(material.specular,TexCoord).rgb *
			light.color.rgb *
			attenuation *
			intensity;
			;


		vec4 resultColor = vec4((ambient + diffuse + specular + emission) ,1.0f) ;
		FragmentColor =   resultColor ;
	}
	else
	{
		FragmentColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoord)), 1.0);
	}
}
