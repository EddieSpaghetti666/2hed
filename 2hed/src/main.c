#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdlib.h>
#include "util.h"
#include "sdl_extra.h"
#include "editor.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "font.h"

SDL_Window* window;
SDL_Renderer* renderer;

static void initSDL() {
	sce(SDL_Init(SDL_INIT_VIDEO));
	window = scp(SDL_CreateWindow("2hed", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN));
	renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    
}

int main(int argc, char* argv[]) {
	initSDL();
    
    Editor editor;
    initEditor(&editor);
    
	Font font = loadFontFromFile("../LiberationMono-Regular.ttf");


	bool quit = false;
	while (!quit) {
		SDL_Event event = { 0 };
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
            
            
		}

		//Clear screen
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 205, 70, 70, SDL_ALPHA_OPAQUE);
        
        Vec2f pos = { STARTING_X_POS, STARTING_Y_POS };
        float scale = 32.0f;

        
        for(int row = 0; row < buf_count(editor.lines); row++) {
            pos.y = row * scale;
            pos.x = STARTING_X_POS;
            Line *line = editor.lines + row;
            drawString(&font, line->text, scale, &pos);
        }
        
        drawCaret(&font, scale, editor.cursorCol, editor.cursorRow);
    

        SDL_RenderPresent(renderer);

	}

    freeEditor(&editor);
	SDL_Quit();

	return 0;
}
