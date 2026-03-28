
#ifndef UTILS_H
#define UTILS_H


#include <string>
#include <iostream>
#include "stb_image.h"
#include <glad/glad.h>

class Utils
{
public:
    static unsigned int TextureFromFile(const std::string &path);
};



#endif // UTILS_H