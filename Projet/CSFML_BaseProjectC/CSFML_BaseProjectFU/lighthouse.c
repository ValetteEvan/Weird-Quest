#include "lighthouse.h"
#include "player.h"
#include "pot.h"
#include "controller.h"
#include "dialogBox.h"
#include "inventory.h"
#include "pause.h"
#include "shaderManager.h"
#include "fadeManager.h"
#include "viewManager.h"
#include "fadeManager.h"
#include "quest.h"
#include "soundManager.h"
#include "particlesManager.h"

#define LOCLH_DRAW (sfRenderTexture_drawSprite(_window->rdrt, loclh_RSprite, NULL))

sfSprite* loclh_RSprite;
sfBool loclh_InputFlag;
sfTexture* loclh_Texture;

float loclh_BedroomBedPos;
sfBool loclh_BedroomFoundNote;
sfBool loclh_BedroomExitBlocked;
float loclh_BedroomReadNoteCooldown;
sfBool loclh_BedroomFoundStaff;
float loclh_BedroomTimerAnimChest;

sfBool loclh_HallFoundNote;
float loclh_HallReadNoteCooldown;
char loclh_HallRiddleProgress;
sfVector2f loclh_HallTablePos;
sfVector2f loclh_HallTableSpd;
sfBool loclh_HallTableReturning;

sfBool loclh_WaterfallPrompt;
char loclh_WaterfallPromptSelect;
float loclh_WaterfallPromptCooldown;
sfText* loclh_PromptText;
sfRectangleShape* loclh_PromptRect;

sfRectangleShape* lolch_Antechamber;
int FrameXRight;
int FrameXRightRepeat;
int FrameXLeft;
int FrameXLeftRepeat;
sfBool IsTorchOnLeft;
sfBool IsTorchOnRight;
sfBool prtTable;

sfBool loclh_RoomChange = sfFalse;
MapState loclh_NextRoom = 0;
sfVector2f loclh_NextPos;

struct {
	float candelabra;
	float flowers;
	float lighthouse;
	float waterfall;
	float clouds_osc;
	float candle_flame;
} loclh_AnimTimers = { 0 };

int loclh_FrameX;
float loclh_FrameAnim;
float loclh_FrameAnimCooldownRight;
float loclh_TimerParticles;
float loclh_FrameAnimCooldownLeft;
float loclh_TimerDebug;
float loclh_Timerprt;
sfBool loclh_IsFirstTorchOn;
sfBool loclh_IsSecondTorchOn;
sfBool loclh_PlaySound;

void loclh_Init(Window* _window) {
	static sfBool onePass = sfTrue;
	if (onePass) {
		onePass = sfFalse;

		loclh_Jumped = sfFalse;
		loclh_RSprite = sfSprite_create();
		loclh_BedroomBedPos = 790.f;
		loclh_BedroomFoundNote = sfFalse;
		loclh_BedroomExitBlocked = sfTrue;
		loclh_BedroomReadNoteCooldown = 0.f;
		loclh_BedroomFoundStaff = sfFalse;
		loclh_BedroomTimerAnimChest = 0.f;
		loclh_Timerprt = 0.f;

		loclh_HallFoundNote = sfFalse;
		loclh_HallReadNoteCooldown = 0.f;
		loclh_TimerParticles = 0.f;
		/// 4 to skip the puzzle 
		loclh_HallRiddleProgress = 0;

		loclh_Texture = GetSfTexture("DoorOppacity");
		loclh_FrameX = 0;
		loclh_FrameAnim = 0.f;
		loclh_IsFirstTorchOn = sfFalse;
		loclh_IsSecondTorchOn = sfFalse;
		IsTorchOnLeft = sfFalse;
		IsTorchOnRight = sfFalse;
		loclh_PlaySound = sfFalse;

		loclh_AnimTimers.candelabra = 0.f;
		loclh_HallTablePos = vector2f(854.f, 564.f);
		loclh_HallTableSpd = NULLVECTF;
		loclh_HallTableReturning = sfFalse;

		loclh_PromptText = sfText_create();
		loclh_PromptRect = sfRectangleShape_create();
		lolch_Antechamber = sfRectangleShape_create();
		sfRectangleShape_setTexture(lolch_Antechamber, sfTexture_createFromFile("../Ressources/Textures/antechamber/spritesheet.png", NULL), sfTrue);

		sfText_setFont(loclh_PromptText, GetFont("UGp"));
		sfText_setColor(loclh_PromptText, sfWhite);
		sfText_setCharacterSize(loclh_PromptText, 20);

		sfRectangleShape_setFillColor(loclh_PromptRect, sfBlack);
		sfRectangleShape_setOutlineColor(loclh_PromptRect, sfWhite);
		sfRectangleShape_setOutlineThickness(loclh_PromptRect, 3.f);

		FrameXRightRepeat = 6;
		FrameXRight = 0;
		FrameXLeftRepeat = 6;
		FrameXLeft = 0;
		plr_SetPos(vector2f(1250.f, 576.f));
		stopMusicIfPlaying("Music_Main");
		stopMusicIfPlaying("Music_Menu");
		playMusicIfNotPlaying("Music_Phare");
	}
}

void loclh_Update(Window* _window) {
	if (getState() != GAME) return;
	if (fade_GetOpacity() != 0) plr_StopMoving();

	switch (map_GetState()) {
	case MAP_STATE_LIGHTHOUSE_BEDROOM: loclh_UpdateBedroom(_window); break;
	case MAP_STATE_LIGHTHOUSE_HALL: loclh_UpdateHall(_window); break;
	case MAP_STATE_LIGHTHOUSE_OUTSIDE: loclh_UpdateOut(_window); break;
	case MAP_STATE_LIGHTHOUSE_MENTOR: loclh_UpdateMentor(_window); break;
	case MAP_STATE_LIGHTHOUSE_ANTECHAMBER:loclh_UpdateAntechamber(_window); break;
	case MAP_STATE_LIGHTHOUSE_ROOF: break; // Gustavo: (null)
	}

	if (loclh_RoomChange) {
		if (loclh_NextRoom != map_GetState()) {
			fadeOut(0.5f);
			if (fade_GetOpacity() == 255) {
				map_changeMapState(_window, loclh_NextRoom);
				plr_SetPos(loclh_NextPos);
			}
		}
		else {
			fadeIn(0.5f);
			if (fade_GetOpacity() == 0) loclh_RoomChange = sfFalse;
		}
	}
	MapState currentMapState = map_GetState();
	if (currentMapState < MAP_STATE_DUNGEON_JOY_1)
		setViewPosition(mainView, vector2f(960.f, 540.f));
	fade_SetPosition(vector2f(960.f, 540.f));
}

void loclh_DisplayBG(Window* _window) {
	switch (map_GetState()) {
	case MAP_STATE_LIGHTHOUSE_BEDROOM: loclh_DisplayBedroomBG(_window); break;
	case MAP_STATE_LIGHTHOUSE_HALL: loclh_DisplayHallBG(_window); break;
	case MAP_STATE_LIGHTHOUSE_OUTSIDE: loclh_DisplayOutBG(_window); break;
	case MAP_STATE_LIGHTHOUSE_MENTOR: loclh_DisplayMentorBG(_window); break;
	case MAP_STATE_LIGHTHOUSE_ANTECHAMBER: loclh_DisplayAntechamber(_window); break;
	case MAP_STATE_LIGHTHOUSE_ROOF: break;
	default: break;
	}
}

void loclh_DisplayFG(Window* _window) {
	switch (map_GetState()) {
	case MAP_STATE_LIGHTHOUSE_BEDROOM: loclh_DisplayBedroomFG(_window); break;
	case MAP_STATE_LIGHTHOUSE_HALL: loclh_DisplayHallFG(_window); break;
	case MAP_STATE_LIGHTHOUSE_OUTSIDE: loclh_DisplayOutFG(_window); break;
	case MAP_STATE_LIGHTHOUSE_MENTOR: loclh_DisplayMentorFG(_window); break;
	case MAP_STATE_LIGHTHOUSE_ANTECHAMBER: break;
	case MAP_STATE_LIGHTHOUSE_ROOF: break;
	default: break;
	}
	fade_Display(_window);
}

void loclh_Deinit() {
	if (getState() != GAME) return;

	//	sfSprite_destroy(loclh_RSprite);
}

void lhloc_Begin(Window* _window) {
	map_changeMapState(_window, MAP_STATE_LIGHTHOUSE_BEDROOM); /// CHANGE THIS BACK TO [[MAP_STATE_LIGHTHOUSE_BEDROOM]] FOR THE DEMO VERSION

	/////////////////////////////////////////////////////////////////////////////////////////
	/*pot_Create(vector2f(592.f, 718.f), sfFalse, POT_REG);
	pot_Create(vector2f(592.f + 64.f, 718.f), sfFalse, POT_REG);
	pot_Create(vector2f(592.f, 718.f + 64.f), sfFalse, POT_REG);
	pot_Create(vector2f(592.f + 64.f, 718.f + 64.f), sfFalse, POT_REG);*/
//	plr_SetPos(vector2f(964.f, 560.f));
}


void loclh_UpdateBedroom(Window* _window) {
	loclh_TimerParticles += getDeltaTime();
	/// Computing trapdoor updates (checking if there are still pots on top of it, as well as testing if player is interacting with it)
	sfFloatRect trapdoorHitbox = FlRect(588.f, 730.f, 130.f, 118.f);
	sfFloatRect trapdoorHitboxOff = FlRect(588.f - TILE_SIZE, 716.f - TILE_SIZE, 152.f, 118.f);
	loclh_BedroomExitBlocked = sfFalse;
	for (int i = 0; i < pot_List->size(pot_List); i++) {
		PotData* tmp = STD_LIST_GETDATA(pot_List, PotData, i);
		if (sfFloatRect_contains(&trapdoorHitboxOff, tmp->pos.x, tmp->pos.y)) {
			loclh_BedroomExitBlocked = sfTrue;
			break;
		}
	}	
	if (loclh_TimerParticles > 0.3f)
	{
		prt_CreateFireParticles(vector2f(915.f, 230.f));
		prt_CreateFireParticles(vector2f(913.f, 295.f));

		prt_CreateFireParticles(vector2f(1015.f, 240.f));
		prt_CreateFireParticles(vector2f(980.f, 310.f));
		prt_CreateFireParticles(vector2f(735.f, 434.f));
		loclh_TimerParticles = 0.f;
	}

	/// Computing collisions with the bed (X-axis)
	sfVector2f plrPos = addVectorsrf(plr_GetPos(), multiplyVectorsrf(vector2f(plr_GetVel().x, 0.f), getDeltaTime()));
	sfFloatRect plrHitbox = FlRect(plrPos.x - PLR_X_SIZE * .5f, plrPos.y - PLR_Y_SIZE * .5f, PLR_X_SIZE, PLR_X_SIZE);
	sfFloatRect bedHitbox = FlRect(loclh_BedroomBedPos, 442.f, 128.f, 210.f);
	if (sfFloatRect_intersects(&plrHitbox, &bedHitbox, NULL)) {
		plr_ForceMove(vector2f(plr_GetVel().x * -getDeltaTime(), 0.f));
		if (plr_GetForward().x > 0 && plrPos.x <= bedHitbox.left) {
			if (loclh_BedroomBedPos <= 894.f && loclh_BedroomFoundNote) loclh_BedroomBedPos += 25.f * getDeltaTime();
		}
	}

	/// Computing collisions with the bed (Y-axis)
	plrPos = addVectorsrf(plr_GetPos(), multiplyVectorsrf(vector2f(0.f, plr_GetVel().y), getDeltaTime()));
	plrHitbox = FlRect(plrPos.x - PLR_X_SIZE * .5f, plrPos.y - PLR_Y_SIZE * .5f, PLR_X_SIZE, PLR_X_SIZE);
	if (sfFloatRect_intersects(&plrHitbox, &bedHitbox, NULL)) {
		plr_ForceMove(vector2f(0.f, plr_GetVel().y * -getDeltaTime()));
	}


	sfFloatRect ladderHitbox = FlRect(1150.f, 500.f, 100.f, 100.f);
	if (sfFloatRect_intersects(&ladderHitbox, &plrHitbox, NULL)) {
		plr_toggleFX(sfTrue);
	}

	if (sfFloatRect_intersects(&trapdoorHitbox, &plrHitbox, NULL)) {
		plr_toggleFX(sfTrue); 
	}

	if (!loclh_BedroomFoundStaff && loclh_BedroomBedPos >= 894.f && getSqrMagnitudeBetween2Vectorsf(plr_GetPos(), vector2f(856.f, 560.f)) < 10000.f) {
		plr_toggleFX(sfTrue);
	}

	/// Computing interactions with the world
	if (Gamepad_isButtonPressed(0, A, _window->renderWindow)) {
		if (!loclh_InputFlag) {
			/// Interacting with the ladder to enter the mentor's bedroom
			sfFloatRect ladderHitbox = FlRect(1150.f, 500.f, 100.f, 100.f);
			if (sfFloatRect_intersects(&ladderHitbox, &plrHitbox, NULL)) {
				loclh_RoomChange = sfTrue;
				map_SaveMap();
				loclh_NextRoom = MAP_STATE_LIGHTHOUSE_MENTOR;
				playSoundFX("SFX_LightHouse_Ladder");
				loclh_NextPos = vector2f(670.f, 790.f);
			}

			/// Interacting with the trapdoor, but only if there are no pots on top of it
			if (!loclh_BedroomExitBlocked) {
				if (sfFloatRect_intersects(&trapdoorHitbox, &plrHitbox, NULL)) {
					playSoundFX("SFX_LightHouse_Trap");
					map_SaveMap();
					loclh_RoomChange = sfTrue;
					loclh_NextRoom = MAP_STATE_LIGHTHOUSE_HALL;
					loclh_NextPos = vector2f(1160.f, 580.f);
				}
			}

			/// Opening the chest, but only if the bed has been pushed aside
			if (!loclh_BedroomFoundStaff && loclh_BedroomBedPos >= 894.f && getSqrMagnitudeBetween2Vectorsf(plr_GetPos(), vector2f(856.f, 560.f)) < 10000.f) {
				loclh_BedroomFoundStaff = sfTrue;
				plr_SetUnlockState(PLRUNL_STAFF);
				db_Queue("", lang_GetFromKey("game.lighthouse.found_staff_1"), vector2f(856.f, 560.f));
				db_Queue("", lang_GetFromKey("game.lighthouse.found_staff_2"), vector2f(856.f, 560.f));
				db_Queue("", lang_GetFromKey("game.lighthouse.found_staff_3"), vector2f(856.f, 560.f));

				/// How do I add Fjall to inventory - Þorn, 2024
				pause_itemRecup(PAUSE_GUARDIAN_STICK);
			}
		}
		loclh_InputFlag = sfTrue;
	}
	else loclh_InputFlag = sfFalse;

	if (loclh_BedroomReadNoteCooldown >= 0.f) {
		if (db_IsUp()) loclh_BedroomReadNoteCooldown = .5f;
		loclh_BedroomReadNoteCooldown -= getDeltaTime();
	}

	if (loclh_BedroomFoundStaff) {
		if (loclh_BedroomTimerAnimChest <= .5f) loclh_BedroomTimerAnimChest += getDeltaTime();
	}
}

void loclh_DisplayBedroomBG(Window* _window) {
	_window->rs = GetRenderState("shd_overlay");
	sfShader_setTextureUniform(windowGetShader(_window), "u_TexBlend", GetSfTexture("lighthouse_mentor_shd_light"));
	sfSprite_setPosition(loclh_RSprite, NULLVECTF);
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_bg_phaos"), sfTrue);
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(588.f, 716.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_trapdoor"), sfTrue);
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(820.f, 524.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_chest"), sfTrue);
	if (loclh_BedroomTimerAnimChest >= .5f) sfSprite_setTextureRect(loclh_RSprite, IntRect(128, 0, 64, 64));
	else if (loclh_BedroomTimerAnimChest >= .25f) sfSprite_setTextureRect(loclh_RSprite, IntRect(64, 0, 64, 64));
	else sfSprite_setTextureRect(loclh_RSprite, IntRect(0, 0, 64, 64));
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(loclh_BedroomBedPos, 442.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_bed"), sfTrue);
	LOCLH_DRAW;
}

void loclh_DisplayBedroomFG(Window* _window) {

	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_candelabra"), sfTrue);
	sfSprite_setPosition(loclh_RSprite, vector2f(800.f, 102.f));
	sfSprite_setTextureRect(loclh_RSprite, IntRect(234 * (int)(loclh_AnimTimers.candelabra * 5), 0, 234, 256));
	LOCLH_DRAW;
}

void loclh_UpdateHall(Window* _window) {
	loclh_TimerParticles += getDeltaTime();
	if (loclh_TimerParticles > 0.3f)
	{
		prt_CreateFireParticles(vector2f(935.f, 134.f));
		prt_CreateFireParticles(vector2f(915.f, 230.f));
		prt_CreateFireParticles(vector2f(913.f, 295.f));

		prt_CreateFireParticles(vector2f(1015.f, 240.f));
		prt_CreateFireParticles(vector2f(980.f, 310.f));
		loclh_TimerParticles = 0.f;
	}
	loclh_Timerprt += getDeltaTime();
	/// Computing collisions with the table (X-axis)
	sfVector2f plrPos = addVectorsrf(plr_GetPos(), multiplyVectorsrf(vector2f(plr_GetVel().x, 0.f), getDeltaTime()));
	sfFloatRect plrHitbox = FlRect(plrPos.x - PLR_X_SIZE * .5f, plrPos.y - PLR_Y_SIZE * .5f, PLR_X_SIZE, PLR_X_SIZE);
	sfFloatRect tableHitbox = FlRect(loclh_HallTablePos.x, loclh_HallTablePos.y + 50.f, 210.f, 126.f);
	if (sfFloatRect_intersects(&plrHitbox, &tableHitbox, NULL)) {
		plr_ForceMove(vector2f(plr_GetVel().x * -getDeltaTime(), 0.f));
		if (loclh_HallTableReturning) plr_ForceMove(vector2f(loclh_HallTableSpd.x * getDeltaTime(), 0.f));
		if (loclh_HallFoundNote && !loclh_HallTableReturning && loclh_HallRiddleProgress < 4) {
			if (plr_GetForward().x < 0 && plrPos.x >= tableHitbox.left + tableHitbox.width) {
				if (loclh_HallTablePos.x >= 704.f) loclh_HallTablePos.x -= 40.f * getDeltaTime();
			}
			else if (plr_GetForward().x > 0 && plrPos.x <= tableHitbox.left) {
				if (loclh_HallTablePos.x <= 1004.f) loclh_HallTablePos.x += 40.f * getDeltaTime();
			}
		}
	}

	/// Computing collisions with the table (Y-axis)
	plrPos = addVectorsrf(plr_GetPos(), multiplyVectorsrf(vector2f(0.f, plr_GetVel().y), getDeltaTime()));
	loclh_HallTablePos = addVectorsrf(loclh_HallTablePos, multiplyVectorsrf(loclh_HallTableSpd, getDeltaTime()));
	plrHitbox = FlRect(plrPos.x - PLR_X_SIZE * .5f, plrPos.y - PLR_Y_SIZE * .5f, PLR_X_SIZE, PLR_X_SIZE);
	if (sfFloatRect_intersects(&plrHitbox, &tableHitbox, NULL)) {
		plr_ForceMove(vector2f(0.f, plr_GetVel().y * -getDeltaTime()));
		if (loclh_HallTableReturning) plr_ForceMove(vector2f(0.f, loclh_HallTableSpd.y * getDeltaTime()));
		if (loclh_HallFoundNote && !loclh_HallTableReturning && loclh_HallRiddleProgress < 4) {
			if (plr_GetForward().y < 0 && plrPos.y >= tableHitbox.top + tableHitbox.height) {
				if (loclh_HallTablePos.y >= 414.f) loclh_HallTablePos.y -= 40.f * getDeltaTime();
			}
			else if (plr_GetForward().y > 0 && plrPos.y <= tableHitbox.top) {
				if (loclh_HallTablePos.y <= 714.f) loclh_HallTablePos.y += 40.f * getDeltaTime();
			}
		}
	}

	/// Computing collisions with the exit area
	if (loclh_HallRiddleProgress == 4) {
		sfFloatRect exitHitbox = FlRect(800.f, 1000.f, 320.f, 50.f);
		if (sfFloatRect_contains(&exitHitbox, plrPos.x, plrPos.y)) {
			loclh_RoomChange = sfTrue;
			loclh_NextRoom = MAP_STATE_LIGHTHOUSE_OUTSIDE;
			loclh_NextPos = vector2f(960.f, 610.f);
		}
		if (loclh_Timerprt > 15.f)
		{
			loclh_Timerprt = 0.f;
		}
		else if (loclh_Timerprt > 1.5f)
		{
			if (!prtTable)
			{
				prt_CreateTableParticles(vector2f(955.f, 625.f));
				prtTable = sfTrue;
			}
		}
	}

	/// Computing table updates
	if (getSqrMagnitudeBetween2Vectorsf(loclh_HallTablePos, vector2f(854.f, 564.f)) < 10.f) {
		loclh_HallTableReturning = sfFalse;
		loclh_HallTableSpd = NULLVECTF;
	}
	if (!loclh_HallTableReturning && loclh_HallRiddleProgress < 4) { /// Checking if the table has been placed on the triangle carpets, if the riddle hasn't been solved yet
		sfFloatRect carpetHitbox1 = FlRect(829.f, 689.f, 50.f, 50.f);
		sfFloatRect carpetHitbox2 = FlRect(979.f, 539.f, 50.f, 50.f);
		sfFloatRect carpetHitbox3 = FlRect(679.f, 539.f, 50.f, 50.f);
		sfFloatRect carpetHitbox4 = FlRect(829.f, 389.f, 50.f, 50.f);
		if (sfFloatRect_contains(&carpetHitbox1, loclh_HallTablePos.x, loclh_HallTablePos.y)) {
			loclh_HallReturnTable();
			if (loclh_HallRiddleProgress == 0) loclh_HallRiddleProgress = 1;
			else loclh_HallRiddleProgress = 0;
		}
		else if (sfFloatRect_contains(&carpetHitbox2, loclh_HallTablePos.x, loclh_HallTablePos.y)) {
			loclh_HallReturnTable();
			if (loclh_HallRiddleProgress == 1) loclh_HallRiddleProgress = 2;
			else loclh_HallRiddleProgress = 0;
		}
		else if (sfFloatRect_contains(&carpetHitbox3, loclh_HallTablePos.x, loclh_HallTablePos.y)) {
			loclh_HallReturnTable();
			if (loclh_HallRiddleProgress == 2) loclh_HallRiddleProgress = 3;
			else loclh_HallRiddleProgress = 0;
		}
		else if (sfFloatRect_contains(&carpetHitbox4, loclh_HallTablePos.x, loclh_HallTablePos.y)) {
			loclh_HallReturnTable();
			if (loclh_HallRiddleProgress == 3) loclh_HallRiddleProgress = 4;
			else loclh_HallRiddleProgress = 0;
		}
	}
	sfFloatRect ladderHitbox = FlRect(1150.f, 500.f, 100.f, 100.f);
	if (sfFloatRect_intersects(&ladderHitbox, &plrHitbox, NULL)) {
		plr_toggleFX(sfTrue);
	}

	if (loclh_HallReadNoteCooldown <= 0.f && getSqrMagnitudeBetween2Vectorsf(plr_GetPos(), vector2f(980.f, 500.f)) < 10000.f) {
		plr_toggleFX(sfTrue);
	}
	/// Computing interactions with the world
	if (Gamepad_isButtonPressed(0, A, _window->renderWindow)) {
		if (!loclh_InputFlag) {
			/// Interacting with the ladder to enter the bedroom
			sfFloatRect ladderHitbox = FlRect(1150.f, 500.f, 100.f, 100.f);
			if (sfFloatRect_intersects(&ladderHitbox, &plrHitbox, NULL)) {
				loclh_RoomChange = sfTrue;
				loclh_NextRoom = MAP_STATE_LIGHTHOUSE_BEDROOM;
				playSoundFX("SFX_LightHouse_Ladder");
				loclh_NextPos = vector2f(670.f, 790.f);
			}

			/// Interacting with the lectern to read the note
			/// The chosen point roughly corresponds to the center of the lectern
			if (loclh_HallReadNoteCooldown <= 0.f && getSqrMagnitudeBetween2Vectorsf(plr_GetPos(), vector2f(980.f, 500.f)) < 10000.f) {
				loclh_HallFoundNote = sfTrue;
				loclh_HallReadNoteCooldown = .5f;

				db_Queue("", lang_GetFromKey("notes.lighthouse.hall_1"), vector2f(980.f, 450.f));
				db_Queue(lang_GetFromKey("name.note_mentor"), lang_GetFromKey("notes.lighthouse.hall_2"), vector2f(980.f, 450.f));
				db_Queue(lang_GetFromKey("name.note_mentor"), lang_GetFromKey("notes.lighthouse.hall_3"), vector2f(980.f, 450.f));
				db_Queue(lang_GetFromKey("name.note_mentor"), lang_GetFromKey("notes.lighthouse.hall_4"), vector2f(980.f, 450.f));
			}
		}
		loclh_InputFlag = sfTrue;
	}
	else loclh_InputFlag = sfFalse;

	loclh_AnimTimers.candelabra += getDeltaTime();
	if (loclh_AnimTimers.candelabra > 1.6f) loclh_AnimTimers.candelabra -= 1.6f;

	if (loclh_HallReadNoteCooldown >= 0.f) {
		if (db_IsUp()) loclh_HallReadNoteCooldown = .5f;
		loclh_HallReadNoteCooldown -= getDeltaTime();
	}
}

void loclh_DisplayHallBG(Window* _window) {
	_window->rs = GetRenderState("shd_overlay");
	sfShader_setTextureUniform(windowGetShader(_window), "u_TexBlend", GetSfTexture("lighthouse_shader_hall"));

	sfSprite_setPosition(loclh_RSprite, NULLVECTF);
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_bg_hall"), sfTrue);
	LOCLH_DRAW;

	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_carpet"), sfTrue);
	sfSprite_setPosition(loclh_RSprite, vector2f(911.f, 776.f));
	sfSprite_setTextureRect(loclh_RSprite, IntRect(0, (loclh_HallRiddleProgress >= 1) ? 96 : 0, 96, 96));
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(1094.f, 653.f));
	sfSprite_setTextureRect(loclh_RSprite, IntRect(96, (loclh_HallRiddleProgress >= 2) ? 96 : 0, 96, 96));
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(732.f, 653.f));
	sfSprite_setTextureRect(loclh_RSprite, IntRect(192, (loclh_HallRiddleProgress >= 3) ? 96 : 0, 96, 96));
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(911.f, 518.f));
	sfSprite_setTextureRect(loclh_RSprite, IntRect(288, (loclh_HallRiddleProgress >= 4) ? 96 : 0, 96, 96));
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(854.f, 630.f));
	sfSprite_setTextureRect(loclh_RSprite, IntRect((loclh_HallRiddleProgress >= 4) ? 214 : 0, 192, 214, 128));
	LOCLH_DRAW;

	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_table"), sfTrue);
	sfSprite_setPosition(loclh_RSprite, loclh_HallTablePos);
	LOCLH_DRAW;
}

void loclh_DisplayHallFG(Window* _window) {
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_candelabra"), sfTrue);
	sfSprite_setPosition(loclh_RSprite, vector2f(800.f, 102.f));
	sfSprite_setTextureRect(loclh_RSprite, IntRect(234 * (int)(loclh_AnimTimers.candelabra * 5), 0, 234, 256));
	LOCLH_DRAW;

	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_table"), sfTrue);
	sfSprite_setPosition(loclh_RSprite, loclh_HallTablePos);
	sfSprite_setTextureRect(loclh_RSprite, IntRect(0, 0, 210, TILE_SIZE));
	LOCLH_DRAW;

	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_fg_hall"), sfTrue);
	sfSprite_setPosition(loclh_RSprite, NULLVECTF);
	LOCLH_DRAW;
}

void loclh_UpdateOut(Window* _window) {
	playMusicIfNotPlaying("Music_River");
	loclh_AnimTimers.flowers += getDeltaTime();
	if (loclh_AnimTimers.flowers >= 2.f) loclh_AnimTimers.flowers -= 2.f;

	loclh_AnimTimers.lighthouse += getDeltaTime();
	if (loclh_AnimTimers.lighthouse >= 3.f) loclh_AnimTimers.lighthouse -= 3.f;

	loclh_AnimTimers.waterfall += getDeltaTime();
	if (loclh_AnimTimers.waterfall >= .6f) loclh_AnimTimers.waterfall -= .6f;

	loclh_AnimTimers.clouds_osc += getDeltaTime();

	sfVector2f plrPos = addVectorsrf(plr_GetPos(), multiplyVectorsrf(plr_GetVel(), getDeltaTime()));
	sfFloatRect entranceHitbox = FlRect(910.f, 470.f, 100.f, 50.f);
	if (sfFloatRect_contains(&entranceHitbox, plrPos.x, plrPos.y)) {
		loclh_RoomChange = sfTrue;
		playSoundFX("SFX_LightHouse_Trapdoor");
		loclh_NextRoom = MAP_STATE_LIGHTHOUSE_HALL;
		loclh_NextPos = vector2f(960.f, 950.f);
	}

	/// Computing interactions with the world
	if (Gamepad_isButtonPressed(0, A, _window->renderWindow)) {
		if (!loclh_InputFlag) {
			sfFloatRect wfHitbox = FlRect(835.f, 670.f, 250.f, 50.f);

			if (sfFloatRect_contains(&wfHitbox, plr_GetPos().x, plr_GetPos().y)) {
				loclh_WaterfallPrompt = sfTrue;
				loclh_WaterfallPromptSelect = 1;
			}
		}
		loclh_InputFlag = sfTrue;
	}
	else loclh_InputFlag = sfFalse;

	if (loclh_WaterfallPrompt) {
		plr_StopMoving();
		if (loclh_WaterfallPromptCooldown >= 0.f) loclh_WaterfallPromptCooldown -= getDeltaTime();
		else {
			if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).y < 0.f || Gamepad_isButtonPressed(0, DPAD_UP, _window->renderWindow)) {
				loclh_WaterfallPromptSelect--;
				if (loclh_WaterfallPromptSelect < 0) loclh_WaterfallPromptSelect = 1;
				loclh_WaterfallPromptCooldown = .25f;
			}
			else if (Gamepad_getStickPos(0, STICKL, _window->renderWindow).y > 0.f || Gamepad_isButtonPressed(0, DPAD_DOWN, _window->renderWindow)) {
				loclh_WaterfallPromptSelect++;
				if (loclh_WaterfallPromptSelect > 1) loclh_WaterfallPromptSelect = 0;
				loclh_WaterfallPromptCooldown = .25f;
			}

			if (Gamepad_isButtonPressed(0, B, _window->renderWindow)) loclh_WaterfallPromptSelect = 1;
			if (Gamepad_isButtonPressed(0, A, _window->renderWindow)) {
				loclh_WaterfallPrompt = sfFalse;
				if (!loclh_WaterfallPromptSelect && fade_GetOpacity() == 255) {
					/// Find an actual spawn point
			//		plr_SetPos(vector2f(960.f, 950.f));
					qst_Create(QST_ID_P_EXPLORE, PAUSE_MENTOR);
					map_changeMapState(_window, MAP_STATE_OVERWORLD);
					loclh_Jumped = sfTrue;
				}
			}
		}
	}
}

void loclh_DisplayOutBG(Window* _window) {

	sfSprite_setPosition(loclh_RSprite, NULLVECTF);
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_out_sky"), sfTrue);
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(100.f + sin(loclh_AnimTimers.clouds_osc * .02f + 1.f) * 20.f, 50.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_out_cloud_b1"), sfTrue);
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(1600.f + sin(loclh_AnimTimers.clouds_osc * .035f + 3.f) * 20.f, 135.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_out_cloud_b2"), sfTrue);
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(342.f, 194.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_out_island"), sfTrue);
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(720.f, 664.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_out_waterfall"), sfTrue);
	sfSprite_setTextureRect(loclh_RSprite, IntRect(478 * (int)(loclh_AnimTimers.waterfall * 10), 0, 474, 416));
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(420.f, 186.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_out_decor"), sfTrue);
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(652.f, 0.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_out_lighthouse"), sfTrue);
	sfSprite_setTextureRect(loclh_RSprite, IntRect(768 * (int)(loclh_AnimTimers.lighthouse * 2), 0, 768, 596));
	LOCLH_DRAW;

	loclh_DisplayFlowerOut(_window, vector2f(620.f, 335.f), 0);
	loclh_DisplayFlowerOut(_window, vector2f(614.f, 502.f), 1);
	loclh_DisplayFlowerOut(_window, vector2f(536.f, 565.f), 2);
	loclh_DisplayFlowerOut(_window, vector2f(428.f, 602.f), 3);
	loclh_DisplayFlowerOut(_window, vector2f(584.f, 616.f), 0);
	loclh_DisplayFlowerOut(_window, vector2f(537.f, 709.f), 1);
	loclh_DisplayFlowerOut(_window, vector2f(677.f, 714.f), 2);
	loclh_DisplayFlowerOut(_window, vector2f(1346.f, 460.f), 3);
	loclh_DisplayFlowerOut(_window, vector2f(1415.f, 479.f), 0);
	loclh_DisplayFlowerOut(_window, vector2f(1302.f, 578.f), 1);
	loclh_DisplayFlowerOut(_window, vector2f(1480.f, 560.f), 2);
	loclh_DisplayFlowerOut(_window, vector2f(1229.f, 708.f), 3);
	loclh_DisplayFlowerOut(_window, vector2f(1347.f, 703.f), 0);
	loclh_DisplayFlowerOut(_window, vector2f(1439.f, 657.f), 1);

	sfSprite_setPosition(loclh_RSprite, vector2f(424.f, 0.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_out_light"), sfTrue);
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(398.f, 0.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_out_shade"), sfTrue);
	LOCLH_DRAW;
}

void loclh_DisplayOutFG(Window* _window) {
	sfSprite_setPosition(loclh_RSprite, vector2f(3.f + sin(loclh_AnimTimers.clouds_osc * .05f) * 300.f, 596.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_out_cloud_f1"), sfTrue);
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(700.f + sin(loclh_AnimTimers.clouds_osc * .03f + 1.f) * 400.f, 908.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_out_cloud_f2"), sfTrue);
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(300.f, 358.f + sin(loclh_AnimTimers.clouds_osc * .2f + 5.f) * 30.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_out_cloud_f3"), sfTrue);
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(1650.f, 700.f + sin(loclh_AnimTimers.clouds_osc * .12f + 2.f) * 60.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_out_cloud_f4"), sfTrue);
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(1500.f, 300.f + sin(loclh_AnimTimers.clouds_osc * .16f + 2.f) * 30.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_out_cloud_f5"), sfTrue);
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(1300.f + sin(loclh_AnimTimers.clouds_osc * .07f + 5.f) * 200.f, 514.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_out_cloud_f6"), sfTrue);
	LOCLH_DRAW;

	/// COME UP WITH A BETTER RENDER FOR THIS SOMEDAY
	if (loclh_WaterfallPrompt) {
		sfRectangleShape_setPosition(loclh_PromptRect, vector2f(15.f, 730.f));
		sfRectangleShape_setSize(loclh_PromptRect, vector2f(750.f, 70.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, loclh_PromptRect, NULL);
		sfText_setPosition(loclh_PromptText, vector2f(25.f + (loclh_WaterfallPromptSelect == 0 ? 20.f : 0.f), 740.f));
		sfText_setString(loclh_PromptText, "JUMP");
		sfRenderTexture_drawText(_window->rdrt, loclh_PromptText, NULL);
		sfText_setPosition(loclh_PromptText, vector2f(25.f + (loclh_WaterfallPromptSelect == 1 ? 20.f : 0.f), 765.f));
		sfText_setString(loclh_PromptText, "DON'T JUMP");
		sfRenderTexture_drawText(_window->rdrt, loclh_PromptText, NULL);
	}
}

void loclh_UpdateMentor(Window* _window) {
	loclh_TimerParticles += getDeltaTime();
	if (loclh_TimerParticles > 0.3f)
	{
		prt_CreateFireParticles(vector2f(915.f, 230.f));
		prt_CreateFireParticles(vector2f(913.f, 295.f));

		prt_CreateFireParticles(vector2f(1015.f, 240.f));
		prt_CreateFireParticles(vector2f(980.f, 310.f));
		loclh_TimerParticles = 0.f;
	}
	sfVector2f plrPos = addVectorsrf(plr_GetPos(), multiplyVectorsrf(vector2f(0.f, plr_GetVel().y), getDeltaTime()));
	sfFloatRect plrHitbox = FlRect(plrPos.x - PLR_X_SIZE * .5f, plrPos.y - PLR_Y_SIZE * .5f, PLR_X_SIZE, PLR_X_SIZE);

	sfFloatRect ladderHitbox = FlRect(1150.f, 500.f, 100.f, 100.f);
	if (sfFloatRect_intersects(&ladderHitbox, &plrHitbox, NULL)) {
		plr_toggleFX(sfTrue);
	}

	sfFloatRect trapdoorHitbox = FlRect(588.f, 716.f, 152.f, 118.f);
	if (sfFloatRect_intersects(&trapdoorHitbox, &plrHitbox, NULL)) {
		plr_toggleFX(sfTrue);
	}

	if (loclh_BedroomReadNoteCooldown <= 0.f && getSqrMagnitudeBetween2Vectorsf(plr_GetPos(), vector2f(640.f, 600.f)) < 10000.f) { 
		plr_toggleFX(sfTrue); 
	}
	/// Computing interactions with the world
	if (Gamepad_isButtonPressed(0, A, _window->renderWindow)) {
		if (!loclh_InputFlag) {
			/// Interacting with the ladder to enter the antechamber
			sfFloatRect ladderHitbox = FlRect(1150.f, 500.f, 100.f, 100.f);
			if (sfFloatRect_intersects(&ladderHitbox, &plrHitbox, NULL)) {
				loclh_RoomChange = sfTrue;
				loclh_NextRoom = MAP_STATE_LIGHTHOUSE_ANTECHAMBER;
				playSoundFX("SFX_LightHouse_Ladder");
				loclh_NextPos = vector2f(670.f, 790.f);
			}

			/// Interacting with the trapdoor
			sfFloatRect trapdoorHitbox = FlRect(588.f, 716.f, 152.f, 118.f);
			if (sfFloatRect_intersects(&trapdoorHitbox, &plrHitbox, NULL)) {
				loclh_RoomChange = sfTrue;
				loclh_NextRoom = MAP_STATE_LIGHTHOUSE_BEDROOM;
				playSoundFX("SFX_LightHouse_Trap");
				loclh_NextPos = vector2f(1160.f, 580.f);
			}

			/// Interacting with the desk to read the note
			/// The chosen point roughly corresponds to the book on the desk
			if (loclh_BedroomReadNoteCooldown <= 0.f && getSqrMagnitudeBetween2Vectorsf(plr_GetPos(), vector2f(640.f, 600.f)) < 10000.f) {
				loclh_BedroomFoundNote = sfTrue;
				loclh_BedroomReadNoteCooldown = .5f;

				db_Queue("", lang_GetFromKey("notes.lighthouse.bedroom_1"), vector2f(640.f, 600.f));
				db_Queue(lang_GetFromKey("name.note_mentor"), lang_GetFromKey("notes.lighthouse.bedroom_2"), vector2f(640.f, 600.f));
				db_Queue(lang_GetFromKey("name.note_mentor"), lang_GetFromKey("notes.lighthouse.bedroom_3"), vector2f(640.f, 600.f));
				db_Queue(lang_GetFromKey("name.note_mentor"), lang_GetFromKey("notes.lighthouse.bedroom_4"), vector2f(640.f, 600.f));
			}
		}
		loclh_InputFlag = sfTrue;
	}
	else loclh_InputFlag = sfFalse;

	loclh_AnimTimers.candle_flame += getDeltaTime();
	if (loclh_AnimTimers.candle_flame > .7f) loclh_AnimTimers.candle_flame -= .7f;
}




void loclh_DisplayMentorBG(Window* _window) {
	_window->rs = GetRenderState("shd_overlay");
	sfShader_setTextureUniform(windowGetShader(_window), "u_TexBlend", GetSfTexture("lighthouse_mentor_shd_light"));
	

	sfSprite_setPosition(loclh_RSprite, NULLVECTF);
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_bg_mentor"), sfTrue);
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(588.f, 716.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_trapdoor"), sfTrue);
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(586.f, 526.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_flame"), sfTrue);
	sfSprite_setTextureRect(loclh_RSprite, IntRect(28 * (int)(loclh_AnimTimers.candle_flame * 10), 0, 28, 44));
	LOCLH_DRAW;

	sfSprite_setPosition(loclh_RSprite, vector2f(1214.f, 398.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_bookcase"), sfTrue);
	LOCLH_DRAW;
}

void loclh_DisplayMentorFG(Window* _window) {
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_candelabra"), sfTrue);
	sfSprite_setPosition(loclh_RSprite, vector2f(800.f, 102.f));
	sfSprite_setTextureRect(loclh_RSprite, IntRect(234 * (int)(loclh_AnimTimers.candelabra * 5), 0, 234, 256));
	LOCLH_DRAW;
	sfSprite_setPosition(loclh_RSprite, vector2f(1180.f, 680.f));
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_fg_mentor"), sfTrue);
	LOCLH_DRAW;
}

void loclh_UpdateAntechamber(Window* _window)
{
	sfVector2f plrPos = addVectorsrf(plr_GetPos(), multiplyVectorsrf(vector2f(0.f, plr_GetVel().y), getDeltaTime()));
	sfFloatRect plrHitbox = FlRect(plrPos.x - PLR_X_SIZE * .5f, plrPos.y - PLR_Y_SIZE * .5f, PLR_X_SIZE, PLR_X_SIZE);

	sfFloatRect ladderHitboxDown = FlRect(572.f, 744.f, 154.f, 118.f);
	if (sfFloatRect_intersects(&ladderHitboxDown, &plrHitbox, NULL)) {
		plr_toggleFX(sfTrue);
	}
	/// Computing interactions with the world
	if (Gamepad_isButtonPressed(0, A, _window->renderWindow)) {
		if (!loclh_InputFlag) {
			/// Interacting with the trap to enter the mentor's room
			sfFloatRect ladderHitboxDown = FlRect(572.f, 744.f, 154.f, 118.f);
			if (sfFloatRect_intersects(&ladderHitboxDown, &plrHitbox, NULL)) {
				loclh_RoomChange = sfTrue;
				loclh_NextRoom = MAP_STATE_LIGHTHOUSE_MENTOR;
				playSoundFX("SFX_LightHouse_Trap");
				loclh_NextPos = vector2f(1160.f, 580.f);
			}
		}
		loclh_InputFlag = sfTrue;
	}
	else loclh_InputFlag = sfFalse;
}

void loclh_DisplayAntechamber(Window* _window)
{
	sfSprite_setPosition(loclh_RSprite, NULLVECTF);

	sfSprite_setTexture(loclh_RSprite, GetSfTexture("antechamber_stairs"), sfTrue);
	sfSprite_setPosition(loclh_RSprite, vector2f(846.f, 332.f));
	sfSprite_setScale(loclh_RSprite, vector2f(1.f, 1.f));
	sfRenderTexture_drawSprite(_window->rdrt, loclh_RSprite, NULL);

	sfRectangleShape_setSize(lolch_Antechamber, vector2f(292.f, 196.f));
	sfRectangleShape_setPosition(lolch_Antechamber, vector2f(822.f, 324.f));
	sfRectangleShape_setTextureRect(lolch_Antechamber, IntRect(0, 0, 252, 196));
	sfRenderTexture_drawRectangleShape(_window->rdrt, lolch_Antechamber, NULL);

	sfSprite_setTexture(loclh_RSprite, GetSfTexture("antechamber_bg"), sfTrue);
	sfSprite_setPosition(loclh_RSprite, vector2f(0.f, 0.f));
	sfRenderTexture_drawSprite(_window->rdrt, loclh_RSprite, NULL);

	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_trapdoor"), sfTrue);
	sfSprite_setPosition(loclh_RSprite, vector2f(572.f, 744.f));
	sfRenderTexture_drawSprite(_window->rdrt, loclh_RSprite, NULL);


	sfVector2f tempPlrPos = plr_GetPos();
	if (tempPlrPos.y < 620.f && !loclh_IsFirstTorchOn && !loclh_IsSecondTorchOn)
	{
		loclh_IsFirstTorchOn = sfTrue;
		loclh_IsSecondTorchOn = sfTrue;
		IsTorchOnLeft = sfFalse;
		IsTorchOnRight = sfFalse;

	}

	if (loclh_IsFirstTorchOn)
	{
		if (!IsTorchOnLeft)
		{
			loclh_FrameAnimCooldownLeft += getDeltaTime();
			sfRectangleShape_setSize(lolch_Antechamber, vector2f(90.f, 152.f));
			sfRectangleShape_setPosition(lolch_Antechamber, vector2f(700.f, 294.f));
			sfRectangleShape_setTextureRect(lolch_Antechamber, animatorAnim("lightOnLeft", &loclh_FrameX, &loclh_FrameAnim, 0));
			sfRenderTexture_drawRectangleShape(_window->rdrt, lolch_Antechamber, NULL);
			
			if (!loclh_PlaySound)
			{
				playSoundFX("SFX_LightHouse_Torch");
				loclh_PlaySound = sfTrue;
			}

			if (FrameXLeft >= 6)
			{
				FrameXLeft = 0;
				IsTorchOnLeft = sfTrue;
				loclh_FrameAnimCooldownLeft = 0.f;
			}
			else if (loclh_FrameAnimCooldownLeft > 0.2f)
			{
				FrameXLeft++;
				loclh_FrameAnimCooldownLeft = 0.f;
			}
		}
		else if (IsTorchOnLeft)
		{
			loclh_FrameAnimCooldownLeft += getDeltaTime();
			sfRectangleShape_setSize(lolch_Antechamber, vector2f(90.f, 152.f));
			sfRectangleShape_setPosition(lolch_Antechamber, vector2f(700.f, 294.f));
			sfRectangleShape_setTextureRect(lolch_Antechamber, IntRect(FrameXLeftRepeat * 90.f, 354, 90, 152));
			sfRenderTexture_drawRectangleShape(_window->rdrt, lolch_Antechamber, NULL);

			if (FrameXLeftRepeat >= 12)
			{
				FrameXLeftRepeat = 6;
			}
			else if (loclh_FrameAnimCooldownLeft > 0.2f)
			{
				FrameXLeftRepeat++;
				loclh_FrameAnimCooldownLeft = 0.f;
			}
		}
	}
	else
	{
		sfRectangleShape_setSize(lolch_Antechamber, vector2f(90.f, 152.f));
		sfRectangleShape_setPosition(lolch_Antechamber, vector2f(700.f, 294.f));
		sfRectangleShape_setTextureRect(lolch_Antechamber, IntRect(0, 354, 90, 152));
		sfRenderTexture_drawRectangleShape(_window->rdrt, lolch_Antechamber, NULL);
	}


	if (loclh_IsSecondTorchOn)
	{
		if (!IsTorchOnRight)
		{
			loclh_FrameAnimCooldownRight += getDeltaTime();
			sfRectangleShape_setSize(lolch_Antechamber, vector2f(90.f, 152.f));
			sfRectangleShape_setPosition(lolch_Antechamber, vector2f(1130.f, 294.f));
			sfRectangleShape_setTextureRect(lolch_Antechamber, animatorAnim("LightOnRightTEST", &loclh_FrameX, &loclh_FrameAnim, 0));
			sfRenderTexture_drawRectangleShape(_window->rdrt, lolch_Antechamber, NULL);

			if (FrameXRight >= 6)
			{
				FrameXRight = 0;
				IsTorchOnRight = sfTrue;
				loclh_FrameAnimCooldownRight = 0.f;
			}
			else if (loclh_FrameAnimCooldownRight > 0.2)
			{
				FrameXRight++;
				loclh_FrameAnimCooldownRight = 0.f;
			}
		}
		else if (IsTorchOnRight)
		{
			loclh_FrameAnimCooldownRight += getDeltaTime();
			sfRectangleShape_setSize(lolch_Antechamber, vector2f(90.f, 152.f));
			sfRectangleShape_setPosition(lolch_Antechamber, vector2f(1130.f, 294.f));
			sfRectangleShape_setTextureRect(lolch_Antechamber, IntRect(FrameXRightRepeat * 90.f, 202, 90, 152));
			sfRenderTexture_drawRectangleShape(_window->rdrt, lolch_Antechamber, NULL);

			if (FrameXRightRepeat >= 12)
			{
				FrameXRightRepeat = 6;
			}
			else if (loclh_FrameAnimCooldownRight > 0.2f)
			{
				FrameXRightRepeat++;
				loclh_FrameAnimCooldownRight = 0.f;
			}
		}
	}
	else
	{
		sfRectangleShape_setSize(lolch_Antechamber, vector2f(90.f, 152.f));
		sfRectangleShape_setPosition(lolch_Antechamber, vector2f(1130.f, 294.f));
		sfRectangleShape_setTextureRect(lolch_Antechamber, IntRect(0, 202, 90, 152));
		sfRenderTexture_drawRectangleShape(_window->rdrt, lolch_Antechamber, NULL);
	}

	sfSprite_setTexture(loclh_RSprite, GetSfTexture("antechamber_objets"), sfTrue);
	sfSprite_setPosition(loclh_RSprite, vector2f(0.f, 0.f));
	sfSprite_setScale(loclh_RSprite, vector2f(1.f, 1.f));
	sfRenderTexture_drawSprite(_window->rdrt, loclh_RSprite, NULL);

	sfSprite_setTexture(loclh_RSprite, GetSfTexture("antechamber_checkmark"), sfTrue);
	sfSprite_setPosition(loclh_RSprite, vector2f(668.f, 146.f));
	sfRenderTexture_drawSprite(_window->rdrt, loclh_RSprite, NULL);
}


void loclh_HallReturnTable() {
	loclh_HallTableReturning = sfTrue;
	loclh_HallTableSpd = multiplyVectorsrf(normalizef(substractVectorsrf(loclh_HallTablePos, vector2f(854.f, 564.f))), -100.f);
}

void loclh_DisplayFlowerOut(Window* _window, sfVector2f _pos, int _frameOff) {
	sfSprite_setTexture(loclh_RSprite, GetSfTexture("lighthouse_out_flower"), sfTrue);
	sfSprite_setPosition(loclh_RSprite, _pos);
	sfSprite_setTextureRect(loclh_RSprite, IntRect(14 * ((int)(loclh_AnimTimers.flowers * 2 + _frameOff) % 4), 0, 14, 16));
	//	sfSprite_setTextureRect(loclh_RSprite, animatorAnim("lighthouse_flower_anim", &tmp->animFrame, &loclh_AnimTimers.flowers, 0));
	LOCLH_DRAW;
}