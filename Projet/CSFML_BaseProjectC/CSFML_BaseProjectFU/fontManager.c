#include "fontManager.h"

static sfBool firstPassFont = sfFalse;

void fontsOnload(State _state)
{
	if (!firstPassFont) {
		firstPassFont = sfTrue;
		fontBegin = 0;
	}

	ressources* tempRessources = ResBegin;
	while (tempRessources != NULL)
	{
		if (tempRessources->type == FONT)
		{
			if (tempRessources->state == _state)
			{
				Font* tempFont = (Font*)calloc(1, sizeof(Font));
				strcpy(tempFont->name, tempRessources->name);
				tempFont->state = tempRessources->state;
				tempFont->font = sfFont_createFromFile(tempRessources->path);
				AddFont(tempFont);
			}
		}
		tempRessources = tempRessources->pNext;
	}
}

sfFont* GetFont(char* _name)
{
	Font* tempFont = fontBegin;
	while (tempFont != NULL)
	{
		if ((strcmp(_name, tempFont->name)) == 0)
		{
			return tempFont->font;
		}
		tempFont = tempFont->pNext;
	}
	return NULL;
}

void AddFont(Font* _font)
{
	_font->pNext = fontBegin;
	fontBegin = _font;
}

Font* RemoveFont(Font* _font)
{
	if (_font == fontBegin)
	{
		Font* tempFont = _font->pNext;
		fontBegin = tempFont;
		sfFont_destroy(_font->font);
		free(_font);
		return tempFont;
	}
	else
	{
		Font* tempFont = fontBegin;
		while (tempFont->pNext != _font)
		{
			tempFont = tempFont->pNext;
		}
		tempFont->pNext = _font->pNext;
		sfFont_destroy(_font->font);
		free(_font);
		return tempFont->pNext;
	}
}

void RemoveAllFonts()
{
	Font* tempFont = fontBegin;
	while (tempFont != NULL)
	{
		Font* todelete = tempFont;
		tempFont = tempFont->pNext;
		sfFont_destroy(todelete->font);
		free(todelete);
	}
	fontBegin = 0;
}

void RemoveAllFontsNotNeededInEveryStates()
{
	Font* tempFont = fontBegin;
	while (tempFont != NULL)
	{
		if (tempFont->state != ALL)
		{
			if (tempFont == fontBegin)
			{
				Font* tempFont2 = tempFont->pNext;
				fontBegin = tempFont2;
				sfFont_destroy(tempFont->font);
				free(tempFont);
				tempFont = tempFont2;
			}
			else
			{
				Font* toDelete = tempFont;
				tempFont = tempFont->pNext;
				sfFont_destroy(tempFont->font);
				free(toDelete);
			}
		}
		else
		{
			tempFont = tempFont->pNext;
		}
	}
}

sfText* createText(char* _string, char* _fontName, sfColor _color, int _size, float _outlineThickness, sfColor _outlineColor, sfVector2f _pos)
{
	sfFont* tmp = GetFont(_fontName);
	sfText* txtTmp = sfText_create();
	sfText_setFont(txtTmp, tmp);
	sfText_setFillColor(txtTmp, _color);
	sfText_setCharacterSize(txtTmp, _size);
	sfText_setOutlineColor(txtTmp, _outlineColor);
	sfText_setOutlineThickness(txtTmp, _outlineThickness);
	sfText_setString(txtTmp, _string);
	sfFloatRect textBounds = sfText_getGlobalBounds(txtTmp);
	sfText_setOrigin(txtTmp, vector2f(textBounds.width / 2, textBounds.height));
	sfText_setPosition(txtTmp, _pos);

	return txtTmp;
}

void centerText(sfText* _txt)
{
	sfFloatRect textBounds = sfText_getGlobalBounds(_txt);
	sfText_setOrigin(_txt, vector2f(textBounds.width / 2, textBounds.height));
}
