#ifndef BOSS_ATTACKS_H
#define BOSS_ATTACKS_H

// Called by boss.c when selecting a random attack
void StartRandomAttack(void);

// Called every frame while boss is in BossAttacking state
void UpdateCurrentAttack(float dt);
void DrawCurrentAttack(void);

// Returns how many attacks exist
int GetAttackCount(void);

#endif