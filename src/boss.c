#include <raylib.h>

#include <game.h>
#include <config.h>
#include <boss.h>
#include <helper.h>
#include <boss_attacks.h>
#include <debug.h>

static Vector2 BossPos;
static const float StartingBossHp = 100.0f;
static float BossHp;
static const float BossRadius = 20.0f;
static const char* BossName = "Boss Name";

static BossState CurrentState;
static BossAttack CurrentAttack;

static float AttackTimer;

static Rectangle PlayArea;

extern int AttacksCount;

void InitBoss() {
    PlayArea = GetPlayArea();
    
    BossPos = (Vector2){ PLAY_WIDTH / 2 + PlayArea.x, PLAY_HEIGHT / 8 + PlayArea.y};
    BossHp = 100;
    
    CurrentState = BossIdle;
    CurrentAttack = BossAttackNone;
    
    AttackTimer = 2.0f;
};

void UpdateBoss(float dt) {
    AttackTimer -= dt;
    AttackTimer = MaxFloat(AttackTimer, 0.0f);
    
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
};

void DrawBossHpBar(void) {
    Rectangle playArea = GetPlayArea();

    Rectangle BossBarOutline = {
        PLAY_WIDTH / 2 - PLAY_WIDTH / 5 + playArea.x,
        playArea.y - 22,
        PLAY_WIDTH / 2.5f,
        25
    };

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

    //boss name
    int fontSize = 20;

    int textWidth = MeasureText(BossName, fontSize);

    float textX = BossBarOutline.x + (BossBarOutline.width - textWidth) / 2.0f;
    float textY = BossBarOutline.y - fontSize - 4;

    DrawText(BossName, textX, textY, fontSize, WHITE);
}

void DamageBoss(float amount) {
    BossHp = MaxFloat(BossHp - amount, 0.0f);
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