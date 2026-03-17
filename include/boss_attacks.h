#ifndef BOSS_ATTACKS_H
#define BOSS_ATTACKS_H

void StartRandomAttack(void);
void UpdateCurrentAttack(float dt);
void DrawCurrentAttack(void);

// Returns how many attacks exist
int GetAttackCount(void);

#endif