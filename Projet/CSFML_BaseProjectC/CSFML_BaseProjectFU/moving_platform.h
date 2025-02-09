/// Function prefix: mvpl_
#pragma once
#include "tools.h"
#include "List.h"
#include "windowManager.h"

#define MVPL_GETDATA_P STD_LIST_GETDATA(mvpl_ListD, mvplData, i) /// Shortcut for fetching LL data for the platform list
#define MVPL_GETDATA_G STD_LIST_GETDATA(mvpl_ListG, mvplGroup, i) /// Shortcut for fetching LL data for the group list
#define MVPL_GETDATA_L(x) STD_LIST_GETDATA(x->list, mvplList, j) /// Shortcut for fetching LL data the given group's platform list

typedef struct mvplData mvplData; /// Data storage for individual moving platforms
typedef struct mvplGroup mvplGroup; /// Data storage for groups of moving platforms. Each node contains a sub-list for platforms contained within that group.
typedef struct mvplList mvplList; /// Data storage for sub-lists of moving platforms contained within mvplGroupList

/// Data storage for invidiaul moving platforms
struct mvplData {
	sfVector2f pos; /// Position of the platform (analog)
	sfVector2i posSnap; /// Position of the platform (snapped onto the tile grid)
	sfVector2f spd; /// Movement speed of the platform
	float mass; /// Mass of the platform (used for computing forces)
	mvplGroup* group; /// Which platform group this one is part of
};
stdList* mvpl_ListD; /// Individual platforms list

/// Data storage for moving platform groups.
/// Each of those nodes contains itself a LL containing pointers to all platforms that are part of a given platform cluster.
struct mvplGroup {
	stdList* list; /// A stdList containing mvplList elements, each holding pointers towards all the platforms contained in this group
};
stdList* mvpl_ListG; /// Platform groups list

/// Data storage for individual members of moving platform groups
struct mvplList {
	mvplData* data; /// Pointer to a mvplData object, standing for a platform which is part of the given group
};

/// Initializes moving platforms.
void mvpl_Init();

/// Creates a new moving platform at the given position.
mvplData* mvpl_CreatePlatform(sfVector2f _pos);

/// Creates a group of moving platforms.
mvplGroup* mvpl_CreateGroup();

/// Adds a moving platform to a group.
/// \param _list Group to which to add the platform
/// \param _mvpl - Platform to add to the group
mvplList* mvpl_AddToGroup(mvplGroup* _list, mvplData* _mvpl);

/// Scans the entire map for moving platforms and gathers them in groups.
/// A group is constituted of a set of moving platforms which all spawn adjacent to each other.
/// Groups are treated as integral, rigid entities, which all move together.
/// Two groups colliding should not merge.
void mvpl_ConstituteGroups();

/// Updates moving platforms.
void mvpl_Update();

/// Renders moving platforms.
void mvpl_Display(Window* _window);

/// Deinitializes moving platforms and frees up allocated memory.
void mvpl_Deinit();

/// \return Whether the moving platform passed as argument is currently moving
sfBool mvpl_IsMoving(mvplData* _mvpl);

/// \return A grid position in tiles corresponding to the passed pixel coordinates
sfVector2i mvpl_SnapPos(sfVector2f _pos);

/// \return The moving platform closest to the passed pixel coordinates
mvplData* mvpl_GetClosestPlatform(sfVector2f _pos);

/// \return The axis-aligned bounding box of the passed moving platform
sfFloatRect mvpl_GetBounds(mvplData* _mvpl);

void mvpl_Attract(mvplData* _mvpl);
void mvpl_Repulse(mvplData* _mvpl);
void mvpl_Stasis(mvplData* _mvpl);
void mvpl_Destroy(mvplData* _mvpl);

sfVector2f* mvpl_listToSave();
void mvpl_clearList();

sfBool mvpl_IsPlayerWalkingOnPlatform();