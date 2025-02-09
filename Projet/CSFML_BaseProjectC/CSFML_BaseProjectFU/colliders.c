#include "colliders.h"

sfVector2f cld_Triangle(sfVector2f* triangle, int tileX, int tileY, CollisionType _type) {
	switch (_type)
	{
	case CLD_DIAG_TOP_LEFT:
		triangle[0] = vector2f(tileX * TILE_SIZE, tileY * TILE_SIZE);
		triangle[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, tileY * TILE_SIZE);
		triangle[2] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		break;
	case CLD_DIAG_TOP_RIGHT:
		triangle[0] = vector2f(tileX * TILE_SIZE, tileY * TILE_SIZE);
		triangle[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, tileY * TILE_SIZE);
		triangle[2] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		break;
	case CLD_DIAG_BOTTOM_RIGHT:
		triangle[0] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, tileY * TILE_SIZE);
		triangle[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		triangle[2] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		break;
	case CLD_DIAG_BOTTOM_LEFT:
		triangle[0] = vector2f(tileX * TILE_SIZE, tileY * TILE_SIZE);
		triangle[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		triangle[2] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		break;
	default:
		break;
	}

	return *triangle;
}

sfVector2f cld_CornerSqr(sfVector2f* corner, int tileX, int tileY, CollisionType _type) {
	switch (_type)
	{
	case CLD_CORN_TOP_LEFT:
		corner[0] = vector2f(tileX * TILE_SIZE, tileY * TILE_SIZE);
		corner[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, tileY * TILE_SIZE);
		corner[2] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		corner[3] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		break;
	case CLD_CORN_TOP_RIGHT:
		corner[0] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, tileY * TILE_SIZE);
		corner[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, tileY * TILE_SIZE);
		corner[2] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		corner[3] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		break;
	case CLD_CORN_BOTTOM_RIGHT:
		corner[0] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		corner[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		corner[2] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		corner[3] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, (tileY * TILE_SIZE) + TILE_SIZE);
		break;
	case CLD_CORN_BOTTOM_LEFT:
		corner[0] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		corner[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		corner[2] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, (tileY * TILE_SIZE) + TILE_SIZE);
		corner[3] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		break;
	default:
		break;
	}

	return *corner;
}

sfVector2f cld_Half(sfVector2f* _half, int tileX, int tileY, CollisionType _type) {
	switch (_type)
	{
	case CLD_HALF_LEFT:
		_half[0] = vector2f(tileX * TILE_SIZE, tileY * TILE_SIZE);
		_half[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, tileY * TILE_SIZE);
		_half[2] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, (tileY * TILE_SIZE) + TILE_SIZE);
		_half[3] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		break;
	case CLD_HALF_RIGHT:
		_half[0] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, tileY * TILE_SIZE);
		_half[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, tileY * TILE_SIZE);
		_half[2] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		_half[3] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, (tileY * TILE_SIZE) + TILE_SIZE);
		break;
	case CLD_HALF_TOP:
		_half[0] = vector2f(tileX * TILE_SIZE, tileY * TILE_SIZE);
		_half[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, tileY * TILE_SIZE);
		_half[2] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		_half[3] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		break;
	case CLD_HALF_BOTTOM:
		_half[0] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		_half[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		_half[2] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		_half[3] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		break;
	default:
		break;
	}

	return *_half;
}

sfVector2f cld_LShape(sfVector2f* _cornerAngle, int tileX, int tileY, CollisionType _type) {
	switch (_type)
	{
	case CLD_ANGLE_TOP_LEFT:
		_cornerAngle[0] = vector2f(tileX * TILE_SIZE, tileY * TILE_SIZE);
		_cornerAngle[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, tileY * TILE_SIZE);
		_cornerAngle[2] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		_cornerAngle[3] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		_cornerAngle[4] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, (tileY * TILE_SIZE) + TILE_SIZE);
		_cornerAngle[5] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		break;
	case CLD_ANGLE_TOP_RIGHT:
		_cornerAngle[0] = vector2f(tileX * TILE_SIZE, tileY * TILE_SIZE);
		_cornerAngle[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, tileY * TILE_SIZE);
		_cornerAngle[2] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		_cornerAngle[3] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, (tileY * TILE_SIZE) + TILE_SIZE);
		_cornerAngle[4] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		_cornerAngle[5] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		break;
	case CLD_ANGLE_BOTTOM_RIGHT:
		_cornerAngle[0] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, tileY * TILE_SIZE);
		_cornerAngle[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, tileY * TILE_SIZE);
		_cornerAngle[2] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		_cornerAngle[3] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		_cornerAngle[4] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		_cornerAngle[5] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		break;
	case CLD_ANGLE_BOTTOM_LEFT:
		_cornerAngle[0] = vector2f(tileX * TILE_SIZE, tileY * TILE_SIZE);
		_cornerAngle[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, tileY * TILE_SIZE);
		_cornerAngle[2] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.5f, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		_cornerAngle[3] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.5f);
		_cornerAngle[4] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		_cornerAngle[5] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		break;
	default:
		break;
	}

	return *_cornerAngle;
}

sfVector2f cld_Diagonal(sfVector2f* _smallDiag, int tileX, int tileY, CollisionType _type) {
	switch (_type)
	{
	case CLD_SMALL_DIAG_TOP_LEFT:
		_smallDiag[0] = vector2f(tileX * TILE_SIZE, tileY * TILE_SIZE);
		_smallDiag[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.12f, tileY * TILE_SIZE);
		_smallDiag[2] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.88f);
		_smallDiag[3] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		_smallDiag[4] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.88f, (tileY * TILE_SIZE) + TILE_SIZE);
		_smallDiag[5] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.12f);
		break;
	case CLD_SMALL_DIAG_TOP_RIGHT:
		_smallDiag[0] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.88f, tileY * TILE_SIZE);
		_smallDiag[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, tileY * TILE_SIZE);
		_smallDiag[2] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.12f);
		_smallDiag[3] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.12f, (tileY * TILE_SIZE) + TILE_SIZE);
		_smallDiag[4] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		_smallDiag[5] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.88f);
		break;
	default:
		break;
	}

	return *_smallDiag;
}

sfVector2f cld_CornerTri(sfVector2f* _smallTriangle, int tileX, int tileY, CollisionType _type) {
	switch (_type)
	{
	case CLD_SMALL_TRI_TL:
		_smallTriangle[0] = vector2f(tileX * TILE_SIZE, tileY * TILE_SIZE);
		_smallTriangle[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.20f, tileY * TILE_SIZE);
		_smallTriangle[2] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.20f);
		break;
	case CLD_SMALL_TRI_TR:
		_smallTriangle[0] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.80f, tileY * TILE_SIZE);
		_smallTriangle[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, tileY * TILE_SIZE);
		_smallTriangle[2] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.20f);
		break;
	case CLD_SMALL_TRI_BR:
		_smallTriangle[0] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.80f);
		_smallTriangle[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		_smallTriangle[2] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.80f, (tileY * TILE_SIZE) + TILE_SIZE);
		break;
	case CLD_SMALL_TRI_BL:
		_smallTriangle[0] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE * 0.80f);
		_smallTriangle[1] = vector2f((tileX * TILE_SIZE) + TILE_SIZE * 0.20f, (tileY * TILE_SIZE) + TILE_SIZE);
		_smallTriangle[2] = vector2f(tileX * TILE_SIZE, (tileY * TILE_SIZE) + TILE_SIZE);
		break;
	default:
		break;
	}

	return *_smallTriangle;
}