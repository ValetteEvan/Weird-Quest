#pragma once
#include "tools.h"
#include "windowManager.h"

typedef struct DebuffTear DebuffTear;

struct DebuffTear {
    float timerToDie;
    sfVector2f pos;
    float radius;
    sfIntRect rect;
    sfFloatRect colRect;
    float alpha;
    DebuffTear* pNext;
};

void dbfT_initTearDebuff();
void dbfT_updateTearDebuff(Window* _window);
void dbfT_displayTearDebuff(Window* _window);
void dbfT_Deinit();

void dbfT_addTearDebuff(sfVector2f _pos);
DebuffTear* deleteTearDebuff(DebuffTear* _tear);