#pragma once
#include <SDL.h>
#include "../vendor/stb_truetype.h"



typedef struct Font {
	SDL_Texture* texture;
	stbtt_bakedchar charData[96];
} Font;

Font loadFontFromFile(const char* fileName);
void renderChar(const Font const* font, char c, float scale)
