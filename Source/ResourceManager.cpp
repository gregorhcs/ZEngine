#include "ResourceManager.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

#include <stb/stb_image.h>

#include "ShaderProgram.h"
#include "SampleDataFactory.h"
#include "RectangleMesh.h"

namespace zn {

ResourceManager::ResourceManager()
{
	stbi_set_flip_vertically_on_load(true);
}

ResourceManager::~ResourceManager()
{
	for (Texture* texture : loadedTextures_)
		free(texture);

	for (ShaderProgram* shaderProgram : loadedShaderPrograms_)
		free(shaderProgram);

	for (Mesh* mesh : loadedMeshes_)
		free(mesh);
}

ShaderProgram* ResourceManager::LoadShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath)
{
	ShaderProgram* shaderProgram = new ShaderProgram(
		ReadFile(vertexShaderFilePath),
		ReadFile(fragmentShaderFilePath)
	);
	loadedShaderPrograms_.push_back(shaderProgram);

	return shaderProgram;
}

Texture* ResourceManager::LoadTexture(const std::string& filePath, GLenum textureUnit, GLenum colorFormat)
{
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	if (!data)
	{
		stbi_image_free(data);
		throw std::runtime_error("Failed to load texture.");
	}

	Texture* texture = new Texture(
		data,
		textureIdCounter_++,
		TextureMetadata{
			static_cast<unsigned int>(width),
			static_cast<unsigned int>(height),
			static_cast<unsigned int>(nrChannels)
		},
		textureUnit,
		colorFormat
	);
	loadedTextures_.push_back(texture);

	stbi_image_free(data);

	return texture;
}

Mesh* ResourceManager::LoadMesh_CubeWithTexture()
{
	std::vector<VertexAttribute> vertexAttributes{
		VertexAttribute{ 3, GL_FLOAT, 5 * sizeof(GLfloat), nullptr },						// position
		VertexAttribute{ 2, GL_FLOAT, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)) }	// tex coord
	};

	Mesh* mesh = new Mesh(
		cubeWithTextureVertexData, 
		sizeof(cubeWithTextureVertexData), 
		nullptr, 0,
		vertexAttributes
	);
	loadedMeshes_.push_back(mesh);

	return mesh;
}

Mesh* ResourceManager::LoadMesh_Rectangle(const glm::vec2& topLeft, float width, float height)
{
	std::vector<VertexAttribute> vertexAttributes{
		VertexAttribute{ 3, GL_FLOAT, 3 * sizeof(GLfloat), nullptr }, // position
	};

	Mesh* mesh = new RectangleMesh(topLeft, width, height);

	loadedMeshes_.push_back(mesh);

	return mesh;
}

const std::vector<Mesh*> ResourceManager::GetLoadedMeshes() const
{
	return loadedMeshes_;
}

std::string ResourceManager::ReadFile(const std::string& filePath)
{
	std::ifstream file(filePath);

	if (!file.is_open()) {
		std::cerr << "File content loading log: " << filePath << std::endl;
		throw std::runtime_error("Unable to open file.");
	}

	std::stringstream string_buffer;

	string_buffer << file.rdbuf();

	return string_buffer.str();
}

}
