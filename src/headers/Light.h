#ifndef LIGHT_H
#define LIGHT_H


#include <glm/glm.hpp>

//Virtual class for all lights
struct Light
{
    glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
    glm::vec3 color;

   virtual void AddToShader(Shader& shader, const std::string& name) const
    {
        shader.setVec3f(name + ".ambient", ambient);
        shader.setVec3f(name + ".diffuse", diffuse);
        shader.setVec3f(name + ".specular", specular);
        shader.setVec3f(name + ".color", color);
    }
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

    void AddToShader(Shader& shader, const std::string& name) const override
    {
        std::string fullname = "directionLight[" + name + "]";
        Light::AddToShader(shader, fullname);
        shader.setVec3f(fullname + ".direction", direction);
    }
	
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

#endif // LIGHT_H