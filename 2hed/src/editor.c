#include "editor.h"
#include <string.h>
#include <stdio.h>

void grow(int capacity, size_t elementSize, int sizeToGrow, void** data) {

	int newCapacity = capacity ? (capacity * 2) + sizeToGrow : 8 + sizeToGrow;

	void* pointer;
	if (*data) {
		pointer = realloc(buf_header(*data), (newCapacity * elementSize) + sizeof(buffer_header));
	}
	else {
		pointer = realloc(0, (newCapacity * elementSize) + sizeof(buffer_header));
		((buffer_header*)pointer)->count = 0;
	}
	*data = (void*)((buffer_header*)pointer + 1);
	buf_cap_raw(*data) = newCapacity;
}

static Line* getCurrentLine(Editor* editor) {
	Line* line = editor->lines;
	for (int i = 0; i < editor->cursorRow; ++i) {
		line = line->next;
	}
	return line;
}


static char* getCharacterUnderCursor(Editor* editor) {
	Line* currentLine = getCurrentLine(editor);

	return currentLine->text + editor->cursorCol;
}



static void insertNewLine(Editor* editor) {
	Line newLine;
	memmove((void*)newLine.text,
		(void*)getCharacterUnderCursor(editor),
		sizeof(Line));
	buf_push(editor->lines, newLine);
}




void initEditor(Editor* editor) {
	editor->lines = NULL;
	editor->cursorCol = 0;
	editor->cursorRow = 0;
	editor->lastLine = editor->lines;
}

void freeEditor(Editor* editor) {
	//buf_free(editor->lines);
}



void addTextAtCursor(Editor* editor, char* text) {
	char* temp = getCharacterUnderCursor(editor);
	size_t bytesToShift = strlen(temp) + 1; //+1 because strlen doesn't count null terminator.
	editor->cursorCol++;
	memmove((void*)getCharacterUnderCursor(editor),
		(void*)temp,
		bytesToShift);
	*temp = *text;
}

void backspace(Editor* editor) {
	if (editor->cursorCol <= 0 && editor->cursorRow <= 0) return;
	char* temp = getCharacterUnderCursor(editor);
	if (editor->cursorCol <= 0)
	{
		editor->cursorRow--;
	}
	else
	{
		editor->cursorCol--;
	}
	memmove((void*)getCharacterUnderCursor(editor),
		(void*)temp,
		sizeof(Line));
}

void moveCursorLeft(Editor* editor) {
	if (editor->cursorCol <= 0 && editor->cursorRow <= 0) return;
	if (editor->cursorCol <= 0) {
		editor->cursorRow--;
		editor->cursorCol = getCurrentLineLength(editor);
	}
	else {
		editor->cursorCol--;
	}
}

void moveCursorRight(Editor* editor) {

	if ((editor->cursorCol + 1) > getCurrentLineLength(editor)) {
		if (editor->cursorRow + 1 < buf_count(editor->lines)) {
			editor->cursorRow++;
			editor->cursorCol = 0;
		}
		else {
			return;
		}
	}
	else {
		editor->cursorCol++;
	}
}


void moveCursorUp(Editor* editor) {
	if (editor->cursorRow <= 0) return; // If you're at the top line, return.
	editor->cursorRow--;
	if (*getCharacterUnderCursor(editor) == '\0') {
		editor->cursorCol = getCurrentLineLength(editor);
	}
}


void moveCursorDown(Editor* editor) {
	if ((editor->cursorRow + 1) >= buf_count(editor->lines)) return; // If you're at the bottom line, return.
	editor->cursorRow++;
	if (!getCharacterUnderCursor(editor)) {
		editor->cursorCol = getCurrentLineLength(editor);
	}
}


void carraigeReturn(Editor* editor) {
	addTextAtCursor(editor, "\0");
	insertNewLine(editor);
	editor->cursorRow++;
	editor->cursorCol = 0;
}

void createEditorFromBuffer(Editor* editor, const char* buffer, size_t bufSize) {
	while (*buffer != EOF) {
		Line* line = (Line*)malloc(sizeof(Line));
		int i = 0;
		for (; *buffer != '\n' && *buffer != EOF; ++buffer, ++i) {
			line->text[i] = *buffer;
			//if (*buffer == '\0') break;
		}
		if (*buffer == '\n') {
			line->text[i++] = '\n';
			line->text[i] = '\0';
			++buffer;
		}
		if (*buffer == EOF) {
			line->text[i] = '\0';
		}
		appendLine(editor, line);
	}
}

void appendLine(Editor* editor, Line* line) {
	if (editor->lastLine == NULL) {
		editor->lines = line;
		editor->lastLine = line;
		line->next = NULL;
		line->prev = NULL;
		return;
	}
	else {
		editor->lastLine->next = line;
		line->prev = editor->lastLine;
		editor->lastLine = line;
		line->next = NULL;
	}
}