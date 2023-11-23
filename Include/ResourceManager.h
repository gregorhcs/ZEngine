#pragma once

#include "ShaderProgram.h"

#include <memory>
#include <vector>

#include "ShaderProgram.h"
#include "Texture.h"
#include "Mesh.h"

namespace zn {

// concept
template<class T, class U>
concept Derived = std::is_base_of<U, T>::value;

class ShaderProgram;
class Texture;
class Mesh;

class ResourceManager
{
	public:

		ResourceManager();
		~ResourceManager();

		/* Read a texture from a file.
		 * Throws runtime_error if texture could not be loaded from filePath. */
		Texture* LoadTexture(const std::string& filePath, GLenum textureUnit, GLenum colorFormat);

		// reads and constructs shaders from file
		ShaderProgram* LoadShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		/* Load a sample, unoptimized 3D cube with texture coordinates. */
		Mesh* LoadMesh_CubeWithTexture();

		/* Load a 2D rectangle. */
		Mesh* LoadMesh_Rectangle(const glm::vec2& topLeft, float width, float height);

		/* Load mesh. */
		template<class T>
		T* LoadMesh() requires Derived<T, Mesh>;

		const std::vector<Mesh*> GetLoadedMeshes() const;

	protected:

		std::vector<Texture*> loadedTextures_;
		std::vector<ShaderProgram*> loadedShaderPrograms_;
		std::vector<Mesh*> loadedMeshes_;

		GLuint textureIdCounter_ = 0;

		// Throws runtime_error if the specified file could not be opened.
		static std::string ReadFile(const std::string& filePath);

};

template<class T>
inline T* ResourceManager::LoadMesh() requires Derived<T, Mesh>
{
	T* mesh = new T();
	loadedMeshes_.push_back(mesh);

	return mesh;
}

}
