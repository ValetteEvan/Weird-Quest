#include "note.h"
#include "langManager.h"
#include "fontManager.h"
#include "pause.h"
#include "player.h"
#include "game_HUD.h"
#include "textureManager.h"
#include "viewManager.h"
#include "map.h"
#include "controller.h"
#include "dialogBox.h"
#include "quest.h"

sfSprite* note_Sprite;

sfText* note_Text;

static float note_timer = 0.f;

void note_InitNote()
{
	note_ListNote = stdList_Create(sizeof(Note), 0);
	note_Sprite = sfSprite_create();
	note_Text = sfText_create();
	sfText_setFont(note_Text, GetFont("arial"));
}

void note_AddNote(sfVector2f _pos)
{
	Note tmp = { 0 };
	tmp.pos = _pos;

	if (map_GetState() == MAP_STATE_OVERWORLD)
	{
		if (getSqrMagnitudeBetween2Vectorsf(tmp.pos, vector2f(4730.f, 1390.f)) < 20000.f) {
			tmp.instruction = NOTE_CASCADE;
			tmp.type = NOTE_DRY;
		}
		else if (getSqrMagnitudeBetween2Vectorsf(tmp.pos, vector2f(6070.f, 8140.f)) < 20000.f) {
			tmp.instruction = NOTE_BEACH_1;
			tmp.type = NOTE_DRY;
		}
		else if (getSqrMagnitudeBetween2Vectorsf(tmp.pos, vector2f(9190.f, 9510.f)) < 20000.f) {
			tmp.instruction = NOTE_JOY_DUNGEON;
			tmp.type = NOTE_FLOATING;
		}
		else {
			tmp.instruction = NOTE_NO_MESSAGE;
			tmp.type = NOTE_DRY;
		}
	}

	if (tmp.type == NOTE_DRY) tmp.bounds = FlRect(tmp.pos.x, tmp.pos.y, 60.f, 98.f);
	else if (tmp.type == NOTE_FLOATING) tmp.bounds = FlRect(tmp.pos.x, tmp.pos.y, 84.f, 86.f);

	tmp.isRead = sfFalse;
	tmp.idDB = 0;
	note_ListNote->push_back(&note_ListNote, &tmp);
}

void note_UpdateNote(Window* _window)
{
	if (!note_ListNote) return;
	char* name = lang_GetFromKey("name.note_mentor"); /// Alias to make things more readable

	if (!db_IsUp()) note_timer += getDeltaTime();
	for (int i = 0; i < note_ListNote->size(note_ListNote); i++)
	{
		Note* tmp = STD_LIST_GETDATA(note_ListNote, Note, i); /// Alias to make things more readable

		if (sfFloatRect_intersects(getPointerPlayerBounds(), &(tmp->bounds), NULL))
		{
			plr_toggleFX(sfTrue); 
			if (Gamepad_isButtonPressed(0, A, _window->renderWindow) && note_timer > 0.5)
			{
				db_FlagNext = sfTrue;
				note_timer = 0.f;
				tmp->isRead = sfTrue;

				switch (STD_LIST_GETDATA(note_ListNote, Note, i)->instruction) {
					case NOTE_CASCADE:
						db_Queue(name, lang_GetFromKey("notes.world.intro"), tmp->pos);
						break;
					case NOTE_BEACH_1:
						db_Queue(name, lang_GetFromKey("notes.world.temple_joy"), tmp->pos);
						qst_Create(QST_ID_S_FISH_JOY, PAUSE_SHAMAN);
						break;
					case NOTE_JOY_DUNGEON:
						db_Queue(name, lang_GetFromKey("notes.world.temple_sadness"), tmp->pos);
						break;
				}
			}
		}
	}
}

void note_Deinit()
{
	if (note_ListNote)
	{
		for (int i = 0; i < note_ListNote->size(note_ListNote); i++)
			note_ListNote->erase(&note_ListNote, i);
		note_ListNote->destroy(&note_ListNote);
	}
	sfSprite_destroy(note_Sprite);
	sfText_destroy(note_Text);

}

void note_clearNoteList()
{
	if (!note_ListNote) return;
	note_ListNote->clear(&note_ListNote);
}
