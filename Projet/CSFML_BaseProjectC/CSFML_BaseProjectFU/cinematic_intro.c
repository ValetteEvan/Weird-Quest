#include "cinematic_intro.h"
#include "viewManager.h"
#include "langManager.h"
#include "textureManager.h"
#include "stateManager.h"
#include "controller.h"
#include "fontManager.h"
#include "dialogBox.h"

#define TIME_BETWEEN_FRAMES 6.f

sfText* cinematicIntroTxt;
sfText* cinematicIntroTxt2;
sfBool cinematicIntroFading;
sfBool cinematicIntroFadingIn;
sfBool cinematicIntroOnePass;
sfBool cinematicIntroIsClicking;
int cinematicIntroNb;
int cinematicIntroFrameX;
float cinematicIntroTimer;
float cinematicIntroFadeTimer;
float cinematicIntroAnimTimer;
sfSprite* cinematicIntroSpr;
sfSprite* cinematicIntroSpr2;
sfSprite* cinematicIntroDB;
sfTexture* cinematicIntroTex;
sfTexture* cinematicIntroTex1;
sfTexture* cinematicIntroTex2;
sfTexture* cinematicIntroTex3;
sfTexture* cinematicIntroTex4;
sfTexture* cinematicIntroTex5;
sfTexture* cinematicIntroTex6;
sfTexture* cinematicIntroTex7;
sfTexture* cinematicIntroTex8;

void cinematicIntro_Init() {
	cinematicIntroTxt = sfText_create();
	cinematicIntroTxt2 = sfText_create();

	cinematicIntroNb, cinematicIntroFrameX, cinematicIntroIsClicking = 0;
	cinematicIntroTimer, cinematicIntroFadeTimer, cinematicIntroAnimTimer = 0.f;

	sfText_setString(cinematicIntroTxt, lang_GetFromKey("opening.anim.1"));
	sfText_setFont(cinematicIntroTxt, GetFont("UGp"));
	sfText_setColor(cinematicIntroTxt, sfColor_fromRGBA(13, 46, 47, 0));
	sfText_setString(cinematicIntroTxt2, lang_GetFromKey("opening.anim.2"));
	sfText_setFont(cinematicIntroTxt2, GetFont("UGp"));
	sfText_setColor(cinematicIntroTxt2, sfColor_fromRGBA(13, 46, 47, 0));
	sfText_setCharacterSize(cinematicIntroTxt, 60);
	sfText_setCharacterSize(cinematicIntroTxt2, 60);
	cinematicIntroOnePass = sfTrue;

	fade_SetTextOpacity(0);
	fade_SetTextOpacity2(0);
	fade_SetOpacity(255);

	if (!GetSfTexture("cinematic")) Texture_Onload(MENU);
	cinematicIntroTex = GetSfTexture("cinematic");
	cinematicIntroTex1 = GetSfTexture("cinematic1");
	cinematicIntroTex2 = GetSfTexture("cinematic2");
	cinematicIntroTex3 = GetSfTexture("cinematic3");
	cinematicIntroTex4 = GetSfTexture("cinematic4");
	cinematicIntroTex5 = GetSfTexture("cinematic5");
	cinematicIntroTex6 = GetSfTexture("cinematic6");
	cinematicIntroTex7 = GetSfTexture("cinematic7");
	cinematicIntroTex8 = GetSfTexture("screen_dodo");
	cinematicIntroSpr = sfSprite_create();
	cinematicIntroSpr2 = sfSprite_create();
	cinematicIntroDB = sfSprite_create();

	sfSprite_setOrigin(cinematicIntroDB, vector2f(sfSprite_getGlobalBounds(cinematicIntroDB).width / 2, sfSprite_getGlobalBounds(cinematicIntroDB).height / 2));
	sfSprite_setPosition(cinematicIntroDB, vector2f(114.f, 785.f));
	sfSprite_setTexture(cinematicIntroDB, GetSfTexture("dialBoxOpening"), sfTrue);
	sfSprite_setColor(cinematicIntroDB, sfColor_fromRGBA(255, 255, 255, 0));
	sfSprite_setTexture(cinematicIntroSpr, cinematicIntroTex1, sfTrue);
	sfSprite_setTexture(cinematicIntroSpr2, cinematicIntroTex, sfTrue);
	sfSprite_setColor(cinematicIntroSpr, sfColor_fromRGBA(255, 255, 255, 0));
	sfSprite_setColor(cinematicIntroSpr2, sfColor_fromRGBA(255, 255, 255, 0));

	db_Init();
}

void cinematicIntro_Update(Window* _window) {
	if (cinematicIntroOnePass) {
		fadeIn(1.f);
		if (fade_GetOpacity() == 0) {
			fadeInSprite3(1.f, cinematicIntroDB);
			fadeInText(1.f, cinematicIntroTxt);
		}
		if (fade_GetSpriteOpacity3() == 255 && fade_GetTextOpacity() == 255) cinematicIntroOnePass = sfFalse;
		fadeInSprite2(1.f, cinematicIntroSpr2);
	}
	else {
		cinematicIntroTimer += getDeltaTime();
		if (cinematicIntroIsClicking) {
			if (cinematicIntroFading) {
				cinematicIntroFading = sfFalse;
				cinematicIntroFadingIn = sfTrue;
				cinematicIntroNb++;
				if (cinematicIntroNb == 2) {
					sfText_setString(cinematicIntroTxt, lang_GetFromKey("opening.anim.3"));
				}
				else if (cinematicIntroNb == 3) {
					sfText_setString(cinematicIntroTxt2, lang_GetFromKey("opening.anim.4"));
				}
				else if (cinematicIntroNb == 4) {
					sfText_setString(cinematicIntroTxt, lang_GetFromKey("opening.anim.5"));
				}
				else if (cinematicIntroNb == 5) {
					sfText_setString(cinematicIntroTxt2, lang_GetFromKey("opening.anim.6"));
				}
				else if (cinematicIntroNb == 6) {
				}
				else if (cinematicIntroNb == 7) {
				}
				else if (cinematicIntroNb == 8) {
					sfText_setString(cinematicIntroTxt, lang_GetFromKey("opening.anim.9"));
				}
			}
			if (cinematicIntroNb == 1) {
				fadeInSprite(1.f, cinematicIntroSpr);
				fadeOutSprite2(1.f, cinematicIntroSpr2);
				if (fade_GetSpriteOpacity2() == 0) sfSprite_setTexture(cinematicIntroSpr2, cinematicIntroTex2, sfTrue);
				fadeOutText(1.f, cinematicIntroTxt);
				fadeInText2(1.f, cinematicIntroTxt2);
			}
			else if (cinematicIntroNb == 2) {
				fadeOutSprite(1.f, cinematicIntroSpr);
				fadeInSprite2(1.f, cinematicIntroSpr2);
				if (fade_GetSpriteOpacity2() == 255) sfSprite_setTexture(cinematicIntroSpr, cinematicIntroTex3, sfTrue);
				fadeInText(1.f, cinematicIntroTxt);
				fadeOutText2(1.f, cinematicIntroTxt2);
			}
			else if (cinematicIntroNb == 3) {
				fadeInSprite(1.f, cinematicIntroSpr);
				fadeOutSprite2(1.f, cinematicIntroSpr2);
				if (fade_GetSpriteOpacity2() == 0) sfSprite_setTexture(cinematicIntroSpr2, cinematicIntroTex4, sfTrue);
				fadeOutText(1.f, cinematicIntroTxt);
				fadeInText2(1.f, cinematicIntroTxt2);
			}
			else if (cinematicIntroNb == 4) {
				fadeOutSprite(1.f, cinematicIntroSpr);
				fadeInSprite2(1.f, cinematicIntroSpr2);
				if (fade_GetSpriteOpacity2() == 255) sfSprite_setTexture(cinematicIntroSpr, cinematicIntroTex5, sfTrue);
				fadeInText(1.f, cinematicIntroTxt);
				fadeOutText2(1.f, cinematicIntroTxt2);
			}
			else if (cinematicIntroNb == 5) {
				fadeInSprite(1.f, cinematicIntroSpr);
				fadeOutSprite2(1.f, cinematicIntroSpr2);
				if (fade_GetSpriteOpacity2() == 0) sfSprite_setTexture(cinematicIntroSpr2, cinematicIntroTex6, sfTrue);
				fadeOutText(1.f, cinematicIntroTxt);
				fadeInText2(1.f, cinematicIntroTxt2);
			}
			 else if (cinematicIntroNb == 6) {
			 	fadeOutSprite(1.f, cinematicIntroSpr);
			 	fadeInSprite2(1.f, cinematicIntroSpr2);
			 	if (fade_GetSpriteOpacity2() == 255) sfSprite_setTexture(cinematicIntroSpr, cinematicIntroTex7, sfTrue);
			 	fadeOutText(1.f, cinematicIntroTxt);
			 	fadeOutText2(1.f, cinematicIntroTxt2);
			 }
			 else if (cinematicIntroNb == 7) {
			 	fadeInSprite(1.f, cinematicIntroSpr);
			 	fadeOutSprite2(1.f, cinematicIntroSpr2);
			 	if (fade_GetSpriteOpacity2() == 0) sfSprite_setTexture(cinematicIntroSpr2, cinematicIntroTex8, sfTrue);
			 }
			 else if (cinematicIntroNb == 8) {
			 	fadeOutSprite(1.f, cinematicIntroSpr);
			 	fadeInSprite2(1.f, cinematicIntroSpr2);
			 	static sfBool onePass = sfTrue;
			 	if (onePass && fade_GetSpriteOpacity() == 0) {
			 		db_Queue(lang_GetFromKey("name.mentor"), lang_GetFromKey("mentor.phaos.wake_up"), vector2f(1000.f, 520.f));
			 		onePass = sfFalse;
			 	}
			 	fadeInText(1.f, cinematicIntroTxt);
			 	sfText_setColor(cinematicIntroTxt2, sfColor_fromRGBA(13, 46, 47, 0));
			 }
			if (cinematicIntroFadingIn) {
				cinematicIntroFadeTimer += getDeltaTime();
				if (cinematicIntroFadeTimer >= 1.f) {
					cinematicIntroFadingIn = sfFalse;
				}
			}
		}
		else if (!cinematicIntroIsClicking) {
			if (cinematicIntroFading) {
				cinematicIntroFading = sfFalse;
				cinematicIntroFadingIn = sfTrue;
				cinematicIntroNb++;
			}
			if (cinematicIntroNb == 1) {
				if (fade_GetTextOpacity2() == 0) {
					fadeOutSprite2(1.f, cinematicIntroSpr2);
					fadeInSprite(1.f, cinematicIntroSpr);
					if (fade_GetSpriteOpacity2() == 0) {
						sfSprite_setTexture(cinematicIntroSpr2, cinematicIntroTex2, sfTrue);
						sfText_setString(cinematicIntroTxt, lang_GetFromKey("opening.anim.3"));
						fadeInText(1.f, cinematicIntroTxt2);
						fadeInSprite3(1.f, cinematicIntroDB);
					}
				}
				else {
					fadeOutText2(1.f, cinematicIntroTxt);
					fadeOutSprite3(1.f, cinematicIntroDB);
				}
			}
			else if (cinematicIntroNb == 2) {
				if (fade_GetTextOpacity() == 0) {
					fadeOutSprite(1.f, cinematicIntroSpr);
					fadeInSprite2(1.f, cinematicIntroSpr2);
					if (fade_GetSpriteOpacity() == 0) {
						sfSprite_setTexture(cinematicIntroSpr, cinematicIntroTex3, sfTrue);
						sfText_setString(cinematicIntroTxt, lang_GetFromKey("opening.anim.3"));
						fadeInText2(1.f, cinematicIntroTxt);
						fadeInSprite3(1.f, cinematicIntroDB);
					}
				}
				else {
					fadeOutText(1.f, cinematicIntroTxt2);
					fadeOutSprite3(1.f, cinematicIntroDB);
				}
			}
			else if (cinematicIntroNb == 3) {
				if (fade_GetTextOpacity2() == 0) {
					fadeOutSprite2(1.f, cinematicIntroSpr2);
					fadeInSprite(1.f, cinematicIntroSpr);
					if (fade_GetSpriteOpacity2() == 0) {
						sfSprite_setTexture(cinematicIntroSpr2, cinematicIntroTex4, sfTrue);
						sfText_setString(cinematicIntroTxt2, lang_GetFromKey("opening.anim.4"));
						fadeInText(1.f, cinematicIntroTxt2);
						fadeInSprite3(1.f, cinematicIntroDB);
					}
				}
				else {
					fadeOutText2(1.f, cinematicIntroTxt);
					fadeOutSprite3(1.f, cinematicIntroDB);
				}
			}
			else if (cinematicIntroNb == 4) {
				if (fade_GetTextOpacity() == 0) {
					fadeOutSprite(1.f, cinematicIntroSpr);
					fadeInSprite2(1.f, cinematicIntroSpr2);
					if (fade_GetSpriteOpacity() == 0) {
						sfSprite_setTexture(cinematicIntroSpr, cinematicIntroTex5, sfTrue);
						sfText_setString(cinematicIntroTxt, lang_GetFromKey("opening.anim.5"));
						fadeInText2(1.f, cinematicIntroTxt);
						fadeInSprite3(1.f, cinematicIntroDB);
					}
				}
				else {
					fadeOutText(1.f, cinematicIntroTxt2);
					fadeOutSprite3(1.f, cinematicIntroDB);
				}
			}
			else if (cinematicIntroNb == 5) {
				if (fade_GetTextOpacity2() == 0) {
					fadeOutSprite2(1.f, cinematicIntroSpr2);
					fadeInSprite(1.f, cinematicIntroSpr);
					if (fade_GetSpriteOpacity2() == 0) {
						sfSprite_setTexture(cinematicIntroSpr2, cinematicIntroTex6, sfTrue);
						sfText_setString(cinematicIntroTxt2, lang_GetFromKey("opening.anim.6"));
						fadeInText(1.f, cinematicIntroTxt2);
						fadeInSprite3(1.f, cinematicIntroDB);
					}
				}
				else {
					fadeOutText2(1.f, cinematicIntroTxt);
					fadeOutSprite3(1.f, cinematicIntroDB);
				}
			}
			else if (cinematicIntroNb == 6) {
				if (fade_GetTextOpacity() == 0) {
					fadeOutSprite(1.f, cinematicIntroSpr);
					fadeInSprite2(1.f, cinematicIntroSpr2);
					if (fade_GetSpriteOpacity() == 0) {
						sfSprite_setTexture(cinematicIntroSpr, cinematicIntroTex7, sfTrue);
						sfText_setString(cinematicIntroTxt, lang_GetFromKey("opening.anim.6"));
						fadeInText2(1.f, cinematicIntroTxt);
						fadeInSprite3(1.f, cinematicIntroDB);
					}
				}
				else {
					fadeOutText(1.f, cinematicIntroTxt2);
					fadeOutSprite3(1.f, cinematicIntroDB);
				}
			}
			else if (cinematicIntroNb == 7) {
				if (fade_GetTextOpacity2() == 0) {
					fadeOutSprite2(1.f, cinematicIntroSpr2);
					fadeInSprite(1.f, cinematicIntroSpr);
					if (fade_GetSpriteOpacity2() == 0) {
						sfText_setString(cinematicIntroTxt2, lang_GetFromKey("opening.anim.9"));
						sfSprite_setTexture(cinematicIntroSpr2, cinematicIntroTex8, sfTrue);
						fadeInText(1.f, cinematicIntroTxt2);
						fadeInSprite3(1.f, cinematicIntroDB);
					}
				}
				else {
					fadeOutText2(1.f, cinematicIntroTxt);
					fadeOutSprite3(1.f, cinematicIntroDB);
				}
			}
			else if (cinematicIntroNb == 8) {
				if (fade_GetTextOpacity() == 0) {
					fadeOutSprite(1.f, cinematicIntroSpr);
					fadeInSprite2(1.f, cinematicIntroSpr2);
					if (fade_GetSpriteOpacity() == 0) {
						sfSprite_setTexture(cinematicIntroSpr, cinematicIntroTex7, sfTrue);
						sfText_setString(cinematicIntroTxt, lang_GetFromKey("opening.anim.9"));
						fadeInText2(1.f, cinematicIntroTxt);
						fadeInSprite3(1.f, cinematicIntroDB);
					}
				}
				else {
					fadeOutText(1.f, cinematicIntroTxt2);
					fadeOutSprite3(1.f, cinematicIntroDB);
				}
				static sfBool onePass = sfTrue;
				if (onePass && fade_GetSpriteOpacity() == 0) {
					db_Queue(lang_GetFromKey("name.mentor"), lang_GetFromKey("mentor.phaos.wake_up"), vector2f(1000.f, 520.f));
					onePass = sfFalse;
				}
			}
		}
		if ((customKeyboardIsKeyPressed(sfKeyEnter, _window->renderWindow) || Gamepad_isButtonPressed(0, A, _window->renderWindow))) {
			cinematicIntroIsClicking = sfTrue;
		}
		else if (cinematicIntroTimer > TIME_BETWEEN_FRAMES && !cinematicIntroIsClicking) {
			cinematicIntroTimer = 0.f;
			cinematicIntroFading = sfTrue;
			if (fade_GetSpriteOpacity() == 255) fade_SetSpriteOpacity(254);
			else if (fade_GetSpriteOpacity2() == 255) fade_SetSprite2Opacity(254);
			if (cinematicIntroNb == 0) {
				fade_SetTextOpacity(0);
				fade_SetTextOpacity2(255);
				fade_SetSpriteOpacity(0);
				fade_SetSprite2Opacity(255);
			}
		}
		if ((fade_GetSpriteOpacity() == 255 || fade_GetSpriteOpacity2() == 255) && !cinematicIntroFadingIn && cinematicIntroIsClicking && (customKeyboardIsKeyPressed(sfKeyEnter, _window->renderWindow) || Gamepad_isButtonPressed(0, A, _window->renderWindow))) {
			cinematicIntroFading = sfTrue;
			if (fade_GetSpriteOpacity() == 255) fade_SetSpriteOpacity(254);
			else if (fade_GetSpriteOpacity2() == 255) fade_SetSprite2Opacity(254);
		}
		if (cinematicIntroNb >= 9) {
			fadeOut(1.f);
			if (fade_GetOpacity() == 255) {
				while (db_IsUp()) db_Next();
				changeState(_window, GAME);
				fade_SetOpacity(0);
			}
		}
		if (cinematicIntroFadingIn) {
			cinematicIntroFadeTimer += getDeltaTime();
			if (cinematicIntroFadeTimer >= 1.f) {
				cinematicIntroFadingIn = sfFalse;
			}
		}
	}
}

void cinematicIntro_Display(Window* _window) {
	sfText_setOrigin(cinematicIntroTxt, vector2f(sfText_getGlobalBounds(cinematicIntroTxt).width / 2.f, sfText_getGlobalBounds(cinematicIntroTxt).height / 2.f));
	sfSprite_setTextureRect(cinematicIntroDB, animatorAnim("dialBoxOpening", &cinematicIntroFrameX, &cinematicIntroAnimTimer, 0));
	sfText_setPosition(cinematicIntroTxt, vector2f(114.f + sfSprite_getGlobalBounds(cinematicIntroDB).width / 2, 785.f + sfSprite_getGlobalBounds(cinematicIntroDB).height / 2 - 30.f));
	sfText_setOrigin(cinematicIntroTxt2, vector2f(sfText_getGlobalBounds(cinematicIntroTxt2).width / 2.f, sfText_getGlobalBounds(cinematicIntroTxt2).height / 2.f));
	sfText_setPosition(cinematicIntroTxt2, vector2f(114.f + sfSprite_getGlobalBounds(cinematicIntroDB).width / 2, 785.f + sfSprite_getGlobalBounds(cinematicIntroDB).height / 2 - 30.f));
	sfRenderTexture_drawSprite(_window->rdrt, cinematicIntroSpr, NULL);
	sfRenderTexture_drawSprite(_window->rdrt, cinematicIntroSpr2, NULL);
	if ((fade_GetTextOpacity() > 0 || fade_GetTextOpacity2() > 0) && !((fade_GetTextOpacity2() <= 125 || fade_GetTextOpacity() <= 125) && cinematicIntroNb == 7) && cinematicIntroIsClicking) sfRenderTexture_drawSprite(_window->rdrt, cinematicIntroDB, NULL);
	else if ((fade_GetTextOpacity() > 0 || fade_GetTextOpacity2() > 0) && !cinematicIntroIsClicking) sfRenderTexture_drawSprite(_window->rdrt, cinematicIntroDB, NULL);
	sfRenderTexture_drawText(_window->rdrt, cinematicIntroTxt, NULL);
	sfRenderTexture_drawText(_window->rdrt, cinematicIntroTxt2, NULL);
	db_Display(_window);
	fade_Display(_window);
}

void cinematicIntro_Deinit() {
	if (!cinematicIntroTxt ||
		!cinematicIntroTxt2 ||
		!cinematicIntroSpr ||
		!cinematicIntroSpr2 ||
		!cinematicIntroDB) 
		return;
	sfText_destroy(cinematicIntroTxt);
	sfText_destroy(cinematicIntroTxt2);
	sfSprite_destroy(cinematicIntroSpr);
	sfSprite_destroy(cinematicIntroSpr2);
	sfSprite_destroy(cinematicIntroDB);
	RemoveAllTextureButALL();
}