#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include "util.h"
#include "sdl_extra.h"
#include "stretchbuffer.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "font.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 60

#define FONT_SIZE 32.0f

#define MAX_BUFFER_LEN 2048

SDL_Window* window;
SDL_Renderer* renderer;

char flatBuffer[MAX_BUFFER_LEN];
char *cursor;

static const char* CURRENT_FILE = "./test.txt";

static void initSDL() {
	sce(SDL_Init(SDL_INIT_VIDEO));
	window = scp(SDL_CreateWindow("2hed", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
	renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
}

void inputCharacters(char *text) {
    char *temp = cursor;
    cursor++;
    memmove( (void *) cursor, (void *) temp, strlen(text));
    *temp = *text;
}

void backspace() {
    char *temp = cursor;
    if (*(temp - 1) == '\0') return;
    cursor--;
    memmove( (void *) cursor, (void *) temp, sizeof(flatBuffer));
}

int main(int argc, char* argv[]) {
    char* arr = NULL;

	initSDL();
    
    //memcpy(flatBuffer, "Penis", strlen("Penis"));
    loadFileIntoBuffer(CURRENT_FILE, flatBuffer);
    cursor = &flatBuffer[2];
	
	Font font = loadFontFromFile("../LiberationMono-Regular.ttf", FONT_SIZE);


	bool quit = false;
	while (!quit) {
		SDL_Event event = { 0 };
        const Uint8 start = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
                break;
            case SDL_TEXTINPUT:
                inputCharacters(event.text.text);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_BACKSPACE:
                        backspace();
                        break;
                }
                break;
			}
            
            const Uint8* keyStates = SDL_GetKeyboardState(NULL);
            if (keyStates[SDL_SCANCODE_LCTRL] && keyStates[SDL_SCANCODE_S]) {
                saveFile(CURRENT_FILE, flatBuffer);
            }
            
		}

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 0x28, 0x28, 0x28, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);
  
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
        Vec2f pos = { 0, 0 };
        drawString(&font, flatBuffer, FONT_SIZE, &pos, cursor);
    

        SDL_RenderPresent(renderer);
        
        const Uint8 frameDuration = SDL_GetTicks() - start;
        const Uint8 deltaTimeMS = 1000 / FPS;
        if (frameDuration < deltaTimeMS) {
            SDL_Delay(deltaTimeMS - frameDuration);
        }
	}

	SDL_Quit();

	return 0;
}
