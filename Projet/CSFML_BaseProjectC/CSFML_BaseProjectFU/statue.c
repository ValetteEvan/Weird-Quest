#include "statue.h"
#include "tools.h"
#include "math.h"
#include "npc.h"
#include "player.h"
#include "controller.h"
#include "dialogBox.h"
#include "quest.h"

stdList* stt_List;  
//sfSprite* spriteStatus; 
sfRectangleShape* stt_RenderRect;
sfRectangleShape* stt_MenuRect;
sfText* stt_MenuText;
sfRectangleShape* stt_CursorRect;

char stt_MenuSelection;
float stt_Timer, stt_Cooldown;
float stt_AnimTimer = 0.f;

void stt_Init(Window* _window) {
	static sfBool onePass = sfTrue;
	if (onePass) {
		onePass = sfFalse;
		stt_List = STD_LIST_CREATE(Statue, 0);
		stt_RenderRect = sfRectangleShape_create();
		stt_CursorRect = sfRectangleShape_create();
		stt_MenuText = sfText_create();
		stt_MenuRect = sfRectangleShape_create();

		sfText_setFont(stt_MenuText, GetFont("UGp"));
		sfText_setColor(stt_MenuText, sfWhite);
		sfText_setCharacterSize(stt_MenuText, 20);

		sfRectangleShape_setFillColor(stt_MenuRect, sfBlack);
		sfRectangleShape_setOutlineColor(stt_MenuRect, sfWhite);
		sfRectangleShape_setOutlineThickness(stt_MenuRect, 3.f);

		sfRectangleShape_setSize(stt_CursorRect, vector2f(10.f, 10.f));
	}

	stt_MenuSelection = 0;
	stt_Timer = 0.f;
	stt_Cooldown = 0.f;
}

void stt_Update(Window* _window) {
	//stt_AnimTimer += getDeltaTime();
	//if (stt_List == NULL || stt_List->size(stt_List) == 0) return;


	//for (int i = 0; i < stt_List->size(stt_List); i++) {
	//	Statue* tmp = STD_LIST_GETDATA(stt_List, Statue, i);
	//	if (tmp->type == STATUE_TYPE_1) {
	//		//! Mettre la zone de détection ici avec le display de la dialogue box : 
	//	}
	//}

	//if (stt_Cooldown > 0.f) stt_Cooldown -= getDeltaTime();
	//if (stt_Timer < .3f) stt_Timer += getDeltaTime();
	////if (plr_IsInteractingWithStatue()) {
	////	if (stt_Timer > .25f && stt_Cooldown <= 0.f) {
	////		if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).y < 0.f || Gamepad_isButtonPressed(0, DPAD_UP, _window->renderWindow)) {
	////			stt_MenuSelection--;
	////			if (stt_MenuSelection < 0) stt_MenuSelection = 3;
	////			stt_Timer = 0.0f;
	////		}
	////		else if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).y > 0.f || Gamepad_isButtonPressed(0, DPAD_DOWN, _window->renderWindow)) {
	////			stt_MenuSelection++;
	////			if (stt_MenuSelection > 3) stt_MenuSelection = 0;
	////			stt_Timer = 0.0f;
	////		}

	////		/*if (Gamepad_isButtonPressed(0, A, _window->renderWindow)) {
	////			char* name = lang_GetFromKey("name.statue"); /// Alias to make things more readable
	////			for (int i = 0; i < stt_List->size(stt_List); i++) {
	////				switch (stt_MenuSelection) {
	////					case 0:
	////						/// Save game
	////						db_Queue(name, lang_GetFromKey("statue.response.save"), STD_LIST_GETDATA(stt_List, Statue, i)->pos);
	////						break;
	////					case 1:
	////						/// Advice depends on what's currently going on
	////						if (qst_IsOngoing(QST_ID_P_JOY)) db_Queue(name, lang_GetFromKey("statue.advice.joy_temple"), STD_LIST_GETDATA(stt_List, Statue, i)->pos);
	////						else if (qst_IsOngoing(QST_ID_S_FISH_JOY)) db_Queue(name, lang_GetFromKey("statue.advice.fish_joy"), STD_LIST_GETDATA(stt_List, Statue, i)->pos);
	////						else if (qst_IsOngoing(QST_ID_S_GET_YOYO)) db_Queue(name, lang_GetFromKey("statue.advice.get_yoyo"), STD_LIST_GETDATA(stt_List, Statue, i)->pos);
	////						else if (qst_IsOngoing(QST_ID_S_SEE_SHAMAN)) db_Queue(name, lang_GetFromKey("statue.advice.see_shaman"), STD_LIST_GETDATA(stt_List, Statue, i)->pos);
	////						else if (qst_IsOngoing(QST_ID_S_KILL_SNAKES)) db_Queue(name, lang_GetFromKey("statue.advice.snakes"), STD_LIST_GETDATA(stt_List, Statue, i)->pos);
	////						else db_Queue(name, lang_GetFromKey("statue.advice.none"), STD_LIST_GETDATA(stt_List, Statue, i)->pos);
	////						break;
	////					case 2:
	////						/// /tp Xx_pha0s_xX The_Mentor_Ever
	////						db_Queue(name, lang_GetFromKey("statue.response.warp"), STD_LIST_GETDATA(stt_List, Statue, i)->pos);
	////						break;
	////					case 3:
	////						/// Nothing
	////						db_Queue(name, lang_GetFromKey("statue.response.back"), STD_LIST_GETDATA(stt_List, Statue, i)->pos);
	////						break;
	////				}
	////			}
	////			plr_SetInteractStatue(sfFalse);
	////			stt_SetInteractCooldown(.5f);
	////			db_FlagNext = sfTrue;
	////		}*/
	////	}

	////	if (Gamepad_isButtonPressed(0, B, _window->renderWindow)) stt_MenuSelection = 3;
	////}
	//else {
	//	if (db_IsUp()) stt_Cooldown = .5f;
	//	stt_MenuSelection = 0;
	//}
}

void stt_Display(Window* _window) {
	if (stt_List == NULL || stt_List->size(stt_List) == 0)
		return; 

	for (int i = 0; i < stt_List->size(stt_List); i++) {
		Statue* tmp = STD_LIST_GETDATA(stt_List, Statue, i); 

		if (tmp->pos.x < mainView->viewPos.x - (mainView->size.x / 2.f) - 312.f || tmp->pos.x > mainView->viewPos.x + (mainView->size.x / 2.f) + 312.f || tmp->pos.y < mainView->viewPos.y - (mainView->size.y / 2.f) - 324.f || tmp->pos.y > mainView->viewPos.y + (mainView->size.y / 2.f) + 324.f)
			continue;

		if (tmp->type == STATUE_TYPE_1) {
			sfRectangleShape_setFillColor(stt_RenderRect, sfColor_fromRGBA(255, 255, 255, 255));
			sfRectangleShape_setSize(stt_RenderRect, tmp->size);
			sfRectangleShape_setPosition(stt_RenderRect, tmp->pos);
			sfRectangleShape_setTexture(stt_RenderRect, GetSfTexture("assets_general"), sfTrue);
			sfRectangleShape_setTextureRect(stt_RenderRect, IntRect(312 * ((int)(stt_AnimTimer * 2.f) % 4), 0, 312, 324));
			
			sfRenderTexture_drawRectangleShape(_window->rdrt, stt_RenderRect, NULL);    
		}
	}
}

void stt_DisplayMenu(Window* _window) {
	/*sfVector2f v = substractVectorsrf(mainView->viewPos, vector2f(960.f, 540.f));

	if (!plr_IsInteractingWithStatue()) return;
	sfRectangleShape_setPosition(stt_MenuRect, addVectorsrf(v, vector2f(15.f, 730.f)));
	sfRectangleShape_setSize(stt_MenuRect, vector2f(750.f, 120.f));
	sfRenderTexture_drawRectangleShape(_window->rdrt, stt_MenuRect, NULL);
	sfText_setPosition(stt_MenuText, addVectorsrf(v, vector2f(25.f + (stt_MenuSelection == 0 ? 20.f : 0.f), 740.f)));
	sfText_setString(stt_MenuText, lang_GetFromKey("statue.choice.save"));
	sfRenderTexture_drawText(_window->rdrt, stt_MenuText, NULL);			   
	sfText_setPosition(stt_MenuText, addVectorsrf(v, vector2f(25.f + (stt_MenuSelection == 1 ? 20.f : 0.f), 765.f)));
	sfText_setString(stt_MenuText, lang_GetFromKey("statue.choice.advice"));
	sfRenderTexture_drawText(_window->rdrt, stt_MenuText, NULL);			   
	sfText_setPosition(stt_MenuText, addVectorsrf(v, vector2f(25.f + (stt_MenuSelection == 2 ? 20.f : 0.f), 790.f)));
	sfText_setString(stt_MenuText, lang_GetFromKey("statue.choice.warp"));
	sfRenderTexture_drawText(_window->rdrt, stt_MenuText, NULL);			   
	sfText_setPosition(stt_MenuText, addVectorsrf(v, vector2f(25.f + (stt_MenuSelection == 3 ? 20.f : 0.f), 815.f)));
	sfText_setString(stt_MenuText, lang_GetFromKey("statue.choice.back"));
	sfRenderTexture_drawText(_window->rdrt, stt_MenuText, NULL);
	sfRectangleShape_setPosition(stt_CursorRect, addVectorsrf(v, vector2f(25.f, 747.5f + 25.f * stt_MenuSelection)));
	sfRenderTexture_drawRectangleShape(_window->rdrt, stt_CursorRect, NULL);*/
}

void stt_Create(StatueType _type, sfVector2f _pos) {
	Statue tmp = { 0 };
	switch (_type) 
	{
		case STATUE_TYPE_1: 
			tmp.size = vector2f(312.f, 324.f); 
			tmp.pos = _pos;
			//tmp.pos = addVectorsrf(tmp.pos, multiplyVectorsrf(tmp.size, 0.5f));
			tmp.color = sfCyan; 
			tmp.type = _type; 
			break; 
	}
	stt_List->push_back(&stt_List, &tmp);
}

stdList* stt_GetList() {
	return stt_List; 
}

Statue* stt_GetToSave() {
	if (stt_List == NULL) return NULL; 

	int len = stt_List->size(stt_List); 
	if (len == 0) return NULL;
	Statue* tmp = calloc(len, sizeof(Statue));
	if (tmp == NULL) return NULL; 

	for (int i = 0; i < len; i++) {
		tmp[i].pos = STD_LIST_GETDATA(stt_List, Statue, i)->pos;
	}
	return tmp; 
}

sfFloatRect stt_GetBounds(Statue* _status) {
	if (_status == NULL) return (sfFloatRect) { 0 }; 
	return FlRect(_status->pos.x, _status->pos.y, _status->size.x, _status->size.y);
}

sfVector2f* stt_GetCollider(Statue* _statue) {
	if (_statue == NULL) return NULL; 

	sfVector2f tab[4]; 
	tab[0] = _statue->pos;
	tab[1] = addVectorsrf(_statue->pos, vector2f(_statue->size.x, 0.f));
	tab[2] = addVectorsrf(_statue->pos, vector2f(_statue->size.x, _statue->size.y));
	tab[3] = addVectorsrf(_statue->pos, vector2f(0.f, _statue->size.y));
	return tab;
}

sfVector2f stt_GetColliderSinglePoint(int _point, Statue* _statue) {
	if (_statue == NULL) return NULLVECTF;

	switch (_point) {
		case 0: return _statue->pos;
		case 1: return addVectorsrf(_statue->pos, vector2f(_statue->pos.x, 0.f));
		case 2: return addVectorsrf(_statue->pos, vector2f(_statue->pos.x, _statue->pos.y));
		case 3: return addVectorsrf(_statue->pos, vector2f(0.f, _statue->pos.y));
		default: return _statue->pos;
	}
}

float stt_GetInteractCooldown() { return stt_Cooldown; }
void stt_SetInteractCooldown(float _cd) { stt_Cooldown = _cd; }

void stt_clear()
{
	if (!stt_List) return;
	stt_List->clear(&stt_List);
}
