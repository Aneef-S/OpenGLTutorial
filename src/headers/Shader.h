#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
	public:
	unsigned int ID;
	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	Shader();
	// use/activate the shader
	void use() const;
	// utility uniform functions
	void setBool(const std::string &name,const bool value) const;
	void setInt(const std::string &name,const int value) const;
	void setFloat(const std::string &name,const float value) const;
	void setMat4f(const std::string & name,const glm::mat4 &value) const;
	void setVec4f(const std::string& name,const glm::vec4& value) const;
	void setVec3f(const std::string& name,const glm::vec3& value) const;
	void setUniformBlock(const std::string& name, unsigned int bindingPoint) const;
	
};

#endif // SHADER_H