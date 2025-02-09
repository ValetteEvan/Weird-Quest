#include "tears.h"
#include "math.h"
#include "player.h"
#include "textureManager.h"

#define GT_TEARS STD_LIST_GETDATA(tearsList, Tears_struct, x)

sfRectangleShape* t_tears;
sfCircleShape* t_impact;
sfTexture* t_textureSmall;
sfTexture* t_textureBig;
float timer_ttest;

void trs_initTears()
{
	t_impact = sfCircleShape_create();
	t_textureSmall = GetSfTexture("bigSadIdle");
	t_textureBig = GetSfTexture("golemT_IDLE_ANNEAU");
	t_tears = sfRectangleShape_create();
	sfRectangleShape_setFillColor(t_tears, sfWhite);
	sfRectangleShape_setOutlineThickness(t_tears, 0.f);
	timer_ttest = 0.0f;
	tearsList = STD_LIST_CREATE(Tears_struct, 0);
}

void trs_addTears(sfVector2f _startPosition, sfVector2f _endPosition, float _timeOfLerp, Tears_Type _type)
{
	Tears_struct tmp;
	if (_type == SMALL_TEAR) {
		int choice = rand() % 4;
		if (choice == 0) tmp.rect = IntRect(0, 708, 24, 24);
		else if (choice == 1) tmp.rect = IntRect(24, 708, 24, 24);
		else if (choice == 2) tmp.rect = IntRect(48, 708, 24, 24);
		else tmp.rect = IntRect(72, 708, 24, 24);
	}
	
	tmp.angle = getAngleBetweenVectors(NULLVECTF, getNormBetween2Vectorsf(_startPosition, _endPosition)) * RAD2DEG;
	tmp.position = _startPosition;
	tmp.targetPosition = _endPosition;
	tmp.startPosition = tmp.position;
	tmp.radius = 0.0f;
	tmp.timer_radius = 0.0f;
	tmp.colRect = FlRect(0.0f, 0.0f, 0.0f, 0.0f);
	tmp.timeOfLerp = _timeOfLerp;

	tmp.type = _type;
	tmp.isDead = sfFalse;
	tmp.frameX = 0;
	tmp.timerAnim = 0.f; 
	tmp.opacity = 255;
	tmp.scale = 1.f;


	tearsList->push_back(&tearsList, &tmp);
}

void trs_Update(Window* _window)
{
	float delta = getDeltaTime();
	timer_ttest += delta;
	if (customKeyboardIsKeyPressed(sfKeyNum9, _window->renderWindow) && timer_ttest > 0.2f)
	{
		timer_ttest = 0.0f;

		sfVector2f startPos = vector2f(rand_float(0, 1920.f), -100.f);
		trs_addTears(startPos, addVectorsrf(startPos, vector2f(0.f, (float)rand_float(200.f, 980.f))), 2.f, BIG_TEAR); // vector2f(rand_float(100.0f, 1820.0f), rand_float(100.0f, 980.0f))

	}

	for (int x = 0; x < tearsList->size(tearsList); x++)
	{
		if (!GT_TEARS->isDead) {
			GT_TEARS->timer_radius += delta;
			GT_TEARS->position = vec2f_lerp_smooth(GT_TEARS->startPosition, GT_TEARS->targetPosition, GT_TEARS->timer_radius / GT_TEARS->timeOfLerp);

			if (GT_TEARS->type == SMALL_TEAR) {
				if (GT_TEARS->timer_radius < GT_TEARS->timeOfLerp / 2.f)
					GT_TEARS->scale = lerp_smooth(1.f, 2.f, GT_TEARS->timer_radius / (GT_TEARS->timeOfLerp / 2.f));
				else
					GT_TEARS->scale = lerp_smooth(2.f, 1.f, (GT_TEARS->timer_radius - (GT_TEARS->timeOfLerp / 2.f)) / (GT_TEARS->timeOfLerp / 2.f));
			}
			else {
				GT_TEARS->scale = lerp_smooth(2.f, 1.f, GT_TEARS->timer_radius / GT_TEARS->timeOfLerp);
			}

			
			GT_TEARS->radius += 15.0f * delta;

			if (GT_TEARS->timer_radius / GT_TEARS->timeOfLerp >= 1.0f /*- EPSILON*/)
			{
				sfFloatRect tmpPBound = plr_getPlayerBoundsWithEnemies();
				if (sfFloatRect_intersects(&GT_TEARS->colRect, &tmpPBound, NULL))plr_addDamagePlayerLife(1);
				GT_TEARS->isDead = sfTrue;
				GT_TEARS->frameX = 0;
				GT_TEARS->timerAnim = 0.f;
			}
		}
		else {
			if (GT_TEARS->type == BIG_TEAR) {
				if (GT_TEARS->frameX == 7) {
					GT_TEARS->opacity = lerp(255, 0, GT_TEARS->timerAnim * 2);
					GT_TEARS->timerAnim += getDeltaTime();
				}

				if (GT_TEARS->frameX == 7 && GT_TEARS->timerAnim > 0.5f) {
					tearsList->erase(&tearsList, x);
				}
			}
			else {
				if (GT_TEARS->frameX == 1) {
					GT_TEARS->opacity = lerp(255, 0, GT_TEARS->timerAnim * 6.f);
				}

				if (GT_TEARS->frameX == 1 && GT_TEARS->timerAnim > 0.15f) {
					tearsList->erase(&tearsList, x);
				}
			}
			
		}
		
	}
}

void trs_displayTears(Window* _window)
{
	for (int x = 0; x < tearsList->size(tearsList); x++)
	{
		if (!GT_TEARS->isDead) {
			//The Time of impact
			sfCircleShape_setPosition(t_impact, GT_TEARS->targetPosition);
			sfCircleShape_setRadius(t_impact, GT_TEARS->radius);
			sfCircleShape_setOrigin(t_impact, vector2f(GT_TEARS->radius, GT_TEARS->radius));
			sfCircleShape_setFillColor(t_impact, sfColor_fromRGBA(255, 0, 5, 40));
			sfCircleShape_setOutlineColor(t_impact, sfColor_fromRGBA(255, 0, 5, 40));
			sfCircleShape_setOutlineThickness(t_impact, 0.f);
			sfRenderTexture_drawCircleShape(_window->rdrt, t_impact, NULL);

			//The circle of impact
			sfCircleShape_setPosition(t_impact, GT_TEARS->targetPosition);
			sfCircleShape_setRadius(t_impact, 30.0f);
			sfCircleShape_setOrigin(t_impact, vector2f(30.0f, 30.0f));
			sfCircleShape_setFillColor(t_impact, sfTransparent);
			sfCircleShape_setOutlineColor(t_impact, sfColor_fromRGBA(255, 0, 5, 40));
			sfCircleShape_setOutlineThickness(t_impact, 2.f);
			GT_TEARS->colRect = FlRect(GT_TEARS->targetPosition.x - 20.f, GT_TEARS->targetPosition.y - 20.f, 35.0f, 35.0f);
			//GT_TEARS->colRect = sfCircleShape_getGlobalBounds(t_tears);
			sfRenderTexture_drawCircleShape(_window->rdrt, t_impact, NULL);

			sfRectangleShape_setPosition(t_tears, GT_TEARS->position);
			if (GT_TEARS->type == SMALL_TEAR) {
				sfRectangleShape_setRotation(t_tears, GT_TEARS->angle - 90.f);
				sfRectangleShape_setTexture(t_tears, t_textureSmall, sfTrue);
				sfRectangleShape_setSize(t_tears, vector2f(24.0f, 24.0f));
				sfRectangleShape_setOrigin(t_tears, vector2f(10.0f, 10.0f));
				sfRectangleShape_setTextureRect(t_tears, GT_TEARS->rect);
			}
			else {
				sfRectangleShape_setTexture(t_tears, t_textureBig, sfTrue);
				sfRectangleShape_setRotation(t_tears, GT_TEARS->angle - 90.f);
				sfRectangleShape_setSize(t_tears, vector2f(58.f, 122.f));
				sfRectangleShape_setOrigin(t_tears, vector2f(29.f, 122.f));
				GT_TEARS->rect = animatorAnim("golemT_Goute", &GT_TEARS->frameX, &GT_TEARS->timerAnim, 0);
				sfRectangleShape_setTextureRect(t_tears, GT_TEARS->rect);
			}

			sfRectangleShape_setScale(t_tears, vector2f(GT_TEARS->scale, GT_TEARS->scale));
			sfRenderTexture_drawRectangleShape(_window->rdrt, t_tears, NULL);
			sfRectangleShape_setFillColor(t_tears, sfColor_fromRGBA(255, 255, 255, 255));

		}
		
		
	}
}

void trs_displayDeadTears(Window* _window)
{
	for (int x = 0; x < tearsList->size(tearsList); x++)
	{
		if (GT_TEARS->isDead) {
			sfRectangleShape_setPosition(t_tears, GT_TEARS->position);
			if (GT_TEARS->type == SMALL_TEAR) {
				sfRectangleShape_setFillColor(t_tears, sfColor_fromRGBA(255, 255, 255, GT_TEARS->opacity));
				sfRectangleShape_setRotation(t_tears, 0.f);
				sfRectangleShape_setSize(t_tears, vector2f(80.f, 38.f));
				sfRectangleShape_setOrigin(t_tears, vector2f(40.f, 19.f));
				GT_TEARS->rect = animatorAnim("bigSadTears", &GT_TEARS->frameX, &GT_TEARS->timerAnim, 0);
				sfRectangleShape_setTextureRect(t_tears, GT_TEARS->rect);
			}
			else {
				sfRectangleShape_setTexture(t_tears, t_textureBig, sfTrue);
				sfRectangleShape_setFillColor(t_tears, sfColor_fromRGBA(255, 255, 255, GT_TEARS->opacity));
				sfRectangleShape_setRotation(t_tears, 0.f);
				sfRectangleShape_setSize(t_tears, vector2f(206.f, 194.f));
				sfRectangleShape_setOrigin(t_tears, vector2f(103.f, 97.f));
				if(GT_TEARS->frameX < 7)
					GT_TEARS->rect = animatorAnim("golemT_Goute_Impact", &GT_TEARS->frameX, &GT_TEARS->timerAnim, 0);
				sfRectangleShape_setTextureRect(t_tears, GT_TEARS->rect);
			}
			sfRectangleShape_setScale(t_tears, vector2f(1.f, 1.f));
			sfRenderTexture_drawRectangleShape(_window->rdrt, t_tears, NULL);
			sfRectangleShape_setFillColor(t_tears, sfColor_fromRGBA(255, 255, 255, 255));
		}
	}
}

void trs_Deinit() {
	return;
	if (!t_tears || !t_impact) return;
	sfRectangleShape_destroy(t_tears);
	sfCircleShape_destroy(t_impact);
}