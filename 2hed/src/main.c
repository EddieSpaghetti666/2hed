#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include "util.h"

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
	initSDL();
	
	Font font = loadFontFromFile("C:\\Windows\\Fonts\\arial.ttf");


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

		SDL_SetRenderDrawColor(renderer, 205, 70, 70, SDL_ALPHA_OPAQUE);
		Vec2f pos = { SCREEN_WIDTH/2 - 50.0f, SCREEN_HEIGHT/2 - 50.0f };
		drawString(&font, "PENIS!", 32.0f, &pos);

		SDL_RenderPresent(renderer);
	}

	SDL_Quit();

	return 0;
}