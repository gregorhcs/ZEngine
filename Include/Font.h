#pragma once

#include <map>

#include "Texture.h"

namespace zn {

class Font
{
	friend class ResourceManager;

	public:

		static const char supportedCharsStart = 32;
		static const char supportedCharsEnd = 132;

		static const unsigned int cellHeight = 26;
		static const unsigned int cellWidth = 10;

		Texture* FindTextureOfChar(char inChar);

	protected:

		std::map<char, Texture*> characterTextures;

		void AddCharacter(char inChar, Texture* texture);
};

}
