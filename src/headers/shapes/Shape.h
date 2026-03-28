#ifndef SHAPE_H
#define SHAPE_H

#include "Shader.h"

class Shape
{
public:
   virtual void Draw(const Shader& shader) const = 0;
   virtual void SetTexture(const std::string path) = 0;
};

#endif // SHAPE_H