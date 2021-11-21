#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include "util.h"
#include "sdl_extra.h"
#include "editor.h"


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

static const char* CURRENT_FILE = "../test.txt";

static void initSDL() {
	sce(SDL_Init(SDL_INIT_VIDEO));
	window = scp(SDL_CreateWindow("2hed", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
	renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
}

int main(int argc, char* argv[]) {
    char* arr = NULL;

	initSDL();
    
    Editor editor;
    initEditor(&editor);

    char fileBuffer[2048];
    size_t buffSize = loadFileIntoBuffer(CURRENT_FILE, fileBuffer);
    createEditorFromBuffer(&editor, fileBuffer, buffSize);

	Font font = loadFontFromFile("../LiberationMono-Regular.ttf", FONT_SIZE);


	bool quit = false;
	while (!quit) {
		SDL_Event event = { 0 };
        const Uint32 start = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_TEXTINPUT:
                    addTextAtCursor(&editor, event.text.text);
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                        case SDLK_BACKSPACE:
                            backspace(&editor);
                            break;
                        case SDLK_LEFT:
                            moveCursorLeft(&editor);
                            break;
                        case SDLK_RIGHT:
                            moveCursorRight(&editor);
                            break;
                        case SDLK_DOWN:
                            moveCursorDown(&editor);
                            break;
                        case SDLK_UP:
                            moveCursorUp(&editor);
                            break;
                        case SDLK_RETURN:
                            carraigeReturn(&editor);
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

		SDL_SetRenderDrawColor(renderer, 205, 70, 70, SDL_ALPHA_OPAQUE);
        
        Vec2f pos = { STARTING_X_POS, STARTING_Y_POS };
        float scale = 32.0f;

        Line* line = editor.lines;
        for (int row = 0; line != NULL; ++row, line = line->next) {
            pos.y = row * scale;
            pos.x = STARTING_X_POS;
            drawString(&font, line->text, scale, &pos);
        }
        
        drawCaret(&font, scale, editor.cursorCol, editor.cursorRow);

    

        SDL_RenderPresent(renderer);
        
        const Uint32 frameDuration = SDL_GetTicks() - start;
        const Uint8 deltaTimeMS = 1000 / FPS;
        if (frameDuration < deltaTimeMS) {
            SDL_Delay(deltaTimeMS - frameDuration);
        }
	}

    freeEditor(&editor);
	SDL_Quit();

	return 0;
}
