#pragma once
#include <SDL.h>
#include "../vendor/stb_truetype.h"
#include "util.h"

#define PALETTE_HEIGHT 512
#define PALETTE_WIDTH 512

typedef struct Font {
	SDL_Texture* texture;
	stbtt_bakedchar charData[96];
} Font;

Font loadFontFromFile(const char* fileName);
void drawChar(const Font* const font, const char c, const float scale, Vec2f* pos);
void drawString(const Font* const font, const char* string, const float scale, Vec2f* startPos);
void drawCaret(const Font* const font, const float scale, int caretCol, int caretRow);
