#include "raylib.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include <config.h>
#include <beams.h>
#include <player.h>
#include <boss.h>
#include <cooldowns.h>
#include <helper.h>
#include <debug.h>

Beam BossBeams[MAX_BEAMS] = {0};

float ScreenShakeTime = 0.0f;
float ScreenShakeStrength = 0.0f;
Vector2 Shake = {0,0};

static Rectangle PlayArea;

void InitGame(void) {
    
    PlayArea = (Rectangle){
        (GAME_WIDTH - PLAY_WIDTH) / 2.0f,
        (GAME_HEIGHT - PLAY_HEIGHT) / 2.0f,
        PLAY_WIDTH,
        PLAY_HEIGHT
    };
    
    InitPlayer();
    InitBoss();
}

void UpdateGame(float dt) {
    // frame updates
    UpdatePlayer(dt, GetBossPos(), GetBossRadius());
    UpdateBoss(dt);
    UpdateCooldowns(dt);
    
    if (CheckBossHit()) {
        DamageBoss(1.0f);
    }
    
    for (int i = 0; i < MAX_BEAMS; i++) {
        UpdateBeam(&BossBeams[i], dt);
    }
    
    //screen shake
    if (CheckPlayerHit()) {
        ScreenShakeTime = 0.25f;
        ScreenShakeStrength = 5.0f * (5 - GetPlayerLives());
    }
    
    if (ScreenShakeTime > 0.0f)
        ScreenShakeTime -= dt;
    
    Shake = (Vector2){0,0};

    if (ScreenShakeTime > 0.0f) {
        Shake.x = GetRandomValue(-ScreenShakeStrength, ScreenShakeStrength);
        Shake.y = GetRandomValue(-ScreenShakeStrength, ScreenShakeStrength);
    }
}

void DrawGame(void) {
    // actual screen shake
    BeginMode2D((Camera2D){
        .offset = (Vector2){0,0},
        .target = Shake,
        .rotation = 0,
        .zoom = 1
    });
    
    for (int i = 0; i < MAX_BEAMS; i++) DrawBeam(&BossBeams[i]);
    
    BeginScissorMode(PlayArea.x, PlayArea.y, PlayArea.width, PlayArea.height);
    //boss
    DrawBoss();
    
    //player
    DrawPlayer();
    
    DrawRectangleLinesEx(PlayArea, 3, WHITE);
    EndScissorMode();
    
    //boss health bar
    DrawBossHpBar();
    
    DrawCooldowns();
    
    //lives
    DrawText(TextFormat("Lives: %d", GetPlayerLives()), 15, GetScreenHeight() - 40, 25, WHITE);
    
    EndMode2D();
}

Rectangle GetPlayArea(void) {
    return PlayArea;
}