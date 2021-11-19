#pragma once
#include <SDL.h>

/* Check SDL return code for error */
void sce(int code);

/* Check SDL pointer */
void* scp(void* ptr);

SDL_Surface* createSurfaceFromPalette(unsigned char* bitmap);
