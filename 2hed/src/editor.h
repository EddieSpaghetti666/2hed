#pragma once
#include <stdlib.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define STARTING_X_POS 0
#define STARTING_Y_POS 0

#define MAX_LINE_LENGTH 1024


typedef struct Line {
    char text[MAX_LINE_LENGTH];
    struct Line* next;
    struct Line* prev;
} Line;

typedef struct Editor {
    Line *lines;
    int cursorCol;
    int cursorRow;
    Line* lastLine;
    int lineCount;
} Editor;

typedef struct {
    int count;
    int capacity;
} buffer_header;

#define buf_header(b)       ((buffer_header *) (b) - 1)
#define buf_count_raw(b)    buf_header(b)->count
#define buf_cap_raw(b)      buf_header(b)->capacity
#define buf_count(b)        ((b) == NULL ? 0 : buf_count_raw(b))
#define buf_cap(b)          ((b) == NULL ? 0 : buf_cap_raw(b))


#define buf__add(b, n)      (buf_count_raw(b)+=(n), &(b)[buf_count_raw(b)-(n)])
#define buf_add(b, n)       (need_grow(b, n) ? buf_grow(b, n), buf__add(b, n) : buf__add(b, n))
#define buf__push(b, a)     ((b)[buf_count_raw(b)++] = (a))
#define buf_push(b, a)      need_grow(b, 1) ? buf_grow(b, 1), buf__push(b, a) : buf__push(b, a)
#define need_grow(b, n)     (((b) == NULL) || buf_count(b) + (n) > buf_cap(b) ? 1 : 0)
#define buf_grow(b, n)      grow(buf_cap(b), sizeof(*(b)), (n), (void **) &(b))
#define buf_free(b)         ((b) ? free(buf_header(b)),0 : 0)




void grow(int capacity, size_t elementSize, int sizeToGrow, void **data);
void initEditor(Editor *editor);
void freeEditor(Editor *editor);
void addTextAtCursor(Editor *editor, char *text);
void backspace(Editor *editor);
void moveCursorLeft(Editor *editor);
void moveCursorRight(Editor *editor);
void carraigeReturn(Editor *editor);
void moveCursorUp(Editor *editor);
void moveCursorDown(Editor *editor);

void createEditorFromBuffer(Editor* editor, const char* buffer, size_t bufSize);
void appendLine(Editor* editor, Line* line);
