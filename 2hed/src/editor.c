#include "editor.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

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


static inline int getCurrentLineLength(Editor *editor) {
	return (int) strlen(getCurrentLine(editor)->text);
}



static inline unsigned char* getCharacterUnderCursor(Editor* editor) {
	Line* currentLine = getCurrentLine(editor);
	return (unsigned char *) currentLine->text + editor->cursorCol;
}

void deleteLine(Editor *editor) {
    
}



static void insertNewLine(Editor* editor) {

    Line* newLine = (Line*)malloc(sizeof(Line));
    Line *currentLine = getCurrentLine(editor);
    newLine->prev = currentLine;

    if(currentLine->next == NULL) {
        editor->lastLine = newLine;
    }
    else
    {
        newLine->next = currentLine->next;
    }
    
    currentLine->next = newLine;

    memmove( (void *) newLine->text,
             (void *) getCharacterUnderCursor(editor),
             strlen( (char *) getCharacterUnderCursor(editor)) + 1);

    editor->lineCount++;

}




void initEditor(Editor* editor) {
	editor->lines = NULL;
	editor->cursorCol = 0;
	editor->cursorRow = 0;
	editor->lastLine = editor->lines;
	editor->lineCount = 0;
}

void freeEditor(Editor* editor) {
    
}



void addTextAtCursor(Editor* editor, char* text) {
	char* temp = (char *) getCharacterUnderCursor(editor);
	size_t bytesToShift = strlen(temp) + 1; //+1 because strlen doesn't count null terminator.
	editor->cursorCol++;
	memmove((void*)getCharacterUnderCursor(editor),
		(void*)temp,
		bytesToShift);
	*temp = *text;
}

static void moveLineUp(Line* line) {
	Line* previous = line->prev;
	strncat_s(previous->text, sizeof(previous->text), line->text, strlen(line->text));
	previous->next = line->next;
	if (line->next) {
		line->next->prev = previous;
	}
	//TODO: free the line you don't need anymore!
    free(line);
}

void backspace(Editor* editor) {
	if (editor->cursorCol <= 0 && editor->cursorRow <= 0) return;
	char* temp = (char *) getCharacterUnderCursor(editor);
	if (editor->cursorCol <= 0)
	{
		//TODO: move this to it's own function?
		Line* currentLine = getCurrentLine(editor);
		if (currentLine->prev == NULL) {
			editor->cursorCol = 0;
		}
		else {
			editor->cursorCol = (int) strlen(currentLine->prev->text);
		}
		moveLineUp(getCurrentLine(editor));
		editor->cursorRow--;
		editor->lineCount--;
		return;
	}
	else
	{
		editor->cursorCol--;
		size_t bytesToShift = strlen(temp) + 1;
		memmove((void*)getCharacterUnderCursor(editor),
		(void*)temp,
		bytesToShift);
	}
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
		if (editor->cursorRow + 1 < editor->lineCount) {
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
	if (!isalnum(*getCharacterUnderCursor(editor))) {
		editor->cursorCol = getCurrentLineLength(editor);
	}
}


void moveCursorDown(Editor* editor) {
	if ((editor->cursorRow + 1) >= editor->lineCount) return; // If you're at the bottom line, return.
	size_t nextLineLength = strlen(getCurrentLine(editor)->next->text);
	if (editor->cursorCol > nextLineLength) {
		editor->cursorCol = (int) nextLineLength;
	}
	editor->cursorRow++;

	
}

void moveText(Editor *editor, Line *dest, Line *src)
{
    if(!dest)
    {
        insertNewLine(editor);
    }
    memmove( (void *) &dest->text,
             (void *) &src->text,
             ((strlen(src->text) + 1) * 8));

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
		for (; *buffer != '\r' && *buffer != '\n' && *buffer != EOF; ++buffer, ++i) {
			line->text[i] = *buffer;
			//if (*buffer == '\0') break;
		}
		line->text[i++] = '\0';
		while (*buffer == '\r' || *buffer == '\n') ++buffer;
		appendLine(editor, line);
	}
}

void appendLine(Editor* editor, Line* line) {
	if (editor->lastLine == NULL) {
		editor->lines = line;
		editor->lastLine = line;
		line->next = NULL;
		line->prev = NULL;
	}
	else {
		editor->lastLine->next = line;
		line->prev = editor->lastLine;
		editor->lastLine = line;
		line->next = NULL;
	}

	editor->lineCount++;
}

