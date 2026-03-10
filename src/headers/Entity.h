#ifndef ENTITY_H
#define ENTITY_H

#include "Model.h"
#include "Shader.h"


/// @brief An independet object that can be drawn. Includes Model and transformation.
class Entity
{
public:
    Model* model;
    glm::mat4 transform = glm::mat4(1.0f);
    Entity(Model &_model) : model(&_model){}
    Entity(Model &_model,glm::mat4 _transform): transform(_transform), model(&_model){}
    void Draw(Shader &shader);
};



#endif