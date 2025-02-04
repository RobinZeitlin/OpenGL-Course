#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#include <glm.hpp>
#include <gtc/type_ptr.inl>

class Shader {
private:
	std::string LoadShader(const char* aPath);
	unsigned int LoadVertexShader(const char* aPath);
	unsigned int LoadFragmentShader(const char* aPath);

public:
	unsigned int myShaderProgram;
	void Initialize(const char* aVertexPath, const char* aFragmentPath);
	void Use();
	void SetMatrix4(glm::mat4 aMatrix, const std::string& aName);
	void SetVector3f(glm::vec3 aVector3f, const std::string& aName);
	void SetInt(int aInt, const std::string& aName);
};