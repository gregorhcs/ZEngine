#include "Font.h"

namespace zn {

Texture* Font::FindTextureOfChar(char inChar)
{
	return characterTextures.contains(inChar) ? characterTextures[inChar] : nullptr;
}

void Font::AddCharacter(char inChar, Texture* texture) 
{
	characterTextures[inChar] = texture;
}

}
