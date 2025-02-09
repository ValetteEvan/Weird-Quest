#include "cinematic_outro.h"
#include "viewManager.h"
#include "langManager.h"

sfText* cinematicOutroTxt;
sfBool cinematicOutroFading;
sfBool cinematicOutroFadingIn;
int cinematicOutroNb;
float cinematicOutroTimer;
// sfSprite* cinematicOutroSpr;
// sfTexture* cinematicOutroTex;
// sfTexture* cinematicOutroTex2;
// sfTexture* cinematicOutroTex3;
// sfTexture* cinematicOutroTex4;
// sfTexture* cinematicOutroTex5;
// sfTexture* cinematicOutroTex6;
// sfTexture* cinematicOutroTex7;
// sfTexture* cinematicOutroTex8;
// sfTexture* cinematicOutroTex9;
// sfTexture* cinematicOutroTex10;
// sfTexture* cinematicOutroTex11;
// sfTexture* cinematicOutroTex12;
// sfTexture* cinematicOutroTex13;
// sfTexture* cinematicOutroTex14;
// sfTexture* cinematicOutroTex15;
// sfTexture* cinematicOutroTex16;
// sfTexture* cinematicOutroTex17;
// sfTexture* cinematicOutroTex18;
// sfTexture* cinematicOutroTex19;
// sfTexture* cinematicOutroTex20;
// sfTexture* cinematicOutroTex21;
// sfTexture* cinematicOutroTex22;

void cinematicOutro_Init() {
	cinematicOutroTxt = sfText_create();
	cinematicOutroNb = 0;
	lang_SetLang(1);
	sfText_setPosition(cinematicOutroTxt, vector2f(mainView->viewPos.x, mainView->viewPos.y * 1.5f));
	sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.1"));
	sfText_setFont(cinematicOutroTxt, sfFont_createFromFile("..\\Ressources\\Fonts\\UGp.ttf"));
	cinematicOutroTimer = 0.f;
	// cinematicOutroSpr = sfSprite_create();
	// cinematicIntroTex = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex2 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex3 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex4 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex5 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex6 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex7 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex8 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex9 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex10 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex11 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex12 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex13 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex14 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex15 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex16 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex17 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex18 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex19 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex20 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex21 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// cinematicIntroTex22 = sfTexture_createFromFile("..\\Ressources\\.png", NULL);
	// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex, sfTrue);
}

void cinematicOutro_Update(Window* _window) {
	cinematicOutroTimer += getDeltaTime();
	if (customKeyboardIsKeyPressed(sfKeyEnter, _window->renderWindow) && cinematicOutroTimer > 3.f) {
		cinematicOutroFading = sfTrue;
		cinematicOutroTimer = 0.f;
	}
	if (cinematicOutroFading) {
		fadeOut(1.f);
		if (fade_GetOpacity() == 255) {
			cinematicOutroFading = sfFalse;
			cinematicOutroFadingIn = sfTrue;
			cinematicOutroNb++;
			if (cinematicOutroNb == 1) {
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.2"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex2, sfTrue);
			}
			else if (cinematicOutroNb == 2) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.3"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex3, sfTrue);
			}
			else if (cinematicOutroNb == 3) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.4"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex4, sfTrue);
			}
			else if (cinematicOutroNb == 4) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.5"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex5, sfTrue);
			}
			else if (cinematicOutroNb == 5) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.6"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex6, sfTrue);
			}
			else if (cinematicOutroNb == 6) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.7"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex7, sfTrue);
			}
			else if (cinematicOutroNb == 7) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.8"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex8, sfTrue);
			}
			else if (cinematicOutroNb == 8) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.9"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex9, sfTrue);
			}
			else if (cinematicOutroNb == 9) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.8"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex10, sfTrue);
			}
			else if (cinematicOutroNb == 10) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.11"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex11, sfTrue);
			}
			else if (cinematicOutroNb == 11) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.12"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex12, sfTrue);
			}
			else if (cinematicOutroNb == 12) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.13"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex13, sfTrue);
			}
			else if (cinematicOutroNb == 13) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.14"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex14, sfTrue);
			}
			else if (cinematicOutroNb == 14) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.15"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex15, sfTrue);
			}
			else if (cinematicOutroNb == 15) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey(""));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex16, sfTrue);
			}
			else if (cinematicOutroNb == 16) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.17"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex17, sfTrue);
			}
			else if (cinematicOutroNb == 17) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.18"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex18, sfTrue);
			}
			else if (cinematicOutroNb == 18) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.19"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex19, sfTrue);
			}
			else if (cinematicOutroNb == 19) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.20"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex20, sfTrue);
			}
			else if (cinematicOutroNb == 20) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.8"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex21, sfTrue);
			}
			else if (cinematicOutroNb == 21) {						
				sfText_setString(cinematicOutroTxt, lang_GetFromKey("final.anim.22"));
				// sfSprite_setTexture(cinematicOutroSpr, cinematicOutroTex22, sfTrue);
			}
			if (cinematicOutroNb == 22) {
				changeState(_window, MENU);
				fade_SetOpacity(0);
			}
		}
	}
	if (cinematicOutroFadingIn) {
		fadeIn(1.f);
		if (fade_GetOpacity() == 0) {
			cinematicOutroFadingIn = sfFalse;
		}
	}
}

void cinematicOutro_Display(Window* _window) {
	fade_Display(_window);
	sfText_setOrigin(cinematicOutroTxt, vector2f(sfText_getGlobalBounds(cinematicOutroTxt).width / 2.0f, sfText_getGlobalBounds(cinematicOutroTxt).height / 2.0f));
	sfRenderTexture_drawText(_window->rdrt, cinematicOutroTxt, NULL);
}

void cinematicOutro_Deinit() {
	if (!cinematicOutroTxt) return;
	sfText_destroy(cinematicOutroTxt);
}