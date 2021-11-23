#pragma once
#include <stdlib.h>
#include "editor.h"

typedef struct Vec2f {
	float x;
	float y;
} Vec2f;

size_t loadFileIntoBuffer(const char* fileName, char* buffer);
void saveFile(const char* fileName, Editor *editor);

