#include "opening_screen.h"
#include "stateManager.h"
#include "textureManager.h"
#include "controller.h"
#include "viewManager.h"
#include "soundManager.h"
#include "menu.h"

#define OPENING_TIME_ANIM 0.2f

int opening_Nb;
int opening_NbAnim;
int opening_TempOpacity;
sfText* opening_Txt;
sfSprite* opening_Spr;
sfSprite* opening_Press;
sfSprite* opening_Fond;
sfSprite* opening_Logo;
sfTexture* opening_Tex;
sfTexture* opening_TexCopyright;
sfBool opening_Fading;
sfBool opening_FadingIn;
sfBool opening_CanShowPress;
sfBool opening_PressFade;
float opening_Timer;
float opening_TimerAnim;
float opening_WQTimer;
sfTexture* opening_Tex1;
sfTexture* opening_Tex2;
sfTexture* opening_Tex3;
sfTexture* opening_Tex4;
sfTexture* opening_Tex5;
sfTexture* opening_Tex6;
sfTexture* opening_Tex7;
sfTexture* opening_Tex8;
sfTexture* opening_Tex9;
sfTexture* opening_Tex10;
sfTexture* opening_Tex11;
sfTexture* opening_Tex12;
sfTexture* opening_Tex13;
sfTexture* opening_Tex14;
sfTexture* opening_Tex15;
sfTexture* opening_Tex16;
sfTexture* opening_Tex17;
sfTexture* opening_Tex18;
sfTexture* opening_Tex19;
sfTexture* opening_Tex20;
sfTexture* opening_Tex21;
sfTexture* opening_Tex22;
sfTexture* opening_Tex23;
sfTexture* opening_Tex24;
sfTexture* opening_Tex25;
sfTexture* opening_Tex26;
sfTexture* opening_Tex27;
sfTexture* opening_Tex28;
sfTexture* opening_Tex29;
sfTexture* opening_Tex30;
sfTexture* opening_Tex31;
sfTexture* opening_Tex32;
sfTexture* opening_Tex33;
sfTexture* opening_Tex34;

sfBool onePassPressToStart;

void openingScreen_Init() {
	opening_Txt = sfText_create();
	sfText_setFont(opening_Txt, sfFont_createFromFile("..\\Ressources\\Fonts\\UGp.ttf"));
	sfText_setPosition(opening_Txt, vector2f(900.f, 500.f));
	opening_Fading, opening_FadingIn, opening_CanShowPress = sfFalse;
	opening_PressFade = sfTrue;
	fade_SetSpriteOpacity(0);
	fade_SetOpacity(0);
	if (!GetSfTexture("HUD")) Texture_Onload(MENU);
	opening_Nb, opening_NbAnim = 0;
	opening_Timer, opening_TimerAnim, opening_WQTimer = 0.f;
	sfText_setString(opening_Txt, "Weird quest logo");
	opening_Spr = sfSprite_create();
	opening_Tex = GetSfTexture("openingCopyright");
	opening_TexCopyright = sfTexture_createFromFile("..\\Ressources\\Textures\\animationDebut\\copyright_FR.png", NULL);
	sfSprite_setTexture(opening_Spr, opening_Tex, sfTrue);
	opening_Press = sfSprite_create();
	opening_Logo = sfSprite_create();
	sfSprite_setColor(opening_Logo, sfColor_fromRGBA(sfSprite_getColor(opening_Logo).r, sfSprite_getColor(opening_Logo).g, sfSprite_getColor(opening_Logo).b, fade_GetSpriteOpacity()));
	sfSprite_setTexture(opening_Spr, opening_Tex, sfTrue);
	sfSprite_setTexture(opening_Press, GetSfTexture("openingPress"), sfTrue);
	sfSprite_setPosition(opening_Press, vector2f(670.f, 998.f));
	sfSprite_setScale(opening_Press, vector2f(0.75f, 0.75f));
	opening_TempOpacity = 0;
	opening_Tex1 =  GetSfTexture("opening1");
	opening_Tex2 =  GetSfTexture("opening2");
	opening_Tex3 =  GetSfTexture("opening3");
	opening_Tex4 =  GetSfTexture("opening4");
	opening_Tex5 =  GetSfTexture("opening5");
	opening_Tex6 =  GetSfTexture("opening6");
	opening_Tex7 =  GetSfTexture("opening7");
	opening_Tex8 =  GetSfTexture("opening8");
	opening_Tex9 =  GetSfTexture("opening9");
	opening_Tex10 = GetSfTexture("opening10");
	opening_Tex11 = GetSfTexture("opening11");
	opening_Tex12 = GetSfTexture("opening12");
	opening_Tex13 = GetSfTexture("opening13");
	opening_Tex14 = GetSfTexture("opening14");
	opening_Tex15 = GetSfTexture("opening15");
	opening_Tex16 = GetSfTexture("opening16");
	opening_Tex17 = GetSfTexture("opening17");
	opening_Tex18 = GetSfTexture("opening18");
	opening_Tex19 = GetSfTexture("opening19");
	opening_Tex20 = GetSfTexture("opening20");
	opening_Tex21 = GetSfTexture("opening21");
	opening_Tex22 = GetSfTexture("opening22");
	opening_Tex23 = GetSfTexture("opening23");
	opening_Tex24 = GetSfTexture("opening24");
	opening_Tex25 = GetSfTexture("opening25");
	opening_Tex26 = GetSfTexture("opening26");
	opening_Tex27 = GetSfTexture("opening27");
	opening_Tex28 = GetSfTexture("opening28");
	opening_Tex29 = GetSfTexture("opening29");
	opening_Tex30 = GetSfTexture("opening30");
	opening_Tex31 = GetSfTexture("opening31");
	opening_Tex32 = GetSfTexture("opening32");
	opening_Tex33 = GetSfTexture("opening33");
	opening_Tex34 = GetSfTexture("opening34");
	opening_Fond = sfSprite_create();
	sfSprite_setTexture(opening_Fond, GetSfTexture("openingFond"), sfTrue);
	sfSprite_setPosition(opening_Fond, vector2f(0.f, 0.f));
	onePassPressToStart = sfTrue;
}

void openingScreen_Update(Window* _window) {
	if (opening_Nb != 0)
		opening_Timer += getUnscaledDeltaTime();
	else
		opening_Timer = 3.1f;
	centralText(opening_Txt);
	if ((customKeyboardIsKeyPressed(sfKeyEnter, _window->renderWindow) || Gamepad_isButtonPressed(0, A, _window->renderWindow)) && opening_Timer > 3.f && opening_Nb != 1) {
		opening_Fading = sfTrue;
		opening_Timer = 0.f;
	}
	if (opening_Fading) {
		if (opening_Nb == 0) {
			fadeOutSprite(1.5f, opening_Spr);
			if (fade_GetSpriteOpacity() == 0) {
				opening_Nb = 1;
				opening_Fading = sfFalse;
				opening_FadingIn = sfTrue;
				sfSprite_setTexture(opening_Spr, opening_Tex1, sfTrue);
			}
		}
		else {
			if (opening_Nb == 1) fadeOut(1.5f);
			else {
				fadeOutSprite2(1.5f, opening_Press);
				fadeOutSprite(1.5f, opening_Spr);
			}
			if (fade_GetSpriteOpacity() == 0 || (opening_Nb == 1 && fade_GetOpacity() == 255)) {
				opening_Fading = sfFalse;
				opening_FadingIn = sfTrue;
				opening_Nb++;
				if (opening_Nb == 1) {
					sfText_setString(opening_Txt, "Copyright screen");
					sfSprite_setTexture(opening_Spr, opening_TexCopyright, sfTrue);
				}
				if (opening_Nb == 2) {
					changeState(_window, MENU);
				}
				opening_Timer = 0.f;
			}
		}
	}
	if (opening_Nb == 1 && fade_GetSpriteOpacity2() == 255) {
		opening_TimerAnim += getUnscaledDeltaTime();
		if (opening_TimerAnim > 0.1f) {
			opening_NbAnim++;
			if (opening_NbAnim == 1) sfSprite_setTexture(opening_Spr, opening_Tex1, sfTrue);
			if (opening_NbAnim == 2) sfSprite_setTexture(opening_Spr, opening_Tex2, sfTrue);
			if (opening_NbAnim == 3) sfSprite_setTexture(opening_Spr, opening_Tex3, sfTrue);
			if (opening_NbAnim == 4) sfSprite_setTexture(opening_Spr, opening_Tex4, sfTrue);
			if (opening_NbAnim == 5) sfSprite_setTexture(opening_Spr, opening_Tex5, sfTrue);
			if (opening_NbAnim == 6) sfSprite_setTexture(opening_Spr, opening_Tex6, sfTrue);
			if (opening_NbAnim == 7) sfSprite_setTexture(opening_Spr, opening_Tex7, sfTrue);
			if (opening_NbAnim == 8) sfSprite_setTexture(opening_Spr, opening_Tex8, sfTrue);
			if (opening_NbAnim == 9) sfSprite_setTexture(opening_Spr, opening_Tex9, sfTrue);
			if (opening_NbAnim == 10) sfSprite_setTexture(opening_Spr, opening_Tex10, sfTrue);
			if (opening_NbAnim == 11) sfSprite_setTexture(opening_Spr, opening_Tex11, sfTrue);
			if (opening_NbAnim == 12) sfSprite_setTexture(opening_Spr, opening_Tex12, sfTrue);
			if (opening_NbAnim == 13) sfSprite_setTexture(opening_Spr, opening_Tex13, sfTrue);
			if (opening_NbAnim == 14) sfSprite_setTexture(opening_Spr, opening_Tex14, sfTrue);
			if (opening_NbAnim == 15) sfSprite_setTexture(opening_Spr, opening_Tex15, sfTrue);
			if (opening_NbAnim == 16) sfSprite_setTexture(opening_Spr, opening_Tex16, sfTrue);
			if (opening_NbAnim == 17) sfSprite_setTexture(opening_Spr, opening_Tex17, sfTrue);
			if (opening_NbAnim == 18) sfSprite_setTexture(opening_Spr, opening_Tex18, sfTrue);
			if (opening_NbAnim == 19) sfSprite_setTexture(opening_Spr, opening_Tex19, sfTrue);
			if (opening_NbAnim == 20) sfSprite_setTexture(opening_Spr, opening_Tex20, sfTrue);
			if (opening_NbAnim == 21) sfSprite_setTexture(opening_Spr, opening_Tex21, sfTrue);
			if (opening_NbAnim == 22) sfSprite_setTexture(opening_Spr, opening_Tex22, sfTrue);
			if (opening_NbAnim == 23) sfSprite_setTexture(opening_Spr, opening_Tex23, sfTrue);
			if (opening_NbAnim == 24) sfSprite_setTexture(opening_Spr, opening_Tex24, sfTrue);
			if (opening_NbAnim == 25) sfSprite_setTexture(opening_Spr, opening_Tex25, sfTrue);
			if (opening_NbAnim == 26) {
				sfSprite_setTexture(opening_Spr, opening_Tex26, sfTrue);
				playSoundFX("SFX_TournePage");
			}
			if (opening_NbAnim == 27) sfSprite_setTexture(opening_Spr, opening_Tex27, sfTrue);
			if (opening_NbAnim == 28) sfSprite_setTexture(opening_Spr, opening_Tex28, sfTrue);
			if (opening_NbAnim == 29) sfSprite_setTexture(opening_Spr, opening_Tex29, sfTrue);
			if (opening_NbAnim == 30) sfSprite_setTexture(opening_Spr, opening_Tex30, sfTrue);
			if (opening_NbAnim == 31) sfSprite_setTexture(opening_Spr, opening_Tex31, sfTrue);
			if (opening_NbAnim == 32) sfSprite_setTexture(opening_Spr, opening_Tex32, sfTrue);
			if (opening_NbAnim == 33) sfSprite_setTexture(opening_Spr, opening_Tex33, sfTrue);
			if (opening_NbAnim >= 34) sfSprite_setTexture(opening_Spr, opening_Tex34, sfTrue);
			opening_TimerAnim = 0.f;
		}
	}
	if (opening_NbAnim > 34 && opening_Nb == 1) {
		if (onePassPressToStart)
		{
			setVolume();
			playMusicIfNotPlaying("Music_Menu");
			setOnePassMenu(sfFalse);
			onePassPressToStart = sfFalse;
		}
		opening_CanShowPress = sfTrue;
		if ((Gamepad_isButtonPressed(0, A, _window->renderWindow) || customKeyboardIsKeyPressed(sfKeyEnter, _window->renderWindow))) {
			opening_Fading = sfTrue;
			sfSprite_setColor(opening_Press, sfColor_fromRGBA(sfSprite_getColor(opening_Press).r, sfSprite_getColor(opening_Press).g, sfSprite_getColor(opening_Press).b, 255 / 3));
		}
	}
	if (opening_CanShowPress && !opening_Fading) {
		if (fade_GetSpriteOpacity2() == 255) opening_PressFade = sfFalse;
		else if (fade_GetSpriteOpacity2() == 255 / 3) opening_PressFade = sfTrue;
		if (opening_PressFade) fadeInSprite2(1.f, opening_Press);
		else fadeOutSprite2(1.f, opening_Press);
	}
	/*if (opening_Nb == 3 && opening_Timer > 3.f) {
		opening_Fading = sfTrue;
	}*/
	if (opening_Nb == 0 && !opening_Fading) {
		fadeInSprite(1.5f, opening_Spr);
		if (opening_Timer > 10.f) opening_Fading = sfTrue;
	}
	if (opening_FadingIn) {
		if (opening_Nb == 1) {
			fadeInSprite(1.f, opening_Logo);
			if (fade_GetSpriteOpacity() == 255) {
				opening_FadingIn = sfFalse;
				fade_SetSprite2Opacity(255);
				sfSprite_setColor(opening_Spr, sfColor_fromRGBA(sfSprite_getColor(opening_Logo).r, sfSprite_getColor(opening_Logo).g, sfSprite_getColor(opening_Logo).b, fade_GetSpriteOpacity2()));
			}
		}
		else {
			fadeInSprite(1.f, opening_Spr);
			if (opening_Nb == 1 || opening_Nb == 2) fadeInSprite2(1.5f / 255.f * fade_GetSpriteOpacity2(), opening_Press); else fadeInSprite2(1.5f, opening_Press);
			if (fade_GetSpriteOpacity() == 255) {
				opening_FadingIn = sfFalse;
			}
		}
	}
	if (opening_NbAnim >= 34) {
		opening_WQTimer += getDeltaTime();
		if (opening_WQTimer > 19.f * OPENING_TIME_ANIM) opening_WQTimer = 0.f;
	}
}

void openingScreen_Display(Window* _window) {
	if (opening_Nb > 0) sfRenderTexture_drawSprite(_window->rdrt, opening_Fond, NULL);
	if (opening_Nb == 1 || opening_Nb == 2) {
		sfSprite_setPosition(opening_Logo, vector2f(669.f, 169.f));
		sfSprite_setTexture(opening_Logo, GetSfTexture("openingLogoGA"), sfTrue);
		sfRenderTexture_drawSprite(_window->rdrt, opening_Logo, NULL);
		sfSprite_setPosition(opening_Spr, vector2f(0.f, 0.f));
		if (fade_GetSpriteOpacity2() == 255) sfRenderTexture_drawSprite(_window->rdrt, opening_Spr, NULL);
		if (opening_NbAnim > 34 && opening_Nb == 1) {
			if (opening_NbAnim >= 34) {
				sfSprite_setTexture(opening_Spr, opening_Tex34, sfTrue);
				sfSprite_setPosition(opening_Spr, vector2f(0.f, 0.f));
				sfRenderTexture_drawSprite(_window->rdrt, opening_Spr, NULL);
			}

			if (opening_WQTimer > 18.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_1"), sfTrue);
			else if (opening_WQTimer > 17.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_2"), sfTrue);
			else if (opening_WQTimer > 16.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_3"), sfTrue);
			else if (opening_WQTimer > 15.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_4"), sfTrue);
			else if (opening_WQTimer > 14.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_5"), sfTrue);
			else if (opening_WQTimer > 13.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_6"), sfTrue);
			else if (opening_WQTimer > 12.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_7"), sfTrue);
			else if (opening_WQTimer > 11.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_8"), sfTrue);
			else if (opening_WQTimer > 10.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_9"), sfTrue);

			else if (opening_WQTimer > 9.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_10"), sfTrue);
			else if (opening_WQTimer > 8.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_11"), sfTrue);
			else if (opening_WQTimer > 7.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_12"), sfTrue);
			else if (opening_WQTimer > 6.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_13"), sfTrue);
			else if (opening_WQTimer > 5.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_14"), sfTrue);
			else if (opening_WQTimer > 4.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_15"), sfTrue);
			else if (opening_WQTimer > 3.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_16"), sfTrue);
			else if (opening_WQTimer > 2.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_17"), sfTrue);
			else if (opening_WQTimer > 1.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_18"), sfTrue);
			else if (opening_WQTimer > 0.f * OPENING_TIME_ANIM) sfSprite_setTexture(opening_Spr, GetSfTexture("opening_19"), sfTrue);
			sfSprite_setPosition(opening_Spr, vector2f(0.f, -150.f));
			sfRenderTexture_drawSprite(_window->rdrt, opening_Spr, NULL);

			sfSprite_setPosition(opening_Spr, vector2f(295.f, 788.f));
			sfSprite_setTexture(opening_Spr, GetSfTexture("theMonsterKeeper"), sfTrue);
			sfRenderTexture_drawSprite(_window->rdrt, opening_Spr, NULL);
		}
		if (opening_CanShowPress) sfRenderTexture_drawSprite(_window->rdrt, opening_Press, NULL);
	}
	else {
		sfSprite_setPosition(opening_Spr, vector2f(0.f, 0.f));
		sfRenderTexture_drawSprite(_window->rdrt, opening_Spr, NULL);
		if (opening_CanShowPress) sfRenderTexture_drawSprite(_window->rdrt, opening_Press, NULL);
	}
	fade_Display(_window);
}

void openingScreen_Deinit() {
	if (!opening_Txt) return;
	sfText_destroy(opening_Txt);
	RemoveAllTextureButALL();
	RemoveAllSoundsNotNeededInEveryStates();
}