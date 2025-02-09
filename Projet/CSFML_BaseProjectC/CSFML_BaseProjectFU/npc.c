#include "npc.h"
#include "player.h"
#include "dialogBox.h"
#include "textureManager.h"
#include "pause.h"
#include "controller.h"
#include "map.h"
#include "game_HUD.h"
#include "quest.h"
#include "fish.h"
#include "game.h"

#define NPC_REACH 100.f

sfRectangleShape* npc_EditorRenderRect;
sfTexture* NPC_Antagoniste;
sfTexture* NPC_Arnold;
sfTexture* NPC_Shamane;
sfTexture* NPC_Mentor;
sfTexture* NPC_Speaking;

float npc_TimerInteract;
sfBool npc_CanInteractCollector;
sfBool npc_CanInteractShaman;
sfBool NPC_OnePass;

void npc_loadTexture()
{
	NPC_Antagoniste = GetSfTexture("antagoniste");
	NPC_Arnold = GetSfTexture("arnold");
	NPC_Mentor = GetSfTexture("mentor");
	NPC_Shamane = GetSfTexture("shamane");
	NPC_Speaking = GetSfTexture("feedback_exclam");
}

void npc_Init(Window* _window) {
	npc_List = STD_LIST_CREATE(Npc, 0);

	npc_EditorRenderRect = sfRectangleShape_create();
	sfRectangleShape_setSize(npc_EditorRenderRect, vector2f(164.f, 164.f));
	npc_loadTexture();
	npc_TimerInteract = 0.f;
	NPC_OnePass = sfTrue;
	npc_CanInteractCollector = sfTrue;
	npc_CanInteractShaman = sfFalse;
}

void npc_QueueDialogs() {
	db_FlagNext = sfTrue;
	for (int i = 0; i < npc_List->size(npc_List); i++) {
		Npc* tmp = STD_LIST_GETDATA(npc_List, Npc, i);
		if (sfFloatRect_intersects(&tmp->interact, getPointerPlayerBounds(), NULL)) {
			if (tmp->noDial) tmp->getNoDial(tmp);
			else tmp->getDialog(tmp);
		}
	}
}

void npc_Update(Window* _window) {
	if (npc_TimerInteract < 1.f) npc_TimerInteract += getDeltaTime();
	for (int i = 0; i < npc_List->size(npc_List); i++) {
		Npc* tmp = STD_LIST_GETDATA(npc_List, Npc, i);
		if (sfFloatRect_intersects(&tmp->interact, getPointerPlayerBounds(), NULL)) {
			static sfBool flag = sfFalse;
			if ((customKeyboardIsKeyPressed(sfKeyEnter, _window->renderWindow) || Gamepad_isButtonPressed(0, A, _window->renderWindow))) {
				if (npc_TimerInteract > .5f) {
					if (!db_IsUp()) npc_QueueDialogs();
					else {
						npc_TimerInteract = 0.f;
						tmp->changeDial = sfTrue;
						plr_SetVel(vector2f(0.f, 0.f));
						plr_setPlayerVelocity(vector2f(0.f, 0.f));
						plr_SetAllTimer(0.f);
					}
					flag = sfTrue;
				}
			}
			else {
				if (flag) {
					npc_TimerInteract = 0.f;
					flag = sfFalse;
					if (!tmp->isMagic && tmp->changeDial) {
						tmp->frameX = 0;
						tmp->changeDial = sfFalse;
					}
				}
			}
		}
		tmp->isSpeaking = db_IsUp();

		if (tmp->id == NPC_SHAMAN) tmp->noDial = !npc_CanInteractShaman;
		if (tmp->id == NPC_ARNOLD) tmp->noDial = !npc_CanInteractCollector;
	}
}

void npc_Display(Window* _window) {
	for (int i = 0; i < npc_List->size(npc_List); i++)
	{
		Npc* tmp = STD_LIST_GETDATA(npc_List, Npc, i);
		if (tmp->id == 1) {
			sfRectangleShape_setTexture(npc_EditorRenderRect, NPC_Arnold, sfTrue);
			if (tmp->isSpeaking) {
				tmp->texRect = animatorAnim("arnold_dialogue", &tmp->frameX, &tmp->timerAnim, 0);
				sfRectangleShape_setSize(npc_EditorRenderRect, vector2f(118, 126));
			}
			else {
				tmp->texRect = animatorAnim("arnold", &tmp->frameX, &tmp->timerAnim, 0);
				sfRectangleShape_setSize(npc_EditorRenderRect, vector2f(118, 126));
			}
		}
		else if (tmp->id == 2) {
			sfRectangleShape_setTexture(npc_EditorRenderRect, NPC_Antagoniste, sfTrue);
			tmp->texRect = animatorAnim("antagoniste", &tmp->frameX, &tmp->timerAnim, 0);
			sfRectangleShape_setSize(npc_EditorRenderRect, vector2f(140, 180));
		}
		else if (tmp->id == 3) {
			sfRectangleShape_setTexture(npc_EditorRenderRect, NPC_Mentor, sfTrue);
			if (tmp->isBlooding) {
				tmp->texRect = animatorAnim("mentor_blood", &tmp->frameX, &tmp->timerAnim, 0);
				sfRectangleShape_setSize(npc_EditorRenderRect, vector2f(134, 140));
			}
			else if (tmp->isSpeaking) {
				tmp->texRect = animatorAnim("mentor_dialogue", &tmp->frameX, &tmp->timerAnim, 0);
				sfRectangleShape_setSize(npc_EditorRenderRect, vector2f(132, 160));
			}
			else if (tmp->isBackwards) {
				tmp->texRect = animatorAnim("mentor_backwards", &tmp->frameX, &tmp->timerAnim, 0);
				sfRectangleShape_setSize(npc_EditorRenderRect, vector2f(136, 160));
			}
			else {
				tmp->texRect = animatorAnim("mentor", &tmp->frameX, &tmp->timerAnim, 0);
				sfRectangleShape_setSize(npc_EditorRenderRect, vector2f(136, 160));
			}
		}
		else if (tmp->id == 4) {
			sfRectangleShape_setTexture(npc_EditorRenderRect, NPC_Shamane, sfTrue);
			sfRectangleShape_setSize(npc_EditorRenderRect, vector2f(164, 164));

			if (tmp->isMagic) tmp->texRect = animatorAnim("shamane_magic", &tmp->frameX, &tmp->timerAnim, 0);
			else if (tmp->isSpeaking) tmp->texRect = animatorAnim("shamane_dialogue", &tmp->frameX, &tmp->timerAnim, 0);
			else tmp->texRect = animatorAnim("shamane", &tmp->frameX, &tmp->timerAnim, 0);
		}
		sfRectangleShape_setTextureRect(npc_EditorRenderRect, tmp->texRect);
		sfRectangleShape_setPosition(npc_EditorRenderRect, tmp->position);
		sfRenderTexture_drawRectangleShape(_window->rdrt, npc_EditorRenderRect, NULL);
		tmp->interact = (sfFloatRect){ sfRectangleShape_getGlobalBounds(npc_EditorRenderRect).left - NPC_REACH, sfRectangleShape_getGlobalBounds(npc_EditorRenderRect).top - NPC_REACH, sfRectangleShape_getGlobalBounds(npc_EditorRenderRect).width + NPC_REACH * 2.f, sfRectangleShape_getGlobalBounds(npc_EditorRenderRect).height + NPC_REACH * 2.f };
		if (!tmp->noDial) {
			sfRectangleShape_setTexture(npc_EditorRenderRect, NPC_Speaking, sfTrue);
			sfRectangleShape_setTextureRect(npc_EditorRenderRect, animatorAnim(tmp->isSpeaking ? "feedback_speaking" : "feedback_exclam", &tmp->NPC_FrameX, &tmp->NPC_TimerAnim, 0));
			if (tmp->id == 1) sfRectangleShape_setPosition(npc_EditorRenderRect, vector2f(tmp->position.x + 25.f, tmp->position.y - 75.f));
			else if (tmp->id == 3) sfRectangleShape_setPosition(npc_EditorRenderRect, vector2f(tmp->position.x + 36.f, tmp->position.y - 75.f));
			else if (tmp->id == 4) sfRectangleShape_setPosition(npc_EditorRenderRect, vector2f(tmp->position.x + 52.f, tmp->position.y - 75.f));
			sfRectangleShape_setSize(npc_EditorRenderRect, vector2f(64.f, 64.f));
			sfRenderTexture_drawRectangleShape(_window->rdrt, npc_EditorRenderRect, NULL);
		}
		if (getState() == GAME && sfFloatRect_intersects(&tmp->interact, getPointerPlayerBounds(), NULL)) HUD_DisplayPressA(vector2f(tmp->interact.left + 74.f, tmp->interact.top + 74.f), _window);
	}
}

void npc_Deinit() {
	npc_List->clear(&npc_List);
	sfRectangleShape_destroy(npc_EditorRenderRect);
}

void npc_Create(sfVector2f _position, quetePNJ _id)
{
	Npc tmp = { 0 };
	tmp.position = _position;
	tmp.id = _id;
	tmp.frameX = 0;
	tmp.timerAnim = 0.f;
	tmp.isSpeaking = sfFalse;
	tmp.isBlooding = sfFalse;
	tmp.isBackwards = sfFalse;
	tmp.isMagic = sfFalse;
	tmp.dialId = 1;
	tmp.noDial = sfFalse;
	tmp.onePass = sfTrue;
	tmp.NPC_FrameX = 0;
	tmp.NPC_TimerAnim = 0.f;
	tmp.changeDial = sfFalse;
	tmp.texRect = (sfIntRect){ 0, 0, 0, 0 };
	tmp.dbPos = vector2f(tmp.position.x, tmp.position.y - 180.f);

	switch (tmp.id) {
	case NPC_ARNOLD:
		tmp.getDialog = npc_GetDialogArnold;
		tmp.getNoDial = npc_GetNoDialArnold;
		break;
	case NPC_BBEG:
		tmp.getDialog = npc_GetDialogBBEG;
		tmp.getNoDial = NULL;
		break;
	case NPC_MENTOR:
		tmp.getDialog = npc_GetDialogMentor;
		tmp.getNoDial = NULL;
		break;
	case NPC_SHAMAN:
		tmp.getDialog = npc_GetDialogShaman;
		tmp.getNoDial = npc_GetNoDialShaman;
		break;
	default:
		tmp.getDialog = NULL;
		tmp.getNoDial = NULL;
		/// Gustavo: (null)
		break;
	}

	npc_List->push_back(&npc_List, &tmp);
}

NpcToSave* getNpcListToSave() {
	if (!npc_List) return NULL;

	int len = npc_List->size(npc_List);
	NpcToSave* tmp = calloc(len, sizeof(NpcToSave));
	if (!tmp) return NULL;

	for (int i = 0; i < len; i++) {
		tmp[i].position = STD_LIST_GETDATA(npc_List, Npc, i)->position;
		tmp[i].id = STD_LIST_GETDATA(npc_List, Npc, i)->id;
	}
	return tmp;
}

void npc_GetDialogArnold(Npc* _npc) {
	if (!npc_CanInteractCollector) npc_GetNoDialArnold(_npc);

	char* name = lang_GetFromKey("name.arnold"); /// Alias to make it more readable

	switch (qst_GetState(QST_ID_S_KILL_SNAKES)) {
		case QST_STATE_UNDISCOVERED: /// If just arrived on the island
			db_Queue(name, lang_GetFromKey("dialog.arnold.snakes"), _npc->dbPos);
			qst_Create(QST_ID_S_KILL_SNAKES, PAUSE_ARNOLD);
			return;
		case QST_STATE_PROGRESS: /// Reminder to go kill some snakes
			db_Queue(name, lang_GetFromKey("dialog.arnold.snakes_reminder"), _npc->dbPos);
			return;
		case QST_STATE_FINISH: /// Completing snake quest, invitation to go visit the shaman
			plr_SetUnlockState(PLRUNL_RING_CAPTURE);
			db_Queue(name, lang_GetFromKey("dialog.arnold.snakes_complete"), _npc->dbPos);
			db_Queue(name, lang_GetFromKey("dialog.arnold.see_shaman"), _npc->dbPos);
			//DROP ITEMS RING
			pause_itemRecup(PAUSE_JOY_RING); 
			qst_Archive(QST_ID_S_KILL_SNAKES);
			qst_Create(QST_ID_S_SEE_SHAMAN, PAUSE_ARNOLD);
			npc_CanInteractShaman = sfTrue;
			/// Give ring
			return;
		case QST_STATE_ARCHIVED: break;
	}

	/// Reminder to go visit the shaman
	if (qst_IsOngoing(QST_ID_S_SEE_SHAMAN)) {
		db_Queue(name, lang_GetFromKey("dialog.arnold.see_shaman"), _npc->dbPos);
		return;
	}

	/// Spider-catching tutorial
	if (qst_IsOngoing(QST_ID_S_GET_YOYO)) {
		static sfBool flag = sfFalse; /// Have we already received the tutorial?
		if (!flag) {
			flag = sfTrue;
			plr_SetUnlockState(PLRUNL_SHOULDER_CHARGE);
			db_Queue(name, lang_GetFromKey("dialog.arnold.get_yoyo.desc_1"), _npc->dbPos);
			db_Queue(name, lang_GetFromKey("dialog.arnold.get_yoyo.desc_2"), _npc->dbPos);
			db_Queue(name, lang_GetFromKey("dialog.arnold.get_yoyo.desc_3"), _npc->dbPos);
			db_Queue(name, lang_GetFromKey("dialog.arnold.get_yoyo.desc_4"), _npc->dbPos);
			db_Queue(name, lang_GetFromKey("dialog.arnold.get_yoyo.desc_5"), _npc->dbPos);
			db_Queue(name, lang_GetFromKey("dialog.arnold.get_yoyo.desc_6"), _npc->dbPos);
		}
		else db_Queue(name, lang_GetFromKey("dialog.arnold.greet"), _npc->dbPos);
		// TODO Need to take care of scripting the whole tutorial
		return;
	}

	if (!qst_IsDiscovered(QST_ID_S_FISH_JOY)) {
		if (map_GetState() == MAP_STATE_FOREST) {
			db_Queue(name, lang_GetFromKey("He seems lost in thought"), _npc->dbPos); // TODO Actual JSON key there
			return;
		}
		db_Queue(name, lang_GetFromKey("dialog.arnold.fish"), _npc->dbPos);
		qst_Create(QST_ID_S_FISH_JOY, PAUSE_ARNOLD);
		return;
	}
	else if (qst_IsOngoing(QST_ID_S_FISH_JOY)) {
		game_ChangeFromGameToFishTuto();
		return;
	}
}

void npc_GetDialogBBEG(Npc* _npc) {
	// TODO
}

void npc_GetDialogMentor(Npc* _npc) {
	// TODO
}

void npc_GetDialogShaman(Npc* _npc) {
	if (!npc_CanInteractShaman) npc_GetNoDialShaman(_npc);

	char* name = lang_GetFromKey("name.shamane"); /// Alias to make it more readable

	/// If just arrived on the island
	if (!qst_IsDiscovered(QST_ID_S_KILL_SNAKES)) {
		static sfBool flag = sfFalse; /// Have we already talked to the shaman?
		if (!flag) {
			flag = sfTrue;
			db_Queue(name, lang_GetFromKey("dialog.shaman.forest_1"), _npc->dbPos);
		}
		else db_Queue(name, lang_GetFromKey("dialog.shaman.forest_2"), _npc->dbPos);
		return;
	}

	/// Assigning the yoyo spider quest
	if (qst_IsOngoing(QST_ID_S_SEE_SHAMAN)) {
		db_Queue(name, lang_GetFromKey("dialog.shaman.get_yoyo"), _npc->dbPos);
		qst_Archive(QST_ID_S_SEE_SHAMAN);
		qst_Create(QST_ID_S_GET_YOYO, PAUSE_SHAMAN);
		return;
	}

	/// Reminder to go get these juicy pavouci
	if (qst_IsOngoing(QST_ID_S_GET_YOYO)) {
		db_Queue(name, lang_GetFromKey("dialog.shaman.get_yoyo_reminder"), _npc->dbPos);
		return;
	}

	/// Turning in the spiders and receiving the pig's totem hood
	if (qst_GetState(QST_ID_S_GET_YOYO) == QST_STATE_FINISH) {
		plr_SetUnlockState(PLRUNL_PIG_HAT);
		db_Queue(name, lang_GetFromKey("dialog.shaman.get_yoyo_complete"), _npc->dbPos);
		pause_itemRemove(PAUSE_YOYO, 3);

		/// Give pig totem hood
		pause_itemRecup(PAUSE_TOTEM_PIG); 
		qst_Archive(QST_ID_S_GET_YOYO);
		return;
	}

	/// Pig's totem hood tutorial
	if (qst_IsComplete(QST_ID_S_GET_YOYO)) {
		static sfBool flag = sfFalse; /// Have we already received the tutorial?
		if (!flag) {
			flag = sfTrue;
			db_Queue(name, lang_GetFromKey("dialog.shaman.hood_pig_1"), _npc->dbPos);
			db_Queue(name, lang_GetFromKey("dialog.shaman.hood_pig_2"), _npc->dbPos);
			db_Queue(name, lang_GetFromKey("dialog.shaman.hood_pig_3"), _npc->dbPos);
			npc_CanInteractShaman = sfFalse;
			return;
		}
	}
}

void npc_GetNoDialArnold(Npc* _npc) {
	db_Queue(lang_GetFromKey("name.arnold"), lang_GetFromKey("arnold.wait"), _npc->dbPos);
}

void npc_GetNoDialShaman(Npc* _npc) {
	db_Queue(lang_GetFromKey("name.shamane"), lang_GetFromKey("dialog.shaman.greet"), _npc->dbPos);
}

void npc_Clear() {
	if (!npc_List) return;
	npc_List->clear(&npc_List);
}