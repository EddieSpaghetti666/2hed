#include "editor.h"

void grow(int capacity, size_t elementSize, int sizeToGrow, void **data) {
    
    int newCapacity = capacity ? (capacity * 2) + sizeToGrow : 8 + sizeToGrow;
 
    void *pointer;
    if(*data) {
        pointer = realloc(buf_header(*data), (newCapacity * elementSize) + sizeof(buffer_header));
    }
    else {
        pointer = realloc(0, (newCapacity * elementSize) + sizeof(buffer_header));
        ((buffer_header *)pointer)->count = 0;
    }
    *data = (void *) ((buffer_header *)pointer + 1);
    buf_cap_raw(*data) = newCapacity;
}


static void insertNewLine(Editor *editor) {
    Line newLine;
    memmove( (void *) newLine.text,
             (void *) getCharacterUnderCursor(editor),
             (strlen(getCharacterUnderCursor(editor))  * 8));
    buf_push(editor->lines, newLine);
}


void initEditor(Editor *editor) {
    editor->lines = NULL;
    editor->cursorCol = 0;
    editor->cursorRow = 0;

    Line firstLine;
    firstLine.text[0] = 0;
    buf_push(editor->lines, firstLine);
}

void freeEditor(Editor *editor) {
    buf_free(editor->lines);
}

void addTextAtCursor(Editor *editor, char *text) {
    char *temp = getCharacterUnderCursor(editor);
    editor->cursorCol++;
    memmove( (void *) getCharacterUnderCursor(editor),
             (void *) temp,
             sizeof(Line));
    *temp = *text;
}

void backspace(Editor *editor) {
    if(editor->cursorCol <= 0 && editor->cursorRow <= 0) return; 
    char *temp = getCharacterUnderCursor(editor);
    if(editor->cursorCol <= 0)
    {
        editor->cursorRow--;
    }
    else
    {
        editor->cursorCol--;
    }
    memmove( (void *) getCharacterUnderCursor(editor),
             (void *) temp,
             sizeof(Line));
}

void moveCursorLeft(Editor *editor) {
    if(editor->cursorCol <= 0 && editor->cursorRow <= 0) return;
    if(editor->cursorCol <= 0) {
        editor->cursorRow--;
        editor->cursorCol = getCurrentLineLength(editor);
    }
    else {
        editor->cursorCol--;
    }       
}

void moveCursorRight(Editor *editor) {
    if(*getCharacterUnderCursor(editor) == '\n') {
        editor->cursorRow++;
        editor->cursorCol = 0;
    }
    else if(!getCharacterUnderCursor(editor)[1]) {
        if((editor->cursorRow + 1) < buf_count(editor->lines)) {
            editor->cursorRow++;
        }
        else {
            moveCursorDown(editor);
        }
    }
    else {
        editor->cursorCol++;
    }
}


void moveCursorUp(Editor *editor) {
    if(editor->cursorRow <= 0) return; // If you're at the top line, return.
    editor->cursorRow--;
    if(*getCharacterUnderCursor(editor) == '\n' ||
       *getCharacterUnderCursor(editor) == '\0') {
        editor->cursorCol = getCurrentLineLength(editor);
    }
}


void moveCursorDown(Editor *editor) {
    if((editor->cursorRow + 1) > buf_count(editor->lines)) return; // If you're at the bottom line, return.
    editor->cursorRow++;
    if(!getCharacterUnderCursor(editor)) {
        editor->cursorCol = getCurrentLineLength(editor);
    }
}


void carraigeReturn(Editor *editor) {
    addTextAtCursor(editor, "\n");
    insertNewLine(editor);
    editor->cursorRow++;
    editor->cursorCol = 0;
}

