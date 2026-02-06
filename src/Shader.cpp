#include <fstream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <glad/glad.h>
#include <iostream>
#include "Shader.h"

std::string readFileToString(const std::string& path);

Shader::Shader(const char *vertexShaderFilePath, const char *fragmentShaderFilePath)
{
    std::string vertexShaderSource = readFileToString(vertexShaderFilePath);
    std::string fragmentShaderSource = readFileToString(fragmentShaderFilePath);

    const char* vertexShaderSourceCstr = vertexShaderSource.c_str(); 
    const char* fragmentShaderSourceCstr = fragmentShaderSource.c_str();

    unsigned int vertexShader,fragmentShader;

    //Error Values
    int success;
    char infoLog[512];

    //Creating and compiling vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader,1,&vertexShaderSourceCstr,NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        std::cout << "Vertec Shader code - " << vertexShaderSourceCstr;
        glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //reseting the error values
    success = 0;
    infoLog[0]='\0';

    //Creating and compiling Fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentShaderSourceCstr,NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
    if(!success)
    {
        std::cout << "Fragment Shader code - " << fragmentShaderSourceCstr;
        glGetShaderInfoLog(fragmentShader,512,NULL,infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //Creating and compiling shader program
    shaderProgramID = glCreateProgram();
    glAttachShader(shaderProgramID,vertexShader);
    glAttachShader(shaderProgramID,fragmentShader);
    glLinkProgram(shaderProgramID);

    success = 0;
    infoLog[0]='\0';

    glGetProgramiv(shaderProgramID,GL_LINK_STATUS,&success);
    if (!success)
	{
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		
	}

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


}

void Shader::SetBool(const std::string &boolName, bool value) const
{
    glUniform1i(
        glGetUniformLocation(shaderProgramID,boolName.c_str()),
        int(value)
    );
}

void Shader::SetInt(const std::string &intName, int value) const
{
    glUniform1i(
        glGetUniformLocation(shaderProgramID,intName.c_str()),
        value
    );
}

void Shader::SetFloat(const std::string &floatName, float value) const
{
    glUniform1f(
        glGetUniformLocation(shaderProgramID,floatName.c_str()),
        value
    );
}

void Shader::Use()
{
    glUseProgram(shaderProgramID);
}

std::string readFileToString(const std::string& path)
{
    std::ifstream file(path);
    if (!file)
        throw std::runtime_error("Failed to open file: " + path);

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}