#include "raylib.h"
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <reasings.h>

#include <speedrunTimer.h>
#include <beams.h>
#include <player.h>
#include <boss.h>
#include <cooldowns.h>
#include <helper.h>
#include <ability.h>

#include <debug.h>
#include <config.h>
#include <settings.h>

Beam BossBeams[MAX_BEAMS] = {0};

//screen shake
static float ScreenShakeTime = 0.0f;
static float ScreenShakeStrength = 0.0f;
Vector2 Shake = {0,0};

static Music BossMusic;
static float DeathTimer = 0.0f;
static float MaxDeathTime = 4.0f;

//black screen fade
static float BlackScreenAlpha;

static Rectangle PlayArea;

void InitGame(void) {
    
    PlayArea = (Rectangle){
        (GAME_WIDTH - PLAY_WIDTH) / 2.0f,
        (GAME_HEIGHT - PLAY_HEIGHT) / 2.0f,
        PLAY_WIDTH,
        PLAY_HEIGHT
    };
    
    BlackScreenAlpha = 1.0f;
    DeathTimer = MaxDeathTime;
    
    BossMusic = LoadMusicStream("assets/Music/BossMusic.mp3");
    
    InitPlayer();
    InitBoss();
    InitSpeedrunTimer();
}

void StartGame() {
    BlackScreenAlpha = 1.0f;
    DeathTimer = MaxDeathTime;
    
    SetMusicVolume(BossMusic, Settings.MusicVolume);
    SeekMusicStream(BossMusic, 0.0f);
    PlayMusicStream(BossMusic);
    
    StartPlayer();
    StartBoss();
    
    DestroyAllBeams();
}

void UpdateGame(float dt) {
    // frame updates
    UpdatePlayer(dt, GetBossPos(), GetBossRadius());
    UpdateBoss(dt);
    UpdateCooldowns(dt);    
    UpdateAbilities(dt);
    
    if (!IsPlayerDead()) UpdateSpeedrunTimer(dt);
    
    UpdateMusicStream(BossMusic);
    
    if (IsPlayerDead()) {
        DeathTimer -= dt;
        
        float t = 1.0f - (DeathTimer / MaxDeathTime);
        SetMusicPitch(BossMusic, EaseCubicOut(t, 1.0f, -1.0f, 1.0f));
        
        if (t >= 1.0f) {
            StopMusicStream(BossMusic);
            SetMusicPitch(BossMusic, 1.0f);
        }
    }
    
    if (CheckBossHit()) {
        DamageBoss(1.0f);
    }
    
    for (int i = 0; i < MAX_BEAMS; i++) {
        UpdateBeam(&BossBeams[i], dt);
    }
    
    //black screen fade
    if (BlackScreenAlpha > 0.0f) {
        BlackScreenAlpha -= dt * 0.75f;
    }
    
    //screen shake
    if (Settings.ScreenShake) {
       if (CheckPlayerHit()) {
           ScreenShakeTime = 0.25f;
           ScreenShakeStrength = (100 - GetPlayerHp()) / 3;
        }
        
        if (ScreenShakeTime > 0.0f)
            ScreenShakeTime -= dt;
        
        Shake = (Vector2){0,0};

        if (ScreenShakeTime > 0.0f) {
            Shake.x = GetRandomValue(-ScreenShakeStrength, ScreenShakeStrength);
            Shake.y = GetRandomValue(-ScreenShakeStrength, ScreenShakeStrength);
        } 
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
    
    DrawRectangleRec(PlayArea, BLACK);
    
    for (int i = 0; i < MAX_BEAMS; i++) DrawBeam(&BossBeams[i]);
    
    BeginScissorMode(PlayArea.x, PlayArea.y, PlayArea.width, PlayArea.height);
    //boss
    DrawBoss();
    
    //player
    DrawAbilities();
    DrawPlayer();
    
    DrawRectangleLinesEx(PlayArea, 3, WHITE);
    EndScissorMode();
    
    //boss health bar
    DrawBossHpBar();
    
    DrawCooldowns();
    
    //lives
    DrawPlayerHp();
    
    // speedrun timer
    DrawSpeedrunTimer();
    
    //black screen
    if (BlackScreenAlpha > 0.0f) {
        DrawRectangle(0, 0, GAME_WIDTH, GAME_HEIGHT, Fade(BLACK, BlackScreenAlpha));
    }
    
    if (IsPlayerDead()) {
        float t = ClampFloat((1.0f - (DeathTimer / MaxDeathTime)) * 2.5f, 0.0f, 1.0f);
        DrawRectangle(0,0, GAME_WIDTH, GAME_HEIGHT, Fade(RED, EaseCubicOut(t, 1.0f, -0.75f, 1.0f)));
    }
     
    EndMode2D();
}

void CleanupGame() {
    UnloadMusicStream(BossMusic);
}

Rectangle GetPlayArea(void) {
    return PlayArea;
}