//#include "inventory.h"
//sfRectangleShape* itemSelection;
//
//
//sfSprite* itemSprite;
//sfIntRect itemRect;
//sfBool isSelected;
//sfColor itemColor;
//
//sfText* itemText;
//
//static char* itemNbChar;
//
//static float timer = 0.0f;
//
//static int itemSelected = 0;
//static int yItemSelected = 0;
//
//sfBool triAuto = sfTrue;
//
//void inventory_Init() {
//	itemList = stdList_Create(sizeof(Itemlist), 0);
//	itemSprite = sfSprite_create();
//	sfSprite_setTexture(itemSprite, GetSfTexture("inventoryObject"), sfTrue);
//
//	itemSelection = sfRectangleShape_create();
//	sfRectangleShape_setSize(itemSelection, (sfVector2f) { 160.f, 160.f });
//	sfRectangleShape_setFillColor(itemSelection, sfRed);
//
//	itemText = sfText_create();
//	sfText_setFont(itemText, GetFont("Dico"));
//
//	isSelected = sfFalse;
//
//	itemNbChar = calloc(1, sizeof(char) * 3);
//
//	inventory_AddItem(ITM_NO_ITEM, (sfVector2f) { 0.f, 0.f }, 0, 0);
//	inventory_AddItem(ITM_NO_ITEM, (sfVector2f) { 0.f, 0.f }, 1, 0);
//	inventory_AddItem(ITM_NO_ITEM, (sfVector2f) { 0.f, 0.f }, 2, 0);
//	inventory_AddItem(ITM_NO_ITEM, (sfVector2f) { 0.f, 0.f }, 3, 0);
//	inventory_AddItem(ITM_NO_ITEM, (sfVector2f) { 0.f, 0.f }, 0, 1);
//	inventory_AddItem(ITM_NO_ITEM, (sfVector2f) { 0.f, 0.f }, 1, 1);
//	inventory_AddItem(ITM_NO_ITEM, (sfVector2f) { 0.f, 0.f }, 2, 1);
//	inventory_AddItem(ITM_NO_ITEM, (sfVector2f) { 0.f, 0.f }, 3, 1);
//	inventory_AddItem(ITM_NO_ITEM, (sfVector2f) { 0.f, 0.f }, 0, 2);
//	inventory_AddItem(ITM_NO_ITEM, (sfVector2f) { 0.f, 0.f }, 1, 2);
//	inventory_AddItem(ITM_NO_ITEM, (sfVector2f) { 0.f, 0.f }, 2, 2);
//	inventory_AddItem(ITM_NO_ITEM, (sfVector2f) { 0.f, 0.f }, 3, 2);
//	inventory_AddItem(ITM_NO_ITEM, (sfVector2f) { 0.f, 0.f }, 0, 3);
//	inventory_AddItem(ITM_NO_ITEM, (sfVector2f) { 0.f, 0.f }, 1, 3);
//	inventory_AddItem(ITM_NO_ITEM, (sfVector2f) { 0.f, 0.f }, 2, 3);
//	inventory_AddItem(ITM_NO_ITEM, (sfVector2f) { 0.f, 0.f }, 3, 3);
//
//	inv_ItemsDiscovered = 0;
//}
//
//void inventory_AddItem(Items _item, sfVector2f _pos, int _idX, int _idY)
//{
//	STD_LIST_PUSHBACK(itemList, Itemlist, { _item, _pos, 1, 0, _idX, _idY });
//
//	if (inv_ItemsDiscovered & (ITM_GUARDIAN_RING)) { /* Display guardian ring desc... */ }
//}
//
//void inventory_Update(Window* _window) 
//{
//	timer += getUnscaledDeltaTime();
//	if (!isSelected)
//	{
//		if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).y < 0 && timer > 0.2f)
//		{
//			yItemSelected--;
//			if (yItemSelected < 0)
//			{
//				yItemSelected = 0;
//			}
//			timer = 0.0f;
//		}
//		if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).y > 0 && timer > 0.2f)
//		{
//			yItemSelected++;
//			if (yItemSelected > 3)
//			{
//				yItemSelected = 3;
//			}
//			timer = 0.0f;
//		}
//
//		if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).x < 0 && timer > 0.2f)
//		{
//			itemSelected--;
//			if (itemSelected < 0)
//			{
//				itemSelected = 0;
//			}
//			timer = 0.0f;
//		}
//		if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).x > 0 && timer > 0.2f)
//		{
//			itemSelected++;
//			if (itemSelected > 3)
//			{
//				itemSelected = 3;
//			}
//			timer = 0.0f;
//		}
//	}
//	if (Gamepad_isButtonPressed(0, A, _window->renderWindow) && timer > 0.2f)
//	{
//		isSelected = !isSelected;
//		timer = 0.0f;
//	}
//	
//	
//	if (isSelected == sfTrue)
//	{
//		if (Gamepad_isButtonPressed(0, X, _window->renderWindow) && timer > 0.2f)
//		{
//			for (int i = 0; i < itemList->size(itemList); i++)
//			{
//				if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//				{
//					STD_LIST_GETDATA(itemList, Itemlist, i)->item = ITM_NO_ITEM;
//					isSelected = !isSelected;
//				}
//			}
//			timer = 0.0f;
//		}
//	}
//	
//	if (triAuto == sfFalse)
//	{
//		if (sfKeyboard_isKeyPressed(sfKeyEscape) && timer > 0.2f)
//		{
//			for (int i = 0; i < itemList->size(itemList); i++)
//			{
//				inventory_trieInventory(i);
//			}
//			timer = 0.0f;
//		}
//	}
//	else 
//	{
//		for (int i = 0; i < itemList->size(itemList); i++)
//		{
//			inventory_trieInventory(i);
//		}
//	}
//	if (sfKeyboard_isKeyPressed(sfKeyA) && timer > 0.2f)
//	{
//		triAuto = !triAuto;
//		timer = 0.0f;
//	}
//	if (sfKeyboard_isKeyPressed(sfKeyZ) && timer > 0.2f)
//	{
//		int randomItem = 1 << (1 + rand_int(0, 16));
//		inventory_itemRecup(randomItem);
//		timer = 0.0f;
//	}
//	if (sfKeyboard_isKeyPressed(sfKeyR) && timer > 0.2f)
//	{
//		int randomItem = 1 << (1 + rand_int(0, 16));
//		inventory_itemRemove(randomItem, 1);
//		timer = 0.0f;
//	}
//}
//
//void inventory_Display(Window* _window) {
//
//	char itemNbChar2[30];
//	strcpy(itemNbChar2, lang_GetFromKey("inv.desc.nb"));
//	sfRectangleShape_setPosition(itemSelection, (sfVector2f) { itemSelected * 160 + 145.f, yItemSelected * 160 + 210.f });
//	sfRectangleShape_setSize(itemSelection, (sfVector2f) { 160.f, 160.f });
//	sfRenderTexture_drawRectangleShape(_window->rdrt, itemSelection, NULL);
//	
//	for (int i = 0; i < itemList->size(itemList); i++)
//	{
//		itemColor = (sfColor){ 255, 255, 255, 255 };
//		STD_LIST_GETDATA(itemList, Itemlist, i)->pos.x = STD_LIST_GETDATA(itemList, Itemlist, i)->idX * 160 + 145.f;
//		STD_LIST_GETDATA(itemList, Itemlist, i)->pos.y = STD_LIST_GETDATA(itemList, Itemlist, i)->idY * 160 + 210.f;
//		switch (STD_LIST_GETDATA(itemList, Itemlist, i)->item)
//		{
//		case ITM_NO_ITEM:
//			itemRect = (sfIntRect){ 0, 0, 32, 23 };
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//			{
//				/*sfText_setColor(itemText, sfBlue);
//				sfText_setString(itemText, "L’odorat surdéveloppé de ce magnifique groin\npermet de détecter des objets.");
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 344.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);*/
//			}
//			break;
//		case ITM_GUARDIAN_STICK:
//			itemRect = (sfIntRect){ 32, 0, 32, 23 };
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//			{
//				sfText_setColor(itemText, sfBlue);
//				sfText_setString(itemText, lang_GetFromKey("explain.guardian.stick"));
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 344.f));
// 				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
// 				strcat(itemNbChar2, "%02d");
//				sprintf(itemNbChar, itemNbChar2, STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject);
//				sfText_setString(itemText, itemNbChar);
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 544.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//			}
//			break;
//		case ITM_GUARDIAN_RING:
//			itemRect = (sfIntRect){ 64, 0, 32, 23 };
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//			{
//				sfText_setColor(itemText, sfBlue);
//				sfText_setString(itemText, lang_GetFromKey("explain.guardian.ring"));
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 344.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//				strcat(itemNbChar2, "%02d");
//				sprintf(itemNbChar, itemNbChar2, STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject);
//				sfText_setString(itemText, itemNbChar);
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 544.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//			}
//			break;
//		case ITM_JOY_RING:
//			itemRect = (sfIntRect){ 96, 0, 32, 23 };
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//			{
//				sfText_setColor(itemText, sfBlue);
//				sfText_setString(itemText, lang_GetFromKey("explain.joy.ring"));
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 344.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//				strcat(itemNbChar2, "%02d");
//				sprintf(itemNbChar, itemNbChar2, STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject);
//				sfText_setString(itemText, itemNbChar);
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 544.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//			}
//			break;
//		case ITM_SADNESS_RING:
//			itemRect = (sfIntRect){ 128, 0, 32, 23 };
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//			{
//				sfText_setColor(itemText, sfBlue);
//				sfText_setString(itemText, lang_GetFromKey("explain.sadness.ring"));
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 344.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//				strcat(itemNbChar2, "%02d");
//				sprintf(itemNbChar, itemNbChar2, STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject);
//				sfText_setString(itemText, itemNbChar);
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 544.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//			}
//			break;
//		case ITM_SCARE_RING:
//			itemRect = (sfIntRect){ 160, 0, 32, 23 };
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//			{
//				sfText_setColor(itemText, sfBlue);
//				sfText_setString(itemText, lang_GetFromKey("explain.scare.ring"));
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 344.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//				strcat(itemNbChar2, "%02d");
//				sprintf(itemNbChar, itemNbChar2, STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject);
//				sfText_setString(itemText, itemNbChar);
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 544.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//			}
//			break;
//		case ITM_ANGER_RING:
//			itemRect = (sfIntRect){ 192, 0, 32, 23 };
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//			{
//				sfText_setColor(itemText, sfBlue);
//				sfText_setString(itemText, lang_GetFromKey("explain.anger.ring"));
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 344.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//				strcat(itemNbChar2, "%02d");
//				sprintf(itemNbChar, itemNbChar2, STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject);
//				sfText_setString(itemText, itemNbChar);
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 544.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//			}
//			break;
//		case ITM_JOY_FISH:
//			itemRect = (sfIntRect){ 224, 0, 32, 23 };
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//			{
//				sfText_setColor(itemText, sfBlue);
//				sfText_setString(itemText, lang_GetFromKey("info.joy.fish"));				
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 344.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//				strcat(itemNbChar2, "%02d");
//				sprintf(itemNbChar, itemNbChar2, STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject);
//				sfText_setString(itemText, itemNbChar);
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 544.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//			}
//			break;
//		case ITM_SADNESS_FISH:
//			itemRect = (sfIntRect){ 0, 32, 32, 23 };
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//			{
//				sfText_setColor(itemText, sfBlue);
//				sfText_setString(itemText, lang_GetFromKey("info.sadness.fish"));
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 344.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//				strcat(itemNbChar2, "%02d");
//				sprintf(itemNbChar, itemNbChar2, STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject);
//				sfText_setString(itemText, itemNbChar);
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 544.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//			}
//			break;
//		case ITM_SCARE_FISH:
//			itemRect = (sfIntRect){ 32, 32, 32, 23 };
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//			{
//				sfText_setColor(itemText, sfBlue);
//				sfText_setString(itemText, lang_GetFromKey("info.scare.fish")); 
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 344.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//				strcat(itemNbChar2, "%02d");
//				sprintf(itemNbChar, itemNbChar2, STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject);
//				sfText_setString(itemText, itemNbChar);
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 544.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//			}
//			break;
//		case ITM_ANGER_FISH:
//			itemRect = (sfIntRect){ 64, 32, 32, 23 };
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//			{
//				sfText_setColor(itemText, sfBlue);
//				sfText_setString(itemText, lang_GetFromKey("info.anger.fish")); 
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 344.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//				strcat(itemNbChar2, "%02d");
//				sprintf(itemNbChar, itemNbChar2, STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject);
//				sfText_setString(itemText, itemNbChar);
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 544.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//			}
//			break;
//		case ITM_BEETLE:
//			itemRect = (sfIntRect){ 96, 32, 32, 23 };
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//			{
//				/*sfText_setColor(itemText, sfBlue);
//				sfText_setString(itemText, "L’odorat surdéveloppé de ce magnifique groin\npermet de détecter des objets.");
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 344.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);*/
//				strcat(itemNbChar2, "%02d");
//				sprintf(itemNbChar, itemNbChar2, STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject);
//				sfText_setString(itemText, itemNbChar);
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 544.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//			}
//			break;
//		case ITM_SNAKE:
//			itemRect = (sfIntRect){ 128, 32, 32, 23 };
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//			{
//				sfText_setColor(itemText, sfBlue);
//				sfText_setString(itemText, lang_GetFromKey("info.snake")); 
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 344.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//				strcat(itemNbChar2, "%02d");
//				sprintf(itemNbChar, itemNbChar2, STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject);
//				sfText_setString(itemText, itemNbChar);
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 544.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//			}
//			break;
//		case ITM_SANDAMANDER:
//			itemRect = (sfIntRect){ 160, 32, 32, 23 };
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//			{
//				sfText_setColor(itemText, sfBlue);
//				sfText_setString(itemText, lang_GetFromKey("info.sablamandre")); 
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 344.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//				strcat(itemNbChar2, "%02d");
//				sprintf(itemNbChar, itemNbChar2, STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject);
//				sfText_setString(itemText, itemNbChar);
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 544.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//			}
//			break;
//		case ITM_VOLTIGEONS:
//			itemRect = (sfIntRect){ 192, 32, 32, 23 };
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//			{
//				sfText_setColor(itemText, sfBlue);
//				sfText_setString(itemText, lang_GetFromKey("info.voltigeon")); 
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 344.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//				strcat(itemNbChar2, "%02d");
//				sprintf(itemNbChar, itemNbChar2, STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject);
//				sfText_setString(itemText, itemNbChar);
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 544.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//			}
//			break;
//		case ITM_CABORALS:
//			itemRect = (sfIntRect){ 224, 32, 32, 23 };
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//			{
//				sfText_setColor(itemText, sfBlue);
//				sfText_setString(itemText, lang_GetFromKey("info.craborail"));
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 344.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//				strcat(itemNbChar2, "%02d");
//				sprintf(itemNbChar, itemNbChar2, STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject);
//				sfText_setString(itemText, itemNbChar);
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 544.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//			}
//			break;
//		case ITM_YOYO:
//			itemRect = (sfIntRect){ 32, 0, 32, 23 };
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->idX == itemSelected && STD_LIST_GETDATA(itemList, Itemlist, i)->idY == yItemSelected)
//			{
//				sfText_setColor(itemText, sfBlue);
//				sfText_setString(itemText, lang_GetFromKey("info.yoyo")); 
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 344.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//				strcat(itemNbChar2, "%02d");
//				sprintf(itemNbChar, itemNbChar2, STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject);
//				sfText_setString(itemText, itemNbChar);
//				sfText_setCharacterSize(itemText, 18);
//				sfText_setPosition(itemText, vector2f(1218.f, 544.f));
//				sfRenderTexture_drawText(_window->rdrt, itemText, NULL);
//			}
//			break;
//		default:
//			break;
//		}
//		sfSprite_setColor(itemSprite, itemColor);
//		sfSprite_setScale(itemSprite, (sfVector2f) { 5.f, 5.f });
//		sfSprite_setTextureRect(itemSprite, itemRect);
//		sfSprite_setPosition(itemSprite, STD_LIST_GETDATA(itemList, Itemlist, i)->pos);
//		sfRenderTexture_drawSprite(_window->rdrt, itemSprite, NULL);
//	}
//
//}
//
//void inventory_Deinit() {
//	sfSprite_destroy(itemSprite);
//	sfRectangleShape_destroy(itemSelection);	
//}
//
//void inventory_trieInventory(int _id)
//{
//	
//	if (STD_LIST_GETDATA(itemList, Itemlist, _id)->item == ITM_NO_ITEM)
//	{
//		if (_id < itemList->size(itemList) - 1)
//		{
//			STD_LIST_GETDATA(itemList, Itemlist, _id)->item = STD_LIST_GETDATA(itemList, Itemlist, _id + 1)->item;
//			STD_LIST_GETDATA(itemList, Itemlist, _id + 1)->item = ITM_NO_ITEM;
//			inventory_trieInventory(_id + 1);
//		}
//		else return;
//	}
//	else
//	{
//		return;
//	}
//}
//
//void inventory_itemRecup(Items _item)
//{
//	sfBool thereIsNoItem = sfFalse;
//	sfBool itemAlreadyExist = sfFalse;
//	int idItem = 0;
//	int idItem2 = 0;
//	for (int i = 0; i < itemList->size(itemList); i++)
//	{
//		if (!thereIsNoItem)
//		{
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->item == ITM_NO_ITEM)
//			{
//				thereIsNoItem = sfTrue;
//				idItem2 = i;
//			}
//		}
//		if (!itemAlreadyExist)
//		{
//			if (STD_LIST_GETDATA(itemList, Itemlist, i)->item == _item)
//			{
//				itemAlreadyExist = sfTrue;
//				idItem = i;
//			} 
//		}
//	}
//	if (itemAlreadyExist)
//	{
//		if (STD_LIST_GETDATA(itemList, Itemlist, idItem)->item == _item)
//		{
//			STD_LIST_GETDATA(itemList, Itemlist, idItem)->nbObject++;
//		}
//	}
//	else {
//		STD_LIST_GETDATA(itemList, Itemlist, idItem2)->item = _item;
//		inv_ItemsDiscovered |= _item;
//	}
//}
//
//void inventory_itemRemove(Items _item, int _nbItemToSuppr)
//{
//	for (int i = 0; i < itemList->size(itemList); i++)
//	{
//		if (STD_LIST_GETDATA(itemList, Itemlist, i)->item == _item)
//		{
//			STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject -= _nbItemToSuppr;
//			inventory_verifyNoMoreItem(i);
//		}
//	}
//}
//
//int inventory_getNbItem(Items _item)
//{
//	for (int i = 0; i < itemList->size(itemList); i++)
//	{
//		if (STD_LIST_GETDATA(itemList, Itemlist, i)->item == _item)
//		{
//			return STD_LIST_GETDATA(itemList, Itemlist, i)->nbObject;
//		}
//	}
//	return 0;
//}
//
//void inventory_verifyNoMoreItem(int _i)
//{
//	if (STD_LIST_GETDATA(itemList, Itemlist, _i)->nbObject == 0)
//	{
//		STD_LIST_GETDATA(itemList, Itemlist, _i)->item = ITM_NO_ITEM;
//	}
//}
