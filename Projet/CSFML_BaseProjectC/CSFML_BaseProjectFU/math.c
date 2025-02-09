#include "math.h"

sfVector2f pointSegmentDistance(sfVector2f _circleCenter, sfVector2f _point1, sfVector2f _point2, float* _sqrDist)
{
	sfVector2f contactPoint;

	sfVector2f p1ToP2 = addVectorsrf(_point2, multiplyVectorsrf(_point1, -1.f));
	sfVector2f p1toCircleCenter = addVectorsrf(_circleCenter, multiplyVectorsrf(_point1, -1.f));

	float projection = dotProductf(p1ToP2, p1toCircleCenter);
	float abLenSq = getSqrMagnitudef(p1ToP2);
	float d = projection / abLenSq;

	if (d <= 0.f) {
		contactPoint = _point1;
	}
	else if (d >= 1.f) {
		contactPoint = _point2;
	}
	else {
		contactPoint = addVectorsrf(_point1, multiplyVectorsrf(p1ToP2, d));
	}

	*_sqrDist = getSqrMagnitudef(createVectorWithVectorf(_circleCenter, contactPoint));

	return contactPoint;
}

sfVector2f getCustomPoint(sfVector2f circleMiddle, float angle, float dist, int pointToGet)
{
	angle += 225 + pointToGet * 90;
	dist *= -1;
	return polarCoord(circleMiddle, dist, angle * DEG2RAD);

}

sfBool circleToCircleCollision(sfVector2f _pos1, sfVector2f _pos2, float radius)
{
	if (getSqrMagnitudeBetween2Vectorsf(_pos1, _pos2) < radius * radius) {
		return sfTrue;
	}
	return sfFalse;
}

sfVector2f createVectorWithVectorf(sfVector2f a, sfVector2f b) {
	return vector2f(b.x - a.x, b.y - a.y);
}

sfVector2i createVectorWithVectori(sfVector2i a, sfVector2i b) {
	return vector2i(b.x - a.x, b.y - a.y);
}

sfBool equals(sfVector2f a, sfVector2f b)
{
	float xOffset = b.x - a.x;
	float yOffset = b.y - a.y;

	if (fabs(xOffset) < EPSILON && fabs(yOffset) < EPSILON)
		return sfTrue;
	return sfFalse;
}

sfBool equalsWithOffset(sfVector2f a, sfVector2f b, float _offset)
{
	float xOffset = b.x - a.x;
	float yOffset = b.y - a.y;

	if (fabs(xOffset) < _offset && fabs(yOffset) < _offset)
		return sfTrue;
	return sfFalse;
}

sfBool equalsVectorsri(sfVector2i _a, sfVector2i _b)
{
	if (_a.x == _b.x && _a.y == _b.y)
		return sfTrue;

	return sfFalse;
}

sfVector2f addVectorsrf(sfVector2f a, sfVector2f b) {
	return vector2f(a.x + b.x, a.y + b.y);
}

sfVector2i addVectorsri(sfVector2i a, sfVector2i b) {
	return vector2i(a.x + b.x, a.y + b.y);
}

void addVectorsf(sfVector2f* a, sfVector2f b) {
	a->x = b.x + a->x;
	a->y = b.y + a->y;
}

void addVectorsi(sfVector2i* a, sfVector2i b) {
	a->x = b.x + a->x;
	a->y = b.y + a->y;
}

void substractVectorsf(sfVector2f* a, sfVector2f b) {
	a->x = b.x - a->x;
	a->y = b.y - a->y;
}

void substractVectorsi(sfVector2i* a, sfVector2i b) {
	a->x = b.x - a->x;
	a->y = b.y - a->y;
}

sfVector2f substractVectorsrf(sfVector2f a, sfVector2f b) {
	return vector2f(a.x - b.x, a.y - b.y);
}

sfVector2i substractVectorsri(sfVector2i a, sfVector2i b) {
	return vector2i(a.x - b.x, a.y - b.y);
}

float dotProductf(sfVector2f a, sfVector2f b) {
	return (a.x * b.x) + (a.y * b.y);
}

int dotProducti(sfVector2i a, sfVector2i b) {
	return (a.x * b.x) + (a.y * b.y);
}

float getMagnitudef(sfVector2f a) {
	return sqrtf((a.x * a.x) + (a.y * a.y));
}

int getMagnitudei(sfVector2i a) {
	return sqrt((a.x * a.x) + (a.y * a.y));
}

float getSqrMagnitudef(sfVector2f a) {
	return (a.x * a.x) + (a.y * a.y);
}

int getSqrMagnitudei(sfVector2i a) {
	return (a.x * a.x) + (a.y * a.y);
}

float getSqrMagnitudeBetween2Vectorsf(sfVector2f _a, sfVector2f _b)
{
	return getSqrMagnitudef(createVectorWithVectorf(_a, _b));
}

float getMagnitudeBetween2Vectorsf(sfVector2f _a, sfVector2f _b)
{
	return getMagnitudef(createVectorWithVectorf(_a, _b));
}

sfVector2f normalizef(sfVector2f a) {
	float magnitude = getMagnitudef(a);
	if (magnitude < EPSILON) {
		return a;
	}
	return vector2f(a.x / magnitude, a.y / magnitude);
}

sfVector2f getNormBetween2Vectorsf(sfVector2f _a, sfVector2f _b)
{
	return normalizef(createVectorWithVectorf(_a, _b));
}

void multiplyVectorsf(sfVector2f* a, float m) {
	a->x = a->x * m;
	a->y = a->y * m;
}

void multiplyVectorsi(sfVector2i* a, int m) {
	a->x = a->x * m;
	a->y = a->y * m;
}

sfVector2f multiplyVectorsrf(sfVector2f a, float m) {
	return vector2f(a.x * m, a.y * m);
}

sfVector2f getHalflVectorsrf(sfVector2f a) {
	return multiplyVectorsrf(a, 0.5);
}

sfVector2i multiplyVectorsri(sfVector2i a, int m) {
	return vector2i(a.x * m, a.y * m);
}

float getAngleBetweenVectors(sfVector2f a, sfVector2f b) {
	return atan2f(b.y, b.x) - atan2f(a.y, a.x);
}

sfBool isInFOV(sfVector2f targetObject, sfVector2f lookingObject, sfVector2f forwardE, float angle, float dist) {
	sfVector2f ltVect = createVectorWithVectorf(lookingObject, targetObject);
	if (getSqrMagnitudef(ltVect) > dist * dist)
		return sfFalse;
	sfVector2f normLTVect = normalizef(ltVect);

	float dotprod = dotProductf(forwardE, normLTVect);
	if (dotprod > cosf(DEG2RAD * angle * 0.5f)) {
		return sfTrue;
	}
	return sfFalse;
}

void makeFOV(sfVertexArray* FOV, float teta, float rad, sfVector2f pos, sfVector2f forward) {
	sfVertex* view = malloc(sizeof(sfVertex) * ((int)teta + 1));
	view[0].position = polarCoord(pos, 0.f, atan2(forward.y, forward.x));
	view[0].color = sfColor_fromRGBA(225, 225, 225, 175);
	sfVertexArray_append(FOV, view[0]);
	int count = 1;
	for (int i = (int)(-teta / 2); i < (int)(teta / 2); i++) {
		view[count].position = polarCoord(pos, rad, atan2f(forward.y, forward.x) + i * DEG2RAD);
		view[count].color = sfColor_fromRGBA(0, 255, 20, 255);
		sfVertexArray_append(FOV, view[count]);
		count++;
	}
	sfVertexArray_append(FOV, view[0]);
	free(view);
}

sfVector2f polarCoord(sfVector2f a, float r, float teta) {
	return vector2f(a.x + (r * cosf(teta)), a.y + (r * sinf(teta)));
}

void AddForceWithoutMass(sfVector2f _force, sfVector2f* _velocity)
{
	float massToApply = 1.f;
	*_velocity = addVectorsrf(*_velocity, multiplyVectorsrf(_force, 1.f / massToApply));
}

void AddForce(sfVector2f _force, sfVector2f* _velocity, float _mass)
{
	*_velocity = addVectorsrf(*_velocity, multiplyVectorsrf(_force, 1.f / _mass));
}

sfVector2f vectorSnap(sfVector2f _pos, float _snap) {
	return vector2f((int)(_pos.x / _snap) * _snap, (int)(_pos.y / _snap) * _snap);
}