#ifndef BOSS_H
#define BOSS_H

typedef enum {
    BossIdle,
    BossMoveToCenter,
    BossAttacking
} BossState;

typedef enum {
    BossAttackNone,
    BeamSpin,
    AlternatingCrossBeam,
    CrossBeam,
    Grid,
} BossAttack;

void InitBoss(void);

void UpdateBoss(float dt);
void UpdateBossAttack(float dt);
void DrawBoss(void);

void EndBossAttack(void);

void DamageBoss(float amount);

Vector2 GetBossPos(void);
float GetBossRadius(void);
float GetBossHp(void);

#endif