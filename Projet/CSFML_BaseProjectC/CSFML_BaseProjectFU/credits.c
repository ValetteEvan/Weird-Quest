#include "credits.h"
#include "controller.h"
#include "fadeManager.h"
#include "fontManager.h"

#define CREDITS_NAME_PROG1 "ANSEL Bryan"
#define CREDITS_NAME_PROG2 "BONNEFOY Teddy"
#define CREDITS_NAME_PROG3 "BRACHET Ugo"
#define CREDITS_NAME_PROG4 "CLEENEWERCK Noe"
#define CREDITS_NAME_PROG5 "FORNEROD Adrien"
#define CREDITS_NAME_PROG6 "MORENO Raphael"
#define CREDITS_NAME_PROG7 "RACHIDI-AMPHOUX Paul"
#define CREDITS_NAME_PROG8 "GELOT Mathieu"
#define CREDITS_NAME_PROG9 "MAUREL Maxence"
#define CREDITS_NAME_PROG10 "MICELI Hugo"
#define CREDITS_NAME_PROG11 "TOUSSAINT Jean-Sebastien"
#define CREDITS_NAME_PROG12 "TYMRAKIEWICZ Lukas"
#define CREDITS_NAME_PROG13 "VALETTE Evan"

#define CREDITS_NAME_GRAPH1 "BERGERON DE CHARON Alice"
#define CREDITS_NAME_GRAPH2 "CACHELIN Anthony"
#define CREDITS_NAME_GRAPH3 "FOUARD Cassandra"
#define CREDITS_NAME_GRAPH4 "FOY Mathis"
#define CREDITS_NAME_GRAPH5 "GABET Andrea"
#define CREDITS_NAME_GRAPH6 "GENTY Emma"
#define CREDITS_NAME_GRAPH7 "GIORDANO Leo"
#define CREDITS_NAME_GRAPH8 "HADAVI Aria"
#define CREDITS_NAME_GRAPH9 "JEAN Coralie"
#define CREDITS_NAME_GRAPH10 "LEBRET Aidan"
#define CREDITS_NAME_GRAPH11 "LAUBRE Capucine"
#define CREDITS_NAME_GRAPH12 "MENU Renaud"
#define CREDITS_NAME_GRAPH13 "MOLINA Nicolas"
#define CREDITS_NAME_GRAPH14 "MUNICHA Killian"
#define CREDITS_NAME_GRAPH15 "PENNELLO Maelia"
#define CREDITS_NAME_GRAPH16 "ROBINET Chloe"
#define CREDITS_NAME_GRAPH17 "ROUSSEL-WOEHRLING Adrien"
#define CREDITS_NAME_GRAPH18 "SAINT-ETIENNE Noa"
#define CREDITS_NAME_GRAPH19 "WAGINAIRE Nathan"

#define CREDITS_NAME_SPECIAL1 "DELAPLASSETTE Benjamin"
#define CREDITS_NAME_SPECIAL2 "FAURE Allan"
#define CREDITS_NAME_SPECIAL3 "GIRAULT Tristan"
#define CREDITS_NAME_SPECIAL4 "KHAZNADAR Mariane"
#define CREDITS_NAME_SPECIAL5 "NAUDIN Teddy"
#define CREDITS_NAME_SPECIAL6 "JUVENAL Emma"
#define CREDITS_NAME_SPECIAL7 "SALVAGGO Leonarda"
#define CREDITS_NAME_SPECIAL8 "VIVIER Kevin"

#define CREDITS_NAME_THANKS1 "FOURNIERE Sebastien"
#define CREDITS_NAME_THANKS2 "MONAGO Jonathan"
#define CREDITS_NAME_THANKS3 "LAPRADE Allan"

#define CREDITS_PROG_Y 120.f
#define CREDITS_GRAPH_Y 80.f
#define CREDITS_SPECIAL_Y 200.f
#define CREDITS_THANKS_Y 500.f

#include "textureManager.h"

int credits_Nb;
int credits_NbTxt;
float credits_Timer;
float credits_TransitionX;
sfBool credits_IsFading;
sfBool credits_IsChangingInt;
sfText* credits_Txt;
sfRectangleShape* credits_Rect;

void credits_Transition() {
	if (credits_TransitionX > -1920.f) {
		credits_TransitionX -= getDeltaTime() * 1500.f;
		walkingFade_SetPosition(vector2f(credits_TransitionX, 0.f));
	}
}

void credits_Init() {
	if (!GetSfTexture("menuSprite")) Texture_Onload(MENU);
	credits_Nb = credits_NbTxt = 0;
	credits_Timer = 0.f;
	credits_TransitionX = 1920.f;
	credits_IsFading = sfFalse;
	credits_IsChangingInt = sfTrue;
	credits_Txt = sfText_create();
	sfText_setString(credits_Txt, "temp");
	sfText_setFont(credits_Txt, GetFont("Lilian"));
	walkingFade_Init();
	walkingFade_SetPosition(vector2f(1920, 0.f));
	credits_Rect = sfRectangleShape_create();
	sfRectangleShape_setSize(credits_Rect, vector2f(1446.f, 897.f));
	sfRectangleShape_setPosition(credits_Rect, vector2f(236.f, 96.f));
	sfRectangleShape_setTexture(credits_Rect, GetSfTexture("CREDITS"), sfTrue);
}

void credits_Update(Window* _window) {
	walkingFade_Update();
	credits_Timer += getDeltaTime();
	if ((Gamepad_isButtonPressed(0, A, _window->renderWindow) || customKeyboardIsKeyPressed(sfKeyEnter, _window->renderWindow)) && credits_Timer > 0.2f && credits_TransitionX >= -1920.f) {
		credits_IsFading = sfTrue;
		credits_Timer = 0.f;
	}
	if (credits_IsFading) {
		credits_Transition();
		if (walkingFade_GetPosition().x <= 0.f) {
			if (credits_IsChangingInt) {
				credits_Nb++;
				credits_IsChangingInt = sfFalse;
			}
			if (walkingFade_GetPosition().x <= -1920.f) {
				credits_TransitionX = 1920.f;
				credits_IsChangingInt = sfTrue;
				credits_IsFading = sfFalse;
			}
		}
	}
	if (credits_Nb > 3) {
		changeState(_window, MENU);
		fade_SetOpacity(255);
	}
}

void credits_Display(Window* _window) {
	sfRenderTexture_drawRectangleShape(_window->rdrt, credits_Rect, NULL);
	if (credits_Nb == 0) {
		sfText_setString(credits_Txt, "Programmers :");
		sfText_setPosition(credits_Txt, vector2f(960.f, 20.f));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);

		sfText_setString(credits_Txt, CREDITS_NAME_PROG1);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_PROG_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_PROG2);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_PROG_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_PROG3);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_PROG_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_PROG4);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_PROG_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_PROG5);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_PROG_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_PROG6);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_PROG_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_PROG7);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 150.f - 700.f + credits_NbTxt * CREDITS_PROG_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_PROG8);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 150.f - 700.f + credits_NbTxt * CREDITS_PROG_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_PROG9);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 150.f - 700.f + credits_NbTxt * CREDITS_PROG_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_PROG10);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 150.f - 700.f + credits_NbTxt * CREDITS_PROG_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_PROG11);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 150.f - 700.f + credits_NbTxt * CREDITS_PROG_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_PROG12);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 150.f - 700.f + credits_NbTxt * CREDITS_PROG_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_PROG13);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 150.f - 700.f + credits_NbTxt * CREDITS_PROG_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;
	}
	else if (credits_Nb == 1) {
		sfText_setString(credits_Txt, "Game artists :");
		sfText_setPosition(credits_Txt, vector2f(960.f, 20.f));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH1);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH2);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH3);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH4);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH5);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH6);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH7);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH8);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH9);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH10);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 150.f + (credits_NbTxt - 9) * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH11);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 150.f + (credits_NbTxt - 9) * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH12);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 150.f + (credits_NbTxt - 9) * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH13);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 150.f + (credits_NbTxt - 9) * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH14);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 150.f + (credits_NbTxt - 9) * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH15);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 150.f + (credits_NbTxt - 9) * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH16);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 150.f + (credits_NbTxt - 9) * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH17);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 150.f + (credits_NbTxt - 9) * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH18);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 150.f + (credits_NbTxt - 9) * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_GRAPH19);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 150.f + (credits_NbTxt - 9) * CREDITS_GRAPH_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;
	}
	else if (credits_Nb == 2) {
		sfText_setString(credits_Txt, "Special thanks :");
		sfText_setPosition(credits_Txt, vector2f(960.f, 50.f));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);

		sfText_setString(credits_Txt, CREDITS_NAME_SPECIAL1);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_SPECIAL_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_SPECIAL2);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_SPECIAL_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_SPECIAL3);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_SPECIAL_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_SPECIAL4);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_SPECIAL_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_SPECIAL5);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 200.f + (credits_NbTxt - 4) * CREDITS_SPECIAL_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_SPECIAL6);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 200.f + (credits_NbTxt - 4) * CREDITS_SPECIAL_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_SPECIAL7);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 200.f + (credits_NbTxt - 4) * CREDITS_SPECIAL_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_SPECIAL8);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 200.f + (credits_NbTxt - 4) * CREDITS_SPECIAL_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;
	}
	else if (credits_Nb == 3) {
		sfText_setString(credits_Txt, "Special thanks :");
		sfText_setPosition(credits_Txt, vector2f(960.f, 50.f));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);

		sfText_setString(credits_Txt, CREDITS_NAME_THANKS1);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_THANKS_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_THANKS2);
		sfText_setPosition(credits_Txt, vector2f(540.f, 200.f + credits_NbTxt * CREDITS_THANKS_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;

		sfText_setString(credits_Txt, CREDITS_NAME_THANKS3);
		sfText_setPosition(credits_Txt, vector2f(1300.f, 300.f + (credits_NbTxt / 4.5f) * CREDITS_THANKS_Y));
		centralText(credits_Txt);
		sfRenderTexture_drawText(_window->rdrt, credits_Txt, NULL);
		credits_NbTxt++;
	}
	credits_NbTxt = 0;
	walkingFade_Draw(_window);
}

void credits_Deinit() {
	if (!credits_Txt) return;
	sfText_destroy(credits_Txt);
}