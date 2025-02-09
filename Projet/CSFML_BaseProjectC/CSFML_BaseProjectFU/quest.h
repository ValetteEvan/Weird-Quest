/// Quest system, handling both storage and rendering.
/// Function prefix: qst_

#pragma once
#include "tools.h"
#include "windowManager.h"
#include "List.h"
#include "player.h"
#include "pause.h"

//typedef enum {
//	QST_TYPE_MAIN = 0,
//	QST_TYPE_SECONDARY = 1,
//} QstType;

typedef enum {
	QST_STATE_UNDISCOVERED = 0,
	QST_STATE_PROGRESS = 1,
	QST_STATE_FINISH,
	QST_STATE_ARCHIVED
} QstState;

typedef enum {
	QST_ID_PRIMARY = 0,
	QST_ID_SECONDARY = 1 << 8,

	QST_ID_DUMMY = QST_ID_PRIMARY + 0,

	QST_ID_P_EXPLORE = QST_ID_PRIMARY + 1,
	QST_ID_P_JOY = QST_ID_PRIMARY + 2,

	QST_ID_S_KILL_SNAKES = QST_ID_SECONDARY + 1,
	QST_ID_S_FISH_JOY = QST_ID_SECONDARY + 2,
	QST_ID_S_SEE_SHAMAN = QST_ID_SECONDARY + 3,
	QST_ID_S_GET_YOYO = QST_ID_SECONDARY + 4,
} QstId;

/// Data storage for individual quests
typedef struct {
	QstState state; /// Quest status
	QstId id; /// Quest's numerical ID
	char* title; /// Quest title
	char* message; /// Quest description
	quetePNJ pnj; /// Questgiver
	sfVector2f posTitle;
	sfVector2f posMessage;
	int ctrProgress; /// Used for tallying kills/captures needed in order to progress the quest
	int maxProgress; /// Number of things to kill/capture for completing the quest, set to 0 for quests that don't need that
	sfVector2f posDetection;
} Quest;
stdList* qst_List;

typedef struct {
	Quest* quest;
}QuestDisp;
stdList* qst_ListDisp[3];

QstId quest_CurrentlyRendered; /// Which quest is currently being rendered

/// Adds a new quest to the quest list.
/// \param _id - Numerical ID of the quest to be given
/// \param _type - Whether the quest is a primary or secondary one
/// \param _pnj - Questgiver
void qst_Create(QstId _id, quetePNJ _pnj);

/// Initializes the quest handler.
void qst_Init();

/// Updates quests and checks whether they're complete.
void qst_Update(Window* _window);

/// Updates the quest screen.
void qst_UpdateScreen(Window* _window);

/// Renders the quest screen.
void qst_Display(Window* _window);

/// Deinits the quest screen and frees up allocated memory.
void qst_Deinit();

/// Sets the given quest to a certain status.
void qst_SetState(QstId _id, QstState _state);

/// \return Status of the given quest
QstState qst_GetState(QstId _id);

/// Fills in the given quest's text fields based on its internal ID.
/// This function should be called on all quests upon changing languages, as freeing up memory allocated for storing the JSON file's data may break the contents of the quests.
void qst_FillSpecificData(Quest* _qst);

void qst_SelectPrev();
void qst_SelectNext();

int qst_getNumberofQuestsOfPNJ(int _pnj);
/// \return Whether the given quest has been assigned to the player already
sfBool qst_IsDiscovered(QstId _id);

/// \return Whether the given quest has been assigned to the player but not completed just yet
sfBool qst_IsOngoing(QstId _id);

/// \return Whether the given quest has been completed
sfBool qst_IsComplete(QstId _id);

/// Adds one to the progress count of the given quest, but only if it hasn't been completed.
void qst_IncrementCount(QstId _id);

/// \return Progress count of the given quest
int qst_GetCount(QstId _id);

void qst_Archive(QstId _id);

/// Instantly gives all quests to the player.
/// \deprecated DEBUG ONLY.
void qst_GetAll();

/// \return Position the pig totem hood should point to for the given quest
sfVector2f qst_GetHoodTarget(QstId _id);

void qst_setIndex(int _setter);

void qst_NextCategory();
void qst_PrevCategory();
quetePNJ qst_GetCategory();