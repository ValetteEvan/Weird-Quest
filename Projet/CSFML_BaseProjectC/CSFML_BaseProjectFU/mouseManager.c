#include "mouseManager.h"
#include "viewManager.h"
#include "windowManager.h"

float mouseScrollWheelValue = 0.f;

sfBool leftClick(sfRenderWindow* _window)
{
	if (fullscreen_IsDone) {
		if (sfRenderWindow_hasFocus(_window))
			return sfMouse_isButtonPressed(sfMouseLeft);
	}
	return sfFalse;
}

sfBool rightClick(sfRenderWindow* _window)
{
	if (fullscreen_IsDone) {
		if (sfRenderWindow_hasFocus(_window))
			return sfMouse_isButtonPressed(sfMouseRight);
	}
	return sfFalse;
}

void MouseScrollWheel(sfEvent event)
{
	if (event.type == sfEvtMouseWheelScrolled)
	{
		if (event.mouseWheelScroll.wheel == sfMouseVerticalWheel) {
			// Si la molette de la souris a été déplacée verticalement
			if (event.mouseWheelScroll.delta > 0) {
				// La molette a été déplacée vers le haut
				mouseScrollWheelValue = 1;
			}
			else if (event.mouseWheelScroll.delta < 0) {

				// La molette a été déplacée vers le bas
				mouseScrollWheelValue = -1;
			}
		}
	}
}

int getMouseScrollWheel()
{
	return mouseScrollWheelValue;
}

sfVector2f getfMousePos(sfRenderWindow* _window)
{
	return vector2f(sfMouse_getPositionRenderWindow(_window).x, sfMouse_getPositionRenderWindow(_window).y);
}

sfVector2f getfWorldMousePos(sfRenderWindow* _window)
{
	sfVector2i mousePos = sfMouse_getPositionRenderWindow(_window);
	//return vector2f(mousePos.x, mousePos.y);

	return sfRenderWindow_mapPixelToCoords(_window, mousePos, mainView->view);
}