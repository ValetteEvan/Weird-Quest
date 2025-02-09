#pragma once
#include "tools.h"
#include "stateManager.h"

typedef enum dialBox dialBox;
enum dialBox
{
	TEXT,
	INTERACT
};

typedef struct stDialogBox stDialogBox;
struct stDialogBox
{
	sfVector2f position;
	sfVector2f size;
	dialBox type;
	sfRectangleShape* dialogshape;
	sfRectangleShape* dialogshapeNo;
	sfRectangleShape* dialogshapeYes;
	char* message;
	sfText* txt;
	sfText* txtYes;
	sfText* txtNo;
};

stDialogBox* currentDialogBox;
sfBool isDialogBox;
sfBool DialogBoxChoice;
int DialogBoxCode;

/// <summary>
/// Create a dialogBox
/// </summary>
/// <param name="_type">dialog box type info or alert</param>
/// <param name="_message">the text that will be displayed in the dialog box </param>
/// <param name="_DialogBoxCode">what will you do after the dialog box, 0 : continue, 1 return to menu, 2 exit game</param>
void dialogBoxCreate(dialBox _type, char* _message, int _DialogBoxCode);

/// <summary>
/// update dialog box
/// </summary>
/// <param name="_window">current window</param>
void dialogBoxUpdate(Window* _window);

/// <summary>
/// display dialog box
/// </summary>
/// <param name="_window">the current window</param>
void dialogBoxDisplay(Window* _window);

/// <summary>
/// destroy the dialog box currently displayed
/// </summary>
void dialogBoxDestroy();

/// <summary>
/// update the dialog box when a button is pressed
/// </summary>
/// <param name="_window"> the current window </param>
/// <param name="_code">what will you do after the dialog box, 0 : continue, 1 return to menu, 2 exit game</param>
void DialogBoxManager(Window* _window, int _code);