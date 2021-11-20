#pragma once
#include <SDL.h>
#include "../vendor/stb_truetype.h"
#include "util.h"

#define PALETTE_HEIGHT 512
#define PALETTE_WIDTH 512
#define ASCII_LOW 32
#define ASCII_HIGH 96

#define COLOR_WHITE { 0, 0, 0 }

typedef struct Font {
	SDL_Texture* texture;
	stbtt_bakedchar charData[96];
} Font;

typedef struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
} Color;

Font loadFontFromFile(const char* fileName, const float fontSize);
void drawChar(const Font* const font, const char c, const float scale, Vec2f* pos);
void drawString(const Font* const font, const char* string, const float scale, Vec2f* startPos, char *cursor);
