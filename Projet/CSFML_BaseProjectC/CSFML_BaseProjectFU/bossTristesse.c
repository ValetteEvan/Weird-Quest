#include "bossTristesse.h"
#include "player.h"
#include "fx_boss.h"

#define BOS_X_SIZE 169 
#define BOS_Y_SIZE 320

sfRectangleShape* boss_Rectangle_T;  
sfRectangleShape* boss_Hand_1T; 
sfRectangleShape* boss_Hand_2T; 
sfRectangleShape* boss_Disc_T;  

sfTexture* boss_Texture_T;  
sfTexture* hand_TextureT;   

sfVector2f pos_Hand_1T;
sfVector2f tmp_Pos_Player1; 
sfVector2f tmp_Pos_Player2;
sfVector2f pos_Hand_2T; 

sfBool bool_anneau_Here; 
sfBool bool_start_Hand;
sfBool bool_end_Hand; 
sfBool bool_boss_Sleep; 
sfBool bool_boss_Normal; 
sfBool bool_disc_Start; 
sfBool bool_disc_End;  
sfBool bool_anim_simple; 
sfBool bool_anim_simple2;
sfBool bool_anim_Boom2; 
sfBool bool_anim_Boom3;
sfBool bool_tmp_Lerp2; 
sfBool bool_distance_Boom2;  
sfBool bool_tmp_Lerp3;  
sfBool bool_anim_simple3; 
sfBool bool_distance_Boom3; 
sfBool bool_left_Boss; 
sfBool bool_right_Boss;
sfBool isHereSadnesseBoss; 
sfBool bool_lerp;

float timer_anim_BossT;
float timer_anim_Disc1; 
float timer_anim_Disc2; 
float timer_Attack_Phase1;
float timer_Attack_Phase2; 
float timer_Boss_Chrono;
float timer_Boss_Chrono2;
float timer_Boss_Chrono3;
float timer_Boss_Chrono4;
float timer_Boss_Chrono5;
float timer_Boss_Chrono6;
float timer_Boss_Chrono8;
float timerLerpReset; 
float timerLerpReset2;
float timerLerpFight3; 
float lerp_Valeur; 
float angle_Boss_Sadness;

int fram_X_Disc1; 
int fram_X_Boss;
int fram_X_Disc2;

Player player;  

void bossSadness_loadTexture()
{
	boss_Texture_T = GetSfTexture("golemT_IDLE_ANNEAU");
	hand_TextureT = GetSfTexture("golemT_IDLE_ANNEAU");
	BossAnneau.texture = GetSfTexture("golemT_IDLE_ANNEAU");
}

BossTristesse initBoss(sfVector2f _position, int _life) 
{
	bossSadness_loadTexture();
	boss_Rectangle_T = sfRectangleShape_create();
	boss_Hand_1T = sfRectangleShape_create();
	boss_Hand_2T = sfRectangleShape_create(); 

    tmp_Pos_Player1 = vector2f(100.f, 100.f);  
	tmp_Pos_Player2 = vector2f(100.f, 100.f); 

	bossTristesse.position = _position;
	bossTristesse.size = vector2f(BOS_X_SIZE, BOS_Y_SIZE);
	bossTristesse.origin = vector2f(bossTristesse.size.x / 2, bossTristesse.size.y / 2);
	bossTristesse.life = _life;
	bossTristesse.firstHandHP = 5;
	bossTristesse.secondHandHP = 5;
	bossTristesse.disc = sfTrue;
	bossTristesse.cristalHP = 5;
	bossTristesse.frameXBoss = 0;
	bossTristesse.bossRect = IntRect(0, 1508, BOS_X_SIZE, BOS_Y_SIZE);
	bossTristesse.animTimerBoss = 0.f;
	sfRectangleShape_setSize(boss_Rectangle_T, bossTristesse.size);
	sfRectangleShape_setOrigin(boss_Rectangle_T, bossTristesse.origin);
	sfRectangleShape_setTexture(boss_Rectangle_T, boss_Texture_T, sfTrue);  
	sfRectangleShape_setTextureRect(boss_Rectangle_T, bossTristesse.bossRect);
	sfRectangleShape_setPosition(boss_Rectangle_T, bossTristesse.position);
	 
    //Mains du Boss 
	pos_Hand_1T = vector2f(bossTristesse.position.x + 150.f, bossTristesse.position.y + 50.f);
	pos_Hand_2T = vector2f(bossTristesse.position.x - 150.f, bossTristesse.position.y + 50.f);
	 
	sfIntRect rectHand = IntRect(500, 0, 98.f, 98.f); 
	sfRectangleShape_setSize(boss_Hand_1T, vector2f(128.f, 134.f));  
	sfRectangleShape_setOrigin(boss_Hand_1T, vector2f(128.f / 2, 134.f / 2)); 
	sfRectangleShape_setTexture(boss_Hand_1T, hand_TextureT, sfTrue); 
	sfRectangleShape_setTextureRect(boss_Hand_1T, rectHand);    
	sfRectangleShape_setPosition(boss_Hand_1T, pos_Hand_1T); 

	sfRectangleShape_setSize(boss_Hand_2T, vector2f(128.f, 134.f));
	sfRectangleShape_setOrigin(boss_Hand_2T, vector2f(128.f / 2, 134.f / 2));  
	sfRectangleShape_setTexture(boss_Hand_2T, hand_TextureT, sfTrue);
	sfRectangleShape_setTextureRect(boss_Hand_2T, rectHand);   
	sfRectangleShape_setPosition(boss_Hand_2T, pos_Hand_2T);  

	angle_Boss_Sadness = 0.f;
	timer_Attack_Phase1 = 0.f; 
	timer_Boss_Chrono2 = 0.f; 
	timer_Boss_Chrono3 = 0.f;
	timer_Boss_Chrono4 = 0.f;
	timer_Boss_Chrono5 = 0.f;
	timer_Attack_Phase2 = 0.f;  
	timer_Boss_Chrono6 = 0.f; 
	timer_Boss_Chrono8 = 0.f; 
	timerLerpFight3 = 0.f;
	timerLerpReset = 0.f;
	timerLerpReset2 = 0.f;
	timer_Boss_Chrono = 0.f;  
	
	bossTristesse.timerAnimHand = 0.f;
	bossTristesse.timerAnimHand2 = 0.f;
	bossTristesse.timerAnimHand3 = 0.f; 
	bossTristesse.timerAnimHand4 = 0.f;
	bossTristesse.timerAnimHand5 = 0.f;
	bossTristesse.timerAnimHand6 = 0.f; 
	bossTristesse.timerAnimHand7 = 0.f;

	bossTristesse.frameXHand1 = 0;
	bossTristesse.frameXHand2 = 0;
	bossTristesse.frameXHand3 = 0;
	bossTristesse.frameXHand4 = 0; 
	bossTristesse.frameXHand5 = 0; 
	bossTristesse.frameXHand6 = 0;   
	bossTristesse.frameXHand7 = 0;

	bool_start_Hand = sfFalse; 
	bool_end_Hand = sfFalse; 
	bool_boss_Sleep = sfFalse; 
	bool_boss_Normal = sfFalse; 
	bool_disc_Start = sfFalse;
	bool_anim_Boom2 = sfFalse; 
	bool_anim_Boom3 = sfFalse; 
	bool_anim_simple = sfFalse; 
	bool_anim_simple2 = sfFalse; 
	bool_anim_simple3 = sfFalse; 
	bool_tmp_Lerp2 = sfFalse; 
	bool_tmp_Lerp3 = sfFalse; 
	bool_distance_Boom2 = sfFalse;  
	bool_distance_Boom3 = sfFalse; 
	isHereSadnesseBoss = sfFalse;  
	bool_anneau_Here = sfTrue; 
	bool_lerp = sfFalse; 

	return bossTristesse;
}

BossFragment initBossFragment(sfVector2f _position, int life) {

	boss_Disc_T = sfRectangleShape_create(); 
	BossAnneau.position = vector2f(_position.x , _position.y + 150.f);
	BossAnneau.size = vector2f(92.f,67.f); 
	BossAnneau.origin = vector2f(BossAnneau.size.x / 2, BossAnneau.size.y / 2); 
	BossAnneau.life = life; 
	BossAnneau.elementRect = (sfIntRect) { 0.f , 0.f, 92.f,67.f }; 

	sfRectangleShape_setPosition(boss_Disc_T, BossAnneau.position);  
	sfRectangleShape_setSize(boss_Disc_T, BossAnneau.size); 
	sfRectangleShape_setOrigin(boss_Disc_T, BossAnneau.origin); 
	sfRectangleShape_setTexture(boss_Disc_T, BossAnneau.texture, sfTrue); 
	sfRectangleShape_setTextureRect(boss_Disc_T, BossAnneau.elementRect); 

	bool_disc_End = sfFalse; 

	BossAnneau.timerfram = 0.f; 
	timer_anim_Disc1 = 0.f; 
	timer_anim_Disc2 = 0.f;
	timer_anim_BossT = 0.f;

	BossAnneau.framX = 0;   
	fram_X_Disc1 = 0; 
	fram_X_Disc2 = 0; 
	fram_X_Boss = 0; 

	return BossAnneau;  
}

//! Au lieux de passer par des timer passer a un autre patern quand il fini une attaque
void updateBoss(Window* _window)
{
	static float timerDebubSadnesse = 0.f; 
	timerDebubSadnesse += getDeltaTime(); 

	if (customKeyboardIsKeyPressed(sfKeyNum0, _window->renderWindow) && timerDebubSadnesse >= 0.2f ) {
		isHereSadnesseBoss = !isHereSadnesseBoss;
		timerDebubSadnesse = 0.f; 
	}

	if (customKeyboardIsKeyPressed(sfKeyNum9, _window->renderWindow) && timerDebubSadnesse >= 0.2f) {
		bossTristesse.firstHandHP = 0; 
		bossTristesse.secondHandHP = 0; 
		timerDebubSadnesse = 0.f;
	}
	if (isHereSadnesseBoss) {
		angle_Boss_Sadness += getDeltaTime() * 150.f;
		float distanceF = getSqrMagnitudeBetween2Vectorsf(player.pos, bossTristesse.position);

		switch (BossState)
		{
		case PHASE_SLEEP:
			if (distanceF <= 250000.f) {
				bool_boss_Sleep = sfTrue;
				bool_start_Hand = sfTrue;
				bool_disc_Start = sfTrue;
				BossState = PHASE_1;
			}
			break;
		case PHASE_1:
			if (BossState != PHASE_2) {
				timer_Attack_Phase1 += getDeltaTime();

				if (timer_Attack_Phase1 >= 1.5f) {
					bool_end_Hand = sfTrue;
					bool_disc_End = sfTrue;
				}
				if (timer_Attack_Phase1 >= 3.f) {
					bool_boss_Sleep = sfFalse;
					bool_boss_Normal = sfTrue;
					bool_anim_simple = sfTrue;
					bool_anim_simple2 = sfTrue;
					bool_anim_simple3 = sfTrue;
					BossFight = FIGHT_1;
				}
				if (timer_Attack_Phase1 >= 15.f) {
					bool_left_Boss = sfFalse;
					bool_right_Boss = sfFalse;
					bool_lerp = sfTrue;
				}
				if (timer_Attack_Phase1 >= 25.f) {
					BossFight = FIGHT_2;
				}
				if (timer_Attack_Phase1 >= 35.f) {
					BossFight = FIGHT_3;
					bool_lerp = sfFalse;
				}
				if (timer_Attack_Phase1 >= 40.f) {
					timer_Attack_Phase1 = 0.f;
				}
				//Si les deux point son détruit alors lancement du compte a rebour avant qu'il ne regagne c'est point
				if (bossTristesse.firstHandHP <= 0 && bossTristesse.secondHandHP <= 0) {
					timer_Boss_Chrono += getDeltaTime();
					//plr_addDamagePlayerLife(); 
					if (timer_Boss_Chrono <= 5.f) {
						if (customKeyboardIsKeyPressed(sfKeyI, _window->renderWindow)) {
							bossTristesse.disc = sfFalse;
						}
						//Possibiliter de détruire l'anneau 
					}
					//Si anneau non détruit il regagne ces points  
					else {
						bossTristesse.firstHandHP = 5; 
						bossTristesse.secondHandHP = 5; 
						timer_Boss_Chrono = 0.f; 
					}
				}
				// Si les deux point son détruit et le disc alors passage a la phase 2
				if (bossTristesse.firstHandHP <= 0 && bossTristesse.secondHandHP <= 0 && bossTristesse.disc == sfFalse) {
					bossTristesse.firstHandHP = 5;
					bossTristesse.secondHandHP = 5;
					BossState = PHASE_2;
				}
			}
			break;
		case PHASE_2:
			timer_Attack_Phase2 += getDeltaTime();
			timer_Boss_Chrono = 0.f;   
			if (timer_Attack_Phase2 >= 1.f) {
				BossFight = FIGHT_0; 
			}
			if (timer_Attack_Phase2 >= 5.f) {
				BossFight = FIGHT_1;
			}
			if (timer_Attack_Phase2 >= 10.f) {
				bool_left_Boss = sfFalse;
				bool_right_Boss = sfFalse;
				bool_lerp = sfTrue;
			}
			if (timer_Attack_Phase2 >= 20.f) {
				BossFight = FIGHT_2;
			}
			if (timer_Attack_Phase2 >= 30.f) {
				BossFight = FIGHT_3; 
				bool_lerp = sfFalse;  
			}
			if (timer_Attack_Phase2 >= 35.f) {
				timer_Attack_Phase2 = 0.f;
			}
			if (bossTristesse.firstHandHP == 0 && bossTristesse.secondHandHP == 0 && bossTristesse.cristalHP <= 0) {
				// Mort du BOSS ! 
			}
			break;

		}
		//! Toutes les mécanique du boss ici -> 
		if (BossFight == FIGHT_0) { 
			bool_boss_Normal = sfTrue; 
			bool_left_Boss = sfFalse; 
			bool_right_Boss = sfFalse; 
			bossTristesse.position = vec2f_lerp(bossTristesse.position, vector2f(800.f, 150.f), 1.5 * getDeltaTime());  
			pos_Hand_1T = vec2f_lerp(pos_Hand_1T, vector2f(bossTristesse.position.x + 150.f, bossTristesse.position.y + 50.f), 1.5f * getDeltaTime()); 
			pos_Hand_2T = vec2f_lerp(pos_Hand_2T, vector2f(bossTristesse.position.x - 150.f, bossTristesse.position.y + 50.f), 1.5f * getDeltaTime());  
		}

		if (BossFight == FIGHT_1) {
			static float distance = 100.7f;
			static sfBool left = sfTrue;
			timer_Boss_Chrono2 += getDeltaTime();
			bool_disc_End = sfFalse; 
			// DEPLACEMENT LERP BOSS ---------------------------------------------------------|
			if (bossTristesse.position.x >= 250.f && left == sfTrue) {
				bool_left_Boss = sfTrue;
				bool_right_Boss = sfFalse;
				bossTristesse.position.x = lerp(bossTristesse.position.x, 10.f, 0.5 * getDeltaTime());
			}
			else left = sfFalse;
			
			if (bossTristesse.position.x <= 1000.f && left == sfFalse) {
				bool_right_Boss = sfTrue;
				bool_left_Boss = sfFalse;
				bossTristesse.position.x = lerp(bossTristesse.position.x, 1700.f, 0.5 * getDeltaTime());
			}
			else left = sfTrue;
			
			// -------------------------------------------------------------------------------|

			// ELOIGNEMENT DES MAIN ---------------------------------------------------------------------------|
			if (pos_Hand_1T.y < 245.f) pos_Hand_1T = vec2f_lerp(pos_Hand_1T, vector2f(pos_Hand_1T.x, lerp_Valeur), 0.5f * getDeltaTime());
			if (pos_Hand_2T.y < 245.f) pos_Hand_2T = vec2f_lerp(pos_Hand_2T, vector2f(pos_Hand_2T.x, lerp_Valeur), 0.5f * getDeltaTime());
			// ------------------------------------------------------------------------------------------------|

			// ROTATION DES MAINS -------------------------------------------------------------|
			if (timer_Boss_Chrono2 <= 5.f) {
				distance += getDeltaTime() * 100.f;
				rotationHandBoss(&pos_Hand_1T, &pos_Hand_2T, distance, angle_Boss_Sadness, &bossTristesse);
			}
			else if (timer_Boss_Chrono2 <= 10.f) {
				distance -= getDeltaTime() * 100.f;
				rotationHandBoss(&pos_Hand_1T, &pos_Hand_2T, distance, angle_Boss_Sadness, &bossTristesse);
			}
			else timer_Boss_Chrono2 = 0.f;
			// --------------------------------------------------------------------------------|
		}
		if (BossFight == FIGHT_2) {
			static float timerBoom = 0.f;  
			static float timerBoom2 = 0.f; 
		
			timer_Boss_Chrono3 += getDeltaTime();
			timer_Boss_Chrono4 += getDeltaTime();
			timer_Boss_Chrono5 += getDeltaTime();
			timer_Boss_Chrono6 += getDeltaTime();
			timer_Boss_Chrono8 += getDeltaTime();
			timerLerpReset += getDeltaTime(); 
			timerLerpReset2 += getDeltaTime();
			
			bool_tmp_Lerp2 = sfTrue;
			bool_tmp_Lerp3 = sfTrue;

			if (timerLerpReset <= 3.f) pos_Hand_1T = vec2f_lerp(pos_Hand_1T, vector2f(bossTristesse.position.x + 150.f, bossTristesse.position.y + 50.f), 1.5f * getDeltaTime());
			if (timerLerpReset2 <= 3.f) pos_Hand_2T = vec2f_lerp(pos_Hand_2T, vector2f(bossTristesse.position.x - 150.f, bossTristesse.position.y + 50.f), 1.5f * getDeltaTime());

			if (timer_Boss_Chrono4 >= 4.f) {
				tmp_Pos_Player1 = vector2f(player.pos.x, player.pos.y);
				float hand1_to_Player = getSqrMagnitudeBetween2Vectorsf(tmp_Pos_Player1, pos_Hand_1T);

				if (bool_tmp_Lerp3 == sfTrue) pos_Hand_1T = vec2f_lerp(pos_Hand_1T, tmp_Pos_Player1, 1.5f * getDeltaTime());
				if (timer_Boss_Chrono4 >= 8.5f) {
					bool_anim_Boom3 = sfTrue;
					bool_anim_simple = sfFalse; 
					bool_tmp_Lerp3 = sfFalse;
				}
				//=============================================/
				if (hand1_to_Player <= 3000.f && timer_Boss_Chrono4 <= 8.5f && timer_Boss_Chrono8 >= 1.f) {    
					timerBoom += getDeltaTime(); 
					bool_anim_Boom3 = sfTrue;
					bool_anim_simple = sfFalse;
					bool_tmp_Lerp3 = sfFalse;
					bool_distance_Boom3 = sfTrue;
				}
				//=============================================/
				if (timer_Boss_Chrono4 >= 9.f || bool_distance_Boom3 == sfTrue &&  timerBoom >= 0.5f) {
					//Explosion here
					fx_Create2(IMPACT, vector2f(pos_Hand_1T.x + 300, pos_Hand_1T.y + 200));
					fx_Create1(EXPLOSION_CENTER, vector2f(pos_Hand_1T.x + 300, pos_Hand_1T.y + 200));
					bool_anim_Boom3 = sfFalse;
					bool_anim_simple = sfTrue;
					bossTristesse.frameXHand3 = 0;
					bossTristesse.timerAnimHand3 = 0.f;
					timerBoom = 0.f;
					timer_Boss_Chrono8 = 0.f;
					timerLerpReset = 0.f; 
					bool_distance_Boom3 = sfFalse;
					timer_Boss_Chrono4 = 0.f; 
				}
			}

			if (timer_Boss_Chrono3 >= 4.f) {
				tmp_Pos_Player2 = vector2f(player.pos.x, player.pos.y);
				float hand_to_Player2 = getSqrMagnitudeBetween2Vectorsf(tmp_Pos_Player2, pos_Hand_2T);
				if (bool_tmp_Lerp2 == sfTrue) pos_Hand_2T = vec2f_lerp(pos_Hand_2T, vector2f(tmp_Pos_Player2.x, tmp_Pos_Player2.y), 1.f * getDeltaTime());
				if (timer_Boss_Chrono3 >= 8.5f) {
					bool_anim_Boom2 = sfTrue;
					bool_anim_simple2 = sfFalse;
					bool_tmp_Lerp2 = sfFalse;
				}
				//=============================================/
				if (hand_to_Player2 <= 3000.f && timer_Boss_Chrono3 <= 8.5f && timer_Boss_Chrono6 >= 1.f) {
					timerBoom2 += getDeltaTime(); 
					bool_anim_Boom2 = sfTrue;
					bool_anim_simple2 = sfFalse;
					bool_tmp_Lerp2 = sfFalse;
					bool_distance_Boom2 = sfTrue;
				}
				//=============================================/
				if (timer_Boss_Chrono3 >= 8.7f || bool_distance_Boom2 == sfTrue && timerBoom2 >= 0.5f) {
					//Explosion here
					fx_Create2(IMPACT, vector2f(pos_Hand_2T.x + 300, pos_Hand_2T.y + 200));
					fx_Create1(EXPLOSION_CENTER, vector2f(pos_Hand_2T.x + 300, pos_Hand_2T.y + 200));
					bool_anim_Boom2 = sfFalse;
					bool_anim_simple2 = sfTrue;
					bossTristesse.frameXHand3 = 0;
					bossTristesse.timerAnimHand3 = 0.f;
					bool_distance_Boom2 = sfFalse;
					pos_Hand_1T = vec2f_lerp(pos_Hand_1T, vector2f(bossTristesse.position.x + 150.f, bossTristesse.position.y + 50.f), 1.5f * getDeltaTime());
					pos_Hand_2T = vec2f_lerp(pos_Hand_2T, vector2f(bossTristesse.position.x - 150.f, bossTristesse.position.y + 50.f), 1.5f * getDeltaTime());
					timer_Boss_Chrono6 = 0.f;
					timerLerpReset2 = 0.f; 
					timerBoom2 = 0.f; 
					timer_Boss_Chrono3 = 0.f;
				}
			}
			timerLerpFight3 = 0.f; 
		}
		if (BossFight == FIGHT_3) {
			timerLerpFight3 += getDeltaTime(); 
			if (timerLerpFight3 <= 3.f) {
				pos_Hand_1T = vec2f_lerp(pos_Hand_1T, vector2f(bossTristesse.position.x + 150.f, bossTristesse.position.y + 50.f), 1.5f * getDeltaTime());
				pos_Hand_2T = vec2f_lerp(pos_Hand_2T, vector2f(bossTristesse.position.x - 150.f, bossTristesse.position.y + 50.f), 1.5f * getDeltaTime());
			}
			static float timerFight3 = 0.f;
			timerFight3 += getDeltaTime();
			if (timerFight3 >= 0.2f)
			{
				sfVector2f startPos = vector2f(rand_float(0, 1920.f), -100.f);
				trs_addTears(startPos, addVectorsrf(startPos, vector2f(0.f, (float)rand_float(200.f, 980.f))), 2.f, BIG_TEAR); 
				timerFight3 = 0.f;
			}
		}
		BossAnneau.position.x = bossTristesse.position.x;
		if (bool_disc_Start == sfTrue) sfRectangleShape_setPosition(boss_Disc_T, BossAnneau.position);
		if (bool_disc_End == sfTrue) sfRectangleShape_setPosition(boss_Disc_T, vector2f(BossAnneau.position.x, BossAnneau.position.y ));
		sfRectangleShape_setPosition(boss_Hand_1T, pos_Hand_1T);
		sfRectangleShape_setPosition(boss_Hand_2T, pos_Hand_2T);
		sfRectangleShape_setPosition(boss_Rectangle_T, bossTristesse.position);
	}
}

void displayBoss(Window* window)
{
	if (isHereSadnesseBoss) {
		if (bool_boss_Sleep == sfTrue) sfRectangleShape_setTextureRect(boss_Rectangle_T, animatorAnim("golemT_IDLE_START", &bossTristesse.frameXBoss, &bossTristesse.animTimerBoss, 0));
		if (bool_boss_Normal == sfTrue) {
			sfRectangleShape_setSize(boss_Rectangle_T, vector2f(166.f, 266.f));
			sfRectangleShape_setTextureRect(boss_Rectangle_T, animatorAnim("golemT_IDLE_SHINE", &fram_X_Boss, &timer_anim_BossT, 0));
		}
		if (bool_disc_Start == sfTrue) {
			sfRectangleShape_setPosition(boss_Disc_T, vector2f(BossAnneau.position.x, BossAnneau.position.y)); 
			sfRectangleShape_setTextureRect(boss_Disc_T, animatorAnim("golemT_APPEAR_ANNEAU", &fram_X_Disc1, &timer_anim_Disc1, 0));
		}
		if (bool_disc_End == sfTrue) {
			sfRectangleShape_setSize(boss_Disc_T, vector2f(92.f, 67.f));
			sfRectangleShape_setTextureRect(boss_Disc_T, animatorAnim("golemT_IDLE_ANNEAU", &fram_X_Disc2, &timer_anim_Disc2, 0));
			if (BossFight == FIGHT_2) sfRectangleShape_setPosition(boss_Disc_T, vector2f(BossAnneau.position.x, BossAnneau.position.y)); 
		}
		if (bool_start_Hand == sfTrue) {
			sfIntRect handRect = animatorAnim("golemT_Start_Hand", &bossTristesse.frameXHand1, &bossTristesse.timerAnimHand, 0);
			sfRectangleShape_setTextureRect(boss_Hand_1T, handRect);
			sfRectangleShape_setTextureRect(boss_Hand_2T, handRect);
		}
		if (bool_end_Hand == sfTrue) {
			sfIntRect handRect = animatorAnim("golemT_IDLE_GROW_V1", &bossTristesse.frameXHand2, &bossTristesse.timerAnimHand, 0);
			sfRectangleShape_setTextureRect(boss_Hand_1T, handRect);
			sfRectangleShape_setTextureRect(boss_Hand_2T, handRect);
		}
		if (bool_anim_Boom2 == sfTrue) {
			sfIntRect handRect = animatorAnim("golemT_Hand_Impact", &bossTristesse.frameXHand3, &bossTristesse.timerAnimHand3, 0);
			sfRectangleShape_setSize(boss_Hand_2T, vector2f(256.f, 268.f));
			sfRectangleShape_setOrigin(boss_Hand_2T, vector2f(256.f / 2, 268.f / 2));
			sfRectangleShape_setTextureRect(boss_Hand_2T, handRect);
		}
		if (bool_anim_Boom3 == sfTrue) {
			sfIntRect handRect = animatorAnim("golemT_Hand_Impact", &bossTristesse.frameXHand4, &bossTristesse.timerAnimHand4, 0);
			sfRectangleShape_setSize(boss_Hand_1T, vector2f(256.f, 268.f));
			sfRectangleShape_setOrigin(boss_Hand_1T, vector2f(256.f / 2, 268.f / 2));
			sfRectangleShape_setTextureRect(boss_Hand_1T, handRect);
		}
		if (bool_anim_simple == sfTrue) {
			sfIntRect handRect = animatorAnim("golemT_First_Idle", &bossTristesse.frameXHand5, &bossTristesse.timerAnimHand5, 0);
			sfRectangleShape_setSize(boss_Hand_1T, vector2f(98.f, 102.f));
			sfRectangleShape_setOrigin(boss_Hand_1T, vector2f(98.f / 2, 102.f / 2));
			sfRectangleShape_setTextureRect(boss_Hand_1T, handRect);
		}
		if (bool_anim_simple2 == sfTrue) {
			sfIntRect handRect = animatorAnim("golemT_First_Idle", &bossTristesse.frameXHand6, &bossTristesse.timerAnimHand6, 0);
			sfRectangleShape_setSize(boss_Hand_2T, vector2f(98.f, 102.f));
			sfRectangleShape_setOrigin(boss_Hand_2T, vector2f(98.f / 2, 102.f / 2));
			sfRectangleShape_setTextureRect(boss_Hand_2T, handRect);
		}

		if (bool_left_Boss == sfTrue) {
			sfIntRect bossRect = animatorAnim("golemT_IDLE_SIDE", &bossTristesse.frameXHand7, &bossTristesse.timerAnimHand7, 0);
			sfRectangleShape_setSize(boss_Rectangle_T, vector2f(172.f, 266.f));
			sfRectangleShape_setOrigin(boss_Rectangle_T, vector2f(172.f / 2, 266.f / 2));
			sfRectangleShape_setTextureRect(boss_Rectangle_T, bossRect);
			sfRectangleShape_setScale(boss_Rectangle_T, vector2f(1.f, 1.f));

			sfIntRect discRect = animatorAnim("golemT_SIDE_ANNEAU", &BossAnneau.framX, &BossAnneau.timerfram, 0); 
			sfRectangleShape_setSize(boss_Disc_T, vector2f(94.f, 94.f)); 
			sfRectangleShape_setOrigin(boss_Disc_T, vector2f(94.f / 2, 94.f / 2)); 
			sfRectangleShape_setPosition(boss_Disc_T, vector2f(BossAnneau.position.x, BossAnneau.position.y - 20.f));
			sfRectangleShape_setTextureRect(boss_Disc_T, discRect);
			sfRectangleShape_setScale(boss_Disc_T, vector2f(1.f, 1.f));

			sfIntRect handRect = animatorAnim("golemT_IDLE_GROW_V1", &bossTristesse.frameXHand5, &bossTristesse.timerAnimHand5, 0); 
			sfRectangleShape_setSize(boss_Hand_1T, vector2f(98.f, 102.f));
			sfRectangleShape_setOrigin(boss_Hand_1T, vector2f(98.f / 2, 102.f / 2));
			sfRectangleShape_setScale(boss_Hand_1T, vector2f(1.5f, 1.5f));
			sfRectangleShape_setTextureRect(boss_Hand_1T, handRect);
	
			sfRectangleShape_setSize(boss_Hand_2T, vector2f(98.f, 102.f));
			sfRectangleShape_setOrigin(boss_Hand_2T, vector2f(98.f / 2, 102.f / 2));
			sfRectangleShape_setScale(boss_Hand_2T, vector2f(1.5f, 1.5f)); 
			sfRectangleShape_setTextureRect(boss_Hand_2T, handRect);

		}
		if (bool_right_Boss == sfTrue) {
			sfIntRect bossRect = animatorAnim("golemT_IDLE_SIDE", &bossTristesse.frameXHand7, &bossTristesse.timerAnimHand7, 0);
			sfRectangleShape_setSize(boss_Rectangle_T, vector2f(172.f, 266.f));
			sfRectangleShape_setOrigin(boss_Rectangle_T, vector2f(172.f / 2, 266.f / 2));
			sfRectangleShape_setTextureRect(boss_Rectangle_T, bossRect);
			sfRectangleShape_setScale(boss_Rectangle_T, vector2f(-1.f, 1.f));

			sfIntRect discRect = animatorAnim("golemT_SIDE_ANNEAU", &BossAnneau.framX, &BossAnneau.timerfram, 0); 
			sfRectangleShape_setSize(boss_Disc_T, vector2f(94.f, 94.f));
			sfRectangleShape_setOrigin(boss_Disc_T, vector2f(94.f / 2, 94.f / 2));
			sfRectangleShape_setPosition(boss_Disc_T, vector2f(BossAnneau.position.x, BossAnneau.position.y - 20.f )); 
			sfRectangleShape_setTextureRect(boss_Disc_T, discRect);
			sfRectangleShape_setScale(boss_Disc_T, vector2f(-1.f, 1.f));

			sfIntRect handRect = animatorAnim("golemT_IDLE_GROW_V1", &bossTristesse.frameXHand5, &bossTristesse.timerAnimHand5, 0); 
			sfRectangleShape_setSize(boss_Hand_1T, vector2f(98.f, 102.f)); 
			sfRectangleShape_setOrigin(boss_Hand_1T, vector2f(98.f / 2, 102.f / 2)); 
			sfRectangleShape_setScale(boss_Hand_1T, vector2f(1.5f, 1.5f));
			sfRectangleShape_setTextureRect(boss_Hand_1T, handRect); 

			sfRectangleShape_setSize(boss_Hand_2T, vector2f(98.f, 102.f));
			sfRectangleShape_setOrigin(boss_Hand_2T, vector2f(98.f / 2, 102.f / 2)); 
			sfRectangleShape_setScale(boss_Hand_2T, vector2f(1.5f, 1.5f));
			sfRectangleShape_setTextureRect(boss_Hand_2T, handRect);

		}
		if (bossTristesse.disc)
		sfRenderTexture_drawRectangleShape(window->rdrt, boss_Disc_T, NULL);
		sfRenderTexture_drawRectangleShape(window->rdrt, boss_Rectangle_T, NULL);
		fx_DisplayLayer2(window);
		if (bossTristesse.firstHandHP >= 1) sfRenderTexture_drawRectangleShape(window->rdrt, boss_Hand_1T, NULL);
		if (bossTristesse.secondHandHP >= 1) sfRenderTexture_drawRectangleShape(window->rdrt, boss_Hand_2T, NULL);
		fx_DisplayLayer1(window);
	}
}
 
void  rotationHandBoss(sfVector2f* _pos_Hand_1T, sfVector2f* _pos_Hand_2T, float _distance,float _angle_Boss_Sadness, BossTristesse* _boss) {

	// if (_distance <= 800.f) _distance += 50.5f * getDeltaTime(); 
	*_pos_Hand_1T = polarCoord(_boss->position, _distance, (_angle_Boss_Sadness * DEG2RAD));     
	*_pos_Hand_2T = polarCoord(_boss->position, _distance, (_angle_Boss_Sadness + 180.f) * DEG2RAD);   
}

void deinitBossTristesse() {

}