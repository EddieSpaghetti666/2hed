#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void loadFileIntoBuffer(const char* fileName, void* buffer) {
	size_t fileSize;
	FILE* file;

	if (fopen_s(&file, fileName, "rb") != 0) {
		fprintf(stderr, "Error opening font file");
        exit(4);
	}

	fseek(file, 0, SEEK_END);
	fileSize = ftell(file);    
	rewind(file);

	size_t result = fread(buffer, 1, fileSize, file);

	if (result != fileSize) {
		fprintf(stderr, "Couldn't read entire font file");
		exit(3);
	}
	fclose(file);
}

void saveFile(const char* fileName, const char* buffer) {
	size_t fileSize = strlen(buffer) + 1;
	FILE* file;

	if (fopen_s(&file, fileName, "wb") != 0) {
		fprintf(stderr, "Error opening font file");
        exit(4);
	}


	if (fwrite(buffer, sizeof(char), fileSize, file) != fileSize) {
		fprintf(stderr, "Failed to write all contents to file");
		exit(5);
	}
	fclose(file);

}
