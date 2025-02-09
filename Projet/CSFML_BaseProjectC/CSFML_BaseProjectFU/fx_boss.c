#include "fx_boss.h"
#include "player.h"

sfRectangleShape* fx_Explosion;  
sfRectangleShape* fx_Impact;  

void fx_Init1(Window* _window)
{
	fxList = STD_LIST_CREATE(FX, 0);
	fx_Explosion = sfRectangleShape_create(); 
}

void fx_Init2(Window* _window)
{
	fxList2 = STD_LIST_CREATE(FX2, 0);
	fx_Impact = sfRectangleShape_create();
}

void fx_Update1(Window* _window)
{
	if (fxList == NULL || fxList->size(fxList) == 0)
		return;
	for (int i = 0; i < fxList->size(fxList); i++) {
		FX* tmp = STD_LIST_GETDATA(fxList, FX, i);

		if (tmp->type == EXPLOSION_CENTER) {
			tmp->timeToDie += getDeltaTime();

			if (tmp->timeToDie >= 1.f) {
				fxList->erase(&fxList, i);
			}
		} 
	} 
}

void fx_Update2(Window* _window) 
{
	if (fxList2 == NULL || fxList2->size(fxList2) == 0)
		return;
	for (int i = 0; i < fxList2->size(fxList2); i++) {
		FX2* tmp = STD_LIST_GETDATA(fxList2, FX2, i);
		if (tmp->type == IMPACT) {
			tmp->timeToDie += getDeltaTime();
			sfIntRect rect_Impact = { 0,3258,404,212 };
			sfRectangleShape_setTextureRect(fx_Explosion, rect_Impact);
			if (tmp->timeToDie >= 1.5f) {
				fxList2->erase(&fxList2, i);  
			}
		}
	}
}

void fx_DisplayLayer1(Window* _window) 
{
	if (fxList == NULL || fxList->size(fxList) == 0)
		return;
	for (int i = 0; i < fxList->size(fxList); i++) {
		FX* tmp = STD_LIST_GETDATA(fxList, FX, i);

		if (tmp->type == EXPLOSION_CENTER) {
			sfRectangleShape_setSize(fx_Explosion, vector2f(tmp->size.x, tmp->size.y));
			sfRectangleShape_setOrigin(fx_Explosion, tmp->origin);
			sfRectangleShape_setTexture(fx_Explosion, tmp->texture, sfTrue);
			sfRectangleShape_setTextureRect(fx_Explosion, animatorAnim("golemT_IDLE_explosion_ground", &tmp->frameX, &tmp->timeAnim, 0));
			sfRectangleShape_setPosition(fx_Explosion, tmp->position);
			sfRenderTexture_drawRectangleShape(_window->rdrt, fx_Explosion, NULL);
		}
	}
}

void fx_DisplayLayer2(Window* _window)
{
	if (fxList2 == NULL || fxList2->size(fxList2) == 0)
		return;
	for (int i = 0; i < fxList2->size(fxList2); i++) {
		FX2* tmp = STD_LIST_GETDATA(fxList2, FX2, i);

		if (tmp->type == IMPACT) { 
			sfRectangleShape_setOrigin(fx_Explosion, tmp->origin); 
			sfRectangleShape_setSize(fx_Explosion, tmp->size);     
			sfRectangleShape_setPosition(fx_Explosion, tmp->position); 
			sfRenderTexture_drawRectangleShape(_window->rdrt, fx_Explosion, NULL); 
 		}
	}
}

void fx_Create1(FX_Type _type, sfVector2f _pos)
{
	FX tmp; 
	switch (_type) {
	case EXPLOSION_CENTER:
		tmp.position = _pos;
		tmp.size = vector2f(496.f + 50.f, 404.f + 50.f);
		tmp.origin = vector2f(496.f + 50.f / 2, 404.f + 50.f / 2);
		tmp.texture = GetSfTexture("golemT_IDLE_ANNEAU"); 
		tmp.type = _type;
		tmp.timeToDie = 0.f; 
		tmp.timeAnim = 2.f;
		tmp.frameX = 0;
		break;
	}
	STD_LIST_PUSHBACK(fxList, FX, { tmp.position, tmp.size, tmp.origin, tmp.texture, tmp.type, tmp.timeToDie, tmp.timeAnim, tmp.frameX });
} 

void fx_Create2(FX_Type2 _type, sfVector2f _pos)
{
	FX2 tmp;
	switch (_type) {
	case IMPACT:
		tmp.position = _pos;
		tmp.size = vector2f(404.f, 212.f);
		tmp.origin = vector2f(404.f / 2, 212.f / 2);
		tmp.texture = GetSfTexture("golemT_IDLE_ANNEAU");
		tmp.type = _type;
		tmp.timeToDie = 0.f;
		tmp.timeAnim = 2.f;
		tmp.frameX = 0;
		break;
	}
	STD_LIST_PUSHBACK(fxList2, FX2, { tmp.position, tmp.size, tmp.origin, tmp.texture, tmp.type, tmp.timeToDie, tmp.timeAnim, tmp.frameX });
}


stdList* fx_GetList()
{
	return fxList;
}

stdList* fx_GetList2()
{
	return fxList2;
}

void fx_Deinit() {
	if (!fx_Explosion || !fx_Impact) return;
	sfRectangleShape_destroy(fx_Explosion);
	sfRectangleShape_destroy(fx_Impact);
}