#include "bigRock.h"
#include "textureManager.h"

sfRectangleShape* bigRock_Rect;
sfFloatRect bigRock_Collision;
sfVector2f bigRock_TempPosition;
sfBool bigRock_CanMove;
sfBool bigRock_IsMoving[3][3];
sfBool bigRock_CanPass;
sfBool bigRock_OnePass;
sfTexture* bigRock_Tex;
sfTexture* bigRock_DungeonTex;
int bigRock_FrameX;
int bigRock_Dir;
float bigRock_Timer;
float bigRock_DebugTimer;

typedef enum {
	BIGROCK_LEFT,
	BIGROCK_RIGHT,
	BIGROCK_BOTTOM,
	BIGROCK_TOP
}bigRock_Direction;
bigRock_Direction br_Dir;
sfVector2f tilePosition[3][3];

char bigRock_Map[3][3] = {
	1, 4, 5,
	2, 0, 2,
	3, 4, 6,
};
char bigRock_TempTile;

void bigRock_Init() {
	return;
	bigRock_Rect = sfRectangleShape_create();
	sfRectangleShape_setSize(bigRock_Rect, vector2f(192.f, 192.f));
	bigRock_Tex = GetSfTexture("donjonGlobal");
	bigRock_DungeonTex = GetSfTexture("assets_donjon_global");
	bigRock_CanMove;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			bigRock_IsMoving[i][j] = sfFalse;
			tilePosition[i][j] = vector2f(64.f * 10.f + i * 64.f, 64.f * 10.f + j * 64.f);
		}
	}
	bigRock_FrameX, bigRock_Dir = 0;
	bigRock_Timer, bigRock_DebugTimer = 0.f;
	bigRock_TempTile = bigRock_Map[1][2];
	bigRock_CanPass, bigRock_OnePass = sfTrue;
	bigRock_TempPosition = vector2f(0.f, 0.f);
}

sfBool bigRock_CheckMap(int i, int j) {
	if (bigRock_Map[i][j] == 1 || bigRock_Map[i][j] == 2 || bigRock_Map[i][j] == 3 || bigRock_Map[i][j] == 4 || bigRock_Map[i][j] == 5 || bigRock_Map[i][j] == 6) return sfTrue;
	else return sfFalse;
}

void bigRock_Movement(int i, int j) {
	if (bigRock_Map[i][j] == 7 && bigRock_CanPass) {
		bigRock_CanPass = sfFalse;
		if (br_Dir == 0) {
			if (bigRock_CheckMap(i, j - 1) && j != 0) {
				bigRock_IsMoving[i][j] = sfTrue;
			}
		}
		else if (br_Dir == 1) {
			if (bigRock_CheckMap(i, j + 1) && j != 2) {
				bigRock_IsMoving[i][j] = sfTrue;
			}
		}
		else if (br_Dir == 2) {
			if (bigRock_CheckMap(i - 1, j) && i != 0) {
				bigRock_IsMoving[i][j] = sfTrue;
			}
		}
		else if (br_Dir == 3) {
			if (bigRock_CheckMap(i + 1, j) && i != 2) {
				bigRock_IsMoving[i][j] = sfTrue;
			}
		}
		else return;
	}
}

void bigRock_Update(Window* _window) {
	return;
	bigRock_DebugTimer += getDeltaTime();
	if (customKeyboardIsKeyPressed(sfKeyY, _window->renderWindow)) {
		bigRock_Map[1][2] = 7;
	}
	if (customKeyboardIsKeyPressed(sfKeyU, _window->renderWindow) && bigRock_DebugTimer > 0.2f) {
		bigRock_Dir++;
		if (bigRock_Dir > 3) bigRock_Dir = 0;
		bigRock_DebugTimer = 0.f;
	}
	br_Dir = bigRock_Dir;
	if (customKeyboardIsKeyPressed(sfKeyI, _window->renderWindow) && bigRock_DebugTimer > 0.2f) {
		bigRock_CanPass = sfTrue;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				bigRock_Movement(j, i);
			}
		}
		bigRock_DebugTimer = 0.f;
	}
}

sfBool bigRock_Check(int i, int j) {
	if (bigRock_Map[i + 1][j] != 7 && bigRock_Map[i - 1][j] != 7 && bigRock_Map[i][j + 1] != 7 && bigRock_Map[i][j - 1] != 7 && bigRock_Map[i + 1][j + 1] != 7 && bigRock_Map[i + 1][j - 1] != 7 && bigRock_Map[i - 1][j + 1] != 7 && bigRock_Map[i - 1][j - 1] != 7) return sfTrue;
	else return sfFalse;
}

void bigRock_Display(Window* _window) {
	return;
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			sfRectangleShape_setPosition(bigRock_Rect, vector2f(64.f * 10.f + i * 64.f + 64.f, 64.f * 10.f + j * 64.f + 64.f));
			sfRectangleShape_setFillColor(bigRock_Rect, sfWhite);
			if (bigRock_Map[i][j] == 1) {
				sfRectangleShape_setSize(bigRock_Rect, vector2f(64.f, 64.f));
				sfRectangleShape_setTexture(bigRock_Rect, bigRock_DungeonTex, sfTrue);
				sfRectangleShape_setTextureRect(bigRock_Rect, (sfIntRect) { 0, 512, 64, 64 });
				sfRenderTexture_drawRectangleShape(_window->rdrt, bigRock_Rect, NULL);
			}
			else if (bigRock_Map[i][j] == 2) {
				sfRectangleShape_setSize(bigRock_Rect, vector2f(64.f, 64.f));
				sfRectangleShape_setTexture(bigRock_Rect, bigRock_DungeonTex, sfTrue);
				sfRectangleShape_setTextureRect(bigRock_Rect, (sfIntRect) { 64, 512, 64, 64 });
				sfRenderTexture_drawRectangleShape(_window->rdrt, bigRock_Rect, NULL);
			}
			else if (bigRock_Map[i][j] == 3) {
				sfRectangleShape_setSize(bigRock_Rect, vector2f(64.f, 64.f));
				sfRectangleShape_setTexture(bigRock_Rect, bigRock_DungeonTex, sfTrue);
				sfRectangleShape_setTextureRect(bigRock_Rect, (sfIntRect) { 128, 512, 64, 64 });
				sfRenderTexture_drawRectangleShape(_window->rdrt, bigRock_Rect, NULL);
			}
			else if (bigRock_Map[i][j] == 4) {
				sfRectangleShape_setSize(bigRock_Rect, vector2f(64.f, 64.f));
				sfRectangleShape_setTexture(bigRock_Rect, bigRock_DungeonTex, sfTrue);
				sfRectangleShape_setTextureRect(bigRock_Rect, (sfIntRect) { 0, 576, 64, 64 });
				sfRenderTexture_drawRectangleShape(_window->rdrt, bigRock_Rect, NULL);
			}
			else if (bigRock_Map[i][j] == 5) {
				sfRectangleShape_setSize(bigRock_Rect, vector2f(64.f, 64.f));
				sfRectangleShape_setTexture(bigRock_Rect, bigRock_DungeonTex, sfTrue);
				sfRectangleShape_setTextureRect(bigRock_Rect, (sfIntRect) { 0, 640, 64, 64 });
				sfRenderTexture_drawRectangleShape(_window->rdrt, bigRock_Rect, NULL);
			}
			else if (bigRock_Map[i][j] == 6) {
				sfRectangleShape_setSize(bigRock_Rect, vector2f(64.f, 64.f));
				sfRectangleShape_setTexture(bigRock_Rect, bigRock_DungeonTex, sfTrue);
				sfRectangleShape_setTextureRect(bigRock_Rect, (sfIntRect) { 128, 640, 64, 64 });
				sfRenderTexture_drawRectangleShape(_window->rdrt, bigRock_Rect, NULL);
			}
			else {
				sfRectangleShape_setTexture(bigRock_Rect, NULL, sfTrue);
				sfRectangleShape_setFillColor(bigRock_Rect, sfTransparent);
				sfRenderTexture_drawRectangleShape(_window->rdrt, bigRock_Rect, NULL);
			}
		}
	}
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			if (bigRock_Map[i][j] == 7) {
				if (!bigRock_IsMoving[i][j]) {
					tilePosition[i][j] = vector2f(64.f * 10.f + i * 64.f, 64.f * 10.f + j * 64.f);
					sfRectangleShape_setPosition(bigRock_Rect, tilePosition[i][j]);
				}
				else {
					if (br_Dir == 2) {
						if (bigRock_OnePass) {
							bigRock_TempPosition = vector2f(64.f * 10.f + i * 64.f, 64.f * 10.f + j * 64.f);
							bigRock_OnePass = sfFalse;
						}
						tilePosition[i][j].y -= getDeltaTime() * 35.6125361198f;
						sfRectangleShape_setPosition(bigRock_Rect, tilePosition[i][j]);
						if (bigRock_TempPosition.y <= 64.f * 10.f + (j - 1) * 64.f) {
							bigRock_TempTile = bigRock_Map[i][j - 1];
							bigRock_Map[i][j] = bigRock_TempTile;
							bigRock_Map[i][j - 1] = 7;
							bigRock_IsMoving[i][j] = sfFalse;
							bigRock_OnePass = sfTrue;
						}
					}
					else if (br_Dir == 3) {
						if (bigRock_OnePass) {
							bigRock_TempPosition = vector2f(64.f * 10.f + i * 64.f, 64.f * 10.f + j * 64.f);
							bigRock_OnePass = sfFalse;
						}
						tilePosition[i][j].y += getDeltaTime() * 35.6125361198f;
						sfRectangleShape_setPosition(bigRock_Rect, tilePosition[i][j]);
						if (bigRock_TempPosition.y >= 64.f * 10.f + (j + 1) * 64.f) {
							bigRock_TempTile = bigRock_Map[i][j];
							bigRock_Map[i][j] = bigRock_TempTile;
							bigRock_Map[i][j + 1] = 7;
							bigRock_IsMoving[i][j] = sfFalse;
							bigRock_OnePass = sfTrue;
						}
					}
					else if (br_Dir == 0) {
						if (bigRock_OnePass) {
							bigRock_TempPosition = vector2f(64.f * 10.f + i * 64.f, 64.f * 10.f + j * 64.f);
							bigRock_OnePass = sfFalse;
						}
						tilePosition[i][j].x += getDeltaTime() * 35.6125361198f;
						sfRectangleShape_setPosition(bigRock_Rect, tilePosition[i][j]);
						if (bigRock_TempPosition.x <= 64.f * 10.f + (i - 1) * 64.f) {
							bigRock_TempTile = bigRock_Map[i - 1][j];
							bigRock_Map[i][j] = bigRock_TempTile;
							bigRock_Map[i - 1][j] = 7;
							bigRock_IsMoving[i][j] = sfFalse;
							bigRock_OnePass = sfTrue;
						}
					}
					else if (br_Dir == 1) {
						if (bigRock_OnePass) {
							bigRock_TempPosition = vector2f(64.f * 10.f + i * 64.f, 64.f * 10.f + j * 64.f);
							bigRock_OnePass = sfFalse;
						}
						tilePosition[i][j].x -= getDeltaTime() * 35.6125361198f;
						sfRectangleShape_setPosition(bigRock_Rect, tilePosition[i][j]);
						if (bigRock_TempPosition.x >= 64.f * 10.f + (i + 1) * 64.f) {
							bigRock_TempTile = bigRock_Map[i + 1][j];
							bigRock_Map[i][j] = bigRock_TempTile;
							bigRock_Map[i + 1][j] = 7;
							bigRock_IsMoving[i][j] = sfFalse;
							bigRock_OnePass = sfTrue;
						}
					}
				}
				sfRectangleShape_setSize(bigRock_Rect, vector2f(192.f, 192.f));
				bigRock_Collision = (sfFloatRect){ sfRectangleShape_getGlobalBounds(bigRock_Rect).top + 64.f, sfRectangleShape_getGlobalBounds(bigRock_Rect).left + 64.f, 64.f, 64.f };
				sfRectangleShape_setTexture(bigRock_Rect, bigRock_Tex, sfTrue);
				if (bigRock_IsMoving[i][j]) sfRectangleShape_setTextureRect(bigRock_Rect, animatorAnim("donjonGlobal", &bigRock_FrameX, &bigRock_Timer, 0));
				else sfRectangleShape_setTextureRect(bigRock_Rect, (sfIntRect) { 0, 320, 192, 192 });
				sfRenderTexture_drawRectangleShape(_window->rdrt, bigRock_Rect, NULL);
			}
		}
	}
}

void bigRock_Deinit() {
	if (!bigRock_Rect) return;
	sfRectangleShape_destroy(bigRock_Rect);
}