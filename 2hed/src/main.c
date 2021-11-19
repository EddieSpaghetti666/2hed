#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include "font.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

SDL_Window* window;
SDL_Renderer* renderer;

static void initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) goto ERROR;
	window = SDL_CreateWindow("2hed", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window) goto ERROR;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) goto ERROR;
	return;
ERROR:
	printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
}

int main(int argc, char* argv[]) {
	//unsigned char bitmap[512 * 512];
	//stbtt_bakedchar cdata[96];
	//size_t size;
	//FILE* fontFile;
	//if (fopen_s(&fontFile, "C:\\Windows\\Fonts\\arial.ttf", "rb") != 0) printf("couldn't open font\n");
	//fseek(fontFile, 0, SEEK_END);
	//size = ftell(fontFile);
	//rewind(fontFile);

	//char* ttf_buffer = (char*)malloc(sizeof(char) * size);
	//size_t result = fread(ttf_buffer, 1, size, fontFile);
	//stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, bitmap, 512, 512, 32, 96, cdata);

	//SDL_Surface* bakedfontSurface = SDL_CreateRGBSurfaceWithFormat(0, 512, 512, 8, SDL_PIXELFORMAT_INDEX8);
	//bakedfontSurface->pixels = bitmap;
	//if (!bakedfontSurface) {
	//	printf("Creating surface failed: %s", SDL_GetError());
	//	exit(-1);
	//}

	//SDL_Color colors[256] = { 0 };
	//for (int i = 0; i < 256; ++i) {
	//	colors[i].r = i;
	//	colors[i].g = i;
	//	colors[i].b = i;
	//}
	//SDL_SetPaletteColors(bakedfontSurface->format->palette, colors, 0, 256);

	//fclose(fontFile);
	//free(ttf_buffer);
	
	initSDL();
	
	Font font = loadFontFromFile("C:\\Windows\\Fonts\\arial.ttf");


	//SDL_Texture* fontTexture = SDL_CreateTextureFromSurface(renderer, bakedfontSurface);
	//if (!fontTexture) printf("Creating texture failed! %s", SDL_GetError());

	//stbtt_aligned_quad cquad;
	//float x = 40.0f;
	//float y = 40.0f;
	//stbtt_GetBakedQuad(cdata, 512, 512, 'e' - 32, &x, &y, &cquad, 0);

	//
	//SDL_Rect erect = {
	//	.x = cquad.s0 * 512,
	//	.y = cquad.t0 * 512,
	//	.w = cquad.x1 - cquad.x0,
	//	.h = cquad.y1 - cquad.y0

	//};
	//SDL_Rect drect = {
	//	10, 10, erect.w, erect.h
	//};


	bool quit = false;
	while (!quit) {
		SDL_Event event = { 0 };
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		//Draw Triangle
		SDL_SetRenderDrawColor(renderer, 205, 70, 70, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, 100, 500, 400, 100);
		SDL_RenderDrawLine(renderer, 700, 500, 400, 100);
		SDL_RenderDrawLine(renderer, 100, 500, 700, 500);
	/*	if (SDL_RenderCopy(renderer, fontTexture, &erect, &drect) != 0) {
			printf("failed to render %s", SDL_GetError());
		}*/

		SDL_RenderPresent(renderer);
	}

	SDL_Quit();

	return 0;
}