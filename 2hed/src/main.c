#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

static SDL_Window* window;
static SDL_Renderer* renderer;

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
	bool quit = false;
	while (!quit) {
		SDL_Event event = {0};
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
		SDL_RenderDrawLine(renderer, 100, 500, 700,500);
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();

	return 0;
}