#include "Entity.h"

void Entity::Draw(Shader &shader)
{
    switch (type)
    {
    case EntityType::Shape:
        shape->Draw(shader);
        break;
    case EntityType::Model:
        model->Draw(shader);
        break;
    default:
        break;
    }
}
