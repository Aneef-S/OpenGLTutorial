
#ifndef PLANE_H
#define PLANE_H

#include <string>
#include <Shader.h>

#include "Utils.h"
#include "Shape.h"

class Plane : public Shape
{
private:
    unsigned int VAO, VBO;
    unsigned int textureID;
public:
    Plane();
    void SetTexture(const std::string path) override;
    void Draw(const Shader &shader) const override;
};


#endif