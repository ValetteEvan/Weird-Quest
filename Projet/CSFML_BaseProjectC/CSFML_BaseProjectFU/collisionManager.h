/// Function prefix: cll_

#pragma once
#include "tools.h"
#include "map.h"
#include "colliders.h"

typedef struct
{
	sfVector2f point;
	sfVector2f normal;
	float sqrDistance;
}Contact;

/// \deprecated Not sure what that one is used for, it can probably need a quick refactor
sfBool cll_BoxBox(sfVector2f _playerNextPos, struct BoxData* _box);

/// Checks collisions between a polygon and a rectangle.
/// \param _boxShape - An array of sfVector2f containing data for the rectangle's vertices
/// \param _polygoneShape - An array of sfVector2f containing data for the polygon's vertices
/// \param _vertCount - How many points the polygon contains
sfBool cll_PolygonRect(sfVector2f* _boxShape, sfVector2f* _polygoneShape, int _vertCount);

/// Checks collisions between a rectangle and a line segment.
/// \param _l1 and _l2 - The line's extremities
/// \param _boxShape - An array of sfVector2f containing data for the rectangle's vertices
sfBool cll_RectLine(sfVector2f _l1, sfVector2f _l2, sfVector2f* _boxShape);

/// Checks intersection between two lines.
/// \param _l1_1 and _l1_2 - The first line's extremities
/// \param _l2_1 and _l2_2 - The second line's extremities
sfBool cll_LineLine(sfVector2f _l1_1, sfVector2f _l1_2, sfVector2f _l2_1, sfVector2f _l2_2);

/// Checks collisions between a polygon and a point.
/// \param _p - The point to test collisions for
/// \param _poly - An array of sfVector2f containing data for the polygon's vertices
/// \param _vertCount - How many points the polygon contains
sfBool cll_PolygonPoint(sfVector2f _p, sfVector2f* _poly, int _vertCount);

/// Checks collisions between an axis-aligned bounding box and the map's collision layer, taking its movement speed into account for predictive detection.
/// \param _aabb - The entity to test collisions for
/// \param _spd - The entity's current movement speed
sfBool cll_RectMap(sfFloatRect _aabb, sfVector2f _spd);