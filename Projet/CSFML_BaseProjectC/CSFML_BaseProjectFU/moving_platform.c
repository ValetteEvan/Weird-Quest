#include "moving_platform.h"
#include "map.h"
#include "math.h"
#include "player.h"

#define MVPL_SPD 250.f

sfSprite* mvpl_RenderSprite;
sfBool mvpl_WalkingOnPlatform;

void mvpl_Init() {
	static sfBool onePass = sfTrue;
	if (onePass) {
		onePass = sfFalse;
		mvpl_ListD = STD_LIST_CREATE(mvplData, 0);
		mvpl_ListG = STD_LIST_CREATE(mvplGroup, 0);

		mvpl_RenderSprite = sfSprite_create();
		sfSprite_setTexture(mvpl_RenderSprite, GetSfTexture("assets_dungeon"), sfTrue);
		mvpl_WalkingOnPlatform = sfTrue;
	}
}

mvplData* mvpl_CreatePlatform(sfVector2f _pos) {
	if (!mvpl_ListD) return NULL;

	mvplData tmp = { _pos, NULLVECTI, NULLVECTF, 10.f, NULL };
	tmp.posSnap = mvpl_SnapPos(tmp.pos);
	tmp.pos = vector2f(tmp.posSnap.x * TILE_SIZE, tmp.posSnap.y * TILE_SIZE);
	mvpl_ListD->push_back(&mvpl_ListD, &tmp);
	return STD_LIST_GETDATA(mvpl_ListD, mvplData, mvpl_ListD->size(mvpl_ListD) - 1);
}

mvplGroup* mvpl_CreateGroup() {
	if (!mvpl_ListG) return NULL;
	
	mvplGroup tmp = { 0 };
	tmp.list = STD_LIST_CREATE(mvplList, 0);
	mvpl_ListG->push_back(&mvpl_ListG, &tmp);
	return STD_LIST_GETDATA(mvpl_ListG, mvplGroup, mvpl_ListG->size(mvpl_ListG) - 1);
}

mvplList* mvpl_AddToGroup(mvplGroup* _list, mvplData* _mvpl) {
	if (!_list || !_list->list || !_mvpl) return NULL;

	mvplList tmp = { _mvpl };
	tmp.data->group = _list;
	_list->list->push_back(&_list->list, &tmp);
	return STD_LIST_GETDATA(_list->list, mvplList, _list->list->size(_list->list) - 1);
}

void mvpl_ConstituteGroups() {
	if (!mvpl_ListD || !mvpl_ListG || !mvpl_ListD->size(mvpl_ListD)) return;

	for (int i = 0; i < mvpl_ListD->size(mvpl_ListD); i++) {
		/// If the given platform isn't part of a group already, create one for it
		if (!MVPL_GETDATA_P->group) {
			mvplGroup* group = mvpl_CreateGroup();
			mvpl_AddToGroup(group, MVPL_GETDATA_P);
		}

		/// Test all other platforms and look for adjacent ones...
		for (int j = 0; j < mvpl_ListD->size(mvpl_ListD); j++) {
			mvplData* tmp = STD_LIST_GETDATA(mvpl_ListD, mvplData, j);
			if (tmp == MVPL_GETDATA_P || tmp->group)
				continue; /// ...check if they don't already belong to a group...
			int dist = abs(tmp->posSnap.x - MVPL_GETDATA_P->posSnap.x) + abs(tmp->posSnap.y - MVPL_GETDATA_P->posSnap.y);
			if (dist == 1)
				mvpl_AddToGroup(MVPL_GETDATA_P->group, tmp); /// ...and add them to the former's group
		}
	}
	
	/// Coalesces neighboring groups (in case multiple groups were formed when there should only be one)
	for (int i = 0; i < mvpl_ListG->size(mvpl_ListG); i++) { /// For all created groups
		mvplGroup* tmpG = MVPL_GETDATA_G; /// Currently scanned group

		for (int j = 0; j < tmpG->list->size(tmpG->list); j++) { /// For all platforms in that given group
			mvplData* tmpP = STD_LIST_GETDATA(tmpG->list, mvplList, j)->data; /// Current member of the current group we're looking into

			for (int k = 0; k < mvpl_ListD->size(mvpl_ListD); k++) { /// Test all platforms
				mvplData* tmp = STD_LIST_GETDATA(mvpl_ListD, mvplData, k); /// Looking through all platforms on the field
				if (tmp->group == tmpG)
					continue; /// Checks if the tested platform isn't already part of the group

				int dist = abs(tmp->posSnap.x - tmpP->posSnap.x) + abs(tmp->posSnap.y - tmpP->posSnap.y);
				if (dist == 1) { /// If within reasonable distance...
					mvplGroup* g = tmp->group;
					for (int n = 0; n < g->list->size(g->list); n++) { /// ...for all platforms in the target group...
						mvplList* listEl = STD_LIST_GETDATA(g->list, mvplList, n);
						mvpl_AddToGroup(tmpG, listEl->data); /// ...add it to the receiving group...
						g->list->erase(&(g->list), n); /// ...and remove it from the target group
					}
				}
			}
		}
	}
	
	/// Clears all groups left empty
	for (int i = 0; i < mvpl_ListG->size(mvpl_ListG); i++) { /// For all created groups
		if (MVPL_GETDATA_G->list->size(MVPL_GETDATA_G->list) == 0) mvpl_ListG->erase(&mvpl_ListG, i);
	}
}

void mvpl_Update() {
	for (int i = 0; i < mvpl_ListG->size(mvpl_ListG); i++) {
		/// Testing for collisions
		for (int j = 0; j < MVPL_GETDATA_G->list->size(MVPL_GETDATA_G->list); j++) {
			if (!mvpl_IsMoving(MVPL_GETDATA_L(MVPL_GETDATA_G)->data)) break; /// Do not do anything if the platform isn't moving
			sfVector2i posFuture = MVPL_GETDATA_L(MVPL_GETDATA_G)->data->posSnap; /// Fetching where the platform will be after this tick
			sfVector2f spd = MVPL_GETDATA_L(MVPL_GETDATA_G)->data->spd; /// Platform's current movement speed
			
			/// Manual adjustments to the platform's upcoming pos to account for the fact that their origin is in fact in the upper left corner
			if		(fabs(spd.x) > fabs(spd.y) && spd.x > 0.f) posFuture.x++;
			else if (fabs(spd.x) < fabs(spd.y) && spd.y > 0.f) posFuture.y++;

			sfBool somethingInTheWay = sfFalse; /// Testing if another platform is in the way
			mvplData* cur = STD_LIST_GETDATA(MVPL_GETDATA_G->list, mvplList, j)->data; /// Alias to simplify things
			for (int k = 0; k < mvpl_ListD->size(mvpl_ListD); k++) {
				if (j == k) continue;
				mvplData* tmp = STD_LIST_GETDATA(mvpl_ListD, mvplData, k);
				if (tmp->group == cur->group) continue; /// Do not do anything if whatever is ahead happens to be part of that platform's cluster
				if (posFuture.x == tmp->posSnap.x && posFuture.y == tmp->posSnap.y) {
					somethingInTheWay = sfTrue;
					break;
				}
			}
			
			/// If something is in the way of the platform while it is moving
			if (map.layerCollision[posFuture.y][posFuture.x].type != CLD_VOID || !map_isIndexInArraySwapped(posFuture) || somethingInTheWay) {
				for (int k = 0; k < MVPL_GETDATA_G->list->size(MVPL_GETDATA_G->list); k++) {
					/// Manual adjustments to the platform's pos to account for the fact that their origin is in fact in the upper left corner
					if (fabs(spd.x) > fabs(spd.y) && spd.x < 0.f) STD_LIST_GETDATA(MVPL_GETDATA_G->list, mvplList, k)->data->pos.x += TILE_SIZE;
					else if (fabs(spd.x) < fabs(spd.y) && spd.y < 0.f) STD_LIST_GETDATA(MVPL_GETDATA_G->list, mvplList, k)->data->pos.y += TILE_SIZE;

					/// There's something strange about that "sop" sign.
					STD_LIST_GETDATA(MVPL_GETDATA_G->list, mvplList, k)->data->spd = NULLVECTF;
				}
			}
		}
	}

	/// Individual platform updates
	sfBool isOnPlatform = sfFalse;
	for (int i = 0; i < mvpl_ListD->size(mvpl_ListD); i++) {
		sfVector2f* speed = &(MVPL_GETDATA_P->spd); /// Alias to make things more readable

		if (mvpl_IsMoving(MVPL_GETDATA_P)) {
			/// Snapping platform movement onto the XY axes
			if (fabs(speed->x) > fabs(speed->y)) speed->y = 0.f;
			else speed->x = 0.f;

			MVPL_GETDATA_P->pos = addVectorsrf(MVPL_GETDATA_P->pos, multiplyVectorsrf(*speed, getDeltaTime()));
		}

		/// Moves the player if they're standing on top of the platform
		sfVector2f posPlr = plr_GetPos();
		sfFloatRect hitboxPlr = FlRect(posPlr.x - 5.f, posPlr.y - 5.f, 10.f, 10.f);
		sfFloatRect hitboxMvpl = FlRect(MVPL_GETDATA_P->pos.x, MVPL_GETDATA_P->pos.y, TILE_SIZE, TILE_SIZE);
		if (sfFloatRect_intersects(&hitboxMvpl, &hitboxPlr, NULL)) {
			plr_ForceMove(multiplyVectorsrf(*speed, getDeltaTime()));
			isOnPlatform = sfTrue;
		}

		/// Snapping platform position onto the grid if it isn't moving
		MVPL_GETDATA_P->posSnap = mvpl_SnapPos(MVPL_GETDATA_P->pos);
		if (!mvpl_IsMoving(MVPL_GETDATA_P)) MVPL_GETDATA_P->pos = vector2f(MVPL_GETDATA_P->posSnap.x * TILE_SIZE, MVPL_GETDATA_P->posSnap.y * TILE_SIZE);
	}

	mvpl_WalkingOnPlatform = isOnPlatform;
}

void mvpl_Display(Window* _window) {
	if (mvpl_ListD == NULL)
		return;

	for (int i = 0; i < mvpl_ListD->size(mvpl_ListD); i++) {
		sfSprite_setPosition(mvpl_RenderSprite, MVPL_GETDATA_P->pos);
		sfSprite_setTextureRect(mvpl_RenderSprite, mvpl_IsMoving(MVPL_GETDATA_P) ? IntRect(64, 128, 64, 64) : IntRect(0, 128, 64, 64));
		sfRenderTexture_drawSprite(_window->rdrt, mvpl_RenderSprite, NULL);
	}
}

void mvpl_Deinit() {
	sfSprite_destroy(mvpl_RenderSprite);
}

sfBool mvpl_IsMoving(mvplData* _mvpl) {
	return getSqrMagnitudef(_mvpl->spd) > .1f;
}

sfVector2i mvpl_SnapPos(sfVector2f _pos) {
	return vector2i((int)_pos.x / (int)TILE_SIZE, (int)_pos.y / (int)TILE_SIZE);
}

mvplData* mvpl_GetClosestPlatform(sfVector2f _pos)
{
	if (!mvpl_ListD)
		return NULL;

	float lastDistance = LLONG_MAX;
	int idMvpl = 0;

	for (int i = 0; i < mvpl_ListD->size(mvpl_ListD); i++)
	{
		float dist = getSqrMagnitudeBetween2Vectorsf(_pos, MVPL_GETDATA_P->pos);
		if (dist < lastDistance)
		{
			lastDistance = dist;
			idMvpl = i;
		}
	}
	return STD_LIST_GETDATA(mvpl_ListD, mvplData, idMvpl);
}

sfFloatRect mvpl_GetBounds(mvplData* _mvpl)
{
	if (_mvpl == NULL)
		return (sfFloatRect) { 0 };
	return FlRect(_mvpl->pos.x, _mvpl->pos.y, TILE_SIZE, TILE_SIZE);
}

void mvpl_Attract(mvplData* _mvpl) {
	sfVector2f plrPos = plr_GetFeetPos();
	sfVector2f vecMov = substractVectorsrf(plrPos, _mvpl->pos);

	for (int j = 0; j < _mvpl->group->list->size(_mvpl->group->list); j++) {
		MVPL_GETDATA_L(_mvpl->group)->data->spd = multiplyVectorsrf(normalizef(vecMov), MVPL_SPD);
	}
}

void mvpl_Repulse(mvplData* _mvpl) {
	sfVector2f plrPos = plr_GetFeetPos();
	sfVector2f vecMov = substractVectorsrf(plrPos, _mvpl->pos);

	for (int j = 0; j < _mvpl->group->list->size(_mvpl->group->list); j++) {
		MVPL_GETDATA_L(_mvpl->group)->data->spd = multiplyVectorsrf(normalizef(vecMov), -MVPL_SPD);
	}
}

void mvpl_Stasis(mvplData* _mvpl) {
	for (int j = 0; j < _mvpl->group->list->size(_mvpl->group->list); j++) {
		MVPL_GETDATA_L(_mvpl->group)->data->spd = NULLVECTF;
	}
}

void mvpl_Destroy(mvplData* _mvpl) {
	return;
}

sfVector2f* mvpl_listToSave()
{
	int len = mvpl_ListD->size(mvpl_ListD);
	sfVector2f* tmp = calloc(len, sizeof(sfVector2f));
	if (!tmp) return NULL;

	for (int i = 0; i < len; i++)
	{
		tmp[i] = STD_LIST_GETDATA(mvpl_ListD, mvplData, i)->pos;
	}
	return tmp;
}

void mvpl_clearList()
{
	if (!mvpl_ListD) return;
	mvpl_ListD->clear(&mvpl_ListD);

	if (!mvpl_ListG) return;
	mvpl_ListG->clear(&mvpl_ListG);
}

sfBool mvpl_IsPlayerWalkingOnPlatform() { return mvpl_WalkingOnPlatform; }