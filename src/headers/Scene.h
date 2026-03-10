#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Entity.h"
#include "Camera.hpp"
#include "Light.h"
#include <memory>

class Scene
{
private:
public:
    std::vector<Entity> entities;
    Camera camera;
    // std::vector<std::shared_ptr<Light>> lights;

    std::vector<DirectionLight> dirLights;
    std::vector<PointLight> pointLights;
    std::vector<SpotLight> spotLights;
    
    
};


#endif