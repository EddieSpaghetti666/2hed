#pragma once
#include <stdlib.h>

typedef struct Vec2f {
	float x;
	float y;
} Vec2f;

size_t loadFileIntoBuffer(const char* fileName, void* buffer);
void saveFile(const char* fileName, const char* buffer);

