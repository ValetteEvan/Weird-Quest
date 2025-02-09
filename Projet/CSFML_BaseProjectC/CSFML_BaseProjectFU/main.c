#include <windows.h>
#include "tools.h"
#include "windowManager.h"
#include "stateManager.h"
#include "options.h"

int main(int argc, char** argv)
{
#ifndef _DEBUG
	FreeConsole();
#endif // !DEBUG

	ShowWindow(GetConsoleWindow(), 1); /// Set to 0 to hide console, set to 1 to show it
	initTools();

	windowReadSettings();

	sfVector2i defaultVideoMode = { 1920,1080 };
	Window* mainWindow = windowSetup("Weird Quest 2", defaultVideoMode);

	windowInit(mainWindow);
	sfRenderWindow_setMouseCursorVisible(mainWindow->renderWindow, sfFalse);

	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			/// ENABLE DEBUG
			if (strcmp(argv[i], "-debug") == 0) {
				DEBUG_STATE = sfTrue;
				sfRenderWindow_setMouseCursorVisible(mainWindow->renderWindow, sfTrue);
			}

			/// ENABLE VSYNC
			if (strcmp(argv[i], "-vsync") == 0) 
				sfRenderWindow_setVerticalSyncEnabled(mainWindow->renderWindow, sfTrue);
			else
				sfRenderWindow_setVerticalSyncEnabled(mainWindow->renderWindow, sfFalse);

			/// LAUNCH THE GAME IN EDITOR
			if (strcmp(argv[i], "-editor") == 0) {
				changeState(mainWindow, EDITOR);
				sfRenderWindow_setMouseCursorVisible(mainWindow->renderWindow, sfTrue);
			}
		}
	}

	while (!IsDone(mainWindow))
	{
		windowUpdate(mainWindow);

		BeginDraw(mainWindow);
		windowDraw(mainWindow);
		EndDraw(mainWindow);
	}
	return 0;
}