#include "bossJoie.h"
#include "textureManager.h"
#include "player.h"
#include "ring.h"
#include "shaderManager.h"
#include "soundManager.h"
#include "pause.h"
#include "game_HUD.h"
#include "FXenemies.h"
#include "viewManager.h"
#include "particlesManager.h"
#include "vibrationManager.h"

sfSprite* bossJoieFXsprite;

sfRenderStates bossJoieRenderState;
sfShader* bossJoieShader;

sfBool prt_death;

BossJoie initBossJoie(sfVector2f _position, int _life)
{
    BossJoie tempBoss;

    tempBoss.bossSPR = sfRectangleShape_create();
    tempBoss.hand1SPR = sfRectangleShape_create();
    tempBoss.hand2SPR = sfRectangleShape_create();
    tempBoss.discSpr = sfRectangleShape_create();
    tempBoss.haloSpr = sfRectangleShape_create();

    sfRectangleShape_setSize(tempBoss.bossSPR, vector2f(GetRect("golemJ_spawn").width, GetRect("golemJ_spawn").height));
    sfRectangleShape_setSize(tempBoss.hand1SPR, vector2f(GetRect("golemJ_fistSpawn").width, GetRect("golemJ_fistSpawn").height));
    sfRectangleShape_setSize(tempBoss.hand2SPR, vector2f(GetRect("golemJ_fistSpawn").width, GetRect("golemJ_fistSpawn").height));
    sfRectangleShape_setSize(tempBoss.discSpr, vector2f(GetRect("golemJ_discSpawn").width, GetRect("golemJ_discSpawn").height));
    sfRectangleShape_setSize(tempBoss.haloSpr, vector2f(GetRect("golemJ_haloAttract").width, GetRect("golemJ_haloAttract").height));

    tempBoss.position = _position;
    tempBoss.hand1pos = addVectorsrf(tempBoss.position, vector2f(-190.f, 250.f));
    tempBoss.hand2pos = addVectorsrf(tempBoss.position, vector2f(190.f, 250.f));
    tempBoss.discPos = addVectorsrf(tempBoss.position, vector2f(0.f, 142.f));
    tempBoss.hand1Bounds = sfRectangleShape_getGlobalBounds(tempBoss.hand1SPR);
    tempBoss.hand2Bounds = sfRectangleShape_getGlobalBounds(tempBoss.hand2SPR);
    tempBoss.discBounds = sfRectangleShape_getGlobalBounds(tempBoss.discSpr);
    tempBoss.tempPlayerPos = vector2f(500.f, 500.f);
    tempBoss.getPlayerBounds = plr_getPlayerBoundsWithEnemies();

    tempBoss.bossLife  = 7;
    tempBoss.hand1Life = 6;
    tempBoss.hand2Life = 6;
    tempBoss.discLife  = 10;
    tempBoss.angle = 0.f;
    tempBoss.texture = GetSfTexture("golemJ_spawn");
    tempBoss.textureHand = GetSfTexture("golemJ_spawn");
    tempBoss.textureDisc = GetSfTexture("golemJ_spawn");
    tempBoss.textureHalo = GetSfTexture("golemJ_spawn");
    tempBoss.frameXboss = 0;
    tempBoss.frameXdisc = 0;
    tempBoss.frameXhand1 = 0;
    tempBoss.frameXhand2 = 0;
    tempBoss.frameXhalo = 0;
    tempBoss.frameXjump = 0;
    tempBoss.frameXdeath = 0;
    tempBoss.frameXattractF = 0;
    tempBoss.frameXattractD = 0;
    tempBoss.timerAnimDisc = 0.f;
    tempBoss.timerAnimBoss = 0.f;
    tempBoss.timerAnimHand1 = 0.f;
    tempBoss.timerAnimHand2 = 0.f;
    tempBoss.timerAnimHalo = 0.f;
    tempBoss.timerAnimJump = 0.f;
    tempBoss.timerAnimDeath = 0.f;
    tempBoss.timerAnimAttractF = 0.f;
    tempBoss.timerAnimAttractD = 0.f;

    tempBoss.timerAtk1 = 2.f;
    tempBoss.timerAtk2 = 0.f;
    tempBoss.timerAtk3 = 0.f;
    tempBoss.angleAtk2 = 0.f;

    tempBoss.origin     = vector2f(GetRect("golemJ_idleF").width * 0.5f, GetRect("golemJ_idleF").height * 0.5f);
    tempBoss.originHand = vector2f(GetRect("golemJ_fistF").width * 0.5f, GetRect("golemJ_fistF").height * 0.5f);
    tempBoss.originDisc = vector2f(GetRect("golemJ_discF").width * 0.5f, GetRect("golemJ_discF").height * 0.5f);
    tempBoss.originHalo = vector2f(GetRect("golemJ_haloAttract").width * 0.5f, GetRect("golemJ_haloAttract").height * 0.5f);
    tempBoss.forward    = vector2f(0.f, 0.f);
    tempBoss.oldForward = vector2f(0.f, 0.f);
    tempBoss.hasFirstHand       = sfTrue;
    tempBoss.hasSecondHand      = sfTrue;
    tempBoss.hasDisc            = sfTrue;
    tempBoss.cristal            = sfTrue;
    tempBoss.hasHalo            = sfFalse;
    tempBoss.isLoadingAttract   = sfFalse;
    tempBoss.isAttractingPlayer = sfFalse;
    tempBoss.isJumping          = sfFalse;
    tempBoss.isBossDamagable    = sfFalse;
    tempBoss.isHand1Damagable   = sfFalse;
    tempBoss.isHand2Damagable   = sfFalse;
    tempBoss.isDiscDamagable    = sfFalse;
    tempBoss.hasTookDamage      = sfFalse;
    tempBoss.isBossStuned       = sfFalse;
    tempBoss.isDead             = sfFalse;
    tempBoss.hand1hasTookDamage = sfFalse;
    tempBoss.hand2hasTookDamage = sfFalse;
    tempBoss.discHasTookDamage  = sfFalse;
    tempBoss.bossHasTookDamage  = sfFalse;
    tempBoss.isPlayerCloseSpawn = sfFalse;
    tempBoss.animSpawnBoss      = sfFalse;
    tempBoss.animSpawnDisc      = sfFalse;
    tempBoss.animSpawnHand      = sfFalse;
    tempBoss.sfxLoadingAttract  = sfFalse;
    tempBoss.sfxAttract         = sfFalse;
    tempBoss.sfxHalo            = sfFalse;
    tempBoss.sfxJump            = sfFalse;
    tempBoss.sfxDeath           = sfFalse;
    tempBoss.fxHand1            = sfFalse;
    tempBoss.fxHand2            = sfFalse;
    tempBoss.fxDisc             = sfFalse;

    sfRectangleShape_setTexture(tempBoss.bossSPR, tempBoss.texture, sfTrue);
    sfRectangleShape_setTexture(tempBoss.hand1SPR, tempBoss.textureHand, sfTrue);
    sfRectangleShape_setTexture(tempBoss.hand2SPR, tempBoss.textureHand, sfTrue);
    sfRectangleShape_setTexture(tempBoss.discSpr, tempBoss.textureDisc, sfTrue);
    sfRectangleShape_setTexture(tempBoss.haloSpr, tempBoss.textureHalo, sfTrue);
    sfRectangleShape_setOrigin(tempBoss.bossSPR, tempBoss.origin);
    sfRectangleShape_setOrigin(tempBoss.hand1SPR, tempBoss.originHand);
    sfRectangleShape_setOrigin(tempBoss.hand2SPR, tempBoss.originHand);
    sfRectangleShape_setOrigin(tempBoss.discSpr, tempBoss.originDisc);
    sfRectangleShape_setOrigin(tempBoss.haloSpr, tempBoss.originHalo);
    sfRectangleShape_setPosition(tempBoss.bossSPR, tempBoss.position);
    sfRectangleShape_setPosition(tempBoss.hand1SPR, tempBoss.hand1pos);
    sfRectangleShape_setPosition(tempBoss.hand2SPR, tempBoss.hand2pos);

    BossJoieState = BJ_PHASE_SPAWN;
    bossJoieFight = BJ_ATK_NONE;

    return tempBoss;
}

void bossJoie_restartBoss(BossJoie* _boss, sfVector2f _pos) {
    _boss->position = _pos;
    _boss->bossLife = 7;
    _boss->hand1Life = 6;
    _boss->hand2Life = 6;
    _boss->discLife = 10;
    _boss->angle = 0.f;
    _boss->frameXboss = 0;
    _boss->frameXdisc = 0;
    _boss->frameXhand1 = 0;
    _boss->frameXhand2 = 0;
    _boss->frameXhalo = 0;
    _boss->frameXjump = 0;
    _boss->frameXdeath = 0;
    _boss->frameXattractF = 0;
    _boss->frameXattractD = 0;
    _boss->timerAnimDisc = 0.f;
    _boss->timerAnimBoss = 0.f;
    _boss->timerAnimHand1 = 0.f;
    _boss->timerAnimHand2 = 0.f;
    _boss->timerAnimHalo = 0.f;
    _boss->timerAnimJump = 0.f;
    _boss->timerAnimDeath = 0.f;
    _boss->timerAnimAttractF = 0.f;
    _boss->timerAnimAttractD = 0.f;
    _boss->timerAtk1 = 2.f;
    _boss->timerAtk2 = 0.f;
    _boss->timerAtk3 = 0.f;
    _boss->angleAtk2 = 0.f;
    _boss->hasFirstHand = sfTrue;
    _boss->hasSecondHand = sfTrue;
    _boss->hasDisc = sfTrue;
    _boss->cristal = sfTrue;
    _boss->hasHalo = sfFalse;
    _boss->isLoadingAttract = sfFalse;
    _boss->isAttractingPlayer = sfFalse;
    _boss->isJumping = sfFalse;
    _boss->isBossDamagable = sfFalse;
    _boss->isHand1Damagable = sfFalse;
    _boss->isHand2Damagable = sfFalse;
    _boss->isDiscDamagable = sfFalse;
    _boss->hasTookDamage = sfFalse;
    _boss->isBossStuned = sfFalse;
    _boss->isDead = sfFalse;
    _boss->hand1hasTookDamage = sfFalse;
    _boss->hand2hasTookDamage = sfFalse;
    _boss->discHasTookDamage = sfFalse;
    _boss->bossHasTookDamage = sfFalse;
    _boss->isPlayerCloseSpawn = sfFalse;
    _boss->animSpawnBoss = sfFalse;
    _boss->animSpawnDisc = sfFalse;
    _boss->animSpawnHand = sfFalse;
    _boss->fxHand1 = sfFalse;
    _boss->fxHand2 = sfFalse;
    _boss->fxDisc = sfFalse;
    _boss->sfxLoadingAttract = sfFalse;
    _boss->sfxAttract = sfFalse;
    _boss->sfxHalo = sfFalse;
    _boss->sfxJump = sfFalse;
    _boss->sfxDeath = sfFalse;
    _boss->forward = vector2f(0.f, 0.f);
    _boss->oldForward = vector2f(0.f, 0.f);
    sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_spawn").width, GetRect("golemJ_spawn").height));
    sfRectangleShape_setSize(_boss->hand1SPR, vector2f(GetRect("golemJ_fistSpawn").width, GetRect("golemJ_fistSpawn").height));
    sfRectangleShape_setSize(_boss->hand2SPR, vector2f(GetRect("golemJ_fistSpawn").width, GetRect("golemJ_fistSpawn").height));
    sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discSpawn").width, GetRect("golemJ_discSpawn").height));
    sfRectangleShape_setSize(_boss->haloSpr, vector2f(GetRect("golemJ_haloAttract").width, GetRect("golemJ_haloAttract").height));
    BossJoieState = BJ_PHASE_SPAWN;
    bossJoieFight = BJ_ATK_NONE;
}

void bossJoie_initFx() {
    bossJoieFX = STD_LIST_CREATE(BossJoieFX, 0);
    bossJoieFXsprite = sfSprite_create();
    sfSprite_setTexture(bossJoieFXsprite, GetSfTexture("golemJ_spawn"), sfTrue);

    bossJoieShader = NULL;

    if (sfShader_isAvailable()) {
        bossJoieShader = GetShader("spriteRecolorAlpha");
        bossJoieRenderState.shader = bossJoieShader;
        bossJoieRenderState.blendMode = sfBlendAlpha;
        bossJoieRenderState.transform = sfTransform_Identity;
        bossJoieRenderState.texture = NULL;
    }
}

void bossJoie_placeFX(BossJoieFXEnum _type, sfVector2f _pos) {
    BossJoieFX listTmp;

    switch (_type)
    {
    case BJ_FX_HAND:
        listTmp.pos = _pos;
        listTmp.type = _type;
        listTmp.frameX = 0;
        listTmp.animTime = 0.f;
        listTmp.texture = "golemJ_fxHand";
        break;
    case BJ_FX_JUMP:
        listTmp.pos = _pos;
        listTmp.type = _type;
        listTmp.frameX = 0;
        listTmp.animTime = 0.f;
        listTmp.texture = "golemJ_fxJump";
        break;
    case BJ_FX_STUN:
        listTmp.pos = _pos;
        listTmp.type = _type;
        listTmp.frameX = 0;
        listTmp.animTime = 0.f;
        listTmp.texture = "golemJ_fxStun";
        break;
    case BJ_FX_SLASH:
        listTmp.pos = _pos;
        listTmp.type = _type;
        listTmp.frameX = 0;
        listTmp.animTime = 0.f;
        listTmp.texture = "golemJ_fxSlash";
        break;
    default:
        break;
    }

    bossJoieFX->push_back(&bossJoieFX, &listTmp);
}

void bossJoie_displayFX(Window* _w, BossJoie* _boss) {
    for (int i = 0; i < bossJoieFX->size(bossJoieFX); i++) {
        
        if (GETDATA_FX_BOSS_JOIE->type == BJ_FX_HAND)  sfSprite_setOrigin(bossJoieFXsprite, vector2f(222.f * 0.5f, 238.f * 0.5f));
        if (GETDATA_FX_BOSS_JOIE->type == BJ_FX_JUMP)  sfSprite_setOrigin(bossJoieFXsprite, vector2f(512.f * 0.5f, 406.f * 0.5f));
        if (GETDATA_FX_BOSS_JOIE->type == BJ_FX_STUN)  sfSprite_setOrigin(bossJoieFXsprite, vector2f(188.f * 0.5f, 118.f * 0.5f));

        if (GETDATA_FX_BOSS_JOIE->type == BJ_FX_SLASH) {
            sfSprite_setOrigin(bossJoieFXsprite, vector2f(148.f * 0.5f, 172.f * 0.5f));
            sfSprite_setRotation(bossJoieFXsprite, _boss->angleAtk2);
            _boss->hasFirstHand ? sfSprite_setPosition(bossJoieFXsprite, polarCoord(_boss->hand1pos, 150.f, ((_boss->angleAtk2 + 180.f) * DEG2RAD))) : sfSprite_setPosition(bossJoieFXsprite, polarCoord(_boss->hand2pos, 150.f, ((_boss->angleAtk2 + 180.f) * DEG2RAD)));
        }
        else {
            sfSprite_setRotation(bossJoieFXsprite, 0.f);
            sfSprite_setPosition(bossJoieFXsprite, GETDATA_FX_BOSS_JOIE->pos);
        }

        sfSprite_setTextureRect(bossJoieFXsprite, animatorAnim(GETDATA_FX_BOSS_JOIE->texture, &GETDATA_FX_BOSS_JOIE->frameX, &GETDATA_FX_BOSS_JOIE->animTime, 0));
        sfRenderTexture_drawSprite(_w->rdrt, bossJoieFXsprite, NULL);

        if (GETDATA_FX_BOSS_JOIE->type == BJ_FX_HAND && GETDATA_FX_BOSS_JOIE->frameX >= 4) {
            bossJoieFX->erase(&bossJoieFX, i);
            break;
        }
        if (GETDATA_FX_BOSS_JOIE->type == BJ_FX_JUMP && GETDATA_FX_BOSS_JOIE->frameX >= 3 && GETDATA_FX_BOSS_JOIE->animTime >= 0.15f) {
            bossJoieFX->erase(&bossJoieFX, i);
            break;
        }
        if (GETDATA_FX_BOSS_JOIE->type == BJ_FX_SLASH && GETDATA_FX_BOSS_JOIE->frameX >= 3 && GETDATA_FX_BOSS_JOIE->animTime >= 0.15f) {
            bossJoieFX->erase(&bossJoieFX, i);
            break;
        }
    }

}

void bossJoie_Atk1(BossJoie* _boss) {
    float dt = getDeltaTime();

    _boss->timerAtk1 += getDeltaTime();
    _boss->timerAtk2 = 0.f;
    _boss->angleAtk2 = 0.f;

    static sfBool isHandClap = sfFalse;

    if (_boss->timerAtk1 < 2.f) {
        if (_boss->timerAtk1 < 0.3f && sfFloatRect_intersects(&_boss->hand1Bounds, &_boss->getPlayerBounds, NULL)) {
            // LE JOUEUR PREND DES DEGATS ICI HUGO
            plr_addDamagePlayerLife(2);
        }
        if (_boss->timerAtk1 < 0.3f && sfFloatRect_intersects(&_boss->hand2Bounds, &_boss->getPlayerBounds, NULL)) {
            // LE JOUEUR PREND DES DEGATS ICI HUGO
            plr_addDamagePlayerLife(2);
        }

        _boss->fxHandPos = polarCoord(_boss->position, 250.f, ((_boss->angle - 90.f) * DEG2RAD));

        if (!isHandClap) {
            bossJoie_placeFX(BJ_FX_HAND, _boss->fxHandPos);
            playSoundFX("SFX_Explosion_Boss");
            isHandClap = sfTrue;
        }

        _boss->hand1pos = vec2f_lerp(_boss->hand1pos, polarCoord(_boss->position, 250.f, ((_boss->angle + 8.f - 90.f) * DEG2RAD)), sinf(10.f * dt));
        _boss->hand2pos = vec2f_lerp(_boss->hand2pos, polarCoord(_boss->position, 250.f, ((_boss->angle - 8.f - 90.f) * DEG2RAD)), sinf(10.f * dt));
        _boss->isHand1Damagable = sfTrue;
        _boss->isHand2Damagable = sfTrue;
    }
    else if (_boss->timerAtk1 > 2.f) {
        if (_boss->timerAtk1 > 2.5f)
            _boss->timerAtk1 = 0.f;

        isHandClap = sfFalse;

        if (sfFloatRect_intersects(&_boss->hand1Bounds, &_boss->getPlayerBounds, NULL)) {
            // LE JOUEUR PREND DES DEGATS ICI HUGO
            plr_addDamagePlayerLife(2);
        }
        if (sfFloatRect_intersects(&_boss->hand2Bounds, &_boss->getPlayerBounds, NULL)) {
            // LE JOUEUR PREND DES DEGATS ICI HUGO
            plr_addDamagePlayerLife(2);
        }

        _boss->hand1pos = vec2f_lerp(_boss->hand1pos, polarCoord(_boss->position, 250.f, ((_boss->angle + 40.f - 90.f) * DEG2RAD)), sinf(10.f * dt));
        _boss->hand2pos = vec2f_lerp(_boss->hand2pos, polarCoord(_boss->position, 250.f, ((_boss->angle - 40.f - 90.f) * DEG2RAD)), sinf(10.f * dt));
        _boss->isHand1Damagable = sfFalse;
        _boss->isHand2Damagable = sfFalse;
    }
}

void bossJoie_Atk2(BossJoie* _boss) {
    float dt = getDeltaTime();
    static sfBool isHandSlash1 = sfFalse;
    static sfBool isHandSlash2 = sfFalse;

    _boss->timerAtk1 = 2.f;
    _boss->timerAtk2 += getDeltaTime();

    _boss->isHand1Damagable = sfTrue;
    _boss->isHand2Damagable = sfTrue;

    if (_boss->timerAtk2 < 1.f) {
        isHandSlash2 = sfFalse;
        _boss->angleAtk2 += 200.f * getDeltaTime();

        if (_boss->hasFirstHand && !_boss->hasSecondHand) {
            _boss->hand1pos = vec2f_lerp(_boss->hand1pos, polarCoord(_boss->position, 620.f, ((_boss->angle - _boss->angleAtk2) * DEG2RAD)), sinf(4.f * dt));
            _boss->hand2pos = vec2f_lerp(_boss->hand2pos, polarCoord(_boss->position, 250.f, ((_boss->angle - 40.f - 90.f) * DEG2RAD)), sinf(4.f * dt));
            if (sfFloatRect_intersects(&_boss->hand1Bounds, &_boss->getPlayerBounds, NULL)) {
                // LE JOUEUR PREND DES DEGATS ICI HUGO
                plr_addDamagePlayerLife(2);
            }
            if (!isHandSlash1) {
                bossJoie_placeFX(BJ_FX_SLASH, _boss->hand1pos);
                playSoundFX("SFX_Slash_Boss");
                isHandSlash1 = sfTrue;
            }
        }
        else if (!_boss->hasFirstHand && _boss->hasSecondHand) {
            _boss->hand1pos = vec2f_lerp(_boss->hand1pos, polarCoord(_boss->position, 250.f, ((_boss->angle + 40.f - 90.f) * DEG2RAD)), sinf(4.f * dt));
            _boss->hand2pos = vec2f_lerp(_boss->hand2pos, polarCoord(_boss->position, 620.f, ((_boss->angle - _boss->angleAtk2) * DEG2RAD)), sinf(4.f * dt));
            if (sfFloatRect_intersects(&_boss->hand2Bounds, &_boss->getPlayerBounds, NULL)) {
                // LE JOUEUR PREND DES DEGATS ICI HUGO
                plr_addDamagePlayerLife(2);
            }
            if (!isHandSlash1) {
                bossJoie_placeFX(BJ_FX_SLASH, _boss->hand2pos);
                playSoundFX("SFX_Slash_Boss");
                isHandSlash1 = sfTrue;
            }
        }
        else {
            _boss->hand1pos = vec2f_lerp(_boss->hand1pos, polarCoord(_boss->position, 620.f, ((_boss->angle - _boss->angleAtk2) * DEG2RAD)), sinf(4.f * dt));
            _boss->hand2pos = vec2f_lerp(_boss->hand2pos, polarCoord(_boss->position, 250.f, ((_boss->angle - 40.f - 90.f) * DEG2RAD)), sinf(4.f * dt));
            if (sfFloatRect_intersects(&_boss->hand1Bounds, &_boss->getPlayerBounds, NULL)) {
                // LE JOUEUR PREND DES DEGATS ICI HUGO
                plr_addDamagePlayerLife(2);
            }
            if (!isHandSlash1) {
                bossJoie_placeFX(BJ_FX_SLASH, _boss->hand1pos);
                playSoundFX("SFX_Slash_Boss");
                isHandSlash1 = sfTrue;
            }
        }


    }
    else if (_boss->timerAtk2 > 1.f) {
        if (_boss->timerAtk2 > 2.f)
            _boss->timerAtk2 = 0.f;
        _boss->angleAtk2 -= 200.f * getDeltaTime();
        isHandSlash1 = sfFalse;

        if (_boss->hasFirstHand && !_boss->hasSecondHand) {
            _boss->hand1pos = vec2f_lerp(_boss->hand1pos, polarCoord(_boss->position, 620.f, ((_boss->angle - _boss->angleAtk2) * DEG2RAD)), sinf(4.f * dt));
            _boss->hand2pos = vec2f_lerp(_boss->hand2pos, polarCoord(_boss->position, 250.f, ((_boss->angle - 40.f - 90.f) * DEG2RAD)), sinf(4.f * dt));
            if (sfFloatRect_intersects(&_boss->hand1Bounds, &_boss->getPlayerBounds, NULL)) {
                // LE JOUEUR PREND DES DEGATS ICI HUGO
                plr_addDamagePlayerLife(2);
            }
            if (!isHandSlash2) {
                bossJoie_placeFX(BJ_FX_SLASH, _boss->hand1pos);
                playSoundFX("SFX_Slash_Boss");
                isHandSlash2 = sfTrue;
            }
        }
        else if (!_boss->hasFirstHand && _boss->hasSecondHand) {
            _boss->hand1pos = vec2f_lerp(_boss->hand1pos, polarCoord(_boss->position, 250.f, ((_boss->angle + 40.f - 90.f) * DEG2RAD)), sinf(4.f * dt));
            _boss->hand2pos = vec2f_lerp(_boss->hand2pos, polarCoord(_boss->position, 620.f, ((_boss->angle - _boss->angleAtk2) * DEG2RAD)), sinf(4.f * dt));
            if (sfFloatRect_intersects(&_boss->hand2Bounds, &_boss->getPlayerBounds, NULL)) {
                // LE JOUEUR PREND DES DEGATS ICI HUGO
                plr_addDamagePlayerLife(2);
            }
            if (!isHandSlash2) {
                bossJoie_placeFX(BJ_FX_SLASH, _boss->hand2pos);
                playSoundFX("SFX_Slash_Boss");
                isHandSlash2 = sfTrue;
            }
        }
        else {
            _boss->hand1pos = vec2f_lerp(_boss->hand1pos, polarCoord(_boss->position, 620.f, ((_boss->angle - _boss->angleAtk2) * DEG2RAD)), sinf(4.f * dt));
            _boss->hand2pos = vec2f_lerp(_boss->hand2pos, polarCoord(_boss->position, 250.f, ((_boss->angle - 40.f - 90.f) * DEG2RAD)), sinf(4.f * dt));
            if (sfFloatRect_intersects(&_boss->hand1Bounds, &_boss->getPlayerBounds, NULL)) {
                // LE JOUEUR PREND DES DEGATS ICI HUGO
                plr_addDamagePlayerLife(2);
            }
            if (!isHandSlash2) {
                bossJoie_placeFX(BJ_FX_SLASH, _boss->hand1pos);
                playSoundFX("SFX_Slash_Boss");
                isHandSlash2 = sfTrue;
            }
        }
    }
}

void bossJoie_Atk3(BossJoie* _boss) {
    float dt = getDeltaTime();

    _boss->timerAtk3 += getDeltaTime();

    static sfBool isNewPos = sfFalse;
    static sfBool isJumpFx = sfFalse;

    static sfBool hasTmpPlayerPos = sfFalse;
    //_boss->isLoadingAttract ? playSoundFX("SFX_Spark_Boss") : sfSound_stop(GetSound("SFX_Spark_Boss"));


    if (_boss->timerAtk3 < 2.f) {
        _boss->isAttractingPlayer = sfFalse;
        _boss->isLoadingAttract = sfTrue;
        if (!_boss->sfxLoadingAttract) playSoundFX("SFX_Spark_Boss");
        _boss->sfxLoadingAttract = sfTrue;
    }
    if (_boss->timerAtk3 > 2.f && _boss->timerAtk3 < 3.f) {
        _boss->isLoadingAttract = sfFalse;
        _boss->isAttractingPlayer = sfTrue;
        _boss->sfxLoadingAttract = sfFalse;
        plr_setPlayerIsPushed(sfTrue);
        sfSound_stop(GetSound("SFX_Spark_Boss"));

        if (!_boss->sfxAttract) playSoundFX("SFX_Swoosh_Boss");
        _boss->sfxAttract = sfTrue;

        if (getSqrMagnitudeBetween2Vectorsf(_boss->position, plr_GetPos()) < 100.f)
            plr_setPlayerVelocity(NULLVECTF);
        else
            plr_setPlayerVelocity(multiplyVectorsrf(normalizef(createVectorWithVectorf(plr_GetPos(), _boss->discPos)), 500.f));
    }
    else if (_boss->timerAtk3 > 3.f && _boss->timerAtk3 < 5.f) {
        plr_setPlayerIsPushed(sfFalse);
        bossJoie_attachRing(_boss);
        _boss->isAttractingPlayer = sfFalse;
        _boss->hasHalo = sfTrue;
        _boss->frameXattractD = 0;
        _boss->frameXattractF = 0;
        _boss->timerAnimAttractD = 0.f;
        _boss->timerAnimAttractF = 0.f;
        _boss->sfxAttract = sfFalse;
        sfSound_stop(GetSound("SFX_Swoosh_Boss"));
        if (!_boss->sfxHalo) playSoundFX("SFX_Halo_Boss");
        _boss->sfxHalo = sfTrue;
    }
    else if (_boss->timerAtk3 > 5.f) {
        _boss->isAttractingPlayer = sfFalse;
        _boss->isJumping = sfTrue;
        _boss->hasHalo = sfFalse;
        sfSound_stop(GetSound("SFX_Halo_Boss"));
        _boss->sfxHalo = sfFalse;

        if (_boss->timerAtk3 > 6.f) {
            if (!isJumpFx) {
                playSoundFX("SFX_Explosion_Boss");
                bossJoie_placeFX(BJ_FX_JUMP, vector2f(_boss->position.x - 20.f, _boss->position.y + 50.f));
                shakingView(0.5f);
                SetVibration(0, VIBRATION_IMPACT);
                isJumpFx = sfTrue;
            }
            if (sfFloatRect_intersects(&_boss->bossBounds, &_boss->getPlayerBounds, NULL)) {
                // LE JOUEUR PREND DES DEGATS ICI HUGO
                plr_addDamagePlayerLife(3);
            }
        }
        
        if (!hasTmpPlayerPos) {
            _boss->tempPlayerPos = plr_GetPos();
            hasTmpPlayerPos = sfTrue;
        }

        isNewPos = sfTrue;
        if (_boss->timerAtk3 > 7.f) {
            SetVibration(0, VIBRATION_NONE);
            isJumpFx = sfFalse;
            _boss->isJumping = sfFalse;
            hasTmpPlayerPos = sfFalse;
            _boss->frameXjump = 0;
            _boss->timerAnimJump = 0.f;
            _boss->timerAtk3 = 0.f;
        }
    }

    if (isNewPos) _boss->position = vec2f_lerp_invCube(_boss->position, vector2f(_boss->tempPlayerPos.x, _boss->tempPlayerPos.y - 96.f), sinf(dt));
}

void bossJoie_resetHandPos(BossJoie* _boss) {
    float dt = getDeltaTime();

    _boss->hand1pos = vec2f_lerp(_boss->hand1pos, polarCoord(_boss->position, 250.f, ((_boss->angle + 40.f - 90.f) * DEG2RAD)), sinf(8.f * dt));
    _boss->hand2pos = vec2f_lerp(_boss->hand2pos, polarCoord(_boss->position, 250.f, ((_boss->angle - 40.f - 90.f) * DEG2RAD)), sinf(8.f * dt));
}

void bossJoie_movments(BossJoie* _boss) {

    float distPlayerBoss = getMagnitudef(createVectorWithVectorf(_boss->position, plr_GetFeetPos()));

    switch (bossJoieFight)
    {
    /*case BJ_ATK_NONE:
        _boss->forward = normalizef(createVectorWithVectorf(_boss->position, vector2f(980.f, 1080.f)));

        _boss->oldForward = vec2f_lerp(_boss->oldForward, _boss->forward, 2.f * getDeltaTime());

        _boss->angle = atan2f(_boss->oldForward.y, _boss->oldForward.x) * RAD2DEG + 90.f;

        _boss->position = vec2f_lerp(_boss->position, vector2f(980.f, 200.f), 2.f * getDeltaTime());
        break;*/
    case BJ_ATK_1:
        _boss->forward = normalizef(createVectorWithVectorf(_boss->position, plr_GetPos()));

        _boss->oldForward = vec2f_lerp(_boss->oldForward, _boss->forward, 0.8f * getDeltaTime());

        _boss->angle = atan2f(_boss->oldForward.y, _boss->oldForward.x) * RAD2DEG + 90.f;

        if (distPlayerBoss >= 250.f) {
            _boss->position = vec2f_lerp(_boss->position, plr_GetPos(), 0.3f * getDeltaTime());
        }
        break;
    case BJ_ATK_2:
        _boss->forward = normalizef(createVectorWithVectorf(_boss->position, plr_GetPos()));

        _boss->oldForward = vec2f_lerp(_boss->oldForward, _boss->forward, 0.4f * getDeltaTime());

        _boss->angle = atan2f(_boss->oldForward.y, _boss->oldForward.x) * RAD2DEG + 90.f;

        _boss->position = vec2f_lerp(_boss->position, plr_GetPos(), 0.1f * getDeltaTime());
        break;
    case BJ_ATK_3:
        _boss->forward = normalizef(createVectorWithVectorf(_boss->position, plr_GetPos()));

        _boss->oldForward = vec2f_lerp(_boss->oldForward, _boss->forward, 0.8f * getDeltaTime());

        _boss->angle = atan2f(_boss->oldForward.y, _boss->oldForward.x) * RAD2DEG + 90.f;
        break;
    default:
        break;
    }
}

void bossJoie_Animation(BossJoieAnimate _animationType, BossJoie* _boss) {

    float distPlayerBoss = getMagnitudef(createVectorWithVectorf(_boss->position, plr_GetFeetPos()));

    switch (_animationType)
    {
    case ABJ_SPAWN: // 980 200

        if (distPlayerBoss <= 300.f) {
            _boss->isPlayerCloseSpawn = sfTrue;
        }

        if (!_boss->isPlayerCloseSpawn) {
            _boss->frameXboss  = 0;
            _boss->frameXdisc  = -1;
            _boss->frameXhand1 = -1;
            _boss->frameXhand2 = -1;
            _boss->timerAnimBoss  = 0.f;
            _boss->timerAnimDisc  = 0.f;
            _boss->timerAnimHand1 = 0.f;
            _boss->timerAnimHand2 = 0.f;

            sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_spawn",      &_boss->frameXboss,  &_boss->timerAnimBoss, 0));
            sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discSpawn",  &_boss->frameXdisc,  &_boss->timerAnimDisc, 0));
            sfRectangleShape_setTextureRect(_boss->hand1SPR, animatorAnim("golemJ_fistSpawn", &_boss->frameXhand1, &_boss->timerAnimHand1, 0));
            sfRectangleShape_setTextureRect(_boss->hand2SPR, animatorAnim("golemJ_fistSpawn", &_boss->frameXhand2, &_boss->timerAnimHand2, 0));
        }
        else {
            if (_boss->frameXboss < 8 && !_boss->animSpawnBoss) {
                _boss->origin = vector2f(GetRect("golemJ_spawn").width * 0.5f, GetRect("golemJ_spawn").height * 0.5f);
                sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_spawn").width, GetRect("golemJ_spawn").height));
                sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_spawn", &_boss->frameXboss, &_boss->timerAnimBoss, 0));
            }
            else {
                _boss->animSpawnBoss = sfTrue;
                _boss->origin = vector2f(GetRect("golemJ_idleF").width * 0.5f, GetRect("golemJ_idleF").height * 0.5f);
                _boss->frameXboss = 0;
                sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_idleF").width, GetRect("golemJ_idleF").height));
                sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_idleF", &_boss->frameXboss, &_boss->timerAnimBoss, 0));
                BossJoieState = BJ_PHASE_1;
            }

            if (_boss->frameXdisc < 3 && !_boss->animSpawnDisc) {
                _boss->originDisc = vector2f(GetRect("golemJ_discSpawn").width * 0.5f, GetRect("golemJ_discSpawn").height * 0.5f);
                sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discSpawn").width, GetRect("golemJ_discSpawn").height));
                sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discSpawn", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
                _boss->discPos = addVectorsrf(_boss->position, vector2f(0.f, 155.f));
            }
            else {
                _boss->animSpawnDisc = sfTrue;
                _boss->discPos = addVectorsrf(_boss->position, vector2f(0.f, 142.f));
                _boss->originDisc = vector2f(GetRect("golemJ_discF").width * 0.5f, GetRect("golemJ_discF").height * 0.5f);
                sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discF").width, GetRect("golemJ_discF").height));
                _boss->frameXdisc = 0;
                sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discF", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
            }
        

        
            if (_boss->frameXhand1 < 5 && !_boss->animSpawnHand) {
                sfRectangleShape_setSize(_boss->hand1SPR, vector2f(GetRect("golemJ_fistSpawn").width, GetRect("golemJ_fistSpawn").height));
                _boss->originHand = vector2f(GetRect("golemJ_fistSpawn").width * 0.5f, GetRect("golemJ_fistSpawn").height * 0.5f);
                sfRectangleShape_setTextureRect(_boss->hand1SPR, animatorAnim("golemJ_fistSpawn", &_boss->frameXhand1, &_boss->timerAnimHand1, 0));
                _boss->hand1pos = addVectorsrf(_boss->position, vector2f(-190.f, 250.f));
            }
            else {
                _boss->animSpawnHand = sfTrue;
                _boss->originHand = vector2f(GetRect("golemJ_fistF").width * 0.5f, GetRect("golemJ_fistF").height * 0.5f);
                _boss->frameXhand1 = 0;
                sfRectangleShape_setSize(_boss->hand1SPR, vector2f(GetRect("golemJ_fistF").width, GetRect("golemJ_fistF").height));
                sfRectangleShape_setTextureRect(_boss->hand1SPR, animatorAnim("golemJ_fistF", &_boss->frameXhand1, &_boss->timerAnimHand1, 0));
                _boss->hand1pos = addVectorsrf(_boss->position, vector2f(-190.f, 250.f));
            }

            if (_boss->frameXhand2 < 5 && !_boss->animSpawnHand) {
                _boss->originHand = vector2f(GetRect("golemJ_fistSpawn").width * 0.5f, GetRect("golemJ_fistSpawn").height * 0.5f);
                sfRectangleShape_setSize(_boss->hand2SPR, vector2f(GetRect("golemJ_fistSpawn").width, GetRect("golemJ_fistSpawn").height));
                sfRectangleShape_setTextureRect(_boss->hand2SPR, animatorAnim("golemJ_fistSpawn", &_boss->frameXhand2, &_boss->timerAnimHand2, 0));
                _boss->hand2pos = addVectorsrf(_boss->position, vector2f(190.f, 250.f));
            }
            else {
                _boss->animSpawnHand = sfTrue;
                _boss->originHand = vector2f(GetRect("golemJ_fistF").width * 0.5f, GetRect("golemJ_fistF").height * 0.5f);
                _boss->frameXhand2 = 0;
                sfRectangleShape_setSize(_boss->hand2SPR, vector2f(GetRect("golemJ_fistF").width, GetRect("golemJ_fistF").height));
                sfRectangleShape_setTextureRect(_boss->hand2SPR, animatorAnim("golemJ_fistF", &_boss->frameXhand2, &_boss->timerAnimHand2, 0));
                _boss->hand2pos = addVectorsrf(_boss->position, vector2f(190.f, 250.f));
            }
        }

        break;
    case ABJ_IDLE_F:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_idleF").width, GetRect("golemJ_idleF").height));
        _boss->origin = vector2f(GetRect("golemJ_idleF").width * 0.5f, GetRect("golemJ_idleF").height * 0.5f);
        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_idleF", &_boss->frameXboss, &_boss->timerAnimBoss, 0));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discF").width, GetRect("golemJ_discF").height));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discF", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(1.f, 1.f));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(0.f, 142.f));

        sfRectangleShape_setSize(_boss->hand1SPR, vector2f(GetRect("golemJ_fistF").width, GetRect("golemJ_fistF").height));
        sfRectangleShape_setTextureRect(_boss->hand1SPR, animatorAnim("golemJ_fistF", &_boss->frameXhand1, &_boss->timerAnimHand1, 0));
        sfRectangleShape_setSize(_boss->hand2SPR, vector2f(GetRect("golemJ_fistF").width, GetRect("golemJ_fistF").height));
        sfRectangleShape_setTextureRect(_boss->hand2SPR, animatorAnim("golemJ_fistF", &_boss->frameXhand2, &_boss->timerAnimHand2, 0));
        break;
    case ABJ_IDLE_TFL:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_idleTF").width, GetRect("golemJ_idleTF").height));
        _boss->origin = vector2f(GetRect("golemJ_idleTF").width * 0.5f, GetRect("golemJ_idleTF").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_idleTF", &_boss->frameXboss, &_boss->timerAnimBoss, 0));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discT").width, GetRect("golemJ_discT").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discT", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(0.f, 112.f));

        sfRectangleShape_setSize(_boss->hand1SPR, vector2f(GetRect("golemJ_fistF").width, GetRect("golemJ_fistF").height));
        sfRectangleShape_setTextureRect(_boss->hand1SPR, animatorAnim("golemJ_fistF", &_boss->frameXhand1, &_boss->timerAnimHand1, 0));
        sfRectangleShape_setSize(_boss->hand2SPR, vector2f(GetRect("golemJ_fistF").width, GetRect("golemJ_fistF").height));
        sfRectangleShape_setTextureRect(_boss->hand2SPR, animatorAnim("golemJ_fistF", &_boss->frameXhand2, &_boss->timerAnimHand2, 0));
        break;
    case ABJ_IDLE_TFR:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_idleTF").width, GetRect("golemJ_idleTF").height));
        _boss->origin = vector2f(GetRect("golemJ_idleTF").width * 0.5f, GetRect("golemJ_idleTF").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(-1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_idleTF", &_boss->frameXboss, &_boss->timerAnimBoss, 0));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discT").width, GetRect("golemJ_discT").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(-1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discT", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(0.f, 112.f));

        sfRectangleShape_setSize(_boss->hand1SPR, vector2f(GetRect("golemJ_fistF").width, GetRect("golemJ_fistF").height));
        sfRectangleShape_setTextureRect(_boss->hand1SPR, animatorAnim("golemJ_fistF", &_boss->frameXhand1, &_boss->timerAnimHand1, 0));
        sfRectangleShape_setSize(_boss->hand2SPR, vector2f(GetRect("golemJ_fistF").width, GetRect("golemJ_fistF").height));
        sfRectangleShape_setTextureRect(_boss->hand2SPR, animatorAnim("golemJ_fistF", &_boss->frameXhand2, &_boss->timerAnimHand2, 0));
        break;
    case ABJ_IDLE_TDL:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_idleTD").width, GetRect("golemJ_idleTD").height));
        _boss->origin = vector2f(GetRect("golemJ_idleTD").width * 0.5f, GetRect("golemJ_idleTD").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(-1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_idleTD", &_boss->frameXboss, &_boss->timerAnimBoss, 0));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discT").width, GetRect("golemJ_discT").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(-1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discT", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(0.f, 112.f));

        sfRectangleShape_setSize(_boss->hand1SPR, vector2f(GetRect("golemJ_fistF").width, GetRect("golemJ_fistF").height));
        sfRectangleShape_setTextureRect(_boss->hand1SPR, animatorAnim("golemJ_fistF", &_boss->frameXhand1, &_boss->timerAnimHand1, 0));
        sfRectangleShape_setSize(_boss->hand2SPR, vector2f(GetRect("golemJ_fistF").width, GetRect("golemJ_fistF").height));
        sfRectangleShape_setTextureRect(_boss->hand2SPR, animatorAnim("golemJ_fistF", &_boss->frameXhand2, &_boss->timerAnimHand2, 0));
        break;
    case ABJ_IDLE_TDR:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_idleTD").width, GetRect("golemJ_idleTD").height));
        _boss->origin = vector2f(GetRect("golemJ_idleTD").width * 0.5f, GetRect("golemJ_idleTD").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_idleTD", &_boss->frameXboss, &_boss->timerAnimBoss, 0));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discT").width, GetRect("golemJ_discT").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discT", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(0.f, 112.f));

        sfRectangleShape_setSize(_boss->hand1SPR, vector2f(GetRect("golemJ_fistF").width, GetRect("golemJ_fistF").height));
        sfRectangleShape_setTextureRect(_boss->hand1SPR, animatorAnim("golemJ_fistF", &_boss->frameXhand1, &_boss->timerAnimHand1, 0));
        sfRectangleShape_setSize(_boss->hand2SPR, vector2f(GetRect("golemJ_fistF").width, GetRect("golemJ_fistF").height));
        sfRectangleShape_setTextureRect(_boss->hand2SPR, animatorAnim("golemJ_fistF", &_boss->frameXhand2, &_boss->timerAnimHand2, 0));
        break;
    case ABJ_ATTRACT_LOAD_F:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_attractLoadF").width, GetRect("golemJ_attractLoadF").height));
        _boss->origin = vector2f(GetRect("golemJ_attractLoadF").width * 0.5f, GetRect("golemJ_attractLoadF").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(1.f, 1.f));

        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_attractLoadF", &_boss->frameXboss, &_boss->timerAnimBoss, 0));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discF").width, GetRect("golemJ_discF").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discF", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(3.f, 170.f));
        break;
    case ABJ_ATTRACT_LOAD_TFL:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_attractLoadTF").width, GetRect("golemJ_attractLoadTF").height));
        _boss->origin = vector2f(GetRect("golemJ_attractLoadTF").width * 0.5f, GetRect("golemJ_attractLoadTF").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(1.f, 1.f));

        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_attractLoadTF", &_boss->frameXboss, &_boss->timerAnimBoss, 0));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discT").width, GetRect("golemJ_discT").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discT", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(-25.f, 130.f));
        break;
    case ABJ_ATTRACT_LOAD_TFR:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_attractLoadTF").width, GetRect("golemJ_attractLoadTF").height));
        _boss->origin = vector2f(GetRect("golemJ_attractLoadTF").width * 0.5f, GetRect("golemJ_attractLoadTF").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(-1.f, 1.f));

        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_attractLoadTF", &_boss->frameXboss, &_boss->timerAnimBoss, 0));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discT").width, GetRect("golemJ_discT").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(-1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discT", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(25.f, 130.f));
        break;
    case ABJ_ATTRACT_LOAD_TDL:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_attractLoadTD").width, GetRect("golemJ_attractLoadTD").height));
        _boss->origin = vector2f(GetRect("golemJ_attractLoadTD").width * 0.5f, GetRect("golemJ_attractLoadTD").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(-1.f, 1.f));

        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_attractLoadTD", &_boss->frameXboss, &_boss->timerAnimBoss, 0));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discT").width, GetRect("golemJ_discT").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(-1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discT", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(18.f, 115.f));
        break;
    case ABJ_ATTRACT_LOAD_TDR:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_attractLoadTD").width, GetRect("golemJ_attractLoadTD").height));
        _boss->origin = vector2f(GetRect("golemJ_attractLoadTD").width * 0.5f, GetRect("golemJ_attractLoadTD").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(1.f, 1.f));

        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_attractLoadTD", &_boss->frameXboss, &_boss->timerAnimBoss, 0));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discT").width, GetRect("golemJ_discT").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discT", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(-18.f, 115.f));
        break;
    case ABJ_ATTRACT_F:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_attractF").width, GetRect("golemJ_attractF").height));
        _boss->origin = vector2f(GetRect("golemJ_attractF").width * 0.5f, GetRect("golemJ_attractF").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorWithBeginFrame("golemJ_attractF", &_boss->frameXattractF, &_boss->timerAnimAttractF, 0, 2));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discF").width, GetRect("golemJ_discF").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discF", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(2.f, 170.f));
        break;
    case ABJ_ATTRACT_TFL:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_attractTF").width, GetRect("golemJ_attractTF").height));
        _boss->origin = vector2f(GetRect("golemJ_attractTF").width * 0.5f, GetRect("golemJ_attractTF").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorWithBeginFrame("golemJ_attractTF", &_boss->frameXattractF, &_boss->timerAnimAttractF, 0, 2));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discT").width, GetRect("golemJ_discT").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discT", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(-25.f, 130.f));
        break;
    case ABJ_ATTRACT_TFR:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_attractTF").width, GetRect("golemJ_attractTF").height));
        _boss->origin = vector2f(GetRect("golemJ_attractTF").width * 0.5f, GetRect("golemJ_attractTF").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(-1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorWithBeginFrame("golemJ_attractTF", &_boss->frameXattractF, &_boss->timerAnimAttractF, 0, 2));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discT").width, GetRect("golemJ_discT").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(-1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discT", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(25.f, 130.f));
        break;
    case ABJ_ATTRACT_TDL:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_attractTD").width, GetRect("golemJ_attractTD").height));
        _boss->origin = vector2f(GetRect("golemJ_attractTD").width * 0.5f, GetRect("golemJ_attractTD").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(-1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorWithBeginFrame("golemJ_attractTD", &_boss->frameXattractD, &_boss->timerAnimAttractD, 0, 3));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discT").width, GetRect("golemJ_discT").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(-1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discT", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(18.f, 115.f));
        break;
    case ABJ_ATTRACT_TDR:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_attractTD").width, GetRect("golemJ_attractTD").height));
        _boss->origin = vector2f(GetRect("golemJ_attractTD").width * 0.5f, GetRect("golemJ_attractTD").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorWithBeginFrame("golemJ_attractTD", &_boss->frameXattractD, &_boss->timerAnimAttractD, 0, 3));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discT").width, GetRect("golemJ_discT").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discT", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(-18.f, 115.f));
        break;
    case ABJ_JUMP_TFL:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_jumpTF").width, GetRect("golemJ_jumpTF").height));
        _boss->origin = vector2f(GetRect("golemJ_jumpTF").width * 0.5f, GetRect("golemJ_jumpTF").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_jumpTF", &_boss->frameXjump, &_boss->timerAnimJump, 0));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discT").width, GetRect("golemJ_discT").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discT", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(0.f, 130.f));
        break;
    case ABJ_JUMP_TFR:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_jumpTF").width, GetRect("golemJ_jumpTF").height));
        _boss->origin = vector2f(GetRect("golemJ_jumpTF").width * 0.5f, GetRect("golemJ_jumpTF").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(-1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_jumpTF", &_boss->frameXjump, &_boss->timerAnimJump, 0));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discT").width, GetRect("golemJ_discT").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(-1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discT", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(0.f, 130.f));
        break;
    case ABJ_JUMP_TDL:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_jumpTD").width, GetRect("golemJ_jumpTD").height));
        _boss->origin = vector2f(GetRect("golemJ_jumpTD").width * 0.5f, GetRect("golemJ_jumpTD").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(-1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_jumpTD", &_boss->frameXjump, &_boss->timerAnimJump, 0));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discT").width, GetRect("golemJ_discT").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(-1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discT", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(0.f, 150.f));
        break;
    case ABJ_JUMP_TDR:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_jumpTD").width, GetRect("golemJ_jumpTD").height));
        _boss->origin = vector2f(GetRect("golemJ_jumpTD").width * 0.5f, GetRect("golemJ_jumpTD").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_jumpTD", &_boss->frameXjump, &_boss->timerAnimJump, 0));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discT").width, GetRect("golemJ_discT").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discT", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(0.f, 150.f));
        break;
    case ABJ_JUMP_F:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_jumpF").width, GetRect("golemJ_jumpF").height));
        _boss->origin = vector2f(GetRect("golemJ_jumpF").width * 0.5f, GetRect("golemJ_jumpF").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorAnim("golemJ_jumpF", &_boss->frameXjump, &_boss->timerAnimJump, 0));

        sfRectangleShape_setSize(_boss->discSpr, vector2f(GetRect("golemJ_discF").width, GetRect("golemJ_discF").height));
        sfRectangleShape_setScale(_boss->discSpr, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->discSpr, animatorAnim("golemJ_discF", &_boss->frameXdisc, &_boss->timerAnimDisc, 0));
        _boss->discPos = addVectorsrf(_boss->position, vector2f(0.f, 170.f));
        break;
    case ABJ_DEATH:
        sfRectangleShape_setSize(_boss->bossSPR, vector2f(GetRect("golemJ_death").width, GetRect("golemJ_death").height));
        _boss->origin = vector2f(GetRect("golemJ_death").width * 0.5f, GetRect("golemJ_death").height * 0.5f);
        sfRectangleShape_setScale(_boss->bossSPR, vector2f(1.f, 1.f));
        sfRectangleShape_setTextureRect(_boss->bossSPR, animatorWithBeginFrame("golemJ_death", &_boss->frameXdeath, &_boss->timerAnimDeath, 0, 8));
        break;
    default:
        break;
    }
}

void bossJoie_updateDamage(BossJoie* _boss) {
    static float timerDamage = 0.f;

    if (_boss->hasTookDamage) {
        timerDamage += getDeltaTime();

        if (timerDamage > 0.5f) {
            _boss->hasTookDamage = sfFalse;
            _boss->hand1hasTookDamage = sfFalse;
            _boss->hand2hasTookDamage = sfFalse;
            _boss->discHasTookDamage = sfFalse;
            _boss->bossHasTookDamage = sfFalse;
            timerDamage = 0.f;
        }
    }

    if (plr_IsSlashDamaging()) {
        sfFloatRect playerSwordSlashRect = plr_GetDamageBounds();

        switch (BossJoieState)
        {
        case BJ_PHASE_1:
            if (sfFloatRect_intersects(&_boss->hand1Bounds, &playerSwordSlashRect, NULL)) {
                bossJoie_takeDamage(BJ_HAND_1, _boss);
            }
            if (sfFloatRect_intersects(&_boss->hand2Bounds, &playerSwordSlashRect, NULL)) {
                bossJoie_takeDamage(BJ_HAND_2, _boss);
            }
            break;
        case BJ_PHASE_2:
            break;
        default:
            break;
        }
    }

    if (_boss->hand1Life <= 0) {
        _boss->hasFirstHand  = sfFalse;
        if (!_boss->fxHand1) FXe_addFXenemies(_boss->hand1pos, FX_EXPLOSION);
        _boss->fxHand1 = sfTrue;
    }
    if (_boss->hand2Life <= 0) {
        _boss->hasSecondHand = sfFalse;
        if (!_boss->fxHand2) FXe_addFXenemies(_boss->hand2pos, FX_EXPLOSION);
        _boss->fxHand2 = sfTrue;
    }
    if (_boss->discLife <= 0) {
        _boss->hasDisc = sfFalse;
        if (!_boss->fxDisc) FXe_addFXenemies(_boss->discPos, FX_EXPLOSION);
        _boss->fxDisc = sfTrue;
    }
    if (_boss->bossLife <= 0) {
        if (!_boss->sfxDeath) playSoundFX("SFX_Death_Boss");
            _boss->sfxDeath = sfTrue;
        _boss->isDead = sfTrue;
    }
}

void bossJoie_takeDamage(BossJoieParts _part, BossJoie* _boss) {
    if (!_boss->hasTookDamage) {
        switch (_part)
        {
        case BJ_HAND_1:
            _boss->hand1Life--;
            _boss->hasTookDamage = sfTrue;
            _boss->hand1hasTookDamage = sfTrue;
            break;
        case BJ_HAND_2:
            _boss->hand2Life--;
            _boss->hasTookDamage = sfTrue;
            _boss->hand2hasTookDamage = sfTrue;
            break;
        case BJ_DISC:
            _boss->discLife--;
            _boss->hasTookDamage = sfTrue;
            _boss->discHasTookDamage = sfTrue;
            break;
        case BJ_CORE:
            _boss->bossLife--;
            _boss->hasTookDamage = sfTrue;
            _boss->bossHasTookDamage = sfTrue;
            break;
        default:
            break;
        }
    }
}

void bossJoie_attachRing(BossJoie* _boss) {
    sfFloatRect ringRect = ring_GetBounds();

    if (ring_CanAttach() && ring_GetRingPower() == PLR_ATTRACT) {
        if (sfFloatRect_intersects(&_boss->bossBounds, &ringRect, NULL)) {
            ring_SetAttachment(RING_ATCH_BOSS);
            ring_SetPos(_boss->position);
            _boss->isBossStuned = sfTrue;
        }
    }
}

void bossJoie_stun(BossJoie* _boss) {
    bossJoieFight = BJ_ATK_NONE;
    _boss->hasHalo = sfFalse;

    static float timerStun = 0.f;
    static sfBool isStunFx = sfFalse;

    sfFloatRect playerSwordSlashRect = plr_GetDamageBounds();

    timerStun += getDeltaTime();
    if (timerStun < 5.f) {
        sfSound_stop(GetSound("SFX_Halo_Boss"));

        if (!isStunFx) {
            bossJoie_placeFX(BJ_FX_STUN, vector2f(_boss->position.x, _boss->position.y - 150.f));
            isStunFx = sfTrue;
        }

        if (_boss->hasDisc) {
            if (sfFloatRect_intersects(&_boss->discBounds, &playerSwordSlashRect, NULL)) {
                bossJoie_takeDamage(BJ_DISC, _boss);
            }
        }
        else {
            if (sfFloatRect_intersects(&_boss->bossBounds, &playerSwordSlashRect, NULL)) {
                bossJoie_takeDamage(BJ_CORE, _boss);
            }
        }
    }
    else {
        for (int i = 0; i < bossJoieFX->size(bossJoieFX); i++) {
            if (GETDATA_FX_BOSS_JOIE->type == BJ_FX_STUN) {
                bossJoieFX->erase(&bossJoieFX, i);
                break;
            }
        }

        bossJoieFight = BJ_ATK_3;
        _boss->timerAtk3 = 5.f;
        timerStun = 0.f;
        isStunFx = sfFalse;
        _boss->isBossStuned = sfFalse;
    }
}

void updateBossJoie(BossJoie* _boss, Window* _window)
{
    _boss->getPlayerBounds = plr_getPlayerBounds();
    _boss->hand1Bounds     = FlRect(_boss->hand1pos.x - _boss->originHand.x - 5.f, _boss->hand1pos.y - _boss->originHand.y - 5.f, (_boss->originHand.x * 2.f) + 10.f, (_boss->originHand.y * 2.f) + 10.f);
    _boss->hand2Bounds     = FlRect(_boss->hand2pos.x - _boss->originHand.x - 5.f, _boss->hand2pos.y - _boss->originHand.y - 5.f, (_boss->originHand.x * 2.f) + 10.f, (_boss->originHand.y * 2.f) + 10.f);
    _boss->bossBounds      = sfRectangleShape_getGlobalBounds(_boss->bossSPR);
    _boss->discBounds      = sfRectangleShape_getGlobalBounds(_boss->discSpr);

    static float timerBoss = 0.f;
    static float timerBossAttack = 0.f;
    static float timerBossAttackPhase2 = 0.f;

    float distPlayerBoss = getSqrMagnitudef(createVectorWithVectorf(_boss->position, plr_GetFeetPos()));

    switch (BossJoieState)
    {
    case BJ_PHASE_SPAWN:
        break;
    case BJ_PHASE_1:

        if (distPlayerBoss < 520.f * 520.f && _boss->hasFirstHand && _boss->hasSecondHand) {
            bossJoieFight = BJ_ATK_1;
        }
        else {
            if ((_boss->hasFirstHand && !_boss->hasSecondHand) || (!_boss->hasFirstHand && _boss->hasSecondHand)) {
                timerBossAttack += getDeltaTime();
                if (timerBossAttack < 5.f)
                    bossJoieFight = BJ_ATK_2;
                else if (timerBossAttack > 5.f) {
                    bossJoieFight = BJ_ATK_NONE;
                    if (timerBossAttack > 9.f)
                        timerBossAttack = 0.f;
                }
            }
            else bossJoieFight = BJ_ATK_2;

        }

        //Si les deux point son détruit alors lancement du compte a rebour avant qu'il ne regagne c'est point
        if (!_boss->hasFirstHand && !_boss->hasSecondHand) {
            //sfRectangleShape_setFillColor(_boss->bossSPR, sfBlue);
            bossJoieFight = BJ_ATK_NONE;
            BossJoieState = BJ_PHASE_2;
        }
        break;
    case BJ_PHASE_2:
        timerBossAttackPhase2 += getDeltaTime();

        if (timerBossAttackPhase2 >= 1.5f) {
            bossJoieFight = BJ_ATK_3;
        }

        if (_boss->isBossStuned) bossJoie_stun(_boss);
        if (_boss->isDead) BossJoieState = BJ_PHASE_DEATH;
        break;
    case BJ_PHASE_DEATH:
        _boss->isBossStuned = sfFalse;
        for (int i = 0; i < bossJoieFX->size(bossJoieFX); i++) {
            if (GETDATA_FX_BOSS_JOIE->type == BJ_FX_STUN) {
                prt_CreateBossDeathParticles(_boss->position);
                bossJoieFX->erase(&bossJoieFX, i);
                break;
            }
        }
        break;
    default:
        break;
    }

    //! Toutes les mécanique du boss ici -> 
    if (customKeyboardIsKeyPressed(sfKeyF1, _window->renderWindow)) bossJoieFight = BJ_ATK_1;
    if (customKeyboardIsKeyPressed(sfKeyF2, _window->renderWindow)) bossJoieFight = BJ_ATK_2;
    if (customKeyboardIsKeyPressed(sfKeyF3, _window->renderWindow)) bossJoie_restartBoss(_boss, vector2f(plr_GetPos().x, plr_GetPos().y - 500.f));
    if (customKeyboardIsKeyPressed(sfKeyF4, _window->renderWindow)) bossJoieFight = BJ_ATK_NONE;

    bossJoie_movments(_boss);
    bossJoie_updateDamage(_boss);

    if (bossJoieFight == BJ_ATK_NONE) {
        bossJoie_resetHandPos(_boss);
    }
    if (bossJoieFight == BJ_ATK_1) {
        //Mecanique attaque 1
        bossJoie_Atk1(_boss);
    }
    if (bossJoieFight == BJ_ATK_2) {
        //Mecanique attaque 2 
        bossJoie_Atk2(_boss);
    }
    if (bossJoieFight == BJ_ATK_3) {
        //Mecanique attaque 3  
        bossJoie_Atk3(_boss);
    }

    //! Animation du Boss
    static float timerTmp = 0.f;
    timerTmp += getDeltaTime();


    if (customKeyboardIsKeyPressed(sfKeyLShift, _window->renderWindow) && timerTmp > 0.2f) {
        _boss->hasFirstHand = !_boss->hasFirstHand; 
        timerTmp = 0.f;
    }

    if (customKeyboardIsKeyPressed(sfKeyLControl, _window->renderWindow) && timerTmp > 0.2f) {
        _boss->hasSecondHand = !_boss->hasSecondHand;
        timerTmp = 0.f;
    }

    if (BossJoieState != BJ_PHASE_SPAWN && !_boss->isDead) {
        if (_boss->angle >= -90.f && _boss->angle <= 0.f) {
            // TDL
            if (_boss->isLoadingAttract) {
                bossJoie_Animation(ABJ_ATTRACT_LOAD_TDL, _boss);
            }
            else if (_boss->isAttractingPlayer) {
                bossJoie_Animation(ABJ_ATTRACT_TDL, _boss);
            }
            else if (_boss->isJumping) {
                bossJoie_Animation(ABJ_JUMP_TDL, _boss);
            }
            else
                bossJoie_Animation(ABJ_IDLE_TDL, _boss);
        }
        else if (_boss->angle <= 90.f && _boss->angle >= 0.f) {
            // TDR
            if (_boss->isLoadingAttract) {
                bossJoie_Animation(ABJ_ATTRACT_LOAD_TDR, _boss);
            }
            else if (_boss->isAttractingPlayer) {
                bossJoie_Animation(ABJ_ATTRACT_TDR, _boss);
            }
            else if (_boss->isJumping) {
                bossJoie_Animation(ABJ_JUMP_TDR, _boss);
            }
            else
                bossJoie_Animation(ABJ_IDLE_TDR, _boss);
        }
        else if (_boss->angle <= 270.f && _boss->angle >= 210.f) {
            // TFL
            if (_boss->isLoadingAttract) {
                bossJoie_Animation(ABJ_ATTRACT_LOAD_TFL, _boss);
            }
            else if (_boss->isAttractingPlayer) {
                bossJoie_Animation(ABJ_ATTRACT_TFL, _boss);
            }
            else if (_boss->isJumping) {
                bossJoie_Animation(ABJ_JUMP_TFL, _boss);
            }
            else
                bossJoie_Animation(ABJ_IDLE_TFL, _boss);
        }
        else if (_boss->angle <= 150.f && _boss->angle >= 90.f) {
            // TFR
            if (_boss->isLoadingAttract) {
                bossJoie_Animation(ABJ_ATTRACT_LOAD_TFR, _boss);
            }
            else if (_boss->isAttractingPlayer) {
                bossJoie_Animation(ABJ_ATTRACT_TFR, _boss);
            }
            else if (_boss->isJumping) {
                bossJoie_Animation(ABJ_JUMP_TFR, _boss);
            }
            else
                bossJoie_Animation(ABJ_IDLE_TFR, _boss);
        }
        else {
            if (_boss->isLoadingAttract) {
                bossJoie_Animation(ABJ_ATTRACT_LOAD_F, _boss);
            }
            else if (_boss->isAttractingPlayer) {
                bossJoie_Animation(ABJ_ATTRACT_F, _boss);
            }
            else if (_boss->isJumping) {
                bossJoie_Animation(ABJ_JUMP_F, _boss);
            }
            else
                bossJoie_Animation(ABJ_IDLE_F, _boss);
        }
    }
    else if (BossJoieState == BJ_PHASE_SPAWN) {
        bossJoie_Animation(ABJ_SPAWN, _boss);
    }
    else if (_boss->isDead) {
        if (pause_ItemsDiscovered & (PAUSE_JOY_GOLEM));
        else
            HUD_AddedDataAppered(_window, HUD_BESTIARY_DEMON);
        pause_EnnemisDiscovered |= PAUSE_JOY_GOLEM;
        bossJoie_Animation(ABJ_DEATH, _boss);
    }


    //Set la position du boos dans l'update

    sfRectangleShape_setPosition(_boss->bossSPR, _boss->position);
    sfRectangleShape_setPosition(_boss->discSpr, _boss->discPos);
    sfRectangleShape_setPosition(_boss->hand1SPR, _boss->hand1pos);
    sfRectangleShape_setPosition(_boss->hand2SPR, _boss->hand2pos);
    sfRectangleShape_setPosition(_boss->haloSpr, _boss->position);

    sfRectangleShape_setOrigin(_boss->bossSPR,  _boss->origin);
    sfRectangleShape_setOrigin(_boss->discSpr,  _boss->originDisc);
    sfRectangleShape_setOrigin(_boss->hand1SPR, _boss->originHand);
    sfRectangleShape_setOrigin(_boss->hand2SPR, _boss->originHand);
}

void displayBossJoie(Window* _w, BossJoie* _boss)
{
    sfVector3f vec3 = { 0.5f, 0.f, 0.f };
    sfShader_setVec3Uniform(bossJoieShader, "currentColor", vec3);

   if (_boss->hasHalo) {
        sfRectangleShape_setTextureRect(_boss->haloSpr, animatorAnim("golemJ_haloAttract", &_boss->frameXhalo, &_boss->timerAnimHalo, 0));
        sfRenderTexture_drawRectangleShape(_w->rdrt, _boss->haloSpr, NULL);
    }
        
    if (_boss->hasDisc)        _boss->discHasTookDamage ? sfRenderTexture_drawRectangleShape(_w->rdrt, _boss->discSpr, &bossJoieRenderState) : sfRenderTexture_drawRectangleShape(_w->rdrt, _boss->discSpr, NULL);
    _boss->bossHasTookDamage ? sfRenderTexture_drawRectangleShape(_w->rdrt, _boss->bossSPR, &bossJoieRenderState) : sfRenderTexture_drawRectangleShape(_w->rdrt, _boss->bossSPR, NULL);
    if (_boss->hasFirstHand)  _boss->hand1hasTookDamage ? sfRenderTexture_drawRectangleShape(_w->rdrt, _boss->hand1SPR, &bossJoieRenderState) : sfRenderTexture_drawRectangleShape(_w->rdrt, _boss->hand1SPR, NULL);
    if (_boss->hasSecondHand) _boss->hand2hasTookDamage ? sfRenderTexture_drawRectangleShape(_w->rdrt, _boss->hand2SPR, &bossJoieRenderState) : sfRenderTexture_drawRectangleShape(_w->rdrt, _boss->hand2SPR, NULL);
    bossJoie_displayFX(_w, _boss);

}

void rotationHandBossJoie(sfVector2f* _posHand1, sfVector2f* _posHand2, float _distance, float _angleBoss, BossJoie* _boss) {

    *_posHand1 = polarCoord(_boss->position, _distance, ((_angleBoss + 90.f) * DEG2RAD));
    *_posHand2 = polarCoord(_boss->position, _distance, (_angleBoss + 30.f + 90.f) * DEG2RAD);
}

void deinitBossJoie(BossJoie* _boss) {
    if (!_boss->bossSPR || !_boss->hand1SPR || !_boss->hand2SPR || !_boss->discSpr || !_boss->haloSpr) return;
    sfRectangleShape_destroy(_boss->bossSPR);
    sfRectangleShape_destroy(_boss->hand1SPR);
    sfRectangleShape_destroy(_boss->hand2SPR);
    sfRectangleShape_destroy(_boss->discSpr);
    sfRectangleShape_destroy(_boss->haloSpr);
    sfSprite_destroy(bossJoieFXsprite);
    sfShader_destroy(bossJoieShader);
}