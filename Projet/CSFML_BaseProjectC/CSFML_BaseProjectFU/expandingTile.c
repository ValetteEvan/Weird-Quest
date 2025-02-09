#include "expandingTile.h"
#include "textureManager.h"
#include "player.h"
#include "map.h"
#include "math.h"

#define EXPT_SPD 250.f
#define EXPT_MAX_LEN 3

sfSprite* expt_RenderSprite;

void expt_Init() {
	static sfBool onePass = sfTrue;
	if (onePass) {
		onePass = sfFalse;
		expt_List = STD_LIST_CREATE(exptData, 0);
		expt_RenderSprite = sfSprite_create();
	}
}

exptData* expt_Create(sfVector2f _pos, Direction _dir) {
	if (!expt_List) return NULL;

	exptData tmp;
	tmp.pos = _pos;
	tmp.posSnap = expt_SnapPos(tmp.pos);
	tmp.pos = vector2f(tmp.posSnap.x * TILE_SIZE, tmp.posSnap.y * TILE_SIZE);
	tmp.posEndpoint = tmp.pos;
	tmp.spd = NULLVECTF;
	tmp.dir = _dir;
	expt_List->push_back(&expt_List, &tmp);
	return STD_LIST_GETDATA(expt_List, exptData, expt_List->size(expt_List) - 1);
}

void expt_Update() {
	for (int i = 0; i < expt_List->size(expt_List); i++) {
		exptData* tmp = EXPT_GETDATA;

		tmp->posSnap = expt_SnapPos(tmp->pos);

		/// If the currently considered platform is moving...
		if (getSqrMagnitudef(tmp->spd) > 10.f) {
			/// Endpoint's future position
			sfVector2f posNext = addVectorsrf(tmp->posEndpoint, multiplyVectorsrf(tmp->spd, getDeltaTime()));

			/// Checks if the platform has reached one of its built-in size limits
			if (expt_CheckWedgeStart(posNext, tmp) || expt_CheckWedgeEnd(posNext, tmp)) {
				tmp->spd = NULLVECTF;
				break;
			}
			
			sfVector2i tileNext = expt_SnapPos(posNext);
			if (fabs(tmp->spd.x) > fabs(tmp->spd.y) && tmp->spd.x > 0.f) {
				removeCollision(vector2i(posNext.x / 64, posNext.y / 64));
				tileNext.x++;

			}
			if (fabs(tmp->spd.x) < fabs(tmp->spd.y) && tmp->spd.y > 0.f) {
				removeCollision(vector2i(posNext.x / 64, posNext.y / 64));
				tileNext.y++;
			}
			if (fabs(tmp->spd.x) < fabs(tmp->spd.y) && tmp->spd.y < 0.f) {
				removeCollision(vector2i(posNext.x / 64, posNext.y / 64));
			}
			/// Check for collisions here and set speed to zero if colliding with either the void or another object

			/// If all is well, move
			tmp->posEndpoint = posNext;
		}
		/// Otherwise...
		else {
			/// ...snaps the endpoint to the grid
			sfVector2i posEndSnap = expt_SnapPos(vector2f(tmp->posEndpoint.x + TILE_SIZE * .5f, tmp->posEndpoint.y + TILE_SIZE * .5f));
			tmp->posEndpoint = vector2f(posEndSnap.x * TILE_SIZE, posEndSnap.y * TILE_SIZE);
			removeCollision(vector2i(tmp->posEndpoint.x / 64, tmp->posEndpoint.y / 64));
		}
	}
}

void expt_Display(Window* _window) {
	for (int i = 0; i < expt_List->size(expt_List); i++) {
		exptData* tmp = EXPT_GETDATA;

		sfSprite_setTexture(expt_RenderSprite, GetSfTexture("assets_donjon_global"), sfTrue);
		sfBool isMoving = getSqrMagnitudef(tmp->spd) > 10.f;

		/// Renders a set of directionless platforms behind an expanded one
		sfVector2f posRender = tmp->pos;
		int tiles = (int)(getMagnitudef(substractVectorsrf(tmp->pos, tmp->posEndpoint)) / TILE_SIZE) + 1;
		for (int k = 0; k < tiles; k++) {
			sfSprite_setPosition(expt_RenderSprite, posRender);
			sfSprite_setTextureRect(expt_RenderSprite, IntRect(128 + isMoving * 64, 128, 64, 64));
			sfRenderTexture_drawSprite(_window->rdrt, expt_RenderSprite, NULL);
			switch (tmp->dir) {
				case DIR_UP:	posRender.y -= TILE_SIZE; break;
				case DIR_DOWN:	posRender.y += TILE_SIZE; break;
				case DIR_LEFT:	posRender.x -= TILE_SIZE; break;
				case DIR_RIGHT:	posRender.x += TILE_SIZE; break;
				default: break;
			}
		}

		switch (tmp->dir) {
			case DIR_UP:
				sfSprite_setTextureRect(expt_RenderSprite, IntRect(256 + isMoving * 64, 128, 64, 64));
				break;
			case DIR_RIGHT:
				sfSprite_setTextureRect(expt_RenderSprite, IntRect(128 + isMoving * 64, 192, 64, 64));
				break;
			case DIR_DOWN:
				sfSprite_setTextureRect(expt_RenderSprite, IntRect(256 + isMoving * 64, 192, 64, 64));
				break;
			case DIR_LEFT:
				sfSprite_setTextureRect(expt_RenderSprite, IntRect(128 + isMoving * 64, 256, 64, 64));
				break;
			default:
				sfSprite_setTextureRect(expt_RenderSprite, IntRect(0, 0, 64, 64));
				break;
		}

		sfSprite_setPosition(expt_RenderSprite, tmp->posEndpoint);
		sfRenderTexture_drawSprite(_window->rdrt, expt_RenderSprite, NULL);
	}
}

void expt_Deinit() {

}


sfVector2i expt_SnapPos(sfVector2f _pos) {
	return vector2i((int)_pos.x / (int)TILE_SIZE, (int)_pos.y / (int)TILE_SIZE);
}

exptData* expt_GetClosest(sfVector2f _pos) {
	if (!expt_List) return NULL;

	float lastDistance = LLONG_MAX;
	int idExpt = 0;

	for (int i = 0; i < expt_List->size(expt_List); i++) {
		float dist = getSqrMagnitudeBetween2Vectorsf(_pos, EXPT_GETDATA->posEndpoint);
		if (dist < lastDistance) {
			lastDistance = dist;
			idExpt = i;
		}
	}
	return STD_LIST_GETDATA(expt_List, exptData, idExpt);
}

sfFloatRect expt_GetEndpointBounds(exptData* _expt) {
	if (_expt == NULL) return (sfFloatRect) { 0 };
	return FlRect(_expt->posEndpoint.x, _expt->posEndpoint.y, TILE_SIZE, TILE_SIZE);
}

sfFloatRect expt_GetSurfaceBounds(exptData* _expt) {
	if (_expt == NULL) return (sfFloatRect) { 0 };

	float lowestX = min(_expt->pos.x, _expt->posEndpoint.x);
	float lowestY = min(_expt->pos.y, _expt->posEndpoint.y);
	float distX = fabs(_expt->pos.x - _expt->posEndpoint.x) + TILE_SIZE;
	float distY = fabs(_expt->pos.y - _expt->posEndpoint.y) + TILE_SIZE;
	return FlRect(lowestX, lowestY, distX, distY);
}

sfBool expt_CheckWedgeStart(sfVector2f _pos, exptData* _expt) {
	sfBool dirCheck = sfFalse;
	switch (_expt->dir) {
		case DIR_UP:	if (_expt->spd.y > 0.f) dirCheck = sfTrue; break;
		case DIR_DOWN:	if (_expt->spd.y < 0.f) dirCheck = sfTrue; break;
		case DIR_LEFT:	if (_expt->spd.x > 0.f) dirCheck = sfTrue; break;
		case DIR_RIGHT:	if (_expt->spd.x < 0.f) dirCheck = sfTrue; break;
	}

	return dirCheck && getSqrMagnitudeBetween2Vectorsf(_expt->pos, _pos) < 10.f;
}

sfBool expt_CheckWedgeEnd(sfVector2f _pos, exptData* _expt) {
	sfBool dirCheck = sfFalse;
	switch (_expt->dir) {
		case DIR_UP:	if (_expt->spd.y < 0.f) dirCheck = sfTrue; break;
		case DIR_DOWN:	if (_expt->spd.y > 0.f) dirCheck = sfTrue; break;
		case DIR_LEFT:	if (_expt->spd.x < 0.f) dirCheck = sfTrue; break;
		case DIR_RIGHT:	if (_expt->spd.x > 0.f) dirCheck = sfTrue; break;
	}

	return dirCheck && getSqrMagnitudeBetween2Vectorsf(_expt->pos, _pos) > (TILE_SIZE * TILE_SIZE * EXPT_MAX_LEN * EXPT_MAX_LEN);
}


void expt_Attract(exptData* _expt) {
	sfVector2f vForce = substractVectorsrf(_expt->posEndpoint, plr_GetFeetPos());
	if (getSqrMagnitudef(vForce) > TILE_SIZE * TILE_SIZE * .25f) {
		switch (_expt->dir) {
		case DIR_UP:	if (fabs(vForce.x) < fabs(vForce.y) && vForce.y > 0.f) _expt->spd = vector2f(0.f, -EXPT_SPD); break;
		case DIR_DOWN:	if (fabs(vForce.x) < fabs(vForce.y) && vForce.y < 0.f) _expt->spd = vector2f(0.f, EXPT_SPD); break;
		case DIR_LEFT:	if (fabs(vForce.x) > fabs(vForce.y) && vForce.x > 0.f) _expt->spd = vector2f(-EXPT_SPD, 0.f); break;
		case DIR_RIGHT:	if (fabs(vForce.x) > fabs(vForce.y) && vForce.x < 0.f) _expt->spd = vector2f(EXPT_SPD, 0.f); break;
		}
	}
}

void expt_Repulse(exptData* _expt) {
	/// Uncomment this to disable repelling bridges
	//	return;

	sfVector2f vForce = substractVectorsrf(_expt->posEndpoint, plr_GetFeetPos());
	if (getSqrMagnitudef(vForce) > TILE_SIZE * TILE_SIZE * .25f) {
		switch (_expt->dir) {
		case DIR_UP:	if (fabs(vForce.x) < fabs(vForce.y) && vForce.y > 0.f) _expt->spd = vector2f(0.f, EXPT_SPD); break;
		case DIR_DOWN:	if (fabs(vForce.x) < fabs(vForce.y) && vForce.y < 0.f) _expt->spd = vector2f(0.f, -EXPT_SPD); break;
		case DIR_LEFT:	if (fabs(vForce.x) > fabs(vForce.y) && vForce.x > 0.f) _expt->spd = vector2f(EXPT_SPD, 0.f); break;
		case DIR_RIGHT:	if (fabs(vForce.x) > fabs(vForce.y) && vForce.x < 0.f) _expt->spd = vector2f(-EXPT_SPD, 0.f); break;
		}
	}
}

void expt_Stasis(exptData* _expt) { return; }
void expt_Destroy(exptData* _expt) { return; }

ExptToSave* getExptListToSave()
{
	if (expt_List == NULL)
		return NULL;

	int len = expt_List->size(expt_List);
	ExptToSave* tmp = calloc(len, sizeof(ExptToSave));

	if (tmp == NULL)
		return NULL;

	for (int i = 0; i < len; i++)
	{
		tmp[i].pos = STD_LIST_GETDATA(expt_List, exptData, i)->pos;
		tmp[i].dir = STD_LIST_GETDATA(expt_List, exptData, i)->dir;
	}
	return tmp;
}

void expt_clearList()
{
	if (!expt_List) return;
	expt_List->clear(&expt_List);
}