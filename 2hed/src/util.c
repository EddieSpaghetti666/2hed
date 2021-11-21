#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


size_t loadFileIntoBuffer(const char* fileName, char* buffer) {
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
	buffer[result] = EOF;

	if (result != fileSize) {
		fprintf(stderr, "Couldn't read entire font file");
		exit(3);
	}
	fclose(file);

	return result;
}

void saveFile(const char* fileName, Editor *editor) {
	//size_t fileSize = strlen(buffer) + 1;
	FILE* file;

	if (fopen_s(&file, fileName, "wb") != 0) {
		fprintf(stderr, "Error opening font file");
        exit(4);
	}

    Line *line = editor->lines;
    for(size_t row = 0; row < editor->lineCount; row++) {
        size_t lineSize = strlen(line->text) + 1;
        if (fwrite(line->text, sizeof(char), lineSize, file) != lineSize) {
            fprintf(stderr, "Failed to write all contents to file");
            exit(5);
        }
        fputc('\n', file);
        line = line->next;
        
    }
	fclose(file);

}
