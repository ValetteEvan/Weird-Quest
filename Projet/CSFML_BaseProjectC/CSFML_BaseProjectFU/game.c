#include "game.h"
#include "textureManager.h"
#include "viewManager.h"
#include "stateManager.h"
#include "soundManager.h"
#include "controller.h"
#include "pause.h"
#include "fontManager.h"
#include "player.h"
#include "stateManager.h"
#include "langManager.h"
#include "shaderManager.h"
#include "dialogBox.h"
#include "game_HUD.h"
#include "fadeManager.h"
#include "current.h"

#include "SFML/System/Thread.h"
#include <SFML/System/Mutex.h>
#include <Windows.h>
#include "map.h"
#include "ring.h"
#include "box.h"
#include "pot.h"
#include "moving_platform.h"
#include "enemies.h"
#include "bossJoie.h"
#include "quest.h"
#include "tears.h"
#include "debuffTear.h"
#include "bullets.h"
#include "statue.h"
#include "chest.h"
#include "fish_tank.h"
#include "openDoors.h"
#include "spidersTrees.h"
#include "grass.h"
#include "options.h"
#include "FXenemies.h"

#include "light.h"
#include "particlesManager.h"
#include "fish.h"
#include "hook.h"
#include "npc.h"
#include "bossTristesse.h"
#include "fx_boss.h"

#include "lighthouse.h"
#include "expandingTile.h"
#include "bigRock.h"
#include "fishing_spot.h"
#include "drop_items.h"
#include "note.h"
#include "game_over.h"
#include "brazier.h"

sfThread* thread3;
sfThread* thread2;

//sfSprite* sprite;
sfSprite* spGameState;
sfSprite* spGameState2;
sfTexture* textureLink;

sfBool gamepadChangePos;

float game_Timer;
int nbreJoueur;
int waveCount;

float waveTimer;
float soloTimer;
float defaultwaveTimer;
float defaultsoloTimer;
float bossTimer;

sfBool phaseWave;
sfBool phaseBoss;
sfBool phaseBossCompleted;
sfBool gameover;
sfBool game_FadeOut;
sfBool game_FadeIn;
sfBool game_ChangeNow;
sfBool game_ChangeStateFish_JOY;
sfBool game_ChangeStateFish_JOY_TUTO;
sfBool game_ChangeStateFish_SADNESS;
sfBool game_ChangeStateFish_SADNESS_TUTO;
sfBool game_ChangeStateGame;

int explosionFrameX = 0;
float timerExplosion = 0.f;

sfBool firstLoadGame = sfFalse;

sfBool game_ThreadsRunning;

sfRectangleShape* game_ShaderRenderRect;
sfTexture* game_rtShader;

sfRenderTexture* game_rTex;

void thread2Loop(void* data)
{
	Window* game_RdrWindow = (Window*)data;

	initToolsThread2();

	w.state = sfTrue;

	while (game_ThreadsRunning)
	{
		if (getGameState() != GAME_FISH && !getIsFreeingMap()) {
			restartClockThread2();
			if (fade_GetOpacity() == 0)
			{
				plr_UpdateInteractions(game_RdrWindow);
				pot_Update();
				en_UpdateEnemies(game_RdrWindow);
				blt_updateBullets(plr_GetPos());
			}
			plr_UpdateGameOver(game_RdrWindow);
		}
	}
}

void thread3Loop(void* data)
{
}

void initGame(Window* _window)
{
	initGrass();
	if (!GetSound("SFX_Player_AttackMagic")) soundsOnload(GAME);
	map_setOnePassMap(sfTrue);
	thread2 = sfThread_create(&thread2Loop, _window);
	mutexThread3 = sfMutex_create();
	thread3 = sfThread_create(&thread3Loop, NULL);
	mutexThread2 = sfMutex_create();
	game_ThreadsRunning = sfTrue;

	if (!GetSfTexture("HUD")) Texture_Onload(GAME);
	if (!GetShader("shd_biome_dungeon")) shaderOnload(GAME);
	if (!GetFont("UGb")) fontsOnload(GAME);
	plr_Init();
	npc_Init(_window);
	chest_InitChest();
	brz_Init();
	opDoors_InitDoor(_window);
	stree_initSpidersTrees();
	ring_Init();
	pot_Init();
	qst_Init();
	mvpl_Init();
	expt_Init();
	stt_Init(_window);
	prt_InitParticles();
	en_InitEnemies();
	trs_initTears();
	dbfT_initTearDebuff();
	blt_initBullets();
	bigRock_Init();
	fx_Init1(_window);
	fx_Init2(_window);
	tank_Init();
	HUD_InitGame();
	stree_initSpidersTrees();
	fishspot_Init();
	FXe_initFXenemies();
	initLights();
	initItemsDrop(_window);
	note_InitNote();
	gameOver_init();

	bossJoie_initFx(); 
	entity_BossJoie = initBossJoie(vector2f(980.f, 200.f), 10);
	bossTristesse = initBoss(vector2f(800.f, 150.f), 15.f);
	BossAnneau = initBossFragment(vector2f(800.f, 150.f), 15.f);
	map_InitMap(_window);
	spGameState = sfSprite_create();
	spGameState2 = sfSprite_create();

	setViewPosition(mainView, vector2f(960.f, 540.f));
	GamepadDetection();

	db_Init();
	box_Init();

	game_ShaderRenderRect = sfRectangleShape_create();
	sfRectangleShape_setPosition(game_ShaderRenderRect, NULLVECTF);
	game_rtShader = sfTexture_create(sfRenderTexture_getSize(_window->rdrt).x, sfRenderTexture_getSize(_window->rdrt).y);

	stopMusicIfPlaying("Music_Menu");

	sfThread_launch(thread2);
	sfThread_launch(thread3);
	game_rTex = sfRenderTexture_create(_window->videoMode.width, _window->videoMode.height, sfFalse);
}

void updateGame(Window* _window) {
	if (getIsFreeingMap()) return;

	game_Timer += getDeltaTime();
	if (loclh_Jumped) {
		fadeOut(1.f);
		if (fade_GetOpacity() == 0) {
			loclh_Jumped = sfFalse;
		}
	}


	/// TOUCHES DE DEBUG POUR LA PREZ   /!\ NE PAS TOUCHER
	if (sfKeyboard_isKeyPressed(sfKeyH)) plr_AddLife(12);
	if (sfKeyboard_isKeyPressed(sfKeyJ) && game_Timer > 0.2f)
	{
		game_Timer = 0.f;
		pause_itemRecup(PAUSE_YOYO);
	}
	if (sfKeyboard_isKeyPressed(sfKeyK) && game_Timer > 0.2f)
	{
		game_Timer = 0.f;
		pause_itemRecup(PAUSE_JOY_FISH);
	}

	if (Gamepad_isButtonPressed(0, BACK, _window->renderWindow) && game_Timer > 0.2f)
	{
		screenshot(_window->renderWindow);
		game_Timer = 0.0f;
	}
	if (fade_GetOpacity() == 255 && game_FadeOut)
	{
		game_FadeIn = sfTrue;
	}
	if (game_FadeIn)
	{
		fadeIn(1.5f);
	}
	if (fade_GetOpacity() == 0)
	{
		game_FadeIn = sfFalse;
	}
	if (game_FadeOut)
	{
		fadeOut(1.5f);
	}
	if (fade_GetOpacity() == 255)
	{
		game_FadeOut = sfFalse;
		game_ChangeNow = sfTrue;
	}

	if (game_ChangeNow)
	{
		if (game_ChangeStateFish_JOY)
		{
			fish_InitFish(FISH_JOY, _window);
			game_ChangeStateFish_JOY = sfFalse;
			game_ChangeNow = sfFalse;
			changeGameState(GAME_FISH);
		}
		else if (game_ChangeStateFish_JOY_TUTO)
		{
			fish_InitFish(FISH_JOY_TUTO, _window);
			game_ChangeStateFish_JOY_TUTO = sfFalse;
			game_ChangeNow = sfFalse;
			changeGameState(GAME_FISH);
		}
		else if (game_ChangeStateFish_SADNESS)
		{
			fish_InitFish(FISH_SADNESS, _window);
			game_ChangeStateFish_SADNESS = sfFalse;
			game_ChangeNow = sfFalse;
			changeGameState(GAME_FISH);
		}
		else if (game_ChangeStateFish_SADNESS_TUTO)
		{
			fish_InitFish(FISH_SADNESS_TUTO, _window);
			game_ChangeStateFish_SADNESS_TUTO = sfFalse;
			game_ChangeNow = sfFalse;
			changeGameState(GAME_FISH);

		}
		else if (game_ChangeStateGame)
		{
			hook_DeinitHook();
			fish_DeinitFish();
			game_ChangeStateGame = sfFalse;
			game_ChangeNow = sfFalse;
			changeGameState(GAME_OUTSIDE);
			game_FadeOut = sfTrue;
		}
		else
		{
			game_ChangeNow = sfFalse;
		}
	}

	switch (getGameState())
	{
	case GAME_OUTSIDE:

		if (fade_GetOpacity() == 0 || !(game_FadeOut || game_FadeIn))
		{
			//playMusicIfNotPlaying("Music_Main");
			plr_UpdateMovement(_window);
			ring_Update(_window);
			box_Update();
			//tank_Update(_window);
			mvpl_Update();
			expt_Update();
			npc_Update(_window);
			stt_Update(_window);
			prt_UpdateParticles();
			trs_Update(_window);
			FXe_updateFXenemies();
			chest_UpdateChest(_window);
			brz_Update();
			opDoors_UpdateDoor(_window);
			stree_updateSpidersTrees(_window);
			fishspot_Update(_window);

			dbfT_updateTearDebuff(_window);
			HUD_UpdateGame(_window);
			qst_Update(_window);
			updateItemsDrop(_window);
			note_UpdateNote(_window);

			map_UpdateMap(_window); 

			updateBoss(_window);

			fx_Update1(_window);
			fx_Update2(_window);
			//bigRock_Update(_window);
		}
		fade_Update();
		break;
	case GAME_FISH:
		setViewPosition(mainView, vector2f(960.f, 540.f));
		hook_UpdateHook(_window);
		fish_UpdateFish(_window);
		fade_Update();
		prt_UpdateParticles();
		break;
	default:
		break;
	}

	for (int i = 0; i < 8; i++)
	{
		if (Gamepad_isButtonPressedNF(i, START) && game_Timer > 0.2f && !plr_IsInteractingWithStatue())
		{
			if (getGameState() == GAME_FISH) 
				toggleOptions();
			else
				togglePause();
			game_Timer = 0.0f;
		}
	}

	if (customKeyboardIsKeyPressed(sfKeyEscape, _window->renderWindow) == sfTrue && game_Timer > 0.2f && !plr_IsInteractingWithStatue())
	{
		togglePause();
		game_Timer = 0.0f;
	}
	gamepadChangePos = sfFalse;

	if (customKeyboardIsKeyPressed(sfKeyNum8, _window->renderWindow) && DEBUG_STATE) lhloc_Begin(_window);
	if (customKeyboardIsKeyPressed(sfKeyNum7, _window->renderWindow) && DEBUG_STATE) map_changeMapState(_window, MAP_STATE_OVERWORLD);
	if (customKeyboardIsKeyPressed(sfKeyNum6, _window->renderWindow) && DEBUG_STATE) map_changeMapState(_window, MAP_STATE_DUNGEON_JOY_1);

	/// Game State test
	static float gameStateTestTimer = 0.f;
	gameStateTestTimer += getDeltaTime();
	if (customKeyboardIsKeyPressed(sfKeyNumpad2, _window->renderWindow) && gameStateTestTimer > 0.5f) {
		gameStateTestTimer = 0.f;
		game_ChangeStateFish_SADNESS = sfTrue;
		game_FadeOut = sfTrue;
	}
	if (customKeyboardIsKeyPressed(sfKeyNumpad1, _window->renderWindow) && gameStateTestTimer > 0.5f) {
		game_ChangeFromGameToFish();
	}
	if (customKeyboardIsKeyPressed(sfKeyNumpad4, _window->renderWindow) && gameStateTestTimer > 0.5f) {
		game_ChangeFromGameToFishTuto();
	}
	if (customKeyboardIsKeyPressed(sfKeyNumpad5, _window->renderWindow) && gameStateTestTimer > 0.5f) {
		gameStateTestTimer = 0.f;
		game_ChangeStateFish_SADNESS_TUTO = sfTrue;
		game_FadeOut = sfTrue;
	}
	if (customKeyboardIsKeyPressed(sfKeyNumpad0, _window->renderWindow) && gameStateTestTimer > 0.5f) {
		gameStateTestTimer = 0.f;
		game_ChangeFromFishToGame();
	}
	db_Update(_window);
}

void displayGame(Window* _window)
{
	if (getIsFreeingMap()) {
		if (getGameState() == GAME_OUTSIDE && get_gameOverState()) gameOver_display(_window);
		return;
	}
	sfSprite_setScale(spGameState, vector2f(1.f, 1.f));
	sfRenderStates* rs;
	sfVector2u wSize = sfRenderWindow_getSize(_window->renderWindow);

	switch (getGameState())
	{
	case GAME_OUTSIDE:
		if (getIsFreeingMap()) return;
		map_DisplayMapBehindPlayer(_window);
		en_DisplayEnemiesShadows(_window);
		plr_DisplayShadow(_window); // TODO display behind props
		mvpl_Display(_window);
		expt_Display(_window);
		sfMutex_lock(mutexThread2);
		dbfT_displayTearDebuff(_window);
		prt_DisplayParticlesBehind(_window);
		en_DisplayEnemies(_window);
		sfMutex_unlock(mutexThread2);
		trs_displayDeadTears(_window);
		blt_displayBullets(_window);
		//tank_Display(_window);
		// current_Display(_window);
		bigRock_Display(_window);
		npc_Display(_window);
		plr_Display(_window, spGameState);
		plr_DisplayHood(_window);
		if (getIsFreeingMap()) return;
		map_DisplayMapInFrontOfPlayer(_window);
		trs_displayTears(_window);
		displayLights(_window);
        displayBoss(_window); 
		FXe_displayFXenemies(_window);
		prt_DisplayParticlesInFront(_window);
		displayItemsDrop(_window); 
		fx_DisplayLayer2(_window); 


		switch (getCurrentBiome()) {
		case BIOME_BEACH:
			rs = GetRenderState("shd_biome_beach");
			sfShader_setTextureUniform(rs->shader, "u_TexSolid", GetSfTexture("shd_biome_beach_solid"));
			sfShader_setTextureUniform(rs->shader, "u_TexGodrays", GetSfTexture("shd_biome_beach_godrays"));
			break;

		case BIOME_MOUNTAIN:
			rs = GetRenderState("shd_biome_mountain");
			sfShader_setTextureUniform(rs->shader, "u_TexDust", GetSfTexture("shd_biome_mountain_dust"));
			sfShader_setTextureUniform(rs->shader, "u_TexGodrays", GetSfTexture("shd_biome_mountain_godrays"));
			sfShader_setTextureUniform(rs->shader, "u_TexClouds", GetSfTexture("shd_biome_mountain_clouds"));
			break;

		case BIOME_CAVE:
			rs = GetRenderState("shd_biome_cave");
			sfShader_setTextureUniform(rs->shader, "u_TexVignette", GetSfTexture("shd_biome_cave_vignette"));
			break;

		case BIOME_JOY_DUNGEON: case BIOME_SADNESS_DUNGEON: case BIOME_FEAR_DUNGEON: case BIOME_ANGRY_DUNGEON:
			rs = GetRenderState("shd_biome_dungeon");
			sfShader_setTextureUniform(rs->shader, "u_TexVignette", GetSfTexture("shd_biome_dungeon_vignette"));
			break;

		default:
			rs = NULL;
			break;
		}
		if (getPause())
		{
			rs = GetRenderState("shd_flou");
			sfShader_setVec2Uniform(rs->shader, "u_Res", vector2f(wSize.x, wSize.y));
		}
		if (rs) {
			/// SOMEONE FIND A MORE OPTIMAL WAY TO DO THIS
			/// NEVERMIND THAT? WE DON'T HAVE TIME ANYMORE, JUST KEEP IN MIND THIS METHOD IS TERRIBLE, DO NOT TRY AT HOME
			game_rtShader = sfTexture_copy(sfRenderTexture_getTexture(_window->rdrt));
			sfRectangleShape_setTexture(game_ShaderRenderRect, game_rtShader, sfTrue);
			sfRectangleShape_setSize(game_ShaderRenderRect, vector2f(wSize.x, wSize.y));
			sfRectangleShape_setPosition(game_ShaderRenderRect, substractVectorsrf(mainView->viewPos, vector2f(wSize.x * .5f, wSize.y * .5f)));
			sfShader_setTextureUniform(rs->shader, "u_TexBase", game_rtShader);
			sfRenderTexture_clear(_window->rdrt, sfTransparent);
			sfRenderTexture_drawRectangleShape(_window->rdrt, game_ShaderRenderRect, rs);
			sfTexture_destroy(game_rtShader);

			/// I've got some ideas there, might need fine-tuning to adjust the view's position
//			sfRenderTexture_clear(game_rTex, sfTransparent);
//			sfRectangleShape_setTexture(game_ShaderRenderRect, sfRenderTexture_getTexture(_window->rdrt), sfTrue);
//			sfRectangleShape_setSize(game_ShaderRenderRect, vector2f(wSize.x, wSize.y));
//			sfShader_setTextureUniform(rs->shader, "u_TexBase", sfRenderTexture_getTexture(_window->rdrt));
//			sfRenderTexture_drawRectangleShape(game_rTex, game_ShaderRenderRect, rs);
//			sfRectangleShape_setTexture(game_ShaderRenderRect, sfRenderTexture_getTexture(game_rTex), sfTrue);
//			sfRenderTexture_drawRectangleShape(_window->rdrt, game_ShaderRenderRect, NULL);
		}

		ring_Display(_window);
		plr_DisplaySlash(_window);
		plr_DisplayEnduranceBar(_window);
		db_Display(_window);
		HUD_DisplayGame(_window);
		stt_DisplayMenu(_window);

		static float timerGameOver = 0.f;
		timerGameOver += getDeltaTime();

		fade_Display(_window);
		if (get_gameOverState()) gameOver_display(_window);
		break;
	case GAME_FISH:
		if (!customKeyboardIsKeyPressed(sfKeyZ, _window->renderWindow)) {
			rs = GetRenderState("shd_fishing");
			sfShader_setTextureUniform(rs->shader, "u_TexLight", GetSfTexture("shd_fishing_light"));
			sfShader_setTextureUniform(rs->shader, "u_TexShade", GetSfTexture("shd_fishing_shade"));
			sfShader_setTextureUniform(rs->shader, "u_TexGodrays", GetSfTexture("shd_fishing_godrays"));
			sfShader_setTextureUniform(rs->shader, "u_TexNoise", GetSfTexture("shd_fishing_noise"));
			_window->rsShader = rs;
		}
		hook_DisplayHook(_window);
		prt_DisplayParticlesBehind(_window);
		fish_DisplayFish(_window);
		fade_Display(_window);
		prt_DisplayParticlesInFront(_window);
		break;
	default:
		break;
	}
}

void deinitGame()
{
	game_ThreadsRunning = sfFalse;
	if (!spGameState || !spGameState2) return;
	deinitPlayer();
	blt_deinitBullets();
	deinitPause();
	npc_Deinit();
	sfSprite_destroy(spGameState);
	sfSprite_destroy(spGameState2);
	sfRenderTexture_destroy(game_rTex);
	fx_Deinit();
	trs_Deinit();
	chest_Deinit();
	opDoors_DeinitDoor();
	deinitEnemies();
	RemoveAllTextureButALL();
	RemoveAllSoundsNotNeededInEveryStates();
	RemoveAllFontsNotNeededInEveryStates();
}

void game_ChangeFromFishToGame()
{
	game_ChangeStateGame = sfTrue;
	game_FadeOut = sfTrue;
	stopMusicIfPlaying("Music_Peche");
	stopMusicIfPlaying("Music_Fishing_BulleSousLeau");
	stopMusicIfPlaying("Music_Fishing_CrickCaneAPeche");
	playMusicIfNotPlaying("Music_Main");
}

void game_ChangeFromGameToFish()
{
	game_ChangeStateFish_JOY = sfTrue;
	game_FadeOut = sfTrue;
}

void game_ChangeFromGameToFishTuto()
{
	game_ChangeStateFish_JOY_TUTO = sfTrue;
	game_FadeOut = sfTrue;
}