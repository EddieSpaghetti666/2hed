#include "font.h"
#include <stdlib.h>
#include <stdio.h>
#include "sdl_extra.h"
#include "util.h"

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

	unsigned char bitmap[512 * 512];
	unsigned char* ttf_buffer = createTtfBuffer(fileName);
	stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, bitmap, 512, 512, 32, 96, font.charData);
	
	free(ttf_buffer);


	SDL_Surface* bakedfontSurface = scp(SDL_CreateRGBSurfaceWithFormat(0, 512, 512, 8, SDL_PIXELFORMAT_INDEX8));
	bakedfontSurface->pixels = bitmap;

	SDL_Color colors[256] = { 0 };
	for (int i = 0; i < 256; ++i) {
		colors[i].r = i;
		colors[i].g = i;
		colors[i].b = i;
	}

	sce(SDL_SetPaletteColors(bakedfontSurface->format->palette, colors, 0, 256));

	font.texture = scp(SDL_CreateTextureFromSurface(renderer, bakedfontSurface));

	SDL_FreeSurface(bakedfontSurface);

	return font;

}

