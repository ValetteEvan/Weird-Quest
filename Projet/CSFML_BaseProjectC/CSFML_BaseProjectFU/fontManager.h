#pragma once
#include "tools.h"
#include "SFML/Audio.h"
#include "ressourcesManager.h"

typedef struct Font Font;
struct Font
{
	Font* pNext;
	char name[30];
	State state;
	sfFont* font;
};

Font* fontBegin;

/// <summary>
/// Load the fonts of a given state
/// </summary>
/// <param name="_state">the current state</param>
void fontsOnload(State _state);

/// <summary>
/// Give a font with a given name
/// </summary>
/// <param name="_name">the font name</param>
/// <returns>the font</returns>
sfFont* GetFont(char* _name);

/// <summary>
/// Add a font to the loaded ressources
/// </summary>
/// <param name="_font">The font</param>
void AddFont(Font* _font);

/// <summary>
/// Remove a font to the loaded ressources
/// </summary>
/// <param name="_font"></param>
/// <returns></returns>
Font* RemoveFont(Font* _font);

/// <summary>
/// Remove all the loaded fonts
/// </summary>
void RemoveAllFonts();

/// <summary>
/// Remove all the loaded fonts which are not needed in every states
/// </summary>
void RemoveAllFontsNotNeededInEveryStates();

/// <summary>
/// Create a text setup ready to be used
/// </summary>
/// <param name="_string">the string of the text</param>
/// <param name="_fontName">the font name</param>
/// <param name="_color">the text fill color</param>
/// <param name="_size">the text size</param>
/// <param name="_outlineThickness">the outline thickness</param>
/// <param name="_outlineColor">the outline color</param>
/// <param name="_pos">the text pos</param>
/// <param name="_origin">the text origin</param>
/// <returns>the text setup</returns>
sfText* createText(char* _string, char* _fontName, sfColor _color, int _size, float _outlineThickness, sfColor _outlineColor, sfVector2f _pos);

/// <summary>
/// Center a sf text with their global bounds
/// </summary>
/// <param name="_txt">the text to center</param>
void centerText(sfText* _txt);
