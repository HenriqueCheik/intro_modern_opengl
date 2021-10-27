#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// the program ID
	unsigned int id;
	// constructor will read and build the shader
	Shader(const char* vertexPath, const char* fragmentPath);
	// function to use/activate this shader
	void use();
	// utility functions to set values to uniforms in the shader program
	void setBool(const std::string &uniformName, bool value) const;
	void setInt(const std::string &uniformName, int value) const;
	void setFloat(const std::string &uniformName, float value) const;
	void setVec4(const std::string &uniformName, float value[4]) const;
};
#endif