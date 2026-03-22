#ifndef HEALPOOL_H
#define HEALPOOL_H

#include <ability.h>

void InitHealPool(void);

void StartHealPool(void);
void UpdateHealPool(float dt);
void DrawHealPool(void);

Ability GetHealPoolAbility(void);

#endif