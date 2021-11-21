#pragma once

typedef struct Vec2f {
	float x;
	float y;
} Vec2f;

void loadFileIntoBuffer(const char* fileName, void* buffer);
void saveFile(const char* fileName, const char* buffer);

