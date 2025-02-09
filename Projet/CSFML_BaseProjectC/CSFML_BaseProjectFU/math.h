#pragma once
#include "tools.h"

#define EPSILON (float)1e-6
#define MAX (a,b) a >= b ? a : b
#define MIN (a,b) a <= b ? a : b
#define DEG2RAD 0.01745329
#define RAD2DEG 57.295779
#define PI 3.141592
#define LERP(a, b, t) (b-a) * t + a
#define SIGN(nbr) nbr >= 0.f ? 1.f : -1.f

/// <summary>
/// Create a vector between 2 given float vectors
/// </summary>
/// <param name="a">the start vector</param>
/// <param name="b">the end vector</param>
/// <returns>the vector going from a to b</returns>
sfVector2f createVectorWithVectorf(sfVector2f a, sfVector2f b);

/// <summary>
/// Create a vector between 2 given int vectors
/// </summary>
/// <param name="a">the start vector</param>
/// <param name="b">the end vector</param>
/// <returns>the vector going from a to b</returns>
sfVector2i createVectorWithVectori(sfVector2i a, sfVector2i b);

/// <summary>
/// add 2 float vectors together
/// </summary>
/// <param name="a">the first vector</param>
/// <param name="b">the 2nd vector</param>
/// <returns>the adition between those 2 vectors</returns>
sfVector2f addVectorsrf(sfVector2f a, sfVector2f b);

/// <summary>
/// add 2 int vectors together
/// </summary>
/// <param name="a">the first vector</param>
/// <param name="b">the 2nd vector</param>
/// <returns>the adition between those 2 vectors</returns>
sfVector2i addVectorsri(sfVector2i a, sfVector2i b);

/// <summary>
/// add 2 float vectors together and put the result in the first vector
/// </summary>
/// <param name="a">the first vector</param>
/// <param name="b">the 2nd vector</param>
void addVectorsf(sfVector2f* a, sfVector2f b);

/// <summary>
/// add 2 int vectors together and put the result in the first vector
/// </summary>
/// <param name="a">the first vector</param>
/// <param name="b">the 2nd vector</param>
void addVectorsi(sfVector2i* a, sfVector2i b);

/// <summary>
/// substract 2 float vectors together
/// </summary>
/// <param name="a">the first vector</param>
/// <param name="b">the 2nd vector</param>
/// <returns>the substraction between those 2 vectors</returns>
sfVector2f substractVectorsrf(sfVector2f a, sfVector2f b);

/// <summary>
/// substract 2 int vectors together
/// </summary>
/// <param name="a">the first vector</param>
/// <param name="b">the 2nd vector</param>
/// <returns>the substraction between those 2 vectors</returns>
sfVector2i substractVectorsri(sfVector2i a, sfVector2i b);

/// <summary>
/// substract 2 float vectors together and store the result in the first vector
/// </summary>
/// <param name="a">the first vector</param>
/// <param name="b">the 2nd vector</param>
/// <returns>the substraction between those 2 vectors</returns>
void substractVectorsf(sfVector2f* a, sfVector2f b);

/// <summary>
/// substract 2 int vectors together and store the result in the first vector
/// </summary>
/// <param name="a">the first vector</param>
/// <param name="b">the 2nd vector</param>
/// <returns>the substraction between those 2 vectors</returns>
void substractVectorsi(sfVector2i* a, sfVector2i b);

/// <summary>
/// calculate the dot product of 2 float vectors
/// </summary>
/// <param name="a">the first vector</param>
/// <param name="b">the 2nd vector</param>
/// <returns>the dot product</returns>
float dotProductf(sfVector2f a, sfVector2f b);

/// <summary>
/// calculate the dot product of 2 int vectors
/// </summary>
/// <param name="a">the first vector</param>
/// <param name="b">the 2nd vector</param>
/// <returns>the dot product</returns>
int dotProducti(sfVector2i a, sfVector2i b);

/// <summary>
/// get the magnitude of a float vector
/// </summary>
/// <param name="a">the vector</param>
/// <returns>the magnitude of a</returns>
float getMagnitudef(sfVector2f a);

/// <summary>
/// get the magnitude of a int vector
/// </summary>
/// <param name="a">the vector</param>
/// <returns>the magnitude of a</returns>
int getMagnitudei(sfVector2i a);

/// <summary>
/// get the squared magnitude of a float vector
/// </summary>
/// <param name="a">the vector</param>
/// <returns>the squared magnitude of a</returns>
float getSqrMagnitudef(sfVector2f a);

/// <summary>
/// get the squared magnitude of a float vector
/// </summary>
/// <param name="a">the vector</param>
/// <returns>the squared magnitude of a</returns>
int getSqrMagnitudei(sfVector2i a);

/// <summary>
/// get the squared distance between 2 vectors (the order doesn't matter)
/// </summary>
/// <param name="_a">the first vector</param>
/// <param name="_b">the 2nd vecor</param>
/// <returns>the squared magnitude between both vectors</returns>
float getSqrMagnitudeBetween2Vectorsf(sfVector2f _a, sfVector2f _b);

/// <summary>
/// get the distance between 2 vectors (the order doesn't matter)
/// </summary>
/// <param name="_a">the first vector</param>
/// <param name="_b">the 2nd vecor</param>
/// <returns>the magnitude between both vectors</returns>
float getMagnitudeBetween2Vectorsf(sfVector2f _a, sfVector2f _b);


/// <summary>
/// normalize a float vector
/// </summary>
/// <param name="a">the vector to normalize</param>
/// <returns>the normalized vector</returns>
sfVector2f normalizef(sfVector2f a);

/// <summary>
/// gives the nomalized vector between 2 vectors
/// </summary>
/// <param name="_a">the start point</param>
/// <param name="_b">the end point</param>
/// <returns>the nomalized vector between both vectors</returns>
sfVector2f getNormBetween2Vectorsf(sfVector2f _a, sfVector2f _b);

/// <summary>
/// Multiply a float vector by a float number and store the result in the vector
/// </summary>
/// <param name="a">The vector</param>
/// <param name="m">The number to multiply the vector by</param>
void multiplyVectorsf(sfVector2f* a, float m);

/// <summary>
/// Multiply a int vector by a int number and store the result in the vector
/// </summary>
/// <param name="a">The vector</param>
/// <param name="m">The number to multiply the vector by</param>
void multiplyVectorsi(sfVector2i* a, int m);

/// <summary>
/// Multiply a float vector by a float number
/// </summary>
/// <param name="a">The vector</param>
/// <param name="m">The number to multiply the vector by</param>
/// <returns>the multiplied vector</returns>
sfVector2f multiplyVectorsrf(sfVector2f a, float m);

/// <summary>
/// Multiply a int vector by a int number
/// </summary>
/// <param name="a">The vector</param>
/// <param name="m">The number to multiply the vector by</param>
/// <returns>the multiplied vector</returns>
sfVector2i multiplyVectorsri(sfVector2i a, int m);

/// <summary>
/// check if 2 vectors are equal to eachother
/// </summary>
/// <param name="a">the first vector</param>
/// <param name="b">the 2nd vector</param>
/// <returns>sftrue if they are equals, sffalse if otherwise</returns>
sfBool equals(sfVector2f a, sfVector2f b);

sfBool equalsWithOffset(sfVector2f a, sfVector2f b, float _offset);

/// <summary>
/// check if 2 int vectors are equal to eachother
/// </summary>
/// <param name="a">the first vector</param>
/// <param name="b">the 2nd vector</param>
/// <returns>sftrue if they are equals, sffalse if otherwise</returns>
sfBool equalsVectorsri(sfVector2i _a, sfVector2i _b);

/// <summary>
/// check if an object is in the fov of another object
/// </summary>
/// <param name="targetObject">the pos of the object targeted</param>
/// <param name="lookingObject">the pos of the looking object</param>
/// <param name="forwardE">the forward of the looking object</param>
/// <param name="hook_Angle">the hook_Angle of the fov used for the size of it</param>
/// <param name="dist">the max dist of the fov</param>
/// <returns>true if the looking object is in fov false if otherwise</returns>
sfBool isInFOV(sfVector2f targetObject, sfVector2f lookingObject, sfVector2f forwardE, float angle, float dist);

/// <summary>
/// calculate a position with an hook_Angle and a distance
/// </summary>
/// <param name="a">the initial pos</param>
/// <param name="r">the distance</param>
/// <param name="teta">the hook_Angle</param>
/// <returns>the new pos</returns>
sfVector2f polarCoord(sfVector2f a, float r, float teta);

/// <summary>
/// display the fov
/// </summary>
/// <param name="FOV">a vertex array</param>
/// <param name="teta">the hook_Angle of the fov</param>
/// <param name="rad">the radius of the fov</param>
/// <param name="pos">the pos of the looking object</param>
/// <param name="forward">the forward of the looking object</param>
void makeFOV(sfVertexArray* FOV, float teta, float rad, sfVector2f pos, sfVector2f forward);

/// <summary>
/// give you half of a vector
/// </summary>
/// <param name="a">the initial vector</param>
/// <returns>half of a</returns>
sfVector2f getHalflVectorsrf(sfVector2f a);

/// <summary>
/// get the hook_Angle between 2 vectors
/// </summary>
/// <param name="a">the first vector</param>
/// <param name="b">the 2nd vector</param>
/// <returns>the hook_Angle between those 2 vectors in radiant</returns>
float getAngleBetweenVectors(sfVector2f a, sfVector2f b);

/// <summary>
/// create a float vector
/// </summary>
/// <param name="_x">the x pos</param>
/// <param name="_y">the y pos</param>
/// <returns>the vector</returns>
sfVector2f vector2f(float _x, float _y);

/// <summary>
/// gives you the closest point of a circle on a rectangle
/// </summary>
/// <param name="_circleCenter">the center of the circle</param>
/// <param name="_point1">the first point of the segment of the rectangle to check</param>
/// <param name="_point2">the 2nd point of the segment of the rectangle to check</param>
/// <param name="_sqrDist">the adress of the distance that will be updated to know if there is a collision you can check if the sqr dist is >= to the squared radius of the circle</param>
/// <returns>the closest point to the circle on the segment of the rectangle</returns>
sfVector2f pointSegmentDistance(sfVector2f _circleCenter, sfVector2f _point1, sfVector2f _point2, float* _sqrDist);

/// <summary>
/// get the position of a point on a rectangle that can be rotated
/// </summary>
/// <param name="circleMiddle"></param>
/// <param name="hook_Angle"></param>
/// <param name="dist"></param>
/// <param name="pointToGet"></param>
/// <returns></returns>
sfVector2f getCustomPoint(sfVector2f circleMiddle, float angle, float dist, int pointToGet);

/// <summary>
/// tells you if 2 circles are colliding together
/// </summary>
/// <param name="_pos1">the pos of the first circle</param>
/// <param name="_pos2">the pos of the 2nd circle</param>
/// <param name="radius">the radius of both circle added together</param>
/// <returns>true if there is a collision false if there is no collision</returns>
sfBool circleToCircleCollision(sfVector2f _pos1, sfVector2f _pos2, float radius);

/// <summary>
/// add force with a default mass of 1
/// </summary>
/// <param name="_force">the force vector</param>
/// <param name="_velocity">the adress of the velocity vector</param>
void AddForceWithoutMass(sfVector2f _force, sfVector2f* _velocity);

/// <summary>
/// add force with a defined mass
/// </summary>
/// <param name="_force">the force vector</param>
/// <param name="_velocity">the adress of the velocity vector</param>
/// <param name="_mass">the mass to apply</param>
void AddForce(sfVector2f _force, sfVector2f* _velocity, float _mass);

/// Snaps the given vector onto a discrete coordinate grid.
/// \param _pos - Non-snapped position
/// \param _snap - Size of the grid to snap the position onto
sfVector2f vectorSnap(sfVector2f _pos, float _snap);