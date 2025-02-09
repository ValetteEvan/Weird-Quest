#include "editorHUD.h"
#include "math.h"
#include "shaderManager.h"
#include "editor.h"

sfText* editorLayerText;
sfText* editorMapText;
sfText* editorMapSizeText;
sfText* editorTabsText;
sfText* editorButtonText;
sfText* editorLightButtonText;
sfText* editorMapTextId;
sfText* editorMapTextSave;
sfText* editorDetailButton;

sfRectangleShape* editorHUDRect;
sfRectangleShape* editorTabsButton;
sfRectangleShape* editorButton;
sfRectangleShape* squareMenuLight;
sfRectangleShape* editorHudSquareFade;

sfSprite* editorHudSprite;

EditorState editorHudState;
EditorTilesLayer editorLayer;
EditorEntity editorHudEntity;
EditorEffects editorHudEffects;
NpcId editorHudPnjChoice;
EditorLightForm lightForm;
EditorGroundType editorHudGroundType;
EditorWallType editorHudWallType;
EnemiesType editorHudMobType;
Biomes editorCurrentBiome;

PropsType editorHudPropsType;
int editorHudCurrentProps;

sfBool togglePnjChoice;
sfBool toggleMobChoice;
sfBool toggleLight;
sfBool toggleParticles;
sfBool wantMapSave;

sfVector2f lightSize;
char lightSizeValue[19];
char coneAngleValue[19];
char coneRotateValue[19];
char circleStrengthValue[19];
int lightColorR;
int lightColorG;
int lightColorB;
float normR;
float normG;
float normB;
float lightAngle;
float lightRotate;
float lightStrength;
sfVector2f lightPosSliderR;
sfVector2f lightPosSliderG;
sfVector2f lightPosSliderB;
sfColor lightColorPreview;

int firstOrSecond;
char strSubLayer[30] = "1";
char strBiome[30] = "All";
char strCurrentBiome[30] = "All";
char strValueRateT1[30] = "100";
char strValueRateT2[30] = "25";
char strValueRateT3[30] = "25";
char strValueRateT4[30] = "25";
char stringSize[40];
char stringBrushSize[10];
char stringViewSpeed[30];
char stringEnlargement[30];
char stringMobRotation[30];

int valRateT1;
int valRateT2;
int valRateT3;
int valRateT4;

float editorHudViewSpeed;
float editorHudMobRotation;

int editorHudMapEnlargement;

void initEditorHUD(Window* _window)
{
	editorLayerText = sfText_create();
	sfText_setFont(editorLayerText, GetFont("fEdit"));
	sfText_setPosition(editorLayerText, vector2f(20.f, 10.f));
	sfText_setString(editorLayerText, "Layer : Ground 1");
	sfText_setOutlineThickness(editorLayerText, 2.f);

	editorMapText = sfText_create();
	sfText_setFont(editorMapText, GetFont("fEdit"));
	sfText_setPosition(editorMapText, vector2f(20.f, 40.f));
	sfText_setString(editorMapText, "Map : 1 (LShift to save)");
	sfText_setOutlineThickness(editorMapText, 2.f);

	editorMapSizeText = sfText_create();
	sfText_setFont(editorMapSizeText, GetFont("fEdit"));
	sfText_setPosition(editorMapSizeText, vector2f(20.f, 70.f));
	sfText_setOutlineThickness(editorMapSizeText, 2.f);

	editorTabsText = sfText_create();
	sfText_setFont(editorTabsText, GetFont("fEdit"));
	sfText_setPosition(editorTabsText, vector2f(20.f, 70.f));
	sfText_setOutlineThickness(editorTabsText, 2.f);
	sfText_setRotation(editorTabsText, 90.f);

	editorButtonText = sfText_create();
	sfText_setFont(editorButtonText, GetFont("fEdit"));
	sfText_setOutlineThickness(editorButtonText, 4.f);

	editorLightButtonText = sfText_create();
	sfText_setFont(editorLightButtonText, GetFont("fEdit"));
	sfText_setOutlineThickness(editorLightButtonText, 4.f);

	editorMapTextId = sfText_create();
	sfText_setFont(editorMapTextId, GetFont("fEdit"));
	sfText_setString(editorMapTextId, "Map 1 :");
	sfText_setOutlineThickness(editorMapTextId, 4.f);

	editorMapTextSave = sfText_create();
	sfText_setFont(editorMapTextSave, GetFont("fEdit"));
	sfText_setString(editorMapTextSave, "Press to Save");
	sfText_setOutlineThickness(editorMapTextSave, 4.f);

	editorDetailButton = sfText_create();
	sfText_setFont(editorDetailButton, GetFont("fEdit"));
	sfText_setString(editorDetailButton, "-");
	sfText_setCharacterSize(editorDetailButton, 20);
	sfText_setOutlineThickness(editorDetailButton, 3.f);

	editorHUDRect = sfRectangleShape_create();
	sfRectangleShape_setSize(editorHUDRect, vector2f(220.f, 1080.f));
	sfRectangleShape_setFillColor(editorHUDRect, color(177, 227, 212, 180));
	sfRectangleShape_setPosition(editorHUDRect, vector2f(1700.f, 0.f));

	editorButton = sfRectangleShape_create();
	sfRectangleShape_setSize(editorButton, vector2f(120.f, 120.f));
	sfRectangleShape_setFillColor(editorButton, sfWhite);
	sfRectangleShape_setOutlineThickness(editorButton, 4.f);
	sfRectangleShape_setOutlineColor(editorButton, sfBlack);

	editorTabsButton = sfRectangleShape_create();
	sfRectangleShape_setSize(editorTabsButton, vector2f(30.f, 130.f));

	editorHudSquareFade = sfRectangleShape_create();
	sfRectangleShape_setSize(editorHudSquareFade, vector2f(1920.f, 1080.f));
	sfRectangleShape_setFillColor(editorHudSquareFade, color(0, 0, 0, 150));

	editorHudSprite = sfSprite_create();
	sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_ground_sand"), sfTrue);

	squareMenuLight = sfRectangleShape_create();
	sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 30.f));
	sfRectangleShape_setOutlineThickness(squareMenuLight, 1.f);
	sfRectangleShape_setOutlineColor(squareMenuLight, sfCyan);
	lightPosSliderR = vector2f(1755.f, 300.f);
	lightPosSliderG = vector2f(1883.f, 350.f);
	lightPosSliderB = vector2f(1883.f, 400.f);

	editorHudState = EDITOR_STATE_TILES;
	editorLayer = EDITOR_LAYER_GROUND;
	editorHudEntity = EDITOR_ENTITY_PLAYER;
	editorHudEffects = 0;
	editorHudPnjChoice = EDITOR_PNJ_MENTOR;
	lightForm = EDITOR_LIGHT_CIRCLE;
	editorHudGroundType = EDITOR_GROUND_SAND;
	editorHudWallType = EDITOR_WALL_EXTERIOR;
	editorHudMobType = EN_YOYO_SPIDER;
	editorCurrentBiome = getCurrentBiome();
	editorHudPropsType = PROPS_TYPE_GENERAL;
	editorHudCurrentProps = 0;

	togglePnjChoice = sfFalse;
	toggleMobChoice = sfFalse;
	toggleLight = sfFalse;
	toggleParticles = sfFalse;
	wantMapSave = sfFalse;

	lightColorR = 255;
	lightColorG = 255;
	lightColorB = 255;
	lightSize = vector2f(75.f, 75.f);
	normR = 0.f;
	normG = 0.f;
	normB = 0.f;
	lightAngle = 45.f;
	lightRotate = 0.f;
	lightStrength = 1.f;

	firstOrSecond = 1;

	valRateT1 = 25;
	valRateT2 = 25;
	valRateT3 = 25;
	valRateT4 = 25;

	editorHudViewSpeed = 600.f;
	editorHudMobRotation = 0.f;

	editorHudMapEnlargement = 1;

	switch (editorCurrentBiome)
	{
	case BIOME_ALL: sprintf(strBiome, "%s", "All"); break;
	case BIOME_FOREST: sprintf(strBiome, "%s", "Forest"); break;
	case BIOME_CAVE: sprintf(strBiome, "%s", "Cave"); break;
	case BIOME_MOUNTAIN: sprintf(strBiome, "%s", "Moutain"); break;
	case BIOME_BEACH: sprintf(strBiome, "%s", "Beach"); break;
	case BIOME_GRASSLAND: sprintf(strBiome, "%s", "Grassland"); break;
	case BIOME_JOY_DUNGEON: sprintf(strBiome, "%s", "Joy dungeon"); break;
	case BIOME_SADNESS_DUNGEON: sprintf(strBiome, "%s", "Sadness dungeon"); break;
	case BIOME_FEAR_DUNGEON: sprintf(strBiome, "%s", "Fear dungeon"); break;
	case BIOME_ANGRY_DUNGEON: sprintf(strBiome, "%s", "Angry dungeon"); break;
	default: sprintf(strBiome, "%s", "-default-"); break;
	}
}

void updateEditorHUD(Window* _window, float _dt)
{
	static float timerInputLight = 0.f;
	timerInputLight += _dt;

	static float timer = 0.f;
	timer += _dt;

	static float timerSave = 0.f;
	timerSave += _dt;

	sprintf(stringSize, "Map size : \nWidth : %d\nHeight : %d", map.mapSize.x, map.mapSize.y);

	switch (getCurrentBiome())
	{
		case BIOME_ALL: sprintf(strCurrentBiome, "Biome :\n%s", "All"); break;
		case BIOME_FOREST: sprintf(strCurrentBiome, "Biome :\n%s", "Forest"); break;
		case BIOME_CAVE: sprintf(strCurrentBiome, "Biome :\n%s", "Cave"); break;
		case BIOME_MOUNTAIN: sprintf(strCurrentBiome, "Biome :\n%s", "Moutain"); break;
		case BIOME_BEACH: sprintf(strCurrentBiome, "Biome :\n%s", "Beach"); break;
		case BIOME_GRASSLAND: sprintf(strCurrentBiome, "Biome :\n%s", "Grassland"); break;
		case BIOME_JOY_DUNGEON: sprintf(strCurrentBiome, "Biome :\n%s", "Joy dungeon"); break;
		case BIOME_SADNESS_DUNGEON: sprintf(strCurrentBiome, "Biome :\n%s", "Sadness dungeon"); break;
		case BIOME_FEAR_DUNGEON: sprintf(strCurrentBiome, "Biome :\n%s", "Fear dungeon"); break;
		case BIOME_ANGRY_DUNGEON: sprintf(strCurrentBiome, "Biome :\n%s", "Angry dungeon"); break;
		default: sprintf(strCurrentBiome, "Biome :\n%s", "-default-"); break;
	}

	if (leftClick(_window->renderWindow))
	{
		sfVector2f mousePos = getfMousePos(_window->renderWindow);

		/// Choix onglets
		sfFloatRect tilesRect = FlRect(1670.f, 70.f, 30.f, 130.f);
		sfFloatRect mobsRect = FlRect(1670.f, 210.f, 30.f, 130.f);
		sfFloatRect propsRect = FlRect(1670.f, 350.f, 30.f, 130.f);
		sfFloatRect effectsRect = FlRect(1670.f, 490.f, 30.f, 130.f);
		sfFloatRect cfgEditorRect = FlRect(1670.f, 630.f, 30.f, 140.f);

		if (sfFloatRect_contains(&tilesRect, mousePos.x, mousePos.y))
		{
			editorHudState = EDITOR_STATE_TILES;
			toggleLight = sfFalse;
			toggleMobChoice = sfFalse;
			togglePnjChoice = sfFalse;
			toggleParticles = sfFalse;
		}
		else if (sfFloatRect_contains(&mobsRect, mousePos.x, mousePos.y))
		{
			editorHudState = EDITOR_STATE_ENTITY;
			toggleLight = sfFalse;
			toggleMobChoice = sfFalse;
			togglePnjChoice = sfFalse;
			toggleParticles = sfFalse;
		}
		else if (sfFloatRect_contains(&propsRect, mousePos.x, mousePos.y))
		{
			editorHudState = EDITOR_STATE_PROPS;
			toggleLight = sfFalse;
			toggleMobChoice = sfFalse;
			togglePnjChoice = sfFalse;
			toggleParticles = sfFalse;
		}
		else if (sfFloatRect_contains(&effectsRect, mousePos.x, mousePos.y))
		{
			editorHudState = EDITOR_STATE_EFFECTS;
			toggleLight = sfFalse;
			toggleMobChoice = sfFalse;
			togglePnjChoice = sfFalse;
			toggleParticles = sfFalse;
		}
		else if (sfFloatRect_contains(&cfgEditorRect, mousePos.x, mousePos.y))
		{
			editorHudState = EDITOR_STATE_CFGEDITOR;
			toggleLight = sfFalse;
			toggleMobChoice = sfFalse;
			togglePnjChoice = sfFalse;
			toggleParticles = sfFalse;
		}
	}
	/// Action en fonction de l'onglet actif
	if (editorHudState == EDITOR_STATE_TILES)
	{
		sfRectangleShape_setFillColor(editorHUDRect, color(177, 227, 212, 180));
		if (leftClick(_window->renderWindow) && timerInputLight > 0.1f)
		{
			sfVector2f mousePos = getfMousePos(_window->renderWindow);
			sfFloatRect buttonOpenTS = FlRect(1720.f, 10.f, 90.f, 40.f);
			sfFloatRect buttonLayerChoiceR = FlRect(1710.f, 100.f, 30.f, 20.f);
			sfFloatRect buttonLayerChoiceL = FlRect(1750.f, 100.f, 30.f, 20.f);
			sfFloatRect buttonSubLayerChoiceR = FlRect(1710.f, 190.f, 30.f, 20.f);
			sfFloatRect buttonSubLayerChoiceL = FlRect(1750.f, 190.f, 30.f, 20.f);
			sfFloatRect buttonBiomeChoiceR = FlRect(1710.f, 300.f, 30.f, 20.f);
			sfFloatRect buttonBiomeChoiceL = FlRect(1750.f, 300.f, 30.f, 20.f);
			sfFloatRect buttonChangeBiome = FlRect(1710.f, 330.f, 120.f, 30.f);

			if (sfFloatRect_contains(&buttonOpenTS, mousePos.x, mousePos.y))
			{
				if (editorHudState == EDITOR_STATE_TILES)
					editorHudState = EDITOR_STATE_TILES_HUD;
				else
					editorHudState = EDITOR_STATE_TILES;
			}
			if (sfFloatRect_contains(&buttonLayerChoiceR, mousePos.x, mousePos.y))
			{
				editorLayer--;
				if (editorLayer < 0)
				{
					editorLayer = 2;
				}
			}
			if (sfFloatRect_contains(&buttonLayerChoiceL, mousePos.x, mousePos.y))
			{
				editorLayer++;
				if (editorLayer > 2)
				{
					editorLayer = 0;
				}
			}
			if (sfFloatRect_contains(&buttonSubLayerChoiceR, mousePos.x, mousePos.y))
			{
				firstOrSecond--;
				if (firstOrSecond < 1)
				{
					firstOrSecond = 2;
				}
			}
			if (sfFloatRect_contains(&buttonSubLayerChoiceL, mousePos.x, mousePos.y))
			{
				firstOrSecond++;
				if (firstOrSecond > 2)
				{
					firstOrSecond = 1;
				}
			}
			if (sfFloatRect_contains(&buttonBiomeChoiceR, mousePos.x, mousePos.y))
			{
				editorCurrentBiome--;
				if (editorCurrentBiome < BIOME_ALL)
					editorCurrentBiome = BIOME_ANGRY_DUNGEON;
			}
			if (sfFloatRect_contains(&buttonBiomeChoiceL, mousePos.x, mousePos.y))
			{
				editorCurrentBiome++;
				if (editorCurrentBiome > BIOME_ANGRY_DUNGEON)
					editorCurrentBiome = BIOME_ALL;
			}
			if (sfFloatRect_contains(&buttonChangeBiome, mousePos.x, mousePos.y))
			{
				if (getCurrentBiome() != editorCurrentBiome)
					setCurrentBiome(editorCurrentBiome);
			}
			sprintf(strSubLayer, "%d", firstOrSecond);
			switch (editorCurrentBiome)
			{
				case BIOME_ALL: sprintf(strBiome, "%s", "All"); break;
				case BIOME_FOREST: sprintf(strBiome, "%s", "Forest"); break;
				case BIOME_CAVE: sprintf(strBiome, "%s", "Cave"); break;
				case BIOME_MOUNTAIN: sprintf(strBiome, "%s", "Moutain"); break;
				case BIOME_BEACH: sprintf(strBiome, "%s", "Beach"); break;
				case BIOME_GRASSLAND: sprintf(strBiome, "%s", "Grassland"); break;
				case BIOME_JOY_DUNGEON: sprintf(strBiome, "%s", "Joy dungeon"); break;
				case BIOME_SADNESS_DUNGEON: sprintf(strBiome, "%s", "Sadness dungeon"); break;
				case BIOME_FEAR_DUNGEON: sprintf(strBiome, "%s", "Fear dungeon"); break;
				case BIOME_ANGRY_DUNGEON: sprintf(strBiome, "%s", "Angry dungeon"); break;
				default: sprintf(strBiome, "%s", "-default-"); break;
			}
			timerInputLight = 0.f;
		}
	}
	else if (editorHudState == EDITOR_STATE_ENTITY)
	{
		sfRectangleShape_setFillColor(editorHUDRect, color(177, 211, 227, 180));

		if (leftClick(_window->renderWindow) && timerInputLight > 0.1f)
		{
			sfVector2f mousePos = getfMousePos(_window->renderWindow);
			sfFloatRect playerRect = FlRect(1750.f, 100.f, 120.f, 120.f);
			sfFloatRect pnjRect = FlRect(1750.f, 250.f, 120.f, 120.f);
			sfFloatRect pnjMentorRect = FlRect(1750.f, 550.f, 120.f, 120.f);
			sfFloatRect pnjArnoldRect = FlRect(1750.f, 700.f, 120.f, 120.f);
			sfFloatRect pnjShamanRect = FlRect(1750.f, 850.f, 120.f, 120.f);
			sfFloatRect statueRect = FlRect(1750.f, 900.f, 120.f, 120.f);
			sfFloatRect mobsRect = FlRect(1750.f, 400.f, 120.f, 120.f);
			sfFloatRect mobChoicePlus = FlRect(1790.f, 630.f, 30.f, 20.f);
			sfFloatRect mobChoiceMinus = FlRect(1750.f, 630.f, 30.f, 20.f);
			sfFloatRect mobRotationPlus = FlRect(1750.f, 720.f, 30.f, 20.f);
			sfFloatRect mobRotationMinus = FlRect(1710.f, 720.f, 30.f, 20.f);

			if (sfFloatRect_contains(&playerRect, mousePos.x, mousePos.y))
			{
				editorHudEntity = EDITOR_ENTITY_PLAYER;
				toggleMobChoice = sfFalse;
				togglePnjChoice = sfFalse;
			}
			else if (sfFloatRect_contains(&pnjRect, mousePos.x, mousePos.y))
			{
				editorHudEntity = EDITOR_ENTITY_PNJ;
				toggleMobChoice = sfFalse;
				togglePnjChoice = sfTrue;
			}
			else if (sfFloatRect_contains(&mobsRect, mousePos.x, mousePos.y))
			{
				editorHudEntity = EDITOR_ENTITY_MOBS;
				toggleMobChoice = sfTrue;
				togglePnjChoice = sfFalse;
			}
			if (editorHudEntity == EDITOR_ENTITY_PNJ)
			{
				if (sfFloatRect_contains(&pnjMentorRect, mousePos.x, mousePos.y))
				{
					editorHudPnjChoice = EDITOR_PNJ_MENTOR;
				}
				else if (sfFloatRect_contains(&pnjArnoldRect, mousePos.x, mousePos.y))
				{
					editorHudPnjChoice = EDITOR_PNJ_ARNOLD;
				}
				else if (sfFloatRect_contains(&pnjShamanRect, mousePos.x, mousePos.y))
				{
					editorHudPnjChoice = EDITOR_PNJ_SHAMAN;
				}
				else if (sfFloatRect_contains(&statueRect, mousePos.x, mousePos.y))
				{
					editorHudPnjChoice = EDITOR_PNJ_SHAMAN;
				}
			}
			if (editorHudEntity == EDITOR_ENTITY_MOBS)
			{
				if (sfFloatRect_contains(&mobChoiceMinus, mousePos.x, mousePos.y))
				{
					editorHudMobType--;
					if (editorHudMobType < EN_YOYO_SPIDER)
						editorHudMobType = EN_SEAGULL;
				}
				if (sfFloatRect_contains(&mobChoicePlus, mousePos.x, mousePos.y))
				{
					editorHudMobType++;
					if (editorHudMobType > EN_SEAGULL)
						editorHudMobType = EN_YOYO_SPIDER;
				}
				if (editorHudMobType == EN_CHAMELEON || editorHudMobType == EN_SCOLOPENDRE)
				{
					if (sfFloatRect_contains(&mobRotationMinus, mousePos.x, mousePos.y))
					{
						editorHudMobRotation -= 90.f;
						if (editorHudMobRotation < 0.f)
						{
							editorHudMobRotation = 270.f;
						}
					}
					if (sfFloatRect_contains(&mobRotationPlus, mousePos.x, mousePos.y))
					{
						editorHudMobRotation += 90.f;
						if (editorHudMobRotation > 270.f)
						{
							editorHudMobRotation = 0.f;
						}
					}
				}
				else
					editorHudMobRotation = 0.f;
			}

			timerInputLight = 0.f;
		}
	}
	else if (editorHudState == EDITOR_STATE_PROPS)
	{
		sfRectangleShape_setFillColor(editorHUDRect, color(177, 178, 227, 180));

		if (leftClick(_window->renderWindow) && timerInputLight > 0.1f)
		{
			sfVector2f mousePos = getfMousePos(_window->renderWindow);
			sfFloatRect buttonPropsTypeR = FlRect(1710.f, 100.f, 30.f, 20.f);
			sfFloatRect buttonPropsTypeL = FlRect(1750.f, 100.f, 30.f, 20.f);
			sfFloatRect buttonChoicePropsR = FlRect(1710.f, 190.f, 30.f, 20.f);
			sfFloatRect buttonChoicePropsL = FlRect(1750.f, 190.f, 30.f, 20.f);
			sfFloatRect buttonChoiceLightOrShadowR = FlRect(1710.f, 390.f, 30.f, 20.f);
			sfFloatRect buttonChoiceLightOrShadowL = FlRect(1750.f, 390.f, 30.f, 20.f);

			int max = PROPS_DUNGEON_ENTRANCE_PRESSURE_PLATE_ANGRY;
			Biomes biome = getCurrentBiome();
			if (biome == BIOME_JOY_DUNGEON || biome == BIOME_SADNESS_DUNGEON || biome == BIOME_FEAR_DUNGEON || biome == BIOME_ANGRY_DUNGEON)
				max = PROPS_DUNGEON_BIG_CHEST;

			if (sfFloatRect_contains(&buttonPropsTypeR, mousePos.x, mousePos.y))
			{
				editorHudPropsType--;
				if (editorHudPropsType < 1)
				{
					editorHudPropsType = 5;
				}
			}
			if (sfFloatRect_contains(&buttonPropsTypeL, mousePos.x, mousePos.y))
			{
				editorHudPropsType++;
				if (editorHudPropsType > 5)
				{
					editorHudPropsType = 1;
				}
			}
			if (sfFloatRect_contains(&buttonChoicePropsR, mousePos.x, mousePos.y))
			{
				editorHudCurrentProps--;
			}
			if (editorHudPropsType == PROPS_TYPE_GENERAL)
			{
				if (editorHudCurrentProps < PROPS_GENERAL_FOUNTAIN)
					editorHudCurrentProps = PROPS_GENERAL_FLOATING_NOTES;
			}
			else if (editorHudPropsType == PROPS_TYPE_DUNGEON)
			{
				if (editorHudCurrentProps < PROPS_DUNGEON_DOOR_TOP)
					editorHudCurrentProps = max;
			}
			else if (editorHudPropsType == PROPS_TYPE_FOREST)
			{
				if (editorHudCurrentProps < PROPS_FOREST_TREE_1)
					editorHudCurrentProps = PROPS_FOREST_WATERFALL;
			}
			else if (editorHudPropsType == PROPS_TYPE_BEACH)
			{
				if (editorHudCurrentProps < PROPS_BEACH_PALM_1)
					editorHudCurrentProps = PROPS_BEACH_ROCK_5;
			}
			else if (editorHudPropsType == PROPS_TYPE_MOUTAIN)
			{
				if (editorHudCurrentProps < PROPS_MOUTAIN_BIG_CRISTAL_1)
					editorHudCurrentProps = PROPS_MOUTAIN_DOOR_LIGHT_TOP;
			}
			if (sfFloatRect_contains(&buttonChoicePropsL, mousePos.x, mousePos.y))
			{
				editorHudCurrentProps++;
			}
			if (editorHudPropsType == PROPS_TYPE_GENERAL)
			{
				if (editorHudCurrentProps > PROPS_GENERAL_FLOATING_NOTES)
					editorHudCurrentProps = PROPS_GENERAL_FOUNTAIN;
			}
			else if (editorHudPropsType == PROPS_TYPE_DUNGEON)
			{
				if (editorHudCurrentProps > max)
					editorHudCurrentProps = PROPS_DUNGEON_DOOR_TOP;
			}
			else if (editorHudPropsType == PROPS_TYPE_FOREST)
			{
				if (editorHudCurrentProps > PROPS_FOREST_WATERFALL)
					editorHudCurrentProps = PROPS_FOREST_TREE_1;
			}
			else if (editorHudPropsType == PROPS_TYPE_BEACH)
			{
				if (editorHudCurrentProps > PROPS_BEACH_ROCK_5)
					editorHudCurrentProps = PROPS_BEACH_PALM_1;
			}
			else if (editorHudPropsType == PROPS_TYPE_MOUTAIN)
			{
				if (editorHudCurrentProps > PROPS_MOUTAIN_DOOR_LIGHT_TOP)
					editorHudCurrentProps = PROPS_MOUTAIN_BIG_CRISTAL_1;
			}

			if (editorHudPropsType == PROPS_TYPE_MOUTAIN && (editorHudCurrentProps == PROPS_MOUTAIN_DOOR_FRAME || editorHudCurrentProps == PROPS_MOUTAIN_DOOR_FRAME_TOP))
			{
				if (sfFloatRect_contains(&buttonChoiceLightOrShadowR, mousePos.x, mousePos.y))
				{
					setLightOrShadow(!getLightOrShadow());
				}
				if (sfFloatRect_contains(&buttonChoiceLightOrShadowL, mousePos.x, mousePos.y))
				{
					setLightOrShadow(!getLightOrShadow());
				}
			}

			timerInputLight = 0.f;
		}
	}
	else if (editorHudState == EDITOR_STATE_EFFECTS)
	{
		if (leftClick(_window->renderWindow) && timerInputLight > 0.1f)
		{
			sfVector2f mousePos = getfMousePos(_window->renderWindow);
			sfFloatRect lightsRect = FlRect(1750.f, 100.f, 120.f, 120.f);
			//	sfFloatRect particlesRect = FlRect(1750.f, 250.f, 120.f, 120.f);
			if (sfFloatRect_contains(&lightsRect, mousePos.x, mousePos.y) && !toggleParticles)
			{
				editorHudEffects = EDITOR_EFFECTS_LIGHT;
				toggleLight = sfTrue;
				toggleParticles = sfFalse;
			}
			/*else if (sfFloatRect_contains(&particlesRect, mousePos.x, mousePos.y) && !toggleLight)
			{
				editorHudEffects = EDITOR_EFFECTS_PARTICLES;
				toggleParticles = sfTrue;
				toggleLight = sfFalse;
			}*/

			timerInputLight = 0.f;
		}
		if (editorHudEffects == EDITOR_EFFECTS_LIGHT)
		{
			sfVector2f mousePos = getfMousePos(_window->renderWindow);
			sfFloatRect sliderR = FlRect(lightPosSliderR.x - 20.f, lightPosSliderR.y - 5.f, 35.f, 25.f);
			sfFloatRect sliderG = FlRect(lightPosSliderG.x - 20.f, lightPosSliderG.y - 5.f, 35.f, 25.f);
			sfFloatRect sliderB = FlRect(lightPosSliderB.x - 20.f, lightPosSliderB.y - 5.f, 35.f, 25.f);
			sfFloatRect sizeXPlus = FlRect(1810.f, 600.f, 30.f, 20.f);
			sfFloatRect sizeYPlus = FlRect(1810.f, 650.f, 30.f, 20.f);
			sfFloatRect sizeXMinus = FlRect(1760.f, 600.f, 30.f, 20.f);
			sfFloatRect sizeYMinus = FlRect(1760.f, 650.f, 30.f, 20.f);
			sfFloatRect switchFormLeft = FlRect(1700.f, 550.f, 30.f, 20.f);
			sfFloatRect switchFormRight = FlRect(1740.f, 550.f, 30.f, 20.f);
			sfFloatRect anglePlus = FlRect(1810.f, 750.f, 30.f, 20.f);
			sfFloatRect angleMinus = FlRect(1740.f, 750.f, 30.f, 20.f);
			sfFloatRect rotatePlus = FlRect(1810.f, 820.f, 30.f, 20.f);
			sfFloatRect rotateMinus = FlRect(1740.f, 820.f, 30.f, 20.f);
			sfFloatRect strengthPlus = FlRect(1810.f, 750.f, 30.f, 20.f);
			sfFloatRect strengthMinus = FlRect(1760.f, 750.f, 30.f, 20.f);

			if (leftClick(_window->renderWindow))
			{
				if (sfFloatRect_contains(&sliderR, mousePos.x, mousePos.y))
				{
					lightPosSliderR.x = mousePos.x;
					if (lightPosSliderR.x < 1755.f)
						lightPosSliderR.x = 1755.f;
					if (lightPosSliderR.x > 1883.f)
						lightPosSliderR.x = 1883.f;
				}
				if (sfFloatRect_contains(&sliderG, mousePos.x, mousePos.y))
				{
					lightPosSliderG.x = mousePos.x;
					if (lightPosSliderG.x < 1755.f && lightPosSliderG.x < 1884.f)
						lightPosSliderG.x = 1755.f;
					if (lightPosSliderG.x > 1883.f)
						lightPosSliderG.x = 1883.f;
				}
				if (sfFloatRect_contains(&sliderB, mousePos.x, mousePos.y))
				{
					lightPosSliderB.x = mousePos.x;
					if (lightPosSliderB.x < 1755.f)
						lightPosSliderB.x = 1755.f;
					if (lightPosSliderB.x > 1883.f)
						lightPosSliderB.x = 1883.f;
				}
				if (timerInputLight > 0.05f)
				{
					if (sfFloatRect_contains(&switchFormLeft, mousePos.x, mousePos.y))
					{
						lightForm--;
						if (lightForm < 0)
						{
							lightForm = 2;
						}
					}
					if (sfFloatRect_contains(&switchFormRight, mousePos.x, mousePos.y))
					{
						lightForm++;
						if (lightForm > 2)
						{
							lightForm = 0;
						}
					}
					//timerInputLight = 0.f;
				}
			}
			// printf("%f\n", timerInputLight);
			normR = (lightPosSliderR.x - 1755) / 128;
			normG = (lightPosSliderG.x - 1755) / 128;
			normB = (lightPosSliderB.x - 1755) / 128;
			lightColorPreview.r = 255 * normR;
			lightColorPreview.g = 255 * normG;
			lightColorPreview.b = 255 * normB;
			lightColorPreview.a = 255;
			if (lightForm == EDITOR_LIGHT_CIRCLE)
			{
				if (leftClick(_window->renderWindow) && timerInputLight > 0.05f)
				{
					if (sfFloatRect_contains(&sizeXPlus, mousePos.x, mousePos.y))
					{
						lightSize.x++;
					}
					if (sfFloatRect_contains(&sizeXMinus, mousePos.x, mousePos.y))
					{
						if (lightSize.x > 0)
							lightSize.x--;
					}
					if (sfFloatRect_contains(&strengthPlus, mousePos.x, mousePos.y))
					{
						if (lightStrength < 2.0)
							lightStrength += 0.1;
					}
					if (sfFloatRect_contains(&strengthMinus, mousePos.x, mousePos.y))
					{
						if (lightStrength > 0.1)
							lightStrength -= 0.1;
					}
					timerInputLight = 0.f;
				}
			}
			else if (lightForm == EDITOR_LIGHT_RECTANGLE)
			{
				rotatePlus = FlRect(1810.f, 820.f, 30.f, 20.f);
				rotateMinus = FlRect(1760.f, 820.f, 30.f, 20.f);
				if (leftClick(_window->renderWindow) && timerInputLight > 0.05f)
				{
					if (sfFloatRect_contains(&sizeXPlus, mousePos.x, mousePos.y))
					{
						lightSize.x++;
					}
					if (sfFloatRect_contains(&sizeXMinus, mousePos.x, mousePos.y))
					{
						if (lightSize.x > 0)
							lightSize.x--;
					}
					if (sfFloatRect_contains(&sizeYPlus, mousePos.x, mousePos.y))
					{
						lightSize.y++;
					}
					if (sfFloatRect_contains(&sizeYMinus, mousePos.x, mousePos.y))
					{
						if (lightSize.y > 0)
							lightSize.y--;
					}
					if (sfFloatRect_contains(&strengthPlus, mousePos.x, mousePos.y))
					{
						if (lightStrength < 2.0)
							lightStrength += 0.1;
					}
					if (sfFloatRect_contains(&strengthMinus, mousePos.x, mousePos.y))
					{
						if (lightStrength > 0.1)
							lightStrength -= 0.1;
					}
					if (sfFloatRect_contains(&rotatePlus, mousePos.x, mousePos.y))
					{
						lightRotate += 5;
						if (lightRotate >= 361.f)
							lightRotate = 0.f;
					}
					if (sfFloatRect_contains(&rotateMinus, mousePos.x, mousePos.y))
					{
						lightRotate -= 5;
						if (lightRotate <= -1.f)
							lightRotate = 360.f;
					}
					timerInputLight = 0.f;
				}
			}
			else if (lightForm == EDITOR_LIGHT_CONE)
			{
				strengthPlus = FlRect(1810.f, 900.f, 30.f, 20.f);
				strengthMinus = FlRect(1740.f, 900.f, 30.f, 20.f);
				if (leftClick(_window->renderWindow) && timerInputLight > 0.05f)
				{
					if (sfFloatRect_contains(&sizeXPlus, mousePos.x, mousePos.y))
					{
						lightSize.y++;
					}
					if (sfFloatRect_contains(&sizeXMinus, mousePos.x, mousePos.y))
					{
						if (lightSize.y > 0)
							lightSize.y--;
					}
					if (sfFloatRect_contains(&anglePlus, mousePos.x, mousePos.y))
					{
						lightAngle += 5;
						if (lightAngle >= 361.f)
							lightAngle = 0.f;
					}
					if (sfFloatRect_contains(&angleMinus, mousePos.x, mousePos.y))
					{
						lightAngle -= 5;
						if (lightAngle <= -1.f)
							lightAngle = 360.f;
					}
					if (sfFloatRect_contains(&rotatePlus, mousePos.x, mousePos.y))
					{
						lightRotate += 5;
						if (lightRotate >= 361.f)
							lightRotate = 0.f;
					}
					if (sfFloatRect_contains(&rotateMinus, mousePos.x, mousePos.y))
					{
						lightRotate -= 5;
						if (lightRotate <= -1.f)
							lightRotate = 360.f;
					}
					if (sfFloatRect_contains(&strengthPlus, mousePos.x, mousePos.y))
					{
						if (lightStrength < 2.0)
							lightStrength += 0.1;
					}
					if (sfFloatRect_contains(&strengthMinus, mousePos.x, mousePos.y))
					{
						if (lightStrength > 0.1)
							lightStrength -= 0.1;
					}
					timerInputLight = 0.f;
				}
			}

			sprintf_s(lightSizeValue, 19, "SIZE : %.0f / %.0f", lightSize.x, lightSize.y);
			sprintf_s(coneAngleValue, 19, "ANGLE : %.0f", lightAngle);
			sprintf_s(coneRotateValue, 19, "ROTATION : %.0f", lightRotate);
			sprintf_s(circleStrengthValue, 19, "STRENGTH : %.1f", lightStrength);
		}
		else if (editorHudEffects == EDITOR_EFFECTS_PARTICLES)
		{

		}
		sfRectangleShape_setFillColor(editorHUDRect, color(207, 177, 227, 180));
	}
	else if (editorHudState == EDITOR_STATE_CFGEDITOR)
	{
		sfVector2f mousePos = getfMousePos(_window->renderWindow);
		if (leftClick(_window->renderWindow) && timerInputLight > 0.1f)
		{
			sfFloatRect buttonLayerChoiceR = FlRect(1710.f, 100.f, 30.f, 20.f);
			sfFloatRect buttonLayerChoiceL = FlRect(1750.f, 100.f, 30.f, 20.f);
			sfFloatRect buttonWidthPlus = FlRect(1750.f, 190.f, 30.f, 20.f);
			sfFloatRect buttonWidthMinus = FlRect(1710.f, 190.f, 30.f, 20.f);
			sfFloatRect buttonHeightPlus = FlRect(1850.f, 190.f, 30.f, 20.f);
			sfFloatRect buttonHeightMinus = FlRect(1810.f, 190.f, 30.f, 20.f);
			sfFloatRect buttonBrushPlus = FlRect(1750.f, 360.f, 30.f, 20.f);
			sfFloatRect buttonBrushMinus = FlRect(1710.f, 360.f, 30.f, 20.f);
			sfFloatRect buttonViewSpeedPlus = FlRect(1750.f, 450.f, 30.f, 20.f);
			sfFloatRect buttonViewSpeedMinus = FlRect(1710.f, 450.f, 30.f, 20.f);
			sfFloatRect buttonEnlargementPlus = FlRect(1750.f, 530.f, 30.f, 20.f);
			sfFloatRect buttonEnlargementMinus = FlRect(1710.f, 530.f, 30.f, 20.f);
			sfFloatRect buttonChangeMapPlus = FlRect(1790.f, 940.f, 30.f, 20.f);
			sfFloatRect buttonChangeMapMinus = FlRect(1750.f, 940.f, 30.f, 20.f);
			sfFloatRect buttonMapSave = FlRect(1720.f, 1010.f, 130.f, 40.f);

			if (sfFloatRect_contains(&buttonLayerChoiceR, mousePos.x, mousePos.y))
			{
				editorLayer--;
				if (editorLayer < 0)
				{
					editorLayer = 2;
				}
			}
			if (sfFloatRect_contains(&buttonLayerChoiceL, mousePos.x, mousePos.y))
			{
				editorLayer++;
				if (editorLayer > 2)
				{
					editorLayer = 0;
				}
			}

			if (sfFloatRect_contains(&buttonWidthMinus, mousePos.x, mousePos.y))
			{
				editor_RemoveColumn();
			}
			if (sfFloatRect_contains(&buttonWidthPlus, mousePos.x, mousePos.y))
			{
				editor_AddColumn();
			}
			if (sfFloatRect_contains(&buttonHeightMinus, mousePos.x, mousePos.y))
			{
				editor_RemoveRow();
			}
			if (sfFloatRect_contains(&buttonHeightPlus, mousePos.x, mousePos.y))
			{
				editor_AddRow();
			}
			if (sfFloatRect_contains(&buttonBrushMinus, mousePos.x, mousePos.y))
			{
				if (getEditorBrushSize() > 1)
					setEditorBrushSize(getEditorBrushSize() - 1);
			}
			if (sfFloatRect_contains(&buttonBrushPlus, mousePos.x, mousePos.y))
			{
				if (getEditorBrushSize() < map.mapSize.x / 2 && getEditorBrushSize() < map.mapSize.y / 2)
					setEditorBrushSize(getEditorBrushSize() + 1);
			}
			sprintf(stringBrushSize, "%d", getEditorBrushSize());

			if (sfFloatRect_contains(&buttonViewSpeedMinus, mousePos.x, mousePos.y))
			{
				editorHudViewSpeed -= 100.f;
				if (editorHudViewSpeed < 300.f)
					editorHudViewSpeed = 300.f;
			}
			if (sfFloatRect_contains(&buttonViewSpeedPlus, mousePos.x, mousePos.y))
			{
				editorHudViewSpeed += 100.f;
				if (editorHudViewSpeed > 10000.f)
					editorHudViewSpeed = 10000.f;
			}
			sprintf(stringViewSpeed, "%.f", editorHudViewSpeed);

			if (sfFloatRect_contains(&buttonEnlargementMinus, mousePos.x, mousePos.y))
			{
				editorHudMapEnlargement -= 10;
				if (editorHudMapEnlargement < 1)
					editorHudMapEnlargement = 0;
			}
			if (sfFloatRect_contains(&buttonEnlargementPlus, mousePos.x, mousePos.y))
			{
				if (editorHudMapEnlargement == 1)
					editorHudMapEnlargement = 10;
				else
					editorHudMapEnlargement += 10;
				if (editorHudMapEnlargement > 10000)
					editorHudMapEnlargement = 10000;
			}
			sprintf(stringEnlargement, "%d", editorHudMapEnlargement);

			if (sfFloatRect_contains(&buttonChangeMapMinus, mousePos.x, mousePos.y))
			{
				if (getEditorIndexMap() > 1)
					setEditorIndexMap(getEditorIndexMap() - 1);
				else setEditorIndexMap(1);
			}
			if (sfFloatRect_contains(&buttonChangeMapPlus, mousePos.x, mousePos.y))
			{
				setEditorIndexMap(getEditorIndexMap() + 1);
			}
			char tmp[30] = "";
			sprintf(tmp, "Map %d : ", getEditorIndexMap());
			sfText_setString(editorMapTextId, tmp);
			if (sfFloatRect_contains(&buttonMapSave, mousePos.x, mousePos.y) && !wantMapSave)
			{
				wantMapSave = sfTrue;
			}

			timerInputLight = 0.f;
		}

		sfRectangleShape_setFillColor(editorHUDRect, color(232, 120, 120, 180));
	}
	else if (editorHudEffects == EDITOR_EFFECTS_PARTICLES)
	{

	}
	if (editorHudState == EDITOR_STATE_TILES || editorHudState == EDITOR_STATE_TILES_HUD)
	{
		if (customKeyboardIsKeyPressed(sfKeySpace, _window->renderWindow) && timer > 0.2f)
		{
			timer = 0.f;
			if (editorHudState == EDITOR_STATE_TILES)
				editorHudState = EDITOR_STATE_TILES_HUD;
			else
				editorHudState = EDITOR_STATE_TILES;
		}

		if (leftClick(_window->renderWindow) && timerInputLight > 0.2f)
		{
			sfVector2f mousePos = getfMousePos(_window->renderWindow);
			sfFloatRect buttonOpenTS = FlRect(1720.f, 10.f, 90.f, 40.f);
			sfFloatRect buttonGroundTypeR = FlRect(1710.f, 100.f, 30.f, 20.f);
			sfFloatRect buttonGroundTypeL = FlRect(1750.f, 100.f, 30.f, 20.f);
			sfFloatRect buttonWallTypeR = FlRect(1710.f, 190.f, 30.f, 20.f);
			sfFloatRect buttonWallTypeL = FlRect(1750.f, 190.f, 30.f, 20.f);
			sfFloatRect buttonMapSave = FlRect(1740.f, 950.f, 130.f, 40.f);
			sfFloatRect buttonRateT1Minus = FlRect(1710.f, 280.f, 30.f, 20.f);
			sfFloatRect buttonRateT1Plus = FlRect(1750.f, 280.f, 30.f, 20.f);
			sfFloatRect buttonRateT2Minus = FlRect(1710.f, 330.f, 30.f, 20.f);
			sfFloatRect buttonRateT2Plus = FlRect(1750.f, 330.f, 30.f, 20.f);
			sfFloatRect buttonRateT3Minus = FlRect(1710.f, 380.f, 30.f, 20.f);
			sfFloatRect buttonRateT3Plus = FlRect(1750.f, 380.f, 30.f, 20.f);
			sfFloatRect buttonRateT4Minus = FlRect(1710.f, 430.f, 30.f, 20.f);
			sfFloatRect buttonRateT4Plus = FlRect(1750.f, 430.f, 30.f, 20.f);


			if (sfFloatRect_contains(&buttonOpenTS, mousePos.x, mousePos.y))
			{
				if (editorHudState == EDITOR_STATE_TILES)
					editorHudState = EDITOR_STATE_TILES_HUD;
				else
					editorHudState = EDITOR_STATE_TILES;
			}
			if (sfFloatRect_contains(&buttonGroundTypeR, mousePos.x, mousePos.y) && editorLayer == EDITOR_LAYER_GROUND)
			{
				editorHudGroundType--;
				if (editorHudGroundType < 0)
				{
					editorHudGroundType = 6;
				}
				///Trouver la formule pour update la tuile
				/*setEditorCurrentTile(getEditorCurrentTile() - pow(2, (editorHudGroundType + 4)));
				if (getEditorCurrentTile() < 16)
				{
					setEditorCurrentTile(getEditorCurrentTile() + 16 * 7);
				}*/
			}
			if (sfFloatRect_contains(&buttonGroundTypeL, mousePos.x, mousePos.y) && editorLayer == EDITOR_LAYER_GROUND)
			{
				editorHudGroundType++;
				if (editorHudGroundType > 6)
				{
					editorHudGroundType = 0;
				}
				///Trouver la formule pour update la tuile
				/*setEditorCurrentTile(getEditorCurrentTile() + pow(2, (editorHudGroundType + 4)));
				if (getEditorCurrentTile() > 142)
				{
					setEditorCurrentTile(getEditorCurrentTile() - 16 * 7);
				}*/
			}
			if (sfFloatRect_contains(&buttonWallTypeR, mousePos.x, mousePos.y) && editorLayer == EDITOR_LAYER_WALLS)
			{
				editorHudWallType--;
				if (editorHudWallType < 0)
				{
					editorHudWallType = 1;
				}
				///Trouver la formule pour update la tuile
			}
			if (sfFloatRect_contains(&buttonWallTypeL, mousePos.x, mousePos.y) && editorLayer == EDITOR_LAYER_WALLS)
			{
				editorHudWallType++;
				if (editorHudWallType > 1)
				{
					editorHudWallType = 0;
				}
				///Trouver la formule pour update la tuile
			}

			if (editorHudState == EDITOR_STATE_TILES && (sfFloatRect_contains(&buttonMapSave, mousePos.x, mousePos.y) && !wantMapSave))
			{
				wantMapSave = sfTrue;
			}
			if ((editorHudGroundType == EDITOR_GROUND_SAND || editorHudGroundType == EDITOR_GROUND_ROCK
				|| editorHudGroundType == EDITOR_GROUND_GRASS || editorHudGroundType == EDITOR_GROUND_LEAF))
			{
				if (editorHudState == EDITOR_STATE_TILES_HUD && (sfFloatRect_contains(&buttonRateT1Minus, mousePos.x, mousePos.y)))
				{
					valRateT1 -= 5;
					if (valRateT1 < 0)
						valRateT1 = 0;
				}
				if (editorHudState == EDITOR_STATE_TILES_HUD && (sfFloatRect_contains(&buttonRateT1Plus, mousePos.x, mousePos.y)))
				{
					valRateT1 += 5;
					if (valRateT1 > 100)
						valRateT1 = 100;
				}
				if (editorHudState == EDITOR_STATE_TILES_HUD && (sfFloatRect_contains(&buttonRateT2Minus, mousePos.x, mousePos.y)))
				{
					valRateT2 -= 5;
					if (valRateT2 < 0)
						valRateT2 = 0;
				}
				if (editorHudState == EDITOR_STATE_TILES_HUD && (sfFloatRect_contains(&buttonRateT2Plus, mousePos.x, mousePos.y)))
				{
					valRateT2 += 5;
					if (valRateT2 > 100)
						valRateT2 = 100;
				}
				if (editorHudState == EDITOR_STATE_TILES_HUD && (sfFloatRect_contains(&buttonRateT3Minus, mousePos.x, mousePos.y)))
				{
					valRateT3 -= 5;
					if (valRateT3 < 0)
						valRateT3 = 0;
				}
				if (editorHudState == EDITOR_STATE_TILES_HUD && (sfFloatRect_contains(&buttonRateT3Plus, mousePos.x, mousePos.y)))
				{
					valRateT3 += 5;
					if (valRateT3 > 100)
						valRateT3 = 100;
				}
				if (editorHudState == EDITOR_STATE_TILES_HUD && (sfFloatRect_contains(&buttonRateT4Minus, mousePos.x, mousePos.y)))
				{
					valRateT4 -= 5;
					if (valRateT4 < 0)
						valRateT4 = 0;
				}
				if (editorHudState == EDITOR_STATE_TILES_HUD && (sfFloatRect_contains(&buttonRateT4Plus, mousePos.x, mousePos.y)))
				{
					valRateT4 += 5;
					if (valRateT4 > 100)
						valRateT4 = 100;
				}
				setEditorValueRateTiles(valRateT1, valRateT2, valRateT3, valRateT4);
				sprintf(strValueRateT1, "%d", valRateT1);
				sprintf(strValueRateT2, "%d", valRateT2);
				sprintf(strValueRateT3, "%d", valRateT3);
				sprintf(strValueRateT4, "%d", valRateT4);
			}

			timerInputLight = 0.f;
		}
		if (editorLayer == EDITOR_LAYER_GROUND)
		{
			switch (editorHudGroundType)
			{
			case EDITOR_GROUND_SAND:
				sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_ground_sand"), sfTrue);
				setGroundMaterialValue(TILE_SAND_FULL);
				break;
			case EDITOR_GROUND_ROCK:
				sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_ground_rock"), sfTrue);
				setGroundMaterialValue(TILE_ROCK_FULL);
				break;
			case EDITOR_GROUND_GRASS:
				sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_ground_grass"), sfTrue);
				setGroundMaterialValue(TILE_GRASS_FULL);
				break;
			case EDITOR_GROUND_WATER:
				sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_ground_water"), sfTrue);
				setGroundMaterialValue(TILE_WATER_FULL);
				break;
			case EDITOR_GROUND_DIRT:
				sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_ground_dirt"), sfTrue);
				setGroundMaterialValue(TILE_GROUND_DIRT_1);
				break;
			case EDITOR_GROUND_DUNGEON:
				sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_ground_dungeon"), sfTrue);
				setGroundMaterialValue(TILE_GROUND_DUNGEON_1);
				break;
			case EDITOR_GROUND_LEAF:
				sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_ground_leaf"), sfTrue);
				setGroundMaterialValue(TILE_LEAF_FULL);
				break;
			}
		}
		else if (editorLayer == EDITOR_LAYER_WALLS)
		{
			switch (editorHudWallType)
			{
			case EDITOR_WALL_EXTERIOR:
				sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_wall_ext"), sfTrue);
				setWallMaterialValue(WALL_MATERIAL_OUT);
				break;
			case EDITOR_WALL_INTERIOR:
				sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_wall_int"), sfTrue);
				setWallMaterialValue(WALL_MATERIAL_IN);
				break;
			}
		}
	}
	else if (editorHudState == EDITOR_STATE_PROPS)
	{
		if (customKeyboardIsKeyPressed(sfKeyU, _window->renderWindow) && timer > 0.2f)
		{
			timer = 0.f;
			editorHudPropsType++;
			if (editorHudPropsType > PROPS_TYPE_BEACH)
				editorHudPropsType = PROPS_TYPE_GENERAL;

			editorHudCurrentProps = 0;
		}

		if (customKeyboardIsKeyPressed(sfKeyNumpad2, _window->renderWindow) && timer > 0.2f)
		{
			timer = 0.f;
			editorHudCurrentProps++;
			switch (editorHudPropsType)
			{
			case PROPS_TYPE_FOREST:
				if (editorHudCurrentProps > PROPS_FOREST_WATERFALL)
					editorHudCurrentProps = PROPS_FOREST_TREE_1;
				break;
			case PROPS_TYPE_MOUTAIN:
				if (editorHudCurrentProps > PROPS_MOUTAIN_DOOR_LIGHT_TOP)
					editorHudCurrentProps = PROPS_MOUTAIN_BIG_CRISTAL_1;
				break;
			case PROPS_TYPE_DUNGEON:
				if (editorHudCurrentProps > PROPS_DUNGEON_ENTRANCE_ANGRY)
					editorHudCurrentProps = PROPS_DUNGEON_DOOR_TOP;
				break;
			case PROPS_TYPE_BEACH:
				if (editorHudCurrentProps > PROPS_BEACH_ROCK_5)
					editorHudCurrentProps = PROPS_BEACH_PALM_1;
				break;
			case PROPS_TYPE_GENERAL:
				if (editorHudCurrentProps > PROPS_GENERAL_FLOATING_NOTES)
					editorHudCurrentProps = PROPS_GENERAL_FOUNTAIN;
				break;
			}
		}

		if (customKeyboardIsKeyPressed(sfKeyNumpad1, _window->renderWindow) && timer > 0.2f)
		{
			timer = 0.f;
			editorHudCurrentProps--;
			switch (editorHudPropsType)
			{
			case PROPS_TYPE_FOREST:
				if (editorHudCurrentProps < PROPS_FOREST_TREE_1)
					editorHudCurrentProps = PROPS_FOREST_WATERFALL;
				break;
			case PROPS_TYPE_MOUTAIN:
				if (editorHudCurrentProps < PROPS_MOUTAIN_BIG_CRISTAL_1)
					editorHudCurrentProps = PROPS_MOUTAIN_DOOR_LIGHT_TOP;
				break;
			case PROPS_TYPE_DUNGEON:
				if (editorHudCurrentProps < PROPS_DUNGEON_DOOR_TOP)
					editorHudCurrentProps = PROPS_DUNGEON_ENTRANCE_ANGRY;
				break;
			case PROPS_TYPE_BEACH:
				if (editorHudCurrentProps < PROPS_BEACH_PALM_1)
					editorHudCurrentProps = PROPS_BEACH_ROCK_5;
				break;
			case PROPS_TYPE_GENERAL:
				if (editorHudCurrentProps < PROPS_GENERAL_FOUNTAIN)
					editorHudCurrentProps = PROPS_GENERAL_FLOATING_NOTES;
				break;
			}
		}
	}
}

void displayEditorHUD(Window* _window)
{
	sfRenderTexture_setView(_window->rdrt, sfRenderTexture_getDefaultView(_window->rdrt));
	switch (editorLayer)
	{
	case EDITOR_LAYER_GROUND:
		sfText_setString(editorLayerText, "Layer : Ground");
		break;
	case EDITOR_LAYER_WALLS:
		sfText_setString(editorLayerText, "Layer : Walls");
		break;
	case EDITOR_LAYER_COLLISION:
		sfText_setString(editorLayerText, "Layer : Collisions");
		break;
	}
	sfRenderTexture_drawText(_window->rdrt, editorLayerText, NULL);

	sfRenderTexture_drawText(_window->rdrt, editorMapText, NULL);

	sfRenderTexture_drawRectangleShape(_window->rdrt, editorHUDRect, NULL);

	sfRectangleShape_setFillColor(editorTabsButton, color(177, 227, 212, 180));
	sfRectangleShape_setPosition(editorTabsButton, vector2f(1670.f, 70.f));
	sfRenderTexture_drawRectangleShape(_window->rdrt, editorTabsButton, NULL);
	sfText_setPosition(editorTabsText, vector2f(1705.f, 105.f));
	sfText_setString(editorTabsText, "Tiles");
	sfRenderTexture_drawText(_window->rdrt, editorTabsText, NULL);

	sfRectangleShape_setFillColor(editorTabsButton, color(177, 211, 227, 180));
	sfRectangleShape_setPosition(editorTabsButton, vector2f(1670.f, 210.f));
	sfRenderTexture_drawRectangleShape(_window->rdrt, editorTabsButton, NULL);
	sfText_setPosition(editorTabsText, vector2f(1705.f, 240.f));
	sfText_setString(editorTabsText, "Entity");
	sfRenderTexture_drawText(_window->rdrt, editorTabsText, NULL);

	sfRectangleShape_setFillColor(editorTabsButton, color(1177, 178, 227, 180));
	sfRectangleShape_setPosition(editorTabsButton, vector2f(1670.f, 350.f));
	sfRenderTexture_drawRectangleShape(_window->rdrt, editorTabsButton, NULL);
	sfText_setPosition(editorTabsText, vector2f(1705.f, 380.f));
	sfText_setString(editorTabsText, "Props");
	sfRenderTexture_drawText(_window->rdrt, editorTabsText, NULL);

	sfRectangleShape_setFillColor(editorTabsButton, color(207, 177, 227, 180));
	sfRectangleShape_setPosition(editorTabsButton, vector2f(1670.f, 490.f));
	sfRenderTexture_drawRectangleShape(_window->rdrt, editorTabsButton, NULL);
	sfText_setPosition(editorTabsText, vector2f(1705.f, 510.f));
	sfText_setString(editorTabsText, "Effects");
	sfRenderTexture_drawText(_window->rdrt, editorTabsText, NULL);

	sfRectangleShape_setFillColor(editorTabsButton, color(222, 120, 120, 180));
	sfRectangleShape_setPosition(editorTabsButton, vector2f(1670.f, 630.f));
	sfRectangleShape_setSize(editorTabsButton, vector2f(30.f, 140.f));
	sfRenderTexture_drawRectangleShape(_window->rdrt, editorTabsButton, NULL);
	sfText_setPosition(editorTabsText, vector2f(1705.f, 635.f));
	sfText_setString(editorTabsText, "Cfg Editor");
	sfRenderTexture_drawText(_window->rdrt, editorTabsText, NULL);

	sfRectangleShape_setSize(editorTabsButton, vector2f(30.f, 130.f));

	switch (editorHudState)
	{
	case EDITOR_STATE_TILES:
		sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
		sfRectangleShape_setSize(squareMenuLight, vector2f(90.f, 40.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1720.f, 10.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorLightButtonText, "Open TS");
		sfText_setCharacterSize(editorLightButtonText, 20);
		sfText_setPosition(editorLightButtonText, vector2f(1730.f, 15.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		sfText_setCharacterSize(editorLightButtonText, 30);

		sfText_setPosition(editorMapTextId, vector2f(1750.f, 970.f));
		sfRenderTexture_drawText(_window->rdrt, editorMapTextId, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
		sfRectangleShape_setSize(squareMenuLight, vector2f(130.f, 40.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1720.f, 1010.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setCharacterSize(editorMapTextSave, 20);
		sfText_setPosition(editorMapTextSave, vector2f(1730.f, 1015.f));
		sfRenderTexture_drawText(_window->rdrt, editorMapTextSave, NULL);

		sfText_setCharacterSize(editorMapTextSave, 30);

		/// Layer choice
		sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 100.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "+");
		sfText_setPosition(editorDetailButton, vector2f(1762.f, 95.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 100.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "-");
		sfText_setPosition(editorDetailButton, vector2f(1722.f, 95.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfText_setString(editorLightButtonText, "Layer_choice :");
		sfText_setPosition(editorLightButtonText, vector2f(1720.f, 50.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		switch (editorLayer)
		{
		case EDITOR_LAYER_GROUND:
			sfText_setString(editorLightButtonText, "Ground");
			break;
		case EDITOR_LAYER_WALLS:
			sfText_setString(editorLightButtonText, "Walls");
			break;
		case EDITOR_LAYER_COLLISION:
			sfText_setString(editorLightButtonText, "Collisions");
			break;
		}
		sfText_setPosition(editorLightButtonText, vector2f(1790.f, 90.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		/// Sub layer choice
		sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 190.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "+");
		sfText_setPosition(editorDetailButton, vector2f(1762.f, 185.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 190.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "-");
		sfText_setPosition(editorDetailButton, vector2f(1722.f, 185.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfText_setCharacterSize(editorLightButtonText, 23);
		sfText_setString(editorLightButtonText, "Sub_Layer_choice :");
		sfText_setPosition(editorLightButtonText, vector2f(1720.f, 140.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);
		sfText_setCharacterSize(editorLightButtonText, 30);

		sfText_setString(editorLightButtonText, strSubLayer);
		sfText_setPosition(editorLightButtonText, vector2f(1790.f, 180.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		/// Biome choice
		sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 300.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "+");
		sfText_setPosition(editorDetailButton, vector2f(1762.f, 295.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 300.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "-");
		sfText_setPosition(editorDetailButton, vector2f(1722.f, 295.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfText_setCharacterSize(editorLightButtonText, 23);
		sfText_setString(editorLightButtonText, strCurrentBiome);
		sfText_setPosition(editorLightButtonText, vector2f(1720.f, 230.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);
		sfText_setCharacterSize(editorLightButtonText, 30);

		sfText_setString(editorLightButtonText, strBiome);
		sfText_setPosition(editorLightButtonText, vector2f(1790.f, 290.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
		sfRectangleShape_setSize(squareMenuLight, vector2f(120.f, 30.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 330.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "Change biome");
		sfText_setPosition(editorDetailButton, vector2f(1722.f, 330.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);
		break;
	case EDITOR_STATE_TILES_HUD:
		sfRenderTexture_drawRectangleShape(_window->rdrt, editorHudSquareFade, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfRed);
		sfRectangleShape_setSize(squareMenuLight, vector2f(90.f, 40.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1720.f, 10.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorLightButtonText, "Close TS");
		sfText_setCharacterSize(editorLightButtonText, 20);
		sfText_setPosition(editorLightButtonText, vector2f(1730.f, 15.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		sfText_setCharacterSize(editorLightButtonText, 30);
		if (editorLayer == EDITOR_LAYER_GROUND)
		{
			int x = 5;
			int y = 3;
			if (editorHudGroundType == EDITOR_GROUND_DIRT)
			{
				x = 1;
				y = 1;
			}
			if (editorHudGroundType == EDITOR_GROUND_DUNGEON)
			{
				x = 1;
				y = 4;
			}

			for (int i = 0; i < x; i++)
			{
				for (int j = 0; j < y; j++)
				{
					int tmp = getGroundMaterialValue();
					sfIntRect rect = map_GetTileTexRect((LayerGround) { i* y + j + getGroundMaterialValue(), 0 });
					sfSprite_setTextureRect(editorHudSprite, rect);
					sfSprite_setPosition(editorHudSprite, vector2f(i * TILE_SIZE, j * TILE_SIZE));
					sfRenderTexture_drawSprite(_window->rdrt, editorHudSprite, NULL);
				}
			}

			if (getGroundMaterialValue() == TILE_MATERIAL_WATER)
			{
				sfSprite_setTextureRect(editorHudSprite, map_GetTileTexRect((LayerGround) { TILE_WATER_FULL_DEEP, 0 }));
				sfSprite_setPosition(editorHudSprite, vector2f(4 * TILE_SIZE, 1 * TILE_SIZE));
				sfRenderTexture_drawSprite(_window->rdrt, editorHudSprite, NULL);
			}

			sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
			sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
			sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 100.f));
			sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

			sfText_setString(editorDetailButton, "+");
			sfText_setPosition(editorDetailButton, vector2f(1762.f, 95.f));
			sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

			sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
			sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
			sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 100.f));
			sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

			sfText_setString(editorDetailButton, "-");
			sfText_setPosition(editorDetailButton, vector2f(1722.f, 95.f));
			sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

			switch (editorHudGroundType)
			{
			case EDITOR_GROUND_SAND: sfText_setString(editorLightButtonText, "SAND"); break;
			case EDITOR_GROUND_ROCK: sfText_setString(editorLightButtonText, "ROCK"); break;
			case EDITOR_GROUND_GRASS: sfText_setString(editorLightButtonText, "GRASS"); break;
			case EDITOR_GROUND_WATER: sfText_setString(editorLightButtonText, "WATER"); break;
			case EDITOR_GROUND_DIRT: sfText_setString(editorLightButtonText, "DIRT"); break;
			case EDITOR_GROUND_DUNGEON: sfText_setString(editorLightButtonText, "DUNGEON"); break;
			case EDITOR_GROUND_LEAF: sfText_setString(editorLightButtonText, "LEAF"); break;
			}
			sfText_setPosition(editorLightButtonText, vector2f(1790.f, 90.f));
			sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

			sfText_setString(editorLightButtonText, "Ground_type :");
			sfText_setPosition(editorLightButtonText, vector2f(1720.f, 50.f));
			sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

			if ((editorHudGroundType == EDITOR_GROUND_SAND || editorHudGroundType == EDITOR_GROUND_ROCK
				|| editorHudGroundType == EDITOR_GROUND_GRASS || editorHudGroundType == EDITOR_GROUND_LEAF))

			{
				sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
				sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
				sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 280.f));
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfText_setString(editorDetailButton, "+");
				sfText_setPosition(editorDetailButton, vector2f(1762.f, 275.f));
				sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

				sfRectangleShape_setFillColor(squareMenuLight, sfRed);
				sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
				sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 280.f));
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfText_setString(editorDetailButton, "-");
				sfText_setPosition(editorDetailButton, vector2f(1722.f, 275.f));
				sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

				sfText_setString(editorLightButtonText, strValueRateT1);
				sfText_setPosition(editorLightButtonText, vector2f(1790.f, 270.f));
				sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

				sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
				sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
				sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 330.f));
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfText_setString(editorDetailButton, "+");
				sfText_setPosition(editorDetailButton, vector2f(1762.f, 325.f));
				sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

				sfRectangleShape_setFillColor(squareMenuLight, sfRed);
				sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
				sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 330.f));
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfText_setString(editorDetailButton, "-");
				sfText_setPosition(editorDetailButton, vector2f(1722.f, 325.f));
				sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

				sfText_setString(editorLightButtonText, strValueRateT2);
				sfText_setPosition(editorLightButtonText, vector2f(1790.f, 320.f));
				sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

				sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
				sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
				sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 380.f));
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfText_setString(editorDetailButton, "+");
				sfText_setPosition(editorDetailButton, vector2f(1762.f, 375.f));
				sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

				sfRectangleShape_setFillColor(squareMenuLight, sfRed);
				sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
				sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 380.f));
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfText_setString(editorDetailButton, "-");
				sfText_setPosition(editorDetailButton, vector2f(1722.f, 375.f));
				sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

				sfText_setString(editorLightButtonText, strValueRateT3);
				sfText_setPosition(editorLightButtonText, vector2f(1790.f, 370.f));
				sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

				sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
				sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
				sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 430.f));
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfText_setString(editorDetailButton, "+");
				sfText_setPosition(editorDetailButton, vector2f(1762.f, 425.f));
				sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

				sfRectangleShape_setFillColor(squareMenuLight, sfRed);
				sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
				sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 430.f));
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfText_setString(editorDetailButton, "-");
				sfText_setPosition(editorDetailButton, vector2f(1722.f, 425.f));
				sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

				sfText_setString(editorLightButtonText, strValueRateT4);
				sfText_setPosition(editorLightButtonText, vector2f(1790.f, 420.f));
				sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

				sfText_setString(editorLightButtonText, "Rate_tiles :");
				sfText_setPosition(editorLightButtonText, vector2f(1720.f, 230.f));
				sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);
			}
		}
		else if (editorLayer == EDITOR_LAYER_WALLS)
		{
			sfSprite_setPosition(editorHudSprite, vector2f(10.f, 10.f));
			sfRenderTexture_drawSprite(_window->rdrt, editorHudSprite, NULL);

			sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
			sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
			sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 190.f));
			sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

			sfText_setString(editorDetailButton, "+");
			sfText_setPosition(editorDetailButton, vector2f(1762.f, 185.f));
			sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

			sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
			sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
			sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 190.f));
			sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

			sfText_setString(editorDetailButton, "-");
			sfText_setPosition(editorDetailButton, vector2f(1722.f, 185.f));
			sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

			switch (editorHudWallType)
			{
			case EDITOR_WALL_EXTERIOR:
				sfText_setString(editorLightButtonText, "EXTERIOR");
				break;
			case EDITOR_WALL_INTERIOR:
				sfText_setString(editorLightButtonText, "INTERIOR");
				break;
			}
			sfText_setPosition(editorLightButtonText, vector2f(1790.f, 180.f));
			sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

			sfText_setString(editorLightButtonText, "Wall_type :");
			sfText_setPosition(editorLightButtonText, vector2f(1720.f, 140.f));
			sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);
		}
		else if (editorLayer == EDITOR_LAYER_COLLISION)
		{
			sfSprite_setTexture(editorHudSprite, GetSfTexture("collision"), sfTrue);
			sfSprite_setPosition(editorHudSprite, vector2f(0, 0));
			sfRenderTexture_drawSprite(_window->rdrt, editorHudSprite, NULL);
			/*for (int i = 0; i < 23; i++)
			{
				sfSprite_setTexture(editorHudSprite, GetSfTexture("collision"), sfTrue);
				sfSprite_setPosition(editorHudSprite, vector2f(i * TILE_SIZE, 0));
				sfRenderTexture_drawSprite(_window->rdrt, editorHudSprite, NULL);
			}*/
		}
		sfRectangleShape_setOrigin(squareMenuLight, vector2f(0.f, 0.f));

		break;
	case EDITOR_STATE_ENTITY:
		sfRectangleShape_setPosition(editorButton, vector2f(1750.f, 100.f));
		if (editorHudEntity == EDITOR_ENTITY_PLAYER)
			sfRectangleShape_setFillColor(editorButton, color(215, 212, 217, 255));
		else
			sfRectangleShape_setFillColor(editorButton, sfWhite);
		sfRenderTexture_drawRectangleShape(_window->rdrt, editorButton, NULL);
		sfText_setPosition(editorButtonText, vector2f(1770.f, 140.f));
		sfText_setString(editorButtonText, "PLAYER");
		sfRenderTexture_drawText(_window->rdrt, editorButtonText, NULL);

		sfRectangleShape_setPosition(editorButton, vector2f(1750.f, 250.f));
		if (editorHudEntity == EDITOR_ENTITY_PNJ)
			sfRectangleShape_setFillColor(editorButton, color(215, 212, 217, 255));
		else
			sfRectangleShape_setFillColor(editorButton, sfWhite);
		sfRenderTexture_drawRectangleShape(_window->rdrt, editorButton, NULL);
		sfText_setPosition(editorButtonText, vector2f(1790.f, 290.f));
		sfText_setString(editorButtonText, "PNJ");
		sfRenderTexture_drawText(_window->rdrt, editorButtonText, NULL);

		sfRectangleShape_setPosition(editorButton, vector2f(1750.f, 400.f));
		if (editorHudEntity == EDITOR_ENTITY_MOBS)
			sfRectangleShape_setFillColor(editorButton, color(215, 212, 217, 255));
		else
			sfRectangleShape_setFillColor(editorButton, sfWhite);
		sfRenderTexture_drawRectangleShape(_window->rdrt, editorButton, NULL);
		sfText_setPosition(editorButtonText, vector2f(1780.f, 440.f));
		sfText_setString(editorButtonText, "MOBS");
		sfRenderTexture_drawText(_window->rdrt, editorButtonText, NULL);

		sfRectangleShape_setFillColor(editorButton, sfWhite);

		if (togglePnjChoice)
		{
			sfRectangleShape_setPosition(editorButton, vector2f(1750.f, 550.f));
			if (editorHudPnjChoice == EDITOR_PNJ_MENTOR)
				sfRectangleShape_setFillColor(editorButton, color(215, 212, 217, 255));
			else
				sfRectangleShape_setFillColor(editorButton, sfWhite);
			sfRenderTexture_drawRectangleShape(_window->rdrt, editorButton, NULL);
			sfText_setPosition(editorButtonText, vector2f(1780.f, 590.f));
			sfText_setString(editorButtonText, "MENTOR");
			sfRenderTexture_drawText(_window->rdrt, editorButtonText, NULL);

			sfRectangleShape_setPosition(editorButton, vector2f(1750.f, 700.f));
			if (editorHudPnjChoice == EDITOR_PNJ_ARNOLD)
				sfRectangleShape_setFillColor(editorButton, color(215, 212, 217, 255));
			else
				sfRectangleShape_setFillColor(editorButton, sfWhite);
			sfRenderTexture_drawRectangleShape(_window->rdrt, editorButton, NULL);
			sfText_setPosition(editorButtonText, vector2f(1780.f, 740.f));
			sfText_setString(editorButtonText, "ARNOLD");
			sfRenderTexture_drawText(_window->rdrt, editorButtonText, NULL);

			sfRectangleShape_setPosition(editorButton, vector2f(1750.f, 850.f));
			if (editorHudPnjChoice == EDITOR_PNJ_SHAMAN)
				sfRectangleShape_setFillColor(editorButton, color(215, 212, 217, 255));
			else
				sfRectangleShape_setFillColor(editorButton, sfWhite);
			sfRenderTexture_drawRectangleShape(_window->rdrt, editorButton, NULL);
			sfText_setPosition(editorButtonText, vector2f(1780.f, 890.f));
			sfText_setString(editorButtonText, "SHAMAN");
			sfRenderTexture_drawText(_window->rdrt, editorButtonText, NULL);

			sfRectangleShape_setFillColor(editorButton, sfWhite);
		}
		if (toggleMobChoice)
		{
			sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
			sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
			sfRectangleShape_setPosition(squareMenuLight, vector2f(1790.f, 630.f));
			sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

			sfText_setString(editorDetailButton, "+");
			sfText_setPosition(editorDetailButton, vector2f(1802.f, 625.f));
			sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

			sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
			sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
			sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 630.f));
			sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

			sfText_setString(editorDetailButton, "-");
			sfText_setPosition(editorDetailButton, vector2f(1762.f, 625.f));
			sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

			sfText_setString(editorLightButtonText, "Choice_mob :");
			sfText_setPosition(editorLightButtonText, vector2f(1720.f, 590.f));
			sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

			switch (editorHudMobType)
			{
			case EN_YOYO_SPIDER:
				sfText_setString(editorLightButtonText, "yoyo spider");
				break;
			case EN_VENIMOUS_YOYO_SPIDER:
				sfText_setString(editorLightButtonText, "venimous yoyo spider");
				break;
			case EN_CRAB:
				sfText_setString(editorLightButtonText, "crab");
				break;
			case EN_GOOSE:
				sfText_setString(editorLightButtonText, "goose");
				break;
			case EN_WHITE_BILLY_GOAT:
				sfText_setString(editorLightButtonText, "white goat");
				break;
			case EN_BLACK_BILLY_GOAT:
				sfText_setString(editorLightButtonText, "black goat");
				break;
			case EN_SNAKE:
				sfText_setString(editorLightButtonText, "snake");
				break;
			case EN_MOSQUITO:
				sfText_setString(editorLightButtonText, "mosquito");
				break;
			case EN_MOLE:
				sfText_setString(editorLightButtonText, "mole");
				break;
			case EN_SCOLOPENDRE:
				sfText_setString(editorLightButtonText, "scolopendre");
				break;
			case EN_CHAMELEON:
				sfText_setString(editorLightButtonText, "chameleon");
				break;
			case EN_BAT:
				sfText_setString(editorLightButtonText, "bat");
				break;
			case EN_LITTLE_GHOST_JOY:
				sfText_setString(editorLightButtonText, "little ghost joy");
				break;
			case EN_BIG_GHOST_JOY:
				sfText_setString(editorLightButtonText, "big ghost joy");
				break;
			case EN_LITTLE_GHOST_SADNESS:
				sfText_setString(editorLightButtonText, "little ghost sadness");
				break;
			case EN_BIG_GHOST_SADNESS:
				sfText_setString(editorLightButtonText, "big ghost sadness");
				break;
			case EN_IMUNITY:
				sfText_setString(editorLightButtonText, "imunity");
				break;
			case EN_SEAGULL:
				sfText_setString(editorLightButtonText, "seagull");
				break;
			}
			sfText_setCharacterSize(editorLightButtonText, 23);
			sfText_setPosition(editorLightButtonText, vector2f(1720.f, 650.f));
			sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);
			sfText_setCharacterSize(editorLightButtonText, 30);

			if (editorHudMobType == EN_CHAMELEON || editorHudMobType == EN_SCOLOPENDRE)
			{
				sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
				sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
				sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 720.f));
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfText_setString(editorDetailButton, "+");
				sfText_setPosition(editorDetailButton, vector2f(1762.f, 715.f));
				sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

				sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
				sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
				sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 720.f));
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfText_setString(editorDetailButton, "-");
				sfText_setPosition(editorDetailButton, vector2f(1722.f, 715.f));
				sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

				sfText_setString(editorLightButtonText, "Mob_Rotation :");
				sfText_setPosition(editorLightButtonText, vector2f(1720.f, 680.f));
				sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

				sprintf(stringMobRotation, "%.f", editorHudMobRotation);
				sfText_setString(editorLightButtonText, stringMobRotation);
				sfText_setPosition(editorLightButtonText, vector2f(1800.f, 710.f));
				sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);
			}
		}
		break;
	case EDITOR_STATE_PROPS:
		sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 100.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "+");
		sfText_setPosition(editorDetailButton, vector2f(1762.f, 95.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 100.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "-");
		sfText_setPosition(editorDetailButton, vector2f(1722.f, 95.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		switch (editorHudPropsType)
		{
		case PROPS_TYPE_GENERAL:
			sfText_setString(editorLightButtonText, "GENERAL");
			break;
		case PROPS_TYPE_DUNGEON:
			sfText_setString(editorLightButtonText, "DUNGEON");
			break;
		case PROPS_TYPE_FOREST:
			sfText_setString(editorLightButtonText, "FOREST");
			break;
		case PROPS_TYPE_BEACH:
			sfText_setString(editorLightButtonText, "BEACH");
			break;
		case PROPS_TYPE_MOUTAIN:
			sfText_setString(editorLightButtonText, "MOUTAIN");
			break;
		}
		sfText_setPosition(editorLightButtonText, vector2f(1790.f, 90.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		sfText_setString(editorLightButtonText, "Props_type :");
		sfText_setPosition(editorLightButtonText, vector2f(1720.f, 50.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);


		sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 190.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "+");
		sfText_setPosition(editorDetailButton, vector2f(1762.f, 185.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 190.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "-");
		sfText_setPosition(editorDetailButton, vector2f(1722.f, 185.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfText_setString(editorLightButtonText, "Choice_props :");
		sfText_setPosition(editorLightButtonText, vector2f(1720.f, 140.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		if (editorHudPropsType == PROPS_TYPE_MOUTAIN && (editorHudCurrentProps == PROPS_MOUTAIN_DOOR_FRAME || editorHudCurrentProps == PROPS_MOUTAIN_DOOR_FRAME_TOP))
		{
			sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
			sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
			sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 390.f));
			sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

			sfText_setString(editorDetailButton, "+");
			sfText_setPosition(editorDetailButton, vector2f(1762.f, 385.f));
			sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

			sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
			sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
			sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 390.f));
			sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

			sfText_setString(editorDetailButton, "-");
			sfText_setPosition(editorDetailButton, vector2f(1722.f, 385.f));
			sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

			sfText_setString(editorLightButtonText, "Choice_Light/Shadow :");
			sfText_setPosition(editorLightButtonText, vector2f(1720.f, 350.f));
			sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

			if (getLightOrShadow())
				sfText_setString(editorLightButtonText, "Shadow");
			else
				sfText_setString(editorLightButtonText, "Light");
			sfText_setPosition(editorLightButtonText, vector2f(1790.f, 380.f));
			sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);
		}
		break;
	case EDITOR_STATE_EFFECTS:
		if (!toggleParticles)
		{
			if (!toggleLight)
			{
				sfRectangleShape_setPosition(editorButton, vector2f(1750.f, 100.f));
				sfRenderTexture_drawRectangleShape(_window->rdrt, editorButton, NULL);
				sfText_setPosition(editorButtonText, vector2f(1780.f, 140.f));
				sfText_setString(editorButtonText, "LIGHTS");
				sfRenderTexture_drawText(_window->rdrt, editorButtonText, NULL);
			}

			if (toggleLight)
			{
				sfVector2f editorHUDPosLightScreen = { 0.f, 0.f };
				editorHUDPosLightScreen.x = 1810.f + mainView->viewPos.x - (mainView->size.x / 2.f);
				editorHUDPosLightScreen.y = 160.f + mainView->viewPos.y - (mainView->size.y / 2.f);

				sfShader* editorHUDShaderLights = GetShader("light");
				if (editorHUDShaderLights)
				{
					sfShader_setVec2Uniform(editorHUDShaderLights, "u_pos", editorHUDPosLightScreen);
					sfShader_setVec2Uniform(editorHUDShaderLights, "u_vpos", mainView->viewPos);
					sfShader_setVec2Uniform(editorHUDShaderLights, "u_size", lightSize);
					sfVector3f editorHUDColorShader = { normR, normG, normB };
					sfShader_setVec3Uniform(editorHUDShaderLights, "u_color", editorHUDColorShader);
					sfShader_setIntUniform(editorHUDShaderLights, "u_lightForm", lightForm);
					sfShader_setFloatUniform(editorHUDShaderLights, "u_angle", lightAngle * DEG2RAD);
					sfShader_setFloatUniform(editorHUDShaderLights, "u_rotation", lightRotate * DEG2RAD);
					sfShader_setFloatUniform(editorHUDShaderLights, "u_strength", lightStrength);
					sfShader_setVec2Uniform(editorHUDShaderLights, "u_vsize", mainView->size);
				}
				sfRectangleShape_setPosition(editorButton, vector2f(0.f, 0.f));
				sfRectangleShape_setSize(editorButton, vector2f(1920.f, 1080.f));
				sfRenderTexture_drawRectangleShape(_window->rdrt, editorButton, GetRenderState("light"));
				sfRectangleShape_setSize(editorButton, vector2f(120.f, 120.f));

				sfRectangleShape_setOrigin(squareMenuLight, vector2f(2.5f, 5.f));

				sfText_setPosition(editorButtonText, vector2f(1710.f, 280.f));
				sfText_setString(editorButtonText, "R:");
				sfRenderTexture_drawText(_window->rdrt, editorButtonText, NULL);

				sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
				sfRectangleShape_setSize(squareMenuLight, vector2f(5.f, 10.f));
				sfRectangleShape_setPosition(squareMenuLight, lightPosSliderR);
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfText_setPosition(editorButtonText, vector2f(1710.f, 330.f));
				sfText_setString(editorButtonText, "G:");
				sfRenderTexture_drawText(_window->rdrt, editorButtonText, NULL);

				sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
				sfRectangleShape_setSize(squareMenuLight, vector2f(5.f, 10.f));
				sfRectangleShape_setPosition(squareMenuLight, lightPosSliderG);
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfText_setPosition(editorButtonText, vector2f(1710.f, 380.f));
				sfText_setString(editorButtonText, "B:");
				sfRenderTexture_drawText(_window->rdrt, editorButtonText, NULL);

				sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
				sfRectangleShape_setSize(squareMenuLight, vector2f(5.f, 10.f));
				sfRectangleShape_setPosition(squareMenuLight, lightPosSliderB);
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfRectangleShape_setFillColor(squareMenuLight, lightColorPreview);
				sfRectangleShape_setSize(squareMenuLight, vector2f(70.f, 50.f));
				sfRectangleShape_setPosition(squareMenuLight, vector2f(1770.f, 450.f));
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfRectangleShape_setOrigin(squareMenuLight, vector2f(0.f, 0.f));

				sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
				sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
				sfRectangleShape_setPosition(squareMenuLight, vector2f(1740.f, 550.f));
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfText_setString(editorDetailButton, "+");
				sfText_setPosition(editorDetailButton, vector2f(1752.f, 545.f));
				sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

				sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
				sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
				sfRectangleShape_setPosition(squareMenuLight, vector2f(1700.f, 550.f));
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfText_setString(editorDetailButton, "-");
				sfText_setPosition(editorDetailButton, vector2f(1712.f, 545.f));
				sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

				switch (lightForm)
				{
				case EDITOR_LIGHT_CIRCLE:
					sfText_setString(editorLightButtonText, "CIRCLE");
					break;
				case EDITOR_LIGHT_RECTANGLE:
					sfText_setString(editorLightButtonText, "RECTANGLE");
					break;
				case EDITOR_LIGHT_CONE:
					sfText_setString(editorLightButtonText, "CONE");
					break;
				}
				sfText_setPosition(editorLightButtonText, vector2f(1780.f, 540.f));
				sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

				sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
				sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
				sfRectangleShape_setPosition(squareMenuLight, vector2f(1810.f, 600.f));
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfText_setString(editorDetailButton, "+");
				sfText_setPosition(editorDetailButton, vector2f(1822.f, 595.f));
				sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

				sfRectangleShape_setFillColor(squareMenuLight, sfRed);
				sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
				sfRectangleShape_setPosition(squareMenuLight, vector2f(1760.f, 600.f));
				sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

				sfText_setString(editorDetailButton, "-");
				sfText_setPosition(editorDetailButton, vector2f(1772.f, 595.f));
				sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

				if (lightForm != EDITOR_LIGHT_RECTANGLE)
				{
					if (lightForm == EDITOR_LIGHT_CIRCLE)
					{
						sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
						sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
						sfRectangleShape_setPosition(squareMenuLight, vector2f(1810.f, 750.f));
						sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

						sfText_setString(editorDetailButton, "+");
						sfText_setPosition(editorDetailButton, vector2f(1822.f, 745.f));
						sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

						sfRectangleShape_setFillColor(squareMenuLight, sfRed);
						sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
						sfRectangleShape_setPosition(squareMenuLight, vector2f(1760.f, 750.f));
						sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

						sfText_setString(editorDetailButton, "-");
						sfText_setPosition(editorDetailButton, vector2f(1772.f, 745.f));
						sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

						sfText_setString(editorLightButtonText, circleStrengthValue);
						sfText_setPosition(editorLightButtonText, vector2f(1740.f, 780.f));
						sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

						sfText_setString(editorLightButtonText, "X");
					}
					if (lightForm == EDITOR_LIGHT_CONE)
					{

						sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
						sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
						sfRectangleShape_setPosition(squareMenuLight, vector2f(1810.f, 750.f));
						sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

						sfText_setString(editorDetailButton, "+");
						sfText_setPosition(editorDetailButton, vector2f(1822.f, 745.f));
						sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

						sfRectangleShape_setFillColor(squareMenuLight, sfRed);
						sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
						sfRectangleShape_setPosition(squareMenuLight, vector2f(1740.f, 750.f));
						sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

						sfText_setString(editorDetailButton, "-");
						sfText_setPosition(editorDetailButton, vector2f(1752.f, 745.f));
						sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

						sfText_setString(editorLightButtonText, coneAngleValue);
						sfText_setPosition(editorLightButtonText, vector2f(1760.f, 770.f));
						sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

						sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
						sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
						sfRectangleShape_setPosition(squareMenuLight, vector2f(1810.f, 820.f));
						sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

						sfText_setString(editorDetailButton, "+");
						sfText_setPosition(editorDetailButton, vector2f(1822.f, 815.f));
						sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

						sfRectangleShape_setFillColor(squareMenuLight, sfRed);
						sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
						sfRectangleShape_setPosition(squareMenuLight, vector2f(1740.f, 820.f));
						sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

						sfText_setString(editorDetailButton, "-");
						sfText_setPosition(editorDetailButton, vector2f(1752.f, 815.f));
						sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

						sfText_setString(editorLightButtonText, coneRotateValue);
						sfText_setPosition(editorLightButtonText, vector2f(1760.f, 850.f));
						sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

						sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
						sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
						sfRectangleShape_setPosition(squareMenuLight, vector2f(1810.f, 900.f));
						sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

						sfText_setString(editorDetailButton, "+");
						sfText_setPosition(editorDetailButton, vector2f(1822.f, 895.f));
						sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

						sfRectangleShape_setFillColor(squareMenuLight, sfRed);
						sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
						sfRectangleShape_setPosition(squareMenuLight, vector2f(1740.f, 900.f));
						sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

						sfText_setString(editorDetailButton, "-");
						sfText_setPosition(editorDetailButton, vector2f(1752.f, 895.f));
						sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

						sfText_setString(editorLightButtonText, circleStrengthValue);
						sfText_setPosition(editorLightButtonText, vector2f(1740.f, 930.f));
						sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

						sfText_setString(editorLightButtonText, "Y");
					}
					sfText_setPosition(editorLightButtonText, vector2f(1860.f, 590.f));
					sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);
				}

				if (lightForm == EDITOR_LIGHT_RECTANGLE)
				{
					sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
					sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
					sfRectangleShape_setPosition(squareMenuLight, vector2f(1810.f, 650.f));
					sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

					sfText_setString(editorDetailButton, "+");
					sfText_setPosition(editorDetailButton, vector2f(1822.f, 645.f));
					sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

					sfRectangleShape_setFillColor(squareMenuLight, sfRed);
					sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
					sfRectangleShape_setPosition(squareMenuLight, vector2f(1760.f, 650.f));
					sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

					sfText_setString(editorDetailButton, "-");
					sfText_setPosition(editorDetailButton, vector2f(1772.f, 645.f));
					sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

					sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
					sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
					sfRectangleShape_setPosition(squareMenuLight, vector2f(1810.f, 750.f));
					sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

					sfText_setString(editorDetailButton, "+");
					sfText_setPosition(editorDetailButton, vector2f(1822.f, 745.f));
					sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

					sfRectangleShape_setFillColor(squareMenuLight, sfRed);
					sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
					sfRectangleShape_setPosition(squareMenuLight, vector2f(1760.f, 750.f));
					sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

					sfText_setString(editorDetailButton, "-");
					sfText_setPosition(editorDetailButton, vector2f(1772.f, 745.f));
					sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

					sfText_setString(editorLightButtonText, circleStrengthValue);
					sfText_setPosition(editorLightButtonText, vector2f(1740.f, 780.f));
					sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

					sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
					sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
					sfRectangleShape_setPosition(squareMenuLight, vector2f(1810.f, 820.f));
					sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

					sfText_setString(editorDetailButton, "+");
					sfText_setPosition(editorDetailButton, vector2f(1822.f, 815.f));
					sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

					sfRectangleShape_setFillColor(squareMenuLight, sfRed);
					sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
					sfRectangleShape_setPosition(squareMenuLight, vector2f(1760.f, 820.f));
					sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

					sfText_setString(editorDetailButton, "-");
					sfText_setPosition(editorDetailButton, vector2f(1772.f, 815.f));
					sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

					sfText_setString(editorLightButtonText, coneRotateValue);
					sfText_setPosition(editorLightButtonText, vector2f(1760.f, 850.f));
					sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

					sfText_setString(editorLightButtonText, "X");
					sfText_setPosition(editorLightButtonText, vector2f(1860.f, 590.f));
					sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

					sfText_setString(editorLightButtonText, "Y");
					sfText_setPosition(editorLightButtonText, vector2f(1860.f, 640.f));
					sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);
				}

				sfText_setString(editorLightButtonText, lightSizeValue);
				sfText_setPosition(editorLightButtonText, vector2f(1720.f, 700.f));
				sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);
			}
		}
		if (!toggleLight)
		{
			sfRectangleShape_setPosition(editorButton, vector2f(1750.f, 250.f));
			sfRenderTexture_drawRectangleShape(_window->rdrt, editorButton, NULL);
			sfText_setPosition(editorButtonText, vector2f(1780.f, 290.f));
			sfText_setString(editorButtonText, "PARTICLES");
			sfRenderTexture_drawText(_window->rdrt, editorButtonText, NULL);
		}
		break;
	case EDITOR_STATE_CFGEDITOR:
		sfRectangleShape_setOrigin(squareMenuLight, vector2f(0.f, 0.f));

		sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 100.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "+");
		sfText_setPosition(editorDetailButton, vector2f(1762.f, 95.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 100.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "-");
		sfText_setPosition(editorDetailButton, vector2f(1722.f, 95.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfText_setString(editorLightButtonText, "Layer_choice :");
		sfText_setPosition(editorLightButtonText, vector2f(1720.f, 50.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		switch (editorLayer)
		{
		case EDITOR_LAYER_GROUND:
			sfText_setString(editorLightButtonText, "Ground");
			break;
		case EDITOR_LAYER_WALLS:
			sfText_setString(editorLightButtonText, "Walls");
			break;
		case EDITOR_LAYER_COLLISION:
			sfText_setString(editorLightButtonText, "Collisions");
			break;
		}
		sfText_setPosition(editorLightButtonText, vector2f(1790.f, 90.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		sfText_setString(editorLightButtonText, "Width");
		sfText_setPosition(editorLightButtonText, vector2f(1710.f, 140.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 190.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "+");
		sfText_setPosition(editorDetailButton, vector2f(1762.f, 185.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfRed);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 190.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "-");
		sfText_setPosition(editorDetailButton, vector2f(1722.f, 185.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfText_setString(editorLightButtonText, "Height");
		sfText_setPosition(editorLightButtonText, vector2f(1810.f, 140.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1850.f, 190.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "+");
		sfText_setPosition(editorDetailButton, vector2f(1862.f, 185.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfRed);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1810.f, 190.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "-");
		sfText_setPosition(editorDetailButton, vector2f(1822.f, 185.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfText_setString(editorLightButtonText, stringSize);
		sfText_setPosition(editorLightButtonText, vector2f(1710.f, 230.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 360.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "+");
		sfText_setPosition(editorDetailButton, vector2f(1762.f, 355.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfRed);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 360.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "-");
		sfText_setPosition(editorDetailButton, vector2f(1722.f, 355.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfText_setString(editorLightButtonText, "Brush Size :");
		sfText_setPosition(editorLightButtonText, vector2f(1720.f, 320.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		sfText_setString(editorLightButtonText, stringBrushSize);
		sfText_setPosition(editorLightButtonText, vector2f(1800.f, 350.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 450.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "+");
		sfText_setPosition(editorDetailButton, vector2f(1762.f, 445.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfRed);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 450.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "-");
		sfText_setPosition(editorDetailButton, vector2f(1722.f, 445.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfText_setString(editorLightButtonText, "View Speed :");
		sfText_setPosition(editorLightButtonText, vector2f(1720.f, 410.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		sfText_setString(editorLightButtonText, stringViewSpeed);
		sfText_setPosition(editorLightButtonText, vector2f(1800.f, 440.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 530.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "+");
		sfText_setPosition(editorDetailButton, vector2f(1762.f, 525.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfRed);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1710.f, 530.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "-");
		sfText_setPosition(editorDetailButton, vector2f(1722.f, 525.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfText_setString(editorLightButtonText, "Change Enlargement :");
		sfText_setPosition(editorLightButtonText, vector2f(1710.f, 490.f));
		sfText_setCharacterSize(editorLightButtonText, 23);
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);
		sfText_setCharacterSize(editorLightButtonText, 30);

		sfText_setString(editorLightButtonText, stringEnlargement);
		sfText_setPosition(editorLightButtonText, vector2f(1800.f, 520.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfGreen);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1790.f, 940.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "+");
		sfText_setPosition(editorDetailButton, vector2f(1802.f, 935.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfRectangleShape_setFillColor(squareMenuLight, sfRed);
		sfRectangleShape_setSize(squareMenuLight, vector2f(30.f, 20.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1750.f, 940.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setString(editorDetailButton, "-");
		sfText_setPosition(editorDetailButton, vector2f(1762.f, 935.f));
		sfRenderTexture_drawText(_window->rdrt, editorDetailButton, NULL);

		sfText_setString(editorLightButtonText, "Change Map :");
		sfText_setPosition(editorLightButtonText, vector2f(1720.f, 900.f));
		sfRenderTexture_drawText(_window->rdrt, editorLightButtonText, NULL);

		sfText_setPosition(editorMapTextId, vector2f(1750.f, 970.f));
		sfRenderTexture_drawText(_window->rdrt, editorMapTextId, NULL);


		sfRectangleShape_setFillColor(squareMenuLight, sfWhite);
		sfRectangleShape_setSize(squareMenuLight, vector2f(130.f, 40.f));
		sfRectangleShape_setPosition(squareMenuLight, vector2f(1720.f, 1010.f));
		sfRenderTexture_drawRectangleShape(_window->rdrt, squareMenuLight, NULL);

		sfText_setCharacterSize(editorMapTextSave, 20);
		sfText_setPosition(editorMapTextSave, vector2f(1730.f, 1015.f));
		sfRenderTexture_drawText(_window->rdrt, editorMapTextSave, NULL);

		sfText_setCharacterSize(editorMapTextSave, 30);
		break;
	}
	sfRenderTexture_setView(_window->rdrt, mainView->view);
}

void deinitHUD(Window* _window)
{
	sfText_destroy(editorLayerText);
	sfText_destroy(editorMapText);
	sfText_destroy(editorMapSizeText);
	sfRectangleShape_destroy(editorHUDRect);
}

void editorHud_ChangeLayer(Window* _window)
{
	float dt = getDeltaTime();
	static float timer = 0.f;
	timer += dt;

	if (customKeyboardIsKeyPressed(sfKeyLControl, _window->renderWindow) && timer > 0.2f)
	{
		timer = 0.f;
		if (firstOrSecond == 1) firstOrSecond = 2;
		else firstOrSecond = 1;

		if (firstOrSecond == 1) sfText_setString(editorLayerText, "Layer : Ground 1");
		else sfText_setString(editorLayerText, "Layer : Ground 2");

		sprintf(strSubLayer, "%d", firstOrSecond);
	}
	if (customKeyboardIsKeyPressed(sfKeyNum1, _window->renderWindow))
	{
		if (firstOrSecond == 1) sfText_setString(editorLayerText, "Layer : Ground 1");
		else sfText_setString(editorLayerText, "Layer : Ground 2");

		editorLayer = EDITOR_LAYER_GROUND;
		switch (editorHudGroundType)
		{
		case EDITOR_GROUND_SAND: sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_ground_sand"), sfTrue); break;
		case EDITOR_GROUND_ROCK: sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_ground_rock"), sfTrue); break;
		case EDITOR_GROUND_GRASS: sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_ground_grass"), sfTrue); break;
		case EDITOR_GROUND_WATER: sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_ground_water"), sfTrue); break;
		case EDITOR_GROUND_DIRT: sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_ground_dirt"), sfTrue); break;
		case EDITOR_GROUND_DUNGEON: sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_ground_dungeon"), sfTrue); break;
		case EDITOR_GROUND_LEAF: sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_ground_leaf"), sfTrue); break;
		}
	}
	else if (customKeyboardIsKeyPressed(sfKeyNum2, _window->renderWindow))
	{
		sfText_setString(editorLayerText, "Layer : Walls");
		editorLayer = EDITOR_LAYER_WALLS;
		switch (editorHudWallType)
		{
		case EDITOR_WALL_EXTERIOR: sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_wall_ext"), sfTrue); break;
		case EDITOR_WALL_INTERIOR: sfSprite_setTexture(editorHudSprite, GetSfTexture("ts_wall_int"), sfTrue); break;
		}
	}
	else if (customKeyboardIsKeyPressed(sfKeyNum3, _window->renderWindow))
	{
		sfText_setString(editorLayerText, "Layer : Collisions");
		editorLayer = EDITOR_LAYER_COLLISION;
	}
}

EditorState getEditorState()
{
	return editorHudState;
}

void setEditorState(EditorTilesLayer _val)
{
	editorHudState = _val;
}

EditorTilesLayer getEditorTilesLayer()
{
	return editorLayer;
}

EditorEntity getEditorEntityState()
{
	return editorHudEntity;
}

EditorEffects getEditorEffectsState()
{
	return editorHudEffects;
}

NpcId getEditorPnjChoice()
{
	return editorHudPnjChoice;
}

EditorGroundType getEditorGroundType()
{
	return editorHudGroundType;
}


sfVector3f getEditorLightColor()
{
	sfVector3f color = { normR, normG, normB };
	return color;
}

sfVector2f getEditorLightSize()
{
	return lightSize;
}

EditorLightForm getEditorLightForm()
{
	return lightForm;
}

float getEditorConeAngle()
{
	return lightAngle;
}

float getEditorConeRotation()
{
	return lightRotate;
}

float getEditorlightStrength()
{
	return lightStrength;
}

void setMapText(char* _txt)
{
	sfText_setString(editorMapText, _txt);
}

void setMapTextID(char* _txt)
{
	sfText_setString(editorMapTextId, _txt);
}

void setMapTextSave(char* _txt)
{
	sfText_setString(editorMapTextSave, _txt);
}

void setWantMapSave(sfBool _state)
{
	wantMapSave = _state;
}

sfBool getWantMapSave()
{
	return wantMapSave;
}

char editorHud_getFirstOrSecondLayer()
{
	return firstOrSecond;
}

void setEditorWallType(EditorWallType _val)
{
	editorHudWallType = _val;
}

PropsType getCurrentPropsType()
{
	return editorHudPropsType;
}

int getCurrentProps()
{
	return editorHudCurrentProps;
}

float getEditorViewSpeed()
{
	return editorHudViewSpeed;
}

EnemiesType getEditorHudMobType()
{
	return editorHudMobType;
}

float getEditorHudMobRotation()
{
	return editorHudMobRotation;
}

void setEditorHudMapEnlargement(int _val)
{
	editorHudMapEnlargement = _val;
}

int getEditorHudMapEnlargement()
{
	return editorHudMapEnlargement;
}