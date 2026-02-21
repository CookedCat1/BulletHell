#ifndef COOLDOWNS_H
#define COOLDOWNS_H

typedef int CooldownID;

CooldownID AddCooldown(const char* name, bool draw);

void TriggerCooldown(CooldownID id, float duration);
float GetCooldownPercent(CooldownID id);
bool OnCooldown(CooldownID id);

void UpdateCooldowns(float dt);
void DrawCooldowns(void);

#endif