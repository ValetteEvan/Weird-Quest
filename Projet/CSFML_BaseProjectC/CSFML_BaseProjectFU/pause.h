#pragma once
#include "tools.h"
#include "windowManager.h"
#include "List.h"

typedef enum
{
	PAUSE_PERSO = 0,
	PAUSE_QUETE,
	PAUSE_BESTIARE,
	PAUSE_OPTIONS
}PauseMenu;

typedef enum
{
	PAUSE_POISON_SPIDER = 1 << 1,
	PAUSE_SNAKE = 1 << 2,
	PAUSE_MOSQUITO = 1 << 3,
	PAUSE_SEAGULL = 1 << 4,
	PAUSE_CENTIPEDE = 1 << 5,
	PAUSE_VARAN = 1 << 6,
	PAUSE_MOLE = 1 << 7,
	PAUSE_BAT = 1 << 8,
	PAUSE_GOOSE = 1 << 9,
	PAUSE_GOAT = 1 << 10,
	PAUSE_BIG_JOY_DEMON = 1 << 11,
	PAUSE_LITTLE_JOY_DEMON = 1 << 12,
	PAUSE_JOY_GOLEM = 1 << 13,
	PAUSE_BIG_SADNESS_DEMON = 1 << 14,
	PAUSE_LITTLE_SADNESS_DEMON = 1 << 15,
	PAUSE_SADNESS_GOLEM = 1 << 16,
	PAUSE_BIG_SCARE_DEMON = 1 << 17,
	PAUSE_LITTLE_SCARE_DEMON = 1 << 18,
	PAUSE_SCARE_GOLEM = 1 << 19,
	PAUSE_BIG_ANGER_DEMON = 1 << 20,
	PAUSE_LITTLE_ANGER_DEMON = 1 << 21,
	PAUSE_ANGER_GOLEM = 1 << 22,
	PAUSE_NECROMANCER = 1 << 23,
	PAUSE_GOAT_BLACK = 1 << 24,
	PAUSE_UNKNOWN_ENNEMIS = 1 << 25,

}PauseEnnemis;

typedef enum Items Items;
enum Items {
	PAUSE_TOTEM_PIG = 1 << 1,
	PAUSE_TOTEM_OCTO = 1 << 2,
	PAUSE_TOTEM_EAGLE = 1 << 3,
	PAUSE_TOTEM_CHAMELEON = 1 << 4,
	PAUSE_GUARDIAN_STICK = 1 << 5,
	PAUSE_GUARDIAN_RING = 1 << 6,
	PAUSE_JOY_RING = 1 << 7,
	PAUSE_SADNESS_RING = 1 << 8,
	PAUSE_SCARE_RING = 1 << 9,
	PAUSE_ANGER_RING = 1 << 10,
	PAUSE_JOY_FISH = 1 << 11,
	PAUSE_SADNESS_FISH = 1 << 12,
	PAUSE_SCARE_FISH = 1 << 13,
	PAUSE_ANGER_FISH = 1 << 14,
	PAUSE_YOYO = 1 << 15,
	PAUSE_CABORALS = 1 << 16,
	PAUSE_SANDAMANDER = 1 << 17,
	PAUSE_VOLTIGEONS = 1 << 18
};

typedef enum
{
	PAUSE_STICK_RING = 0,
	PAUSE_TOTEM,
	PAUSE_COLECTABLE
}PauseInventaire;

typedef enum
{
	PAUSE_PET= 0,
	PAUSE_MONSTER
}Bestiare;

typedef enum quetePNJ
{
	PAUSE_MENTOR = 0,
	PAUSE_ARNOLD,
	PAUSE_SHAMAN	
}quetePNJ;

typedef struct Itemlist Itemlist;
struct Itemlist {
	Items item;
	int nbObject;
};

stdList* itemList;

static PauseMenu pauseMenu = PAUSE_PERSO;
static Bestiare bestiare = PAUSE_PET;
static quetePNJ pause_queteIdPNJ = PAUSE_MENTOR;
static PauseInventaire pause_inventaire = PAUSE_STICK_RING;
PauseEnnemis pause_EnnemisDiscovered;
Items pause_ItemsDiscovered;
/// <summary>
/// init all the variables necessary to pause the game
/// </summary>
/// <param name="_window">the current window</param>
void initPause(Window* _window);

/// A NE PAS TOUCHER S'IL VOUS PLAIT DANS VOS AUTRE FICHIER
void pause_AddItem(Items _item);

/// <summary>
/// update the pause menu
/// </summary>
/// <param name="_window">the current window</param>
void updatePause(Window* _window);

/// <summary>
/// display the pause menu
/// </summary>
/// <param name="_window">the current window</param>
void displayPause(Window* _window);

/// <summary>
/// deinit the pause menu
/// </summary>
void deinitPause();

/// Permet de mettre a jour le nombre d'item dans l'inventaire ou de l'ajouter si il n'était pas présent avant
/// <param Items _item : l'item a ajouter ou a mettre a jour
void pause_itemRecup(Items _item);

/// Permet de supprimer un certain nombre d'item de l'inventaire et de le supprimer si il n'y en a plus
/// <param Items _item : l'item a supprimer
/// <param int _nbItemToSuppr : le nombre d'item a supprimer
void pause_itemRemove(Items _item, int _nbItemToSuppr);

/// Permet de récupérer le nombre d'un certain item dans l'inventaire
/// <param Items _item : l'item a récupérer
int pause_getNbItem(Items _item);

quetePNJ pause_getQueteIdPNJ();