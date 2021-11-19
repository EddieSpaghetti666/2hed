#include <SDL.h>
#include <stdio.h>

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