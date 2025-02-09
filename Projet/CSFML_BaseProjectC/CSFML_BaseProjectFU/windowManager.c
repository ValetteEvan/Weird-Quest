#include "windowManager.h"
#include "viewManager.h"
#include "stateManager.h"
#include "options.h"
#include "controller.h"
#include "fontManager.h"

sfTexture* window_Texture;
sfBool window_FSFlag = sfFalse;
sfText* windowTextFrameRate;

sfBool IsDone(Window* This)
{
	return This->isDone;
}

sfBool IsFullscreen(Window* This)
{
	return This->isFullscreen;
}

void BeginDraw(Window* This)
{
	sfRenderWindow_clear(This->renderWindow, sfBlack);
	sfRenderTexture_clear(This->rdrt, sfTransparent);
	sfRenderTexture_clear(This->rdrtShader, sfTransparent);
}

void EndDraw(Window* This)
{
	window_Texture = sfRenderTexture_getTexture(This->rdrtShader);
	sfSprite_setTexture(This->spr, window_Texture, sfTrue);
	if (This->rsShader)
		sfShader_setTextureUniform(This->rsShader->shader, "u_TexBase", window_Texture);
	sfRenderWindow_drawSprite(This->renderWindow, This->spr, This->rsShader);

	window_Texture = sfRenderTexture_getTexture(This->rdrt);
	sfSprite_setTexture(This->spr, window_Texture, sfTrue);
	if (windowGetShader(This))
		sfShader_setTextureUniform(windowGetShader(This), "u_TexBase", window_Texture);
	sfRenderWindow_drawSprite(This->renderWindow, This->spr, This->rs);

	sfRenderWindow_display(This->renderWindow);
	This->rs = NULL;
	This->rsShader = NULL;
}

Window* windowSetup(const char* _Title, sfVector2i _defaultVideoMode)
{
	Window* This = malloc(sizeof(Window));
	if (This == NULL)
	{
		printf("Allocation pour la window echoue");
		exit(1);
	}
	This->windowTitle = malloc(strlen(_Title)*sizeof(char));
	if (This->windowTitle == NULL)
	{
		printf("Allocation pour Title de la window echoue");
		exit(1);
	}

	strcpy(This->windowTitle, _Title);
	
	This->isFullscreen = seting.fullScreen;
	This->isDone = sfFalse;
	This->videoMode = sfVideoMode_getDesktopMode();
	This->defaultVideoMode = _defaultVideoMode;
	This->rs = NULL;
	This->rsShader = NULL;
	
	windowCreate(This);

	// MainView
	sfFloatRect rectViewPort = { 0.0f, 0.0f, 1.0f, 1.0f };

	mainView = setupView(_defaultVideoMode, rectViewPort, This->videoMode);
	This->rdrt = sfRenderTexture_create(This->videoMode.width, This->videoMode.height, sfFalse);
	This->rdrtShader = sfRenderTexture_create(This->videoMode.width, This->videoMode.height, sfFalse);
	This->spr = sfSprite_create();
	sfSprite_setScale(This->spr, vector2f(1.f, -1.f));
	sfSprite_setPosition(This->spr, vector2f(0.f, 1080.f));

	return This;
}

void windowDestroy(Window* This)
{
	sfRenderWindow_close(This->renderWindow);
	This->renderWindow = NULL;
}

void windowCreate(Window* This)
{
	sfWindowStyle style = (This->isFullscreen ? sfFullscreen : sfDefaultStyle);
	This->renderWindow = sfRenderWindow_create(This->videoMode, This->windowTitle, style, NULL);
	/// TODO Find a system for that. No need to look into VSync but anything that can reduce this obnoxious tearing might be a good asset to have on our side.
	/// Didn't want to enforce a constant framerate, knowing that all our PCs might react differently to it and that it may even break the game in places
//	sfRenderWindow_setFramerateLimit(This->renderWindow, 60);
	fullscreen_IsDone = sfTrue;
}

void ToggleFullscreen(Window* This)
{
	fullscreen_IsDone = sfFalse;
	This->isFullscreen = !This->isFullscreen;
	seting.fullScreen = This->isFullscreen;
	FILE* fichier = fopen("..\\Ressources\\Save\\settings.txt", "w");
	fprintf(fichier, "%d %f %f", seting.fullScreen, seting.musicVol, seting.sfxVol);
	fclose(fichier);
	windowDestroy(This);
	windowCreate(This);
}

void windowInit(Window* This)
{
	static sfBool onePass = sfTrue;
	if (onePass) {
		fullscreen_IsDone = sfTrue;
		onePass = sfFalse;
	}
	stateInit(This);
	windowTextFrameRate = sfText_create();
	sfText_setFont(windowTextFrameRate, GetFont("fEdit"));
	sfText_setFillColor(windowTextFrameRate, sfGreen);
	sfText_setCharacterSize(windowTextFrameRate, 25);
	sfText_setOutlineThickness(windowTextFrameRate, 3.f);
	sfText_setOutlineColor(windowTextFrameRate, sfColor_fromRGBA(0, 0, 0, 255));
}

void windowUpdate(Window* This)
{
	if (fullscreen_IsDone) {
		restartClock();
		while (sfRenderWindow_pollEvent(This->renderWindow, &This->event))
		{
			if (This->event.type == sfEvtClosed) {
				stateDeinit(This);
				This->isDone = sfTrue;
			}
			for (int i = 0; i < 8; i++)
			{
				GamepadManager(i, This->event, This->renderWindow);
			}
			stateEventUpdate(This, &This->event);
		}
		if (customKeyboardIsKeyPressed(sfKeyF10, This->renderWindow) == sfTrue)
		{
			screenshot(This->renderWindow);
		}

		if (customKeyboardIsKeyPressed(sfKeyF11, This->renderWindow)) {
			if (!window_FSFlag) ToggleFullscreen(This);
			window_FSFlag = sfTrue;
		}
		else window_FSFlag = sfFalse;

		// MAINVIEW 
		stateUpdate(This);
		UpdateView(This, mainView);

	}
}

void windowDraw(Window* This)
{
	if (fullscreen_IsDone) {
		sfRenderTexture_setView(This->rdrt, mainView->view);
		sfRenderTexture_setView(This->rdrtShader, mainView->view);
		stateDisplay(This);
		sfRenderTexture_setView(This->rdrt, sfRenderTexture_getDefaultView(This->rdrt));
		sfText_setPosition(windowTextFrameRate, vector2f(20.f, 980.f));
		displayFrameRate(This->rdrt, windowTextFrameRate);
		sfText_setPosition(windowTextFrameRate, vector2f(20.f, 1010.f));
		displayFrameRateThrd2(This->rdrt, windowTextFrameRate);
		sfText_setPosition(windowTextFrameRate, vector2f(20.f, 1040.f));
		displayFrameRateThrd3(This->rdrt, windowTextFrameRate);
		sfRenderTexture_setView(This->rdrt, mainView->view);
		sfRenderTexture_setView(This->rdrtShader, mainView->view);
	}
}

sfShader* windowGetShader(Window* This) { return This->rs ? This->rs->shader : NULL; }

void windowReadSettings()
{
	FILE* setingFile;
	setingFile = fopen("..\\Ressources\\Save\\settings.txt", "r");
	char tmpligne[200];
	if (setingFile == NULL) {
		seting.fullScreen = sfTrue;
		seting.musicVol = 50.f;
		seting.sfxVol = 50.f;
		setingFile = fopen("..\\Ressources\\Save\\settings.txt", "w");
	}
	else {
		while (fgets(tmpligne, 200, setingFile) != NULL) {
			sscanf(tmpligne, "%d %f %f", &seting.fullScreen, &seting.musicVol, &seting.sfxVol);
		}
		if (seting.musicVol < 0.f) seting.musicVol = 0.f;
		if (seting.sfxVol < 0.f) seting.sfxVol = 0.f;
		if (seting.musicVol > 100.f) seting.musicVol = 100.f;
		if (seting.sfxVol > 100.f) seting.sfxVol = 100.f;
	}
	fclose(setingFile);
}
