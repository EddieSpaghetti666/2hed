#include "font.h"
#include <stdlib.h>
#include <stdio.h>
#include "sdl_extra.h"
#include <string.h>

extern SDL_Renderer* renderer;

static unsigned char* createTtfBuffer(const char* fileName) {
	size_t fileSize;
	FILE* fontFile;

	if (fopen_s(&fontFile, fileName, "rb") != 0) {
		fprintf(stderr, "Error opening font file");
	}

	fseek(fontFile, 0, SEEK_END);
	fileSize = ftell(fontFile);
	rewind(fontFile);

	unsigned char* ttf_buffer = (unsigned char*)malloc(sizeof(unsigned char) * fileSize);
	size_t result = fread(ttf_buffer, 1, fileSize, fontFile);

	if (result != fileSize) {
		fprintf(stderr, "Couldn't read entire font file");
		exit(3);
	}

	fclose(fontFile);

	return ttf_buffer;
}

Font loadFontFromFile(const char* fileName) {
	Font font;

	unsigned char* bitmap = (unsigned char*)malloc(sizeof(unsigned char) * PALETTE_HEIGHT * PALETTE_WIDTH);
	unsigned char* ttf_buffer = createTtfBuffer(fileName);

	stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, bitmap, PALETTE_WIDTH, PALETTE_HEIGHT, 32, 96, font.charData);
	
	free(ttf_buffer);

	SDL_Surface* bakedFontSurface = createSurfaceFromPalette(bitmap);
	
	font.texture = scp(SDL_CreateTextureFromSurface(renderer, bakedFontSurface));

	free(bitmap);

	bakedFontSurface->pixels = NULL;
	//SDL_FreeSurface(bakedFontSurface);

	return font;
}

void drawChar(const Font* const font, const char c, const float scale, Vec2f* pos) {

	stbtt_aligned_quad cquad;
	stbtt_GetBakedQuad(font->charData, PALETTE_WIDTH, PALETTE_HEIGHT, c - 32, &pos->x, &pos->y, &cquad, 0);

	SDL_Rect src = {
		.x = (int) (cquad.s0 * PALETTE_WIDTH),
		.y = (int) (cquad.t0 * PALETTE_HEIGHT),
		.w = (int) (cquad.x1 - cquad.x0),
		.h = (int) (cquad.y1 - cquad.y0)
	};
	SDL_Rect dst = {
		.x = (int) cquad.x0,
		.y = (int) (cquad.y0 + scale), //We have to add the scale to the y position here because SDL and openGl have different coord systems. 
		.w = src.w,
		.h = src.h,
	};

	SDL_RenderCopy(renderer, font->texture, &src, &dst);

}

void drawString(const Font* const font, const char* string, const float scale, Vec2f* startPos) {
	for (size_t i = 0; i < strlen(string); ++i) {
		drawChar(font, string[i], scale, startPos);
	}
}
