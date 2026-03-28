#ifndef CUBE_H
#define CUBE_H


#include <glad/glad.h>
#include "Shader.h"
#include "Texture.hpp"
#include "Utils.h"
#include "Shape.h"


class Cube : public Shape
{
private:
    unsigned int VAO, VBO;
    unsigned int textureID;
    

public:
    Cube();
    void SetTexture(const std::string path) override;
    void Draw(const Shader & shader) const override;
};

#endif