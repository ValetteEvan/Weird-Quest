#include "collisionManager.h"
#include "math.h"
#include "map.h"
#include "box.h"
#include "player.h"

sfBool cll_BoxBox(sfVector2f _playerNextPos, BoxData* _box)
{
	if (getSqrMagnitudeBetween2Vectorsf(_playerNextPos, _box->pos) > (BOX_X_SIZE + PLR_X_SIZE) * ((BOX_Y_SIZE) + PLR_Y_SIZE) * 1.5f) return sfFalse;

	sfFloatRect cld_boxRect = FlRect(_box->pos.x, _box->pos.y, BOX_X_SIZE, BOX_Y_SIZE/2.f);
	sfFloatRect cld_playerRect = FlRect(_playerNextPos.x, _playerNextPos.y, PLR_X_SIZE, PLR_Y_SIZE);
	return sfFloatRect_intersects(&cld_boxRect, &cld_playerRect, NULL);
}

sfBool cll_PolygonRect(sfVector2f* _boxShape, sfVector2f* _polygoneShape, int _vertCount) {
	for (int i = 0; i < 4; i++) {
		if (cll_PolygonPoint(_boxShape[i], _polygoneShape, _vertCount)) return sfTrue;
	}
	for (int i = 0; i < _vertCount; i++) {
		int cur = i;
		int next = (i + 1) % _vertCount;
		sfVector2f v1 = _polygoneShape[cur];
		sfVector2f v2 = _polygoneShape[next];
		if (cll_RectLine(v1, v2, _boxShape)) return sfTrue;
	}
	return sfFalse;
}

sfBool cll_PolygonPoint(sfVector2f _p, sfVector2f* _poly, int _vertCount) {
	sfBool coll = sfFalse;
	for (int i = 0; i < _vertCount; i++) {
		int cur = i;
		int next = (i + 1) % _vertCount;
		sfVector2f v1 = _poly[cur];
		sfVector2f v2 = _poly[next];
		if (((v1.y >= _p.y && v2.y < _p.y) || (v1.y < _p.y && v2.y >= _p.y)) && (_p.x < ((v2.x - v1.x) * (_p.y - v1.y) / (v2.y - v1.y) + v1.x))) coll = !coll;
	}
	return coll;
}

sfBool cll_RectLine(sfVector2f _l1, sfVector2f _l2, sfVector2f* _boxShape) {
	for (int i = 0; i < 4; i++) {
		int cur = i;
		int next = (i + 1) % 4;
		sfVector2f v1 = _boxShape[cur];
		sfVector2f v2 = _boxShape[next];
		if (cll_LineLine(_l1, _l2, v1, v2)) return sfTrue;
	}
	return sfFalse;
}

sfBool cll_LineLine(sfVector2f _l1_1, sfVector2f _l1_2, sfVector2f _l2_1, sfVector2f _l2_2) {
	float e = (_l2_2.y - _l2_1.y) * (_l1_2.x - _l1_1.x) - (_l2_2.x - _l2_1.x) * (_l1_2.y - _l1_1.y);
	float a = ((_l2_2.x - _l2_1.x) * (_l1_1.y - _l2_1.y) - (_l2_2.y - _l2_1.y) * (_l1_1.x - _l2_1.x)) / e;
	float b = ((_l1_2.x - _l1_1.x) * (_l1_1.y - _l2_1.y) - (_l1_2.y - _l1_1.y) * (_l1_1.x - _l2_1.x)) / e;
	return a >= 0.f && a <= 1.f && b >= 0.f && b <= 1.f;
}

sfBool cll_RectMap(sfFloatRect _aabb, sfVector2f _spd)
{
	int nextTileX1, nextTileX2, nextTileY1, nextTileY2;
	_spd = multiplyVectorsrf(_spd, 1.5f);

	// Calculate the indices of tiles adjacent to the player's bounding box
	nextTileX1 = (_aabb.left + _spd.x) / TILE_SIZE;
	nextTileX2 = (_aabb.left + _aabb.width + _spd.x) / TILE_SIZE;
	nextTileY1 = (_aabb.top + _spd.y) / TILE_SIZE;
	nextTileY2 = (_aabb.top + _aabb.height + _spd.y) / TILE_SIZE;

	if (nextTileY1 >= map.mapSize.y)
		nextTileY1 = map.mapSize.y - 1;
	else if (nextTileY1 < 0)
		nextTileY1 = 0;
	if (nextTileY2 >= map.mapSize.y)
		nextTileY2 = map.mapSize.y - 1;
	else if (nextTileY2 < 0)
		nextTileY2 = 0;


	if (nextTileX1 >= map.mapSize.x)
		nextTileX1 = map.mapSize.x - 1;
	else if (nextTileX1 < 0)
		nextTileX1 = 0;
	if (nextTileX2 >= map.mapSize.x)
		nextTileX2 = map.mapSize.x - 1;
	else if (nextTileX2 < 0)
		nextTileX2 = 0;

	sfVector2f pPoints[4] = { 0 };
	pPoints[0] = vector2f(_aabb.left + _spd.x, _aabb.top + _spd.y);
	pPoints[1] = vector2f(pPoints[0].x + _aabb.width, pPoints[0].y);
	pPoints[2] = vector2f(pPoints[0].x + _aabb.width, pPoints[0].y + _aabb.height);
	pPoints[3] = vector2f(pPoints[0].x, pPoints[0].y + _aabb.height);

	sfVector2f  tPoints[3];
	sfVector2f  cPoints[4];
	sfVector2f  hPoints[4];
	sfVector2f  aPoints[6];
	sfVector2f sdPoints[6];
	sfVector2f stPoints[3];

	// Check for collisions based on the player's movement direction
	sfVector2i cldPoints[2] = { 0 }; // Temp variable for storing points that will be used for collision computation
	if (_spd.x < 0) // Moving left
	{
		cldPoints[0] = vector2i(nextTileX1, nextTileY1);
		cldPoints[1] = vector2i(nextTileX1, nextTileY2);
	}
	else if (_spd.x > 0) // Moving right
	{
		cldPoints[0] = vector2i(nextTileX2, nextTileY1);
		cldPoints[1] = vector2i(nextTileX2, nextTileY2);
	}
	if (_spd.y < 0) // Moving up
	{
		cldPoints[0] = vector2i(nextTileX1, nextTileY1);
		cldPoints[1] = vector2i(nextTileX2, nextTileY1);
	}
	else if (_spd.y > 0) // Moving down
	{
		cldPoints[0] = vector2i(nextTileX1, nextTileY2);
		cldPoints[1] = vector2i(nextTileX2, nextTileY2);
	}

	// Computing collisions for the points specified in the previous code block
	CollisionType ctype;
	for (int i = 0; i < 2; i++) {
		ctype = map_HasTileCollisions(vector2i(cldPoints[i].x, cldPoints[i].y));
		if (ctype & CLD_BOX) return sfTrue;
		else if (ctype & CLD_HALF) {
			cld_Half(hPoints, cldPoints[i].x, cldPoints[i].y, ctype);
			if (cll_PolygonRect(pPoints, hPoints, 4)) return sfTrue;
		}
		else if (ctype & CLD_CORN) {
			cld_CornerSqr(cPoints, cldPoints[i].x, cldPoints[i].y, ctype);
			if (cll_PolygonRect(pPoints, cPoints, 4)) return sfTrue;
		}
		else if (ctype & CLD_DIAG) {
			cld_Triangle(tPoints, cldPoints[i].x, cldPoints[i].y, ctype);
			if (cll_PolygonRect(pPoints, tPoints, 3))return sfTrue;
		}
		else if (ctype & CLD_ANGLE) {
			cld_LShape(aPoints, cldPoints[i].x, cldPoints[i].y, ctype);
			if (cll_PolygonRect(pPoints, aPoints, 6)) return sfTrue;
		}
		else if (ctype & CLD_S_DIAG) {
			cld_Diagonal(sdPoints, cldPoints[i].x, cldPoints[i].y, ctype);
			if (cll_PolygonRect(pPoints, sdPoints, 6)) return sfTrue;
		}
		else if (ctype & CLD_S_TRI) {
			cld_CornerTri(stPoints, cldPoints[i].x, cldPoints[i].y, ctype);
			if (cll_PolygonRect(pPoints, stPoints, 3)) return sfTrue;
		}
	}

	return sfFalse;
}