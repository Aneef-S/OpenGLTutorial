
#ifndef UTILS_H
#define UTILS_H


#include <string>
#include <iostream>
#include <vector>
#include "stb_image.h"
#include <glad/glad.h>

class Utils
{
public:
    static unsigned int TextureFromFile(const std::string &path);
    static unsigned int LoadCubemap(const std::vector<std::string> &faces);
};



#endif // UTILS_H