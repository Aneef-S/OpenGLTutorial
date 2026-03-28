#ifndef CUBE_H
#define CUBE_H

#include "stb_image.h"
#include <glad/glad.h>
#include "Shader.h"
#include "Texture.hpp"

class Cube
{
private:
    unsigned int VAO, VBO;
    unsigned int textureID;
    unsigned int TextureFromFile(const std::string &path);

public:
    Cube();
    void SetTexture(const std::string path);
    void Draw(const Shader &shader) const;
};

#endif