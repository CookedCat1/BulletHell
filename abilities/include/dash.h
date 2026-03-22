#ifndef DASH_H
#define DASH_H

#include <ability.h>

void InitDash();

void StartDash();
void UpdateDash(float dt);
void DrawDash();

Ability GetDashAbility();

#endif