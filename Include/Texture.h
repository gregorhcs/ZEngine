#pragma once

#include <string>

#include <glad/glad.h>

namespace zn {

struct TextureMetadata 
{
	unsigned int width;
	unsigned int height;
	unsigned int nrChannels;
};

struct TextureRenderParams
{
	GLenum wrapS;
	GLenum wrapT;
	GLenum minFilter;
	GLenum magFilter;
};

class Texture
{
	friend class ResourceManager;

	public:

		inline unsigned int TextureID() const noexcept {
			return textureID_;
		}

		void ActivateAndBind() const noexcept;

	protected:

		Texture(const unsigned char* textureSource, GLuint textureID, const TextureMetadata metadata, GLenum textureUnit, GLenum colorFormat);
		Texture(const unsigned char* textureSource, GLuint textureID, const TextureMetadata metadata, GLenum textureUnit, GLenum colorFormat, const TextureRenderParams renderParams);

		GLuint textureID_;

		const TextureMetadata metadata_;
		const TextureRenderParams renderParams_;

		const GLuint textureUnit_;

};

}
