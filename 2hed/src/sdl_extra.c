#include <SDL.h>
#include <stdio.h>
#include "font.h" //For Palette dimensions

void sce(int code) {
	if (code < 0) {
		fprintf(stderr, "SDL return code error: %s", SDL_GetError());
		exit(1);
	}
}

void* scp(void* ptr) {
	if (ptr == NULL) {
		fprintf(stderr, "SDL pointer error: %s", SDL_GetError());
		exit(2);
	}
	return ptr;
}

SDL_Surface* createSurfaceFromPalette(unsigned char* bitmap) {
	SDL_Surface* bakedfontSurface = SDL_CreateRGBSurfaceWithFormat(0, PALETTE_WIDTH, PALETTE_HEIGHT, 8, SDL_PIXELFORMAT_INDEX8);
	bakedfontSurface->pixels = bitmap;

	SDL_Color colors[256] = { 0 };
	for (int i = 0; i < 256; ++i) {
		colors[i].r = (Uint8) i;
		colors[i].g = (Uint8) i;
		colors[i].b = (Uint8) i;
		colors[i].a = (Uint8) i;
	}

	sce(SDL_SetPaletteColors(bakedfontSurface->format->palette, colors, 0, 256));


	sce(SDL_SetColorKey(bakedfontSurface, SDL_TRUE, SDL_MapRGB(bakedfontSurface->format, 0x00, 0x00, 0x00)));

	return bakedfontSurface;

}
