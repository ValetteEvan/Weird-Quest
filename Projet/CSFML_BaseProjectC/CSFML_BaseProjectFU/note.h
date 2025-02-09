#pragma once
#include "tools.h"
#include "List.h"
#include "windowManager.h"

typedef enum
{
	NOTE_NO_MESSAGE,
	NOTE_CASCADE,
	NOTE_BEACH_1,
	NOTE_JOY_DUNGEON
} NoteMessage;

typedef enum {
	NOTE_DRY,
	NOTE_FLOATING
} NoteType;

typedef struct {
	NoteMessage instruction;
	sfVector2f pos;
	sfFloatRect bounds;
	sfBool isRead;
	int idDB;
	int frameX;
	float timerAnim;
	NoteType type;
}Note;

typedef struct {
	NoteMessage message;
	sfVector2f pos;
}NoteListToSave;

stdList* note_ListNote;

void note_InitNote();

void note_AddNote(sfVector2f _pos);

void note_UpdateNote(Window* _window);

void note_Deinit();

void note_clearNoteList();