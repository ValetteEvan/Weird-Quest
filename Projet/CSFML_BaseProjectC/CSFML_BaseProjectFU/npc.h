#pragma once
#include "tools.h"
#include "windowManager.h"
#include "List.h"
#include "pause.h"

#define NPC_SIZE_X 15.f
#define NPC_SIZE_Y 15.f

typedef struct
{
	sfVector2f position;
	int id;
}NpcToSave;

typedef enum NpcId NpcId;
enum NpcId {
	NPC_ARNOLD = 1,
	NPC_BBEG = 2,
	NPC_MENTOR = 3,
	NPC_SHAMAN = 4
};

typedef struct Npc Npc;
struct Npc {
	NpcId id;
	sfVector2f position;
	sfVector2f dbPos;
	sfFloatRect interact;
	sfIntRect texRect;
	sfBool isSpeaking;
	sfBool isBlooding;
	sfBool isBackwards;
	sfBool isMagic;
	sfBool onePass;
	sfBool noDial;
	sfBool changeDial;
	int frameX;
	int dialId;
	int NPC_FrameX;
	float timerAnim;
	float NPC_TimerAnim;

	void (*getDialog)(struct Npc*);
	void (*getNoDial)(struct Npc*);
};
stdList* npc_List;

/// Loads the textures required by NPC rendering into memory.
void npc_loadTexture();

/// Initializes the NPC system.
void npc_Init(Window* _window);

/// Updates NPCs.
void npc_Update(Window* _window);

/// Renders NPCs.
void npc_Display(Window* _window);

/// Deinits the NPC system and frees up allocated memory.
void npc_Deinit();

/// Creates a new NPC.
/// \param _position - Spawn pos of the NPC
/// \param _id - Which NPC to spawn
void npc_Create(sfVector2f _position, quetePNJ _id);

/// Extracts data from the NPC list and prepares it for saving into a map file.
NpcToSave* getNpcListToSave();

/// Fetches the collector's various dialog lines depending on the game's current state of progress.
void npc_GetDialogArnold(Npc* _npc);

/// Fetches the necromancer's various dialog lines depending on the game's current state of progress.
void npc_GetDialogBBEG(Npc* _npc);

/// Fetches the mentor's various dialog lines depending on the game's current state of progress.
void npc_GetDialogMentor(Npc* _npc);

/// Fetches the shaman's various dialog lines depending on the game's current state of progress.
void npc_GetDialogShaman(Npc* _npc);

/// Fetches the collector's default dialog line, to use in the event there is nothing better to be said.
void npc_GetNoDialArnold(Npc* _npc);

/// Fetches the shaman's default dialog line, to use in the event there is nothing better to be said.
void npc_GetNoDialShaman(Npc* _npc);

/// Clears the NPC buffer and frees up allocated resources.
void npc_Clear();