#pragma once

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
	// use/activate the shader
	void use();
	// utility uniform functions
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setMat4f(const std::string & name, glm::mat4 &value) const;
	void setVec4f(const std::string& name, glm::vec4& value) const;
	void setVec3f(const std::string& name, glm::vec3& value) const;
};
