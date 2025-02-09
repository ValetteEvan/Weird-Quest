#include "quest.h"
#include "fontManager.h"
#include "math.h"
#include "langManager.h"
#include "pause.h"
#include "map.h"

#define QST_GETDATA STD_LIST_GETDATA(qst_List, Quest, i)
#define QST_GETDATA_BY_INDEX STD_LIST_GETDATA(qst_List, Quest, qst_Index)

sfRectangleShape* qst_RenderRect;
sfText* qst_RenderText; 
int qst_Index;
sfBool qst_ToggleLog;
sfVector2f qst_PosSelect;
QstId qst_CurrentlyRendered;

stdList** qst_CategorySel;
char qst_CategorySelId;

void qst_Create(QstId _id, quetePNJ _pnj)
{
	/// Checks if the requested quest already exists before trying to create it
	for (int i = 0; i < qst_List->size(qst_List); i++) {
		if (QST_GETDATA->id == _id) return;
	}

	Quest quest = { 0 };
	quest.id = _id;
	quest.pnj = _pnj;
	quest.state = QST_STATE_PROGRESS;
	quest.posTitle = vector2f(300.f, 600.f);
	quest.posMessage = vector2f(1200.f, 350.f);
	quest.ctrProgress = 0;
	qst_FillSpecificData(&quest);
	qst_List->push_back(&qst_List, &quest);

	Quest* data = STD_LIST_GETDATA(qst_List, Quest, qst_List->size(qst_List) - 1);
	QuestDisp dist = { data };
	qst_ListDisp[_pnj]->push_back(&qst_ListDisp[_pnj], &dist);
}

void qst_Init() {
	static sfBool onePass = sfTrue;
	if (onePass) {
		qst_RenderRect = sfRectangleShape_create();
		qst_RenderText = sfText_create();
		sfText_setFont(qst_RenderText, GetFont("Dico"));
		qst_Index = 0;
		qst_ToggleLog = sfFalse;
		qst_PosSelect = vector2f(490.f, 115.f);

		qst_List = STD_LIST_CREATE(Quest, 0);
		for (int i = 0; i < 3; i++) qst_ListDisp[i] = STD_LIST_CREATE(QuestDisp, 0);
		qst_CategorySelId = 0;
		qst_CategorySel = qst_ListDisp[qst_CategorySelId];
		//qst_GetAll();
	}
}

void qst_Update(Window* _window) {
	for (int i = 0; i < qst_List->size(qst_List); i++) {
		Quest* tmp = QST_GETDATA;
		if (tmp->maxProgress && tmp->ctrProgress >= tmp->maxProgress && tmp->state == QST_STATE_PROGRESS) tmp->state = QST_STATE_FINISH;

		if (qst_IsOngoing(tmp->id)) plr_SetHoodTargetPos(qst_GetHoodTarget(tmp->id));
	}
	qst_CategorySel = &qst_ListDisp[qst_CategorySelId];
}

void qst_UpdateScreen(Window* _window)
{
	static float timerInput = 0.f;
	timerInput += getDeltaTime();

	qst_PosSelect = vector2f(490.f, 115.f + 50.f * qst_Index);

	/// Navigating upwards in the quest log
	if (customKeyboardIsKeyPressed(sfKeyZ, _window->renderWindow) && timerInput > 0.2f)
	{
		qst_SelectPrev();
		timerInput = 0.f;
		// printf("%d\n", qst_Index);
	}
	/// Navigating downwards in the quest log
	if (customKeyboardIsKeyPressed(sfKeyS, _window->renderWindow) && timerInput > 0.2f)
	{
		qst_SelectNext();
		timerInput = 0.f;
		// printf("%d\n", qst_Index);
	}
}

void qst_Display(Window* _window)
{
	int nbOfQuestsByPNJ = 0;
	for (int i = 0; i < (*qst_CategorySel)->size(*qst_CategorySel); i++)
	{
		Quest* tmp = STD_LIST_GETDATA((*qst_CategorySel), QuestDisp, i)->quest;
		sfText_setColor(qst_RenderText, sfWhite);
		sfText_setString(qst_RenderText, tmp->title);
		sfText_setPosition(qst_RenderText, addVectorsrf(tmp->posTitle, vector2f(0.f, 100.f * nbOfQuestsByPNJ)));
		sfRenderTexture_drawText(_window->rdrt, qst_RenderText, NULL);

		if (tmp->id == qst_CurrentlyRendered)
		{

			sfText_setColor(qst_RenderText, sfWhite);
			sfText_setOutlineThickness(qst_RenderText, 2.5f);
			sfText_setOutlineColor(qst_RenderText, color(155, 102, 70, 255));

			sfText_setString(qst_RenderText, tmp->title);
			sfText_setPosition(qst_RenderText, addVectorsrf(tmp->posTitle, vector2f(0.f, 100.f * nbOfQuestsByPNJ)));
			sfRenderTexture_drawText(_window->rdrt, qst_RenderText, NULL);

			sfText_setString(qst_RenderText, tmp->title);
			sfText_setPosition(qst_RenderText, vector2f(1200.f, 250.f));
			sfRenderTexture_drawText(_window->rdrt, qst_RenderText, NULL);

			sfText_setOutlineThickness(qst_RenderText, 0.f);
			sfText_setColor(qst_RenderText, sfWhite);
			sfText_setString(qst_RenderText, tmp->message);
			sfText_setPosition(qst_RenderText, tmp->posMessage);
			sfRenderTexture_drawText(_window->rdrt, qst_RenderText, NULL);

		}
		nbOfQuestsByPNJ++;
	}
}

void qst_Deinit() {
	sfRectangleShape_destroy(qst_RenderRect);
	sfText_destroy(qst_RenderText);
}

void qst_SetState(QstId _id, QstState _state)
{
	for (int i = 0; i < qst_List->size(qst_List); i++)
	{
		if (QST_GETDATA->id == _id)
		{
			QST_GETDATA->state = _state;
			return;
		}
	}
}

QstState qst_GetState(QstId _id)
{
	for (int i = 0; i < qst_List->size(qst_List); i++) {
		if (QST_GETDATA->id == _id) return QST_GETDATA->state;
	}
	return QST_STATE_UNDISCOVERED;
}

void qst_FillSpecificData(Quest* _qst) {
	switch (_qst->id) {
	case QST_ID_P_EXPLORE:
		_qst->title = lang_GetFromKey("quest.p_explore.title");
		_qst->message = lang_GetFromKey("quest.p_explore.desc");
		_qst->maxProgress = 0;
		break;
	case QST_ID_P_JOY:
		_qst->title = lang_GetFromKey("quest.p_joy.title");
		_qst->message = lang_GetFromKey("quest.p_joy.desc");
		_qst->maxProgress = 0;
		break;

	case QST_ID_S_KILL_SNAKES:
		_qst->title = lang_GetFromKey("quest.s_kill_snakes.title");
		_qst->message = lang_GetFromKey("quest.s_kill_snakes.desc");
		_qst->maxProgress = 5;
		break;

	case QST_ID_S_SEE_SHAMAN:
		_qst->title = lang_GetFromKey("quest.s_see_shaman.title");
		_qst->message = lang_GetFromKey("quest.s_see_shaman.desc");
		_qst->maxProgress = 0;
		break;

	case QST_ID_S_GET_YOYO:
		_qst->title = lang_GetFromKey("quest.s_get_yoyo.title");
		_qst->message = lang_GetFromKey("quest.s_get_yoyo.desc");
		_qst->maxProgress = 3;
		break;

	case QST_ID_S_FISH_JOY:
		_qst->title = lang_GetFromKey("quest.s_fish_joy.title");
		_qst->message = lang_GetFromKey("quest.s_fish_joy.desc");
		_qst->maxProgress = 3;
		break;

	case 100:
		_qst->title = lang_GetFromKey("quest.catch.title");
		_qst->message = lang_GetFromKey("quest.catch.desc");
		break;
	case 121:
		_qst->title = lang_GetFromKey("quest.temple1.title");
		_qst->message = lang_GetFromKey("quest.temple1.desc");
		break;
	case 135:
		_qst->title = lang_GetFromKey("quest.temple2.title");
		_qst->message = lang_GetFromKey("quest.temple2.desc");
		break;
	default:
		_qst->id = 0;
		break;
	}
}

void qst_SelectPrev() {
	if (!STD_LIST_GETDATA((*qst_CategorySel), Quest, 0)) return;
	qst_Index--;
	if (qst_Index < 0) qst_Index = (*qst_CategorySel)->size(*qst_CategorySel) - 1;
	if ((*qst_CategorySel)->size(*qst_CategorySel)) qst_CurrentlyRendered = STD_LIST_GETDATA((*qst_CategorySel), QuestDisp, qst_Index)->quest->id;
}

void qst_SelectNext() {
	if (!STD_LIST_GETDATA((*qst_CategorySel), Quest, 0)) return;
	qst_Index++;
	if (qst_Index > (*qst_CategorySel)->size(*qst_CategorySel) - 1) qst_Index = 0;
	if ((*qst_CategorySel)->size(*qst_CategorySel)) qst_CurrentlyRendered = STD_LIST_GETDATA((*qst_CategorySel), QuestDisp, qst_Index)->quest->id;
}

int qst_getNumberofQuestsOfPNJ(int _pnj)
{
	int nbQuest = 0;
	for (int i = 0; i < qst_List->size(qst_List); i++)
	{
		if (QST_GETDATA->pnj == _pnj) nbQuest++;
	}
	return nbQuest;
}

sfBool qst_IsDiscovered(QstId _id) { return qst_GetState(_id) != QST_STATE_UNDISCOVERED; }
sfBool qst_IsOngoing(QstId _id) { return qst_GetState(_id) == QST_STATE_PROGRESS; }
sfBool qst_IsComplete(QstId _id) { return qst_GetState(_id) >= QST_STATE_FINISH; }

void qst_IncrementCount(QstId _id) {
	if (!qst_IsOngoing(_id)) return;
	for (int i = 0; i < qst_List->size(qst_List); i++) {
		Quest* tmp = QST_GETDATA;
		if (tmp->id == _id) {
			tmp->ctrProgress++;
			return;
		}
	}
}

int qst_GetCount(QstId _id) {
	if (!qst_IsOngoing(_id)) return -1;
	for (int i = 0; i < qst_List->size(qst_List); i++) {
		Quest* tmp = QST_GETDATA;
		if (tmp->id == _id) return tmp->ctrProgress;
	}
	return -1;
}

void qst_Archive(QstId _id) {
	qst_SetState(_id, QST_STATE_ARCHIVED);
}

void qst_GetAll() {
	if (!DEBUG_STATE) return;

	qst_Create(QST_ID_P_EXPLORE, PAUSE_MENTOR);
	qst_Create(QST_ID_P_JOY, PAUSE_MENTOR);
	qst_Create(QST_ID_S_KILL_SNAKES, PAUSE_ARNOLD);
	qst_Create(QST_ID_S_FISH_JOY, PAUSE_ARNOLD);
	qst_Create(QST_ID_S_SEE_SHAMAN, PAUSE_SHAMAN);
	qst_Create(QST_ID_S_GET_YOYO, PAUSE_SHAMAN);
}

sfVector2f qst_GetHoodTarget(QstId _id) {
	/// Replace those NULLVECTF's by actual world coords corresponding to the requested stuff
	switch (_id) {
		case QST_ID_P_EXPLORE: return NULLVECTF; /// Bogus

		case QST_ID_P_JOY:
			switch (map_GetState()) {
				case MAP_STATE_OVERWORLD: return vector2f(6650.f, 11070.f); /// Temple of Joy
				case MAP_STATE_FOREST: return vector2f(3060.f, 5400.f); /// Forest exit
				case MAP_STATE_DUNGEON_JOY_1: return NULLVECTF;
				default: return NULLVECTF; /// Bogus
			}

		case QST_ID_S_KILL_SNAKES:
		case QST_ID_S_GET_YOYO:
			switch (map_GetState()) {
				case MAP_STATE_OVERWORLD: return vector2f(2540.f, 100.f); /// Forest entrance
				case MAP_STATE_FOREST: return vector2f(1315.f, 800.f); /// The collector
				case MAP_STATE_DUNGEON_JOY_1: return NULLVECTF;
				default: return NULLVECTF; /// Bogus
			}

		case QST_ID_S_SEE_SHAMAN:		
			switch (map_GetState()) {
				case MAP_STATE_OVERWORLD: return vector2f(7800.f, 3330.f); /// Mountain entrance
				case MAP_STATE_FOREST: return NULLVECTF;
				case MAP_STATE_DUNGEON_JOY_1: return NULLVECTF;
				default: return NULLVECTF; /// Bogus
			}

		case QST_ID_S_FISH_JOY:
			switch (map_GetState()) {
				case MAP_STATE_OVERWORLD:
					switch (qst_GetCount(QST_ID_S_FISH_JOY)) {
						case 0: return vector2f(2360.f, 6400.f);
						case 1: return vector2f(9070.f, 9890.f);
						case 2: return vector2f(3150.f, 4680.f);
						default: return NULLVECTF; /// Bogus
					}

				case MAP_STATE_FOREST: return vector2f(3060.f, 5400.f); /// Forest exit
				case MAP_STATE_DUNGEON_JOY_1: return NULLVECTF;
				default: return NULLVECTF; /// Bogus
			}

		default: return NULLVECTF; /// Bogus
	}
}

void qst_setIndex(int _setter) { qst_Index = _setter; }

void qst_NextCategory() {
	qst_CategorySelId = (++qst_CategorySelId) % 3;
	qst_Index = 0;
	if ((*qst_CategorySel)->size(*qst_CategorySel)) qst_CurrentlyRendered = STD_LIST_GETDATA((*qst_CategorySel), QuestDisp, qst_Index)->quest->id;
}

void qst_PrevCategory() {
	qst_CategorySelId = (qst_CategorySelId + 2) % 3;
	qst_Index = 0;
	if ((*qst_CategorySel)->size(*qst_CategorySel)) qst_CurrentlyRendered = STD_LIST_GETDATA((*qst_CategorySel), QuestDisp, qst_Index)->quest->id;
}

quetePNJ qst_GetCategory() { return qst_CategorySelId; }