#include "raylib.h"
#include <stdlib.h>
#include <math.h>
#include "raymath.h"
#include <stdbool.h>

#include <config.h>
#include <beams.h>
#include <player.h>
#include <boss.h>
#include <cooldowns.h>
#include <helper.h>
#include <debug.h>

typedef struct {
    Vector2 Position;
    Vector2 Velocity;
    bool Active;
} Bullet;

typedef struct {
    Vector2 Pos;
    float Life;
} TrailPoint;

#define MaxBullets 512
#define MaxTrail 12

Bullet Bullets[MaxBullets];
TrailPoint DashTrail[MaxTrail] = {0};
Beam BossBeams[MAX_BEAMS] = {0};

//vars
const int ScreenWidth = SCREEN_WIDTH;
const int ScreenHeight = SCREEN_HEIGHT;

float ScreenShakeTime = 0.0f;
float ScreenShakeStrength = 0.0f;

bool Paused = false;

int main(void) {
    InitWindow(ScreenWidth, ScreenHeight, "Bullet hell");
    SetTargetFPS(2400);
    
    InitPlayer();
    InitBoss();
    InitDebug();
   
    while (!WindowShouldClose()) {
        double dt = GetFrameTime();
        
        if (IsKeyPressed(KEY_TAB)) {
            Paused = !Paused;
        }
        
        AddDebug(TextFormat("Paused: %d", Paused), Paused? RED : GREEN);
        
        Vector2 shakeOffset = {0};
        
        if (Paused) {
            
        } else {
        
            if (IsKeyPressed(KEY_B)) {
                Vector2 TestTHing = {ScreenWidth / 2, ScreenHeight / 2};
                
                SpawnBeam(TestTHing, 45, true);
                SpawnBeam(TestTHing, 135, true);
                SpawnBeam(TestTHing, 90, false);
            }
            
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

            if (ScreenShakeTime > 0.0f) {
                shakeOffset.x = GetRandomValue(-ScreenShakeStrength, ScreenShakeStrength);
                shakeOffset.y = GetRandomValue(-ScreenShakeStrength, ScreenShakeStrength);
            }
        }
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        // actual screen shake
        BeginMode2D((Camera2D){
            .offset = shakeOffset,
            .target = (Vector2){0,0},
            .rotation = 0,
            .zoom = 1
        });
        
        for (int i = 0; i < MAX_BEAMS; i++) DrawBeam(&BossBeams[i]);
        
        //boss
        DrawBoss();
        
        //player
        DrawPlayer();
        
        DrawCooldowns();
        
        DrawFPS(930, 10);
        DrawDebug();
        
        //boss health bar
        DrawBossHpBar();
        
        //lives
        DrawText(TextFormat("Lives: %d", GetPlayerLives()), 15, ScreenHeight - 40, 25, WHITE);
        
        ClearDebug();
        EndMode2D();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}