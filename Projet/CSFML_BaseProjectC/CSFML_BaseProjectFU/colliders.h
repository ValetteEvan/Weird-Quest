/// Function prefix: cld_

#pragma once
#include "tools.h"
#include "map.h"

sfVector2f cld_Triangle(sfVector2f* triangle, int tileX, int tileY, CollisionType _type);
sfVector2f cld_CornerSqr(sfVector2f* corner, int tileX, int tileY, CollisionType _type);
sfVector2f cld_Half(sfVector2f* _half, int tileX, int tileY, CollisionType _type);
sfVector2f cld_LShape(sfVector2f* _cornerAngle, int tileX, int tileY, CollisionType _type);
sfVector2f cld_Diagonal(sfVector2f* _smallDiag, int tileX, int tileY, CollisionType _type);
sfVector2f cld_CornerTri(sfVector2f* _smallTriangle, int tileX, int tileY, CollisionType _type);