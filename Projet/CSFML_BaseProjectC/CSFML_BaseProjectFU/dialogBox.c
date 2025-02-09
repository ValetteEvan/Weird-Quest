#include "dialogBox.h"
#include "controller.h"
#include "textureManager.h"

sfText* db_RenderTxt;
sfRectangleShape* db_RenderRect;
sfTexture* dialBox_Tex;
int db_FrameX;
float db_TimerAnim;

void db_Init() {
	db_RenderTxt = sfText_create();
	db_RenderRect = sfRectangleShape_create();

	sfText_setFont(db_RenderTxt, GetFont("Lilian"));
	sfText_setColor(db_RenderTxt, sfWhite);
	sfText_setCharacterSize(db_RenderTxt, 20);

	// sfRectangleShape_setFillColor(db_RenderRect, sfBlack);
	dialBox_Tex = GetSfTexture("dialogBoxTex");
	sfRectangleShape_setTexture(db_RenderRect, dialBox_Tex, sfTrue);
	db_FrameX = 0;
	db_TimerAnim = 0.f;

	db_Sentinel = calloc(1, sizeof(DBData));
	if (!dialBox_Tex || !db_Sentinel) return;
}

void db_Queue(char* _name, char* _data, sfVector2f _pos) {
	char* cursor = _data; /// How far have we gone through scanning the passed string
	char* cursor_start = _data; /// Pointer to the remaining data that hasn't been scanned yet
	while (1) {
		int ctr = 0; /// How many characters this dialog box will contain
		while (ctr < DB_LEN_DATA) {
			ctr++;
			cursor++;
			if (*cursor == 0) break;
		}
		while (*cursor != ' ') {
			if (*cursor == 0) break;
			ctr--;
			cursor--;
		}

		DBData* new = calloc(1, sizeof(DBData));
		if (!new) return;
		new->name = calloc(strlen(_name) + 1, sizeof(char));
		new->data = calloc(cursor - cursor_start + 1, sizeof(char));
		new->pos = addVectorsrf(_pos, vector2f(-234.f, 50.f));
		if (!new->name || !new->data) return;

		new->prev = NULL;
		new->next = NULL;
		strcpy(new->name, _name);
		int ctr_copy = 0; /// Which character we are looking at
		int ctr_line = 0; /// Counter for computing line breaks
		int ctr_lines = 1; /// Number of lines in the current dialog box
		while (ctr_copy < ctr) {
			new->data[ctr_copy] = *cursor_start;
			ctr_line++; /// Scanning through the input string
			if (new->data[ctr_copy] == ' ' || new->data[ctr_copy + 1] == 0) { /// Whenever a space or an end of string is encountered...
				if (ctr_line > DB_LEN_LINE) { /// ...if we arrived beyond the max line length...
					int ctr_back = 1;
					while (new->data[ctr_copy - ctr_back] != ' ') ctr_back++; /// ...we count how many character since the previous space...
					new->data[ctr_copy - ctr_back] = '\n'; /// ...replace that space by a line break...
					ctr_line = ctr_back; /// ...and consider that all the characters we just rolled back through are part of another line
					ctr_lines++;
				}
			}
			else if (new->data[ctr_copy] == '\n') { /// Carriage return handling
				ctr_lines++;
				ctr_line = 0;
			}

			if (ctr_lines == DB_MAX_LINES + 1) { /// If we reached the maximum amount of lines...
				for (int i = 0; cursor_start + i < cursor; i++) new->data[ctr_copy - ctr_line + i] = 0; /// Add a null character to terminate this string...
				cursor = cursor_start - ctr_line; /// ...and place a amrker here to indicate we'll need to continue from there when composing the next dialog box
				break;
			}
			
			ctr_copy++;
			cursor_start++;
		}

		db_Pushback(new);
		if (*cursor == 0) break;
		cursor_start = cursor;
	}
}

void db_Pushback(DBData* _db) { 
	DBData* itr = db_Sentinel;
	while (itr->next) itr = itr->next;

	_db->prev = itr;
	itr->next = _db;
	db_FlagNext = sfTrue;
}

void db_Next() {
	if (db_IsUp()) db_Destroy(db_Sentinel->next);
}

sfBool db_IsUp() { return db_Sentinel->next != NULL; }

void db_Update(Window* _window) {
	if (db_IsUp()) {
		if (Gamepad_isButtonPressed(0, A, _window->renderWindow)) {
			if (!db_FlagNext) db_Next();
			db_FlagNext = sfTrue;
		}
		else db_FlagNext = sfFalse;
	}
	else db_FlagNext = sfFalse;
}

void db_Display(Window* _window) {
	if (!db_IsUp()) return;

	DBData* tmp = db_Sentinel->next;
	/// DB
	sfRectangleShape_setPosition(db_RenderRect, addVectorsrf(tmp->pos, vector2f(0.f, 40.f)));
	sfRectangleShape_setSize(db_RenderRect, vector2f(234.f, 74.f));
	sfRectangleShape_setScale(db_RenderRect, vector2f(2.f, 2.f));
	sfRectangleShape_setTextureRect(db_RenderRect, animatorAnim("dialogBoxTex", &db_FrameX, &db_TimerAnim, 0));

	/// Title
	sfRenderTexture_drawRectangleShape(_window->rdrt, db_RenderRect, NULL);
	sfText_setPosition(db_RenderTxt, addVectorsrf(tmp->pos, vector2f(40.f, 50.f)));
	sfText_setString(db_RenderTxt, tmp->name);
	sfRenderTexture_drawText(_window->rdrt, db_RenderTxt, NULL);

	/// Body
	sfText_setPosition(db_RenderTxt, addVectorsrf(tmp->pos, vector2f(40.f, 70.f)));
	sfText_setString(db_RenderTxt, tmp->data);
	sfRenderTexture_drawText(_window->rdrt, db_RenderTxt, NULL);
}

DBData* db_Destroy(DBData* _db) {
	if (!_db) return _db;

	DBData* p = _db->prev;
	DBData* n = _db->next;

	if (_db->name) free(_db->name);
	if (_db->data) free(_db->data);
	p->next = n;
	if (n) n->prev = p;
	free(_db);
	return n;
}

void db_ClearBuffer() {
	DBData* itr = db_Sentinel->next;
	while (itr) itr = db_Destroy(itr);
}