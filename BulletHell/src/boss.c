#include <raylib.h>
#include <config.h>
#include <boss.h>
#include <helper.h>
#include <boss_attacks.h>
#include <debug.h>

static Vector2 BossPos;
static const float StartingBossHp = 100.0f;
static float BossHp;
static const float BossRadius = 20.0f;

static BossState CurrentState;
static BossAttack CurrentAttack;

static float AttackTimer;

extern const int ScreenWidth;
extern const int ScreenHeight;

extern int AttacksCount;

void InitBoss() {
    BossPos = (Vector2){ SCREEN_WIDTH / 2, SCREEN_HEIGHT / 8 };
    BossHp = 100;
    
    CurrentState = BossIdle;
    CurrentAttack = BossAttackNone;
    
    AttackTimer = 2.0f;
};

void UpdateBoss(float dt) {
    AttackTimer -= dt;
    AttackTimer = MinFloat(AttackTimer, 0.0f);
    
    switch (CurrentState) {
        case BossIdle:

            if (AttackTimer <= 0.0f) {
                CurrentState = BossAttacking;
                StartRandomAttack();
            }
            break;

        case BossAttacking:
            UpdateCurrentAttack(dt);
            break;
    }
    
    const char* stateText = "Unknown";

    switch (CurrentState)
    {
        case BossIdle:       stateText = "BossIdle"; break;
        case BossAttacking:  stateText = "BossAttacking"; break;
    }
    
    AddDebug(TextFormat("State: %s", stateText), WHITE);
    AddDebug(TextFormat("Cooldown: %.2f", AttackTimer), WHITE);
}

void EndBossAttack(void) {
    CurrentAttack = BossAttackNone;
    CurrentState = BossIdle;
    
    AttackTimer = 2.0f;
}

void DrawBoss() {
    DrawCurrentAttack();
    
    DrawCircleV(BossPos, BossRadius, RED);
    
    //hp bar
    Rectangle BossBarOutline = {ScreenWidth / 2 - ScreenWidth / 5, 5 + 30, ScreenWidth / 2.5, 25};
    DrawRectangleRec(BossBarOutline, DARKGRAY);
    DrawRectangleLinesEx(BossBarOutline, 3.0f, WHITE);
    
    float hpRatio = (float)BossHp / StartingBossHp;
    hpRatio = ClampFloat(hpRatio, 0.0f, 1.0f);
    
    Rectangle BossHpFill = {
        BossBarOutline.x + 3,
        BossBarOutline.y + 3,
        (BossBarOutline.width - 6) * hpRatio,
        BossBarOutline.height - 6
    };
    
    DrawRectangleRec(BossHpFill, MAROON);
};

void DamageBoss(float amount) {
    BossHp = MinFloat(BossHp - amount, 0.0f);
};

Vector2 GetBossPos() {
    return BossPos;
}

float GetBossRadius() {
    return BossRadius;
}

float GetBossHp() {
    return BossHp;
};