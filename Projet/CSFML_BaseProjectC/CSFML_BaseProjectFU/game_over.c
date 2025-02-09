#include "game_over.h"
#include "viewManager.h"
#include "fadeManager.h"
#include "fontManager.h"

sfRectangleShape* gameOverRect;
sfText* gameOverText;

sfVector2f gameOverRectPos;
sfVector2f gameOverTextPos;

void gameOver_init() {
	gameOverRect = sfRectangleShape_create();
	sfRectangleShape_setSize(gameOverRect, vector2f(1920.f, 1080.f));
	sfRectangleShape_setFillColor(gameOverRect, sfColor_fromRGBA(255, 255, 255, 0));

	gameOverText = sfText_create();
	sfText_setFont(gameOverText, GetFont("Dico"));
	sfText_setString(gameOverText, "You Died");
	sfText_setCharacterSize(gameOverText, 85);
	sfText_setFillColor(gameOverText, sfColor_fromRGBA(200, 0, 0, 0));

	gameOverRectPos = vector2f(0.f, 0.f);
	gameOverTextPos = vector2f(0.f, 0.f);
}

void gameOver_display(Window* _w) {
	// Déclaration variable
	static float timerGameOver = 0.f;
	timerGameOver += getDeltaTime();
	static sfBool gameOverIn = sfTrue;

	// Logique et script
	sfRenderTexture_setView(_w->rdrt, sfRenderTexture_getDefaultView(_w->rdrt));

	sfText_setOrigin(gameOverText, vector2f(sfText_getGlobalBounds(gameOverText).width * 0.5f, sfText_getGlobalBounds(gameOverText).height * 0.5f));
	sfText_setPosition(gameOverText, vector2f(1920.f * 0.5f, 1080.f * 0.5f));
	
	if (gameOverIn) {
		if (timerGameOver > 6.5f) {
			gameOverIn = sfFalse;
		}
		fadeOut(4.f);
		fade_SetPosition(vector2f(0.f, 0.f));
		fadeInText(4.f, gameOverText);
	}
	else {
		if (timerGameOver > 9.f) {
			toggle_gameOver();
			fade_SetOpacity(0);
			gameOverIn = sfTrue;
			timerGameOver = 0.f;
		}
		fadeIn(1.f);
		fade_SetPosition(vector2f(0.f, 0.f));
		fadeOutText(1.f, gameOverText);
	}

	sfRectangleShape_setPosition(gameOverRect, gameOverRectPos);
	sfRenderTexture_drawRectangleShape(_w->rdrt, gameOverRect, NULL);

	sfRenderTexture_drawText(_w->rdrt, gameOverText, NULL);
	sfRenderTexture_setView(_w->rdrt, mainView->view);
}

void toggle_gameOver() { 
	gameOverScreen = !gameOverScreen; 
}

sfBool get_gameOverState() {
	return gameOverScreen;
}