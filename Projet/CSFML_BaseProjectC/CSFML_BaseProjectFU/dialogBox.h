/// Dialog boxes, but good ones this time.
/// Function prefix: db_

#pragma once
#include "tools.h"
#include "windowManager.h"
#include "fontManager.h"
#include "langManager.h"
#include "math.h"

#define DB_LEN_NAME 32 /// Length of the string used as the dialog box's title
#define DB_LEN_DATA 160 /// Maximum number of characters displayed at once in a dialog box
#define DB_MAX_LINES 4 /// Maximum number of lines of text in a dialog box
#define DB_LEN_LINE (DB_LEN_DATA / DB_MAX_LINES) /// Maximum number of characters on a single line

/// Data holder for the dialog box list
/// Queuing some text to display in a dialog box produces some of those objects.
/// The display function always displays the first in line.
typedef struct DBData {
	struct DBData* prev; /// Linked list architecture data
	struct DBData* next; /// Linked list architecture data
	char* name; /// Title of the dialog box (most likely the name of the character who's speaking)
	char* data; /// Contents of the dialog box
	sfVector2f pos; /// Position of the dialog box, relatively to the map
} DBData;
DBData* db_Sentinel; /// Sentinel node for the dialog box list

sfBool db_FlagNext;

/// Initializes the dialog box manager.
void db_Init();

/// Adds something to be displayed in dialog boxes.
/// This function always appends new dialog boxes at the end of the queue.
/// Very long strings can be passed as arguments in _data. The function will take care of splitting blocks of text into smaller blocks that each fit in a single dialog box.
/// \param _name - Title of the dialog box (most likely the name of the character who's speaking)
/// \param _data - Contents of the dialog box
/// \param _pos - Position of the dialog box, relatively to the map
void db_Queue(char* _name, char* _data, sfVector2f _pos);

/// Adds a single dialog box at the end of the list.
void db_Pushback(DBData* _db);

/// Destroys the currently displayed dialog box, bringing up the next one in the list.
/// If called when there are no dialog boxes on screen, this function does nothing.
void db_Next();

/// \return Whether there's a dialog box on screen
sfBool db_IsUp();

void db_Update(Window* _window);

/// Displays the current dialog box on screen.
void db_Display(struct Window* _window);

/// Destroys the currently selected dialog box and frees up allocated memory.
DBData* db_Destroy(DBData* _db);

/// Clears the dialog box list and destroys all associated resources.
void db_ClearBuffer();

