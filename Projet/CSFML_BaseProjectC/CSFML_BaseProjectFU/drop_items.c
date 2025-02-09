#include "drop_items.h"
#include "player.h"
#include "particlesManager.h"

sfRectangleShape* itemHeart;  
sfTexture* textureHeart;   

void initItemsDrop(Window* _window) {
	itemsList = STD_LIST_CREATE(ITEM, 0);   
	itemHeart = sfRectangleShape_create(); 
}

void updateItemsDrop(Window* _window) {
	if (itemsList == NULL || itemsList->size(itemsList) == 0)
		return;
	for (int i = 0; i < itemsList->size(itemsList); i++) {
		ITEM* tmp = STD_LIST_GETDATA(itemsList, ITEM, i);
		if (tmp->type == HEART) {
			tmp->timeToDie += getDeltaTime();
			tmp->timerPrt += getDeltaTime();
			if (tmp->timerPrt > 1.f)
			{
				prt_CreateHeartDropParticles(vector2f(tmp->position.x - 3.f, tmp->position.y));  
				tmp->timerPrt = 0.f; 
			}
			sfIntRect rectHeart = { 0,0,36,44 };
			sfRectangleShape_setTextureRect(itemHeart, rectHeart);
			sfFloatRect rectHearFloat = FlRect(tmp->position.x - tmp->size.x / 2, tmp->position.y - tmp->size.y / 2, tmp->size.x, tmp->size.y); 
			sfFloatRect tempRectPlayer = plr_getPlayerBounds();  

			if (sfFloatRect_intersects(&tempRectPlayer, &rectHearFloat, NULL)) { 
				plr_AddLife(4); 
				itemsList->erase(&itemsList, i);  
				continue; 
			}
			if (tmp->timeToDie >= 5.f) {
				itemsList->erase(&itemsList, i);
				continue; 
			}
		}
	}
}
 
void displayItemsDrop(Window* _window) {
	if (itemsList == NULL || itemsList->size(itemsList) == 0)
		return; 
	for (int i = 0; i < itemsList->size(itemsList); i++) {
		ITEM* tmp = STD_LIST_GETDATA(itemsList, ITEM, i); 
		if (tmp->type == HEART) {
			sfRectangleShape_setSize(itemHeart, tmp->size);       
			sfRectangleShape_setOrigin(itemHeart, tmp->origin);        
			sfRectangleShape_setTexture(itemHeart, tmp->texture, sfTrue);
			sfRectangleShape_setPosition(itemHeart, tmp->position);  
			if (tmp->frameX <= 5) 
			sfRectangleShape_setTextureRect(itemHeart, animatorAnim("animHeartDrop", &tmp->frameX, &tmp->timerAnim, 0));     
			else	sfRectangleShape_setTextureRect(itemHeart, (sfIntRect){180,0,36,44});
			sfRenderTexture_drawRectangleShape(_window->rdrt, itemHeart, NULL); 
		} 
	}
}

void createItemsDrop(ITEM_Type _type, sfVector2f _pos) { 
	ITEM tmp; 
	switch (_type) {
	case HEART: 
		tmp.position = addVectorsrf(_pos, vector2f(32.f, 32.f));
		tmp.size = vector2f(36.f, 44.f); 
		tmp.origin = vector2f(tmp.size.x / 2 ,tmp.size.y / 2);  
		tmp.texture = GetSfTexture("animHeartDrop");
		tmp.type = _type;  
		tmp.timeToDie = 0.f; 
		tmp.timerAnim = 0.f; 
		tmp.timerPrt = 0.f;
		tmp.frameX = 0; 
		tmp.timerFlash = 0.f; 
		tmp.timerTempFlash = 5.f; 
		tmp.bool_repeat = sfFalse;  
		break;
	}
	STD_LIST_PUSHBACK(itemsList, ITEM, { tmp.position, tmp.size,tmp.origin, tmp.texture,tmp.type,tmp.timeToDie,tmp.timerAnim,tmp.frameX,tmp.timerFlash,tmp.timerTempFlash , tmp.bool_repeat}); 
}

void clearItemsDrop() {
	itemsList->clear; 
}

stdList* getItemList() {
	return itemsList;
}

