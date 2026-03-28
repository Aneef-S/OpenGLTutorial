#ifndef ENTITY_H
#define ENTITY_H

#include "Model.h"
#include "Shader.h"
#include "Shape.h"


enum class EntityType
{
    Model,
    Shape
};

/// @brief An independet object that can be drawn. Includes Model and transformation.
class Entity
{
public:
    EntityType type;
    Model* model;
    Shape* shape;
    glm::mat4 transform = glm::mat4(1.0f);
    Entity(Shape &_shape) :
        shape(&_shape), type(EntityType::Shape){}   
    Entity(Shape &_shape,glm::mat4 _transform):
        transform(_transform), shape(&_shape), type(EntityType::Shape){}
    Entity(Model &_model) : 
        model(&_model), type(EntityType::Model){}
    Entity(Model &_model,glm::mat4 _transform): 
        transform(_transform), model(&_model), type(EntityType::Model){}
    void Draw(Shader &shader);
};



#endif