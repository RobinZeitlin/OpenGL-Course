#include "Shader.h"

#include <glfw3.h>
#include <glad/glad.h>

std::string Shader::LoadShader(const char* aPath)
{
	std::string shaderCode;
	std::ifstream shaderFile;

	shaderFile.open(aPath);

	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();

	shaderFile.close();
	shaderCode = shaderStream.str();

	return shaderCode;
}

unsigned int Shader::LoadVertexShader(const char* aPath)
{
	int result;
	char Log[512];

	std::string shaderCodeString = LoadShader(aPath);
	const char* shaderCode = shaderCodeString.c_str();

	unsigned int shaderObject;

	shaderObject = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shaderObject, 1, &shaderCode, NULL);
	glCompileShader(shaderObject);

	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(shaderObject, 512, NULL, Log);
		std::cout << "Failed to compile vertex shader \n" << Log << std::endl;
	}

	return shaderObject;
}

unsigned int Shader::LoadFragmentShader(const char* aPath)
{
	int result;
	char Log[512];

	std::string shaderCodeString = LoadShader(aPath);
	const char* shaderCode = shaderCodeString.c_str();

	unsigned int shaderObject;

	shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shaderObject, 1, &shaderCode, NULL);
	glCompileShader(shaderObject);

	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

	if (!result) {
		glGetShaderInfoLog(shaderObject, 512, NULL, Log);
		std::cout << "Failed to compile vertex shader \n" << Log << std::endl;
	}

	return shaderObject;
}

void Shader::Initialize(const char* aVertexPath, const char* aFragmentPath)
{
	int result;
	char Log[512];

	unsigned int VertexShader = LoadVertexShader(aVertexPath);
	unsigned int FragmentShader = LoadFragmentShader(aFragmentPath);

	myShaderProgram = glCreateProgram();
	glAttachShader(myShaderProgram, VertexShader);
	glAttachShader(myShaderProgram, FragmentShader);
	glLinkProgram(myShaderProgram);

	glGetProgramiv(myShaderProgram, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(myShaderProgram, 512, NULL, Log);
		std::cout << "Shader program couldnt not compile\n" << Log << std::endl;
	}

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
}

void Shader::Use()
{
	glUseProgram(myShaderProgram);
}

void Shader::SetMatrix4(glm::mat4 aMatrix, const std::string& aName)
{
	glUniformMatrix4fv(glGetUniformLocation(myShaderProgram, aName.c_str()), 1, GL_FALSE, glm::value_ptr(aMatrix));
}

void Shader::SetVector3f(glm::vec3 aVector3f, const std::string& aName)
{
	glUniform3fv(glGetUniformLocation(myShaderProgram, aName.c_str()), 1, glm::value_ptr(aVector3f));
}

void Shader::SetInt(int aInt, const std::string& aName)
{
	GLint location = glGetUniformLocation(myShaderProgram, aName.c_str());
	if (location == -1)
	{
		std::cerr << "Warning: uniform '" << aName << "' doesn't exist in shader program." << std::endl;
		return;
	}

	glUniform1i(location, aInt);
}

