#include "dialogBox.h"
#include "viewManager.h"
#include "controller.h"
#include "fontManager.h"
#include "game.h"


void dialogBoxCreate(dialBox _type, char* _message, int _DialogBoxCode)
{
	stDialogBox* dialogBox = malloc(sizeof(stDialogBox));
	if (dialogBox == NULL)
	{
		printf("Allocation pour dialogBox echoue");
	}
	dialogBox->message = malloc((strlen(_message) + 2) * sizeof(char));
	if (dialogBox->message == NULL)
	{
		printf("Allocation pour message de dialogBox echoue");
	}
	strcpy(dialogBox->message, _message);
	strcat(dialogBox->message, "\n\n");
	dialogBox->type = _type;

	int occurence = 0;
	int nbrecarc = 0, nbrecarcMax = 0;
	for (int i = 0; dialogBox->message[i] != '\0'; i++)
	{
		if (dialogBox->message[i] == '\n')
		{
			++occurence;
			if (nbrecarc > nbrecarcMax)
				nbrecarcMax = nbrecarc;
			nbrecarc = 0;
		}
		else
			nbrecarc++;
	}

	dialogBox->txt = sfText_create();
	sfText_setString(dialogBox->txt, dialogBox->message);
	sfText_setFont(dialogBox->txt, GetFont("3DF"));
	sfText_setCharacterSize(dialogBox->txt, 20);
	sfText_setColor(dialogBox->txt, sfWhite);

	dialogBox->size.x = 20.0f + sfText_getGlobalBounds(dialogBox->txt).width;
	dialogBox->size.y = 50.0f + occurence * 20.0f;
	dialogBox->position = getViewPosition(mainView);

	isDialogBox = sfTrue;
	dialogBox->dialogshape = sfRectangleShape_create();
	sfRectangleShape_setOrigin(dialogBox->dialogshape, vector2f(dialogBox->size.x / 2.0f, dialogBox->size.y / 2.0f));
	sfRectangleShape_setFillColor(dialogBox->dialogshape, sfBlack);
	sfRectangleShape_setOutlineColor(dialogBox->dialogshape, sfWhite);
	sfRectangleShape_setOutlineThickness(dialogBox->dialogshape, 2.0f);
	sfRectangleShape_setSize(dialogBox->dialogshape, dialogBox->size);
	sfRectangleShape_setPosition(dialogBox->dialogshape, dialogBox->position);

	sfText_setPosition(dialogBox->txt, vector2f(dialogBox->position.x - dialogBox->size.x / 2.0f + 10.0f, dialogBox->position.y - dialogBox->size.y / 2.0f + 10.0f));

	if (dialogBox->type == TEXT)
	{
		dialogBox->dialogshapeYes = sfRectangleShape_create();
		sfRectangleShape_setSize(dialogBox->dialogshapeYes, vector2f(100.0f, 30.0f));
		sfRectangleShape_setOrigin(dialogBox->dialogshapeYes, vector2f(sfRectangleShape_getGlobalBounds(dialogBox->dialogshapeYes).width / 2.0f, sfRectangleShape_getGlobalBounds(dialogBox->dialogshapeYes).height / 2.0f));
		sfRectangleShape_setFillColor(dialogBox->dialogshapeYes, sfBlack);
		sfRectangleShape_setOutlineColor(dialogBox->dialogshapeYes, sfWhite);
		sfRectangleShape_setOutlineThickness(dialogBox->dialogshapeYes, 1.0f);
		sfRectangleShape_setPosition(dialogBox->dialogshapeYes, vector2f(sfRectangleShape_getGlobalBounds(dialogBox->dialogshape).left + sfRectangleShape_getGlobalBounds(dialogBox->dialogshape).width / 2.0f, sfRectangleShape_getGlobalBounds(dialogBox->dialogshape).top + sfRectangleShape_getGlobalBounds(dialogBox->dialogshape).height - 28));

		dialogBox->txtYes = sfText_create();
		sfText_setString(dialogBox->txtYes, "OK");
		sfText_setFont(dialogBox->txtYes, GetFont("3DF"));
		sfText_setCharacterSize(dialogBox->txtYes, 20);
		sfText_setOrigin(dialogBox->txtYes, vector2f(sfText_getGlobalBounds(dialogBox->txtYes).width / 2.0f, sfText_getGlobalBounds(dialogBox->txtYes).height / 2.0f));
		sfText_setPosition(dialogBox->txtYes, vector2f(sfRectangleShape_getPosition(dialogBox->dialogshapeYes).x, sfRectangleShape_getPosition(dialogBox->dialogshapeYes).y - 5.0f));
		sfText_setColor(dialogBox->txtYes, sfWhite);

		DialogBoxChoice = sfTrue;

	}
	else if (dialogBox->type == INTERACT)
	{
		dialogBox->dialogshapeYes = sfRectangleShape_create();
		sfRectangleShape_setSize(dialogBox->dialogshapeYes, vector2f(100.0f, 30.0f));
		sfRectangleShape_setOrigin(dialogBox->dialogshapeYes, vector2f(sfRectangleShape_getGlobalBounds(dialogBox->dialogshapeYes).width / 2.0f, sfRectangleShape_getGlobalBounds(dialogBox->dialogshapeYes).height / 2.0f));
		sfRectangleShape_setFillColor(dialogBox->dialogshapeYes, sfBlack);
		sfRectangleShape_setOutlineColor(dialogBox->dialogshapeYes, sfWhite);
		sfRectangleShape_setOutlineThickness(dialogBox->dialogshapeYes, 1.0f);
		sfRectangleShape_setPosition(dialogBox->dialogshapeYes, vector2f(sfRectangleShape_getGlobalBounds(dialogBox->dialogshape).left + sfRectangleShape_getGlobalBounds(dialogBox->dialogshape).width / 2.0f - 70.0f, sfRectangleShape_getGlobalBounds(dialogBox->dialogshape).top + sfRectangleShape_getGlobalBounds(dialogBox->dialogshape).height - 28));

		dialogBox->txtYes = sfText_create();
		sfText_setString(dialogBox->txtYes, "YES");
		sfText_setFont(dialogBox->txtYes, GetFont("3DF"));
		sfText_setCharacterSize(dialogBox->txtYes, 20);
		sfText_setOrigin(dialogBox->txtYes, vector2f(sfText_getGlobalBounds(dialogBox->txtYes).width / 2.0f, sfText_getGlobalBounds(dialogBox->txtYes).height / 2.0f));
		sfText_setPosition(dialogBox->txtYes, vector2f(sfRectangleShape_getPosition(dialogBox->dialogshapeYes).x, sfRectangleShape_getPosition(dialogBox->dialogshapeYes).y - 5.0f));
		sfText_setColor(dialogBox->txtYes, sfWhite);

		dialogBox->dialogshapeNo = sfRectangleShape_create();
		sfRectangleShape_setSize(dialogBox->dialogshapeNo, vector2f(100.0f, 30.0f));
		sfRectangleShape_setOrigin(dialogBox->dialogshapeNo, vector2f(sfRectangleShape_getGlobalBounds(dialogBox->dialogshapeNo).width / 2.0f, sfRectangleShape_getGlobalBounds(dialogBox->dialogshapeNo).height / 2.0f));
		sfRectangleShape_setFillColor(dialogBox->dialogshapeNo, sfBlack);
		sfRectangleShape_setOutlineColor(dialogBox->dialogshapeNo, sfWhite);
		sfRectangleShape_setOutlineThickness(dialogBox->dialogshapeNo, 1.0f);
		sfRectangleShape_setPosition(dialogBox->dialogshapeNo, vector2f(sfRectangleShape_getGlobalBounds(dialogBox->dialogshape).left + sfRectangleShape_getGlobalBounds(dialogBox->dialogshape).width / 2.0f + 70.0f, sfRectangleShape_getGlobalBounds(dialogBox->dialogshape).top + sfRectangleShape_getGlobalBounds(dialogBox->dialogshape).height - 28));

		dialogBox->txtNo = sfText_create();
		sfText_setString(dialogBox->txtNo, "NO");
		sfText_setFont(dialogBox->txtNo, GetFont("3DF"));
		sfText_setCharacterSize(dialogBox->txtNo, 20);
		sfText_setOrigin(dialogBox->txtNo, vector2f(sfText_getGlobalBounds(dialogBox->txtYes).width / 2.0f, sfText_getGlobalBounds(dialogBox->txtYes).height / 2.0f));
		sfText_setPosition(dialogBox->txtNo, vector2f(sfRectangleShape_getPosition(dialogBox->dialogshapeNo).x, sfRectangleShape_getPosition(dialogBox->dialogshapeNo).y - 5.0f));
		sfText_setColor(dialogBox->txtNo, sfWhite);

		DialogBoxChoice = sfFalse;
	}

	DialogBoxCode = _DialogBoxCode;

	currentDialogBox = dialogBox;
}


void dialogBoxUpdate(Window* _window)
{
	static float timer = 0.0f;
	timer += getUnscaledDeltaTime();


	if (Gamepad_isButtonPressed(0, DPAD_RIGHT, _window->renderWindow) && timer > 0.2f)
	{
		DialogBoxChoice = sfFalse;
		timer = 0.0f;
	}
	if (Gamepad_isButtonPressed(0, DPAD_LEFT, _window->renderWindow) && timer > 0.2f)
	{
		DialogBoxChoice = sfTrue;
		timer = 0.0f;
	}
	if (Gamepad_isButtonPressed(0, A, _window->renderWindow) && timer > 0.2f)
	{
		isDialogBox = sfFalse;
		dialogBoxDestroy();
		timer = 0.0f;
		if (DialogBoxChoice)
			DialogBoxManager(_window, DialogBoxCode);
		else
			DialogBoxManager(_window, 0);
	}

	if (customKeyboardIsKeyPressed(sfKeyEnter, _window->renderWindow) == sfTrue && timer > 0.2f)
	{
		isDialogBox = sfFalse;
		dialogBoxDestroy();
		timer = 0.0f;
		if (DialogBoxChoice)
			DialogBoxManager(_window, DialogBoxCode);
		else
			DialogBoxManager(_window, 0);
	}
	if (customKeyboardIsKeyPressed(sfKeyRight, _window->renderWindow) == sfTrue && timer > 0.2f)
	{
		DialogBoxChoice = sfFalse;
		timer = 0.0f;
	}
	if (customKeyboardIsKeyPressed(sfKeyLeft, _window->renderWindow) == sfTrue && timer > 0.2f)
	{
		DialogBoxChoice = sfTrue;
		timer = 0.0f;
	}

	if (currentDialogBox != NULL)
	{
		if (DialogBoxChoice)
		{
			sfRectangleShape_setFillColor(currentDialogBox->dialogshapeYes, sfBlack);
			sfRectangleShape_setOutlineColor(currentDialogBox->dialogshapeYes, sfWhite);
			sfRectangleShape_setOutlineThickness(currentDialogBox->dialogshapeYes, 2.0f);
			sfText_setColor(currentDialogBox->txtYes, sfWhite);

			if (currentDialogBox->type == INTERACT) {
				sfRectangleShape_setFillColor(currentDialogBox->dialogshapeNo, sfBlack);
				sfRectangleShape_setOutlineColor(currentDialogBox->dialogshapeNo, color(125.0, 125.0, 125.0, 255.0));
				sfRectangleShape_setOutlineThickness(currentDialogBox->dialogshapeNo, 1.0f);
				sfText_setColor(currentDialogBox->txtNo, color(125.0, 125.0, 125.0, 255.0));
			}


		}
		else
		{
			sfRectangleShape_setFillColor(currentDialogBox->dialogshapeYes, sfBlack);
			sfRectangleShape_setOutlineColor(currentDialogBox->dialogshapeYes, color(125.0, 125.0, 125.0, 255.0));
			sfRectangleShape_setOutlineThickness(currentDialogBox->dialogshapeYes, 1.0f);
			sfText_setColor(currentDialogBox->txtYes, color(125.0, 125.0, 125.0, 255.0));

			sfRectangleShape_setFillColor(currentDialogBox->dialogshapeNo, sfBlack);
			sfRectangleShape_setOutlineColor(currentDialogBox->dialogshapeNo, sfWhite);
			sfRectangleShape_setOutlineThickness(currentDialogBox->dialogshapeNo, 2.0f);
			sfText_setColor(currentDialogBox->txtNo, sfWhite);
		}
	}
}

void dialogBoxDisplay(Window* _window)
{
	sfRenderTexture_drawRectangleShape(_window->rdrt, currentDialogBox->dialogshape, NULL);
	sfRenderTexture_drawText(_window->rdrt, currentDialogBox->txt, NULL);

	if (currentDialogBox->type == TEXT)
	{
		sfRenderTexture_drawRectangleShape(_window->rdrt, currentDialogBox->dialogshapeYes, NULL);
		sfRenderTexture_drawText(_window->rdrt, currentDialogBox->txtYes, NULL);
	}
	else if (currentDialogBox->type == INTERACT)
	{
		sfRenderTexture_drawRectangleShape(_window->rdrt, currentDialogBox->dialogshapeYes, NULL);
		sfRenderTexture_drawText(_window->rdrt, currentDialogBox->txtYes, NULL);
		sfRenderTexture_drawRectangleShape(_window->rdrt, currentDialogBox->dialogshapeNo, NULL);
		sfRenderTexture_drawText(_window->rdrt, currentDialogBox->txtNo, NULL);
	}
}

void dialogBoxDestroy()
{
	sfRectangleShape_destroy(currentDialogBox->dialogshape);
	sfRectangleShape_destroy(currentDialogBox->dialogshapeYes);

	sfText_destroy(currentDialogBox->txt);

	if (currentDialogBox->type == TEXT)
	{
		sfText_destroy(currentDialogBox->txtYes);
	}
	else if (currentDialogBox->type == INTERACT)
	{
		sfText_destroy(currentDialogBox->txtYes);
		sfText_destroy(currentDialogBox->txtNo);
		sfRectangleShape_destroy(currentDialogBox->dialogshapeNo);
	}
	//free(currentDialogBox->message);
	currentDialogBox = NULL;
	free(currentDialogBox);
}

void DialogBoxManager(Window* _window, int _code)
{
	switch (_code)
	{
	case 0:
		break;
	case 1:
		changeState(_window, MENU);
		setTimeScale(1.f);
		togglePause();
		break;
	case 2:
		_window->isDone = sfTrue;
		break;
	default:
		break;
	}
}