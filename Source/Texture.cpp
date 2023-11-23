#include "Texture.h"

#include <stdexcept>

#include <stb/stb_image.h>

namespace zn {

Texture::Texture(const unsigned char* textureSource, GLuint textureID, const TextureMetadata metadata, GLenum textureUnit, GLenum colorFormat) :
	Texture(
		textureSource, 
		textureID,
		metadata,
		textureUnit, 
		colorFormat, 
		TextureRenderParams{ 
			GL_CLAMP_TO_EDGE, 
			GL_CLAMP_TO_EDGE, 
			GL_NEAREST_MIPMAP_LINEAR, 
			GL_NEAREST 
		}
	)
{}

Texture::Texture(const unsigned char* textureSource, GLuint textureID, const TextureMetadata metadata, GLenum textureUnit, GLenum colorFormat, const TextureRenderParams renderParams) :
	textureID_(textureID),
	metadata_(metadata),
	textureUnit_(textureUnit),
	renderParams_(renderParams)
{
	glGenTextures(1, &textureID_);

	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, renderParams_.wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, renderParams_.wrapT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, renderParams_.minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, renderParams_.magFilter);

	glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, metadata_.width, metadata_.height, 0, colorFormat, GL_UNSIGNED_BYTE, textureSource);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::ActivateAndBind() const noexcept 
{
	glActiveTexture(textureUnit_);
	glBindTexture(GL_TEXTURE_2D, textureID_);
}

}
