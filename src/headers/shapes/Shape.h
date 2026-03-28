#ifndef SHAPE_H
#define SHAPE_H

#include "Shader.h"

class Shape
{
public:
   virtual void Draw(Shader& shader);
};

#endif // SHAPE_H