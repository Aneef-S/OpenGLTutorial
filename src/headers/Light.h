
#include <glm/glm.hpp>

//Virtual class for all lights
struct Light
{
    glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
    glm::vec3 color;
};

//Virtual class for all attenuating light;
struct AttenuatingLight : public Light
{ 
    float constant;
    float linear;
    float quadratic;

};


struct DirectionLight : public Light
{
	glm::vec3 direction;
	
};


struct PointLight : public AttenuatingLight
{
    glm::vec3 position;

  
};

struct SpotLight : public AttenuatingLight
{
    glm::vec3 position;
    glm::vec3 direction;

    float innterCutOffAngle;
    float outerCutOffAngle;
    
};