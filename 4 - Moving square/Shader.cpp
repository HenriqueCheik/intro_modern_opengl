#include "Shader.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode, fragmentCode;
	std::ifstream vertexShaderFile, fragmentShaderFile;

	vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		std::stringstream vertexShaderStream, fragmentShaderStream;

		// Retrieve code from Vertex shader file
		vertexShaderFile.open(vertexPath);
		vertexShaderStream << vertexShaderFile.rdbuf();
		vertexShaderFile.close();
		vertexCode = vertexShaderStream.str();

		// Retrieve code from Fragment shader file
		fragmentShaderFile.open(fragmentPath);
		fragmentShaderStream << fragmentShaderFile.rdbuf();
		fragmentShaderFile.close();
		fragmentCode = fragmentShaderStream.str();
		
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex Shader compilation
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertexShaderSource, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// fragment Shader compilation
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// linking vertex and fragment shader into a program
	this->id = glCreateProgram();
	glAttachShader(this->id, vertex);
	glAttachShader(this->id, fragment);
	glLinkProgram(this->id);

	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// after program is created, free allocated resources for vertex and fragment shader
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use()
{
	glUseProgram(this->id);
}

void Shader::setBool(const std::string &uniformName, bool value) const
{
	glUniform1i(glGetUniformLocation(this->id, uniformName.c_str()), (int)value);
}

void Shader::setInt(const std::string &uniformName, int value) const
{
	glUniform1i(glGetUniformLocation(this->id, uniformName.c_str()), value);
}

void Shader::setFloat(const std::string &uniformName, float value) const
{
	glUniform1f(glGetUniformLocation(this->id, uniformName.c_str()), value);
}

void Shader::setVec4(const std::string &uniformName, float value[4]) const
{
	glUniform4f(glGetUniformLocation(this->id, uniformName.c_str()), value[0], value[1], value[2], value[3]);
}

void Shader::setVec4(const std::string &uniformName, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(this->id, uniformName.c_str()), 1, &value[0]);
}

void Shader::setMat4(const std::string &uniformName, const glm::mat4 &value) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->id, uniformName.c_str()), 1, GL_FALSE, &value[0][0]);
}