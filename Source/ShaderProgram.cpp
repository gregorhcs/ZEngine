#include "ShaderProgram.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

namespace zn {

ShaderProgram::ShaderProgram() : programID_(0), bHoldsData_(false) {}

ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept :
	programID_(other.programID_), bHoldsData_(true)
{
	other.programID_ = 0;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
	if (this != &other)
	{
		std::swap(programID_, other.programID_);
		std::swap(bHoldsData_, other.bHoldsData_);
	}
	return other;
}

ShaderProgram::~ShaderProgram() noexcept 
{
	if (bHoldsData_) 
	{
		glDeleteProgram(programID_);
	}
}

void ShaderProgram::Use() const noexcept 
{
	glUseProgram(programID_);
}

int ShaderProgram::GetUniformLoc(const std::string &name) const noexcept 
{
	return glGetUniformLocation(programID_, name.c_str());
}

void ShaderProgram::SetBool(const std::string &name, bool value) const noexcept {
	glUniform1i(GetUniformLoc(name), static_cast<int>(value));
}
void ShaderProgram::SetInt(const std::string &name, int value) const noexcept {
	glUniform1i(GetUniformLoc(name), value);
}
void ShaderProgram::SetFloat(const std::string &name, float value) const noexcept {
	glUniform1f(GetUniformLoc(name), value);
}

void ShaderProgram::SetVec2f(const std::string &name, const glm::vec2 &value) const noexcept {
	glUniform2f(GetUniformLoc(name), value.x, value.y);
}
void ShaderProgram::SetVec3f(const std::string &name, const glm::vec3 &value) const noexcept {
	glUniform3f(GetUniformLoc(name), value.x, value.y, value.z);
}
void ShaderProgram::SetVec4f(const std::string &name, const glm::vec4 &value) const noexcept {
	glUniform4f(GetUniformLoc(name), value.x, value.y, value.z, value.w);
}

void ShaderProgram::SetMat3f(const std::string &name, const glm::mat3 &value) const noexcept {
	glUniformMatrix3fv(GetUniformLoc(name), 1, GL_FALSE, glm::value_ptr(value));
}
void ShaderProgram::SetMat4f(const std::string &name, const glm::mat4 &value) const noexcept {
	glUniformMatrix4fv(GetUniformLoc(name), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::SetTextureUniforms(unsigned int numberOfUsedTextures) const noexcept 
{
	const char* textureStrings[] = {
		"texture0",  "texture1",  "texture2",  "texture3",
		"texture4",  "texture5",  "texture6",  "texture7",
		"texture8",  "texture9",  "texture10", "texture11",
		"texture12", "texture13", "texture14", "texture15",
	};

	for (unsigned int i = 0; i < numberOfUsedTextures; i++) 
	{
		SetInt(textureStrings[i], i);
	}
}

ShaderProgram::ShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) :
	programID_(glCreateProgram()),
	bHoldsData_(true)
{
	const char* vertexShaderSrc = vertexShaderSource.c_str();
	const char* fragmentShaderSrc = fragmentShaderSource.c_str();

	// compile shader sourcecodes

	unsigned int vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexShaderSrc, nullptr);
	glCompileShader(vertexShaderID);

	unsigned int fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentShaderSrc, nullptr);
	glCompileShader(fragmentShaderID);

	try {
		CheckCompilationErrors(vertexShaderID, VERTEX_SHADER);
		CheckCompilationErrors(fragmentShaderID, FRAGMENT_SHADER);
	}
	catch (const std::runtime_error& e) {
		glDeleteShader(fragmentShaderID);
		glDeleteShader(vertexShaderID);
		throw;
	}

	// link shader sourcecodes
	glAttachShader(programID_, vertexShaderID);
	glAttachShader(programID_, fragmentShaderID);
	glLinkProgram(programID_);
	CheckLinkingErrors(programID_);

	// delete shader sourcecodes
	glDeleteShader(fragmentShaderID);
	glDeleteShader(vertexShaderID);
}

void ShaderProgram::CheckLinkingErrors(unsigned int ID) const
{
	int success;
	char infoLog[1024];

	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 1024, nullptr, infoLog);
		std::cerr << "Shader linking info log:" << std::endl << infoLog << std::endl;
		throw std::runtime_error("Shader linking failed.");
	}
}

void ShaderProgram::CheckCompilationErrors(unsigned int ID, ShaderType type) const
{
	int success;
	char infoLog[1024];

	glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(ID, 1024, nullptr, infoLog);
		std::cerr << (type == VERTEX_SHADER ? "Vertex" : "Fragment") << " shader compilation info log:" << std::endl << infoLog << std::endl;
		throw std::runtime_error("Shader compilation failed.");
	}
}

}
