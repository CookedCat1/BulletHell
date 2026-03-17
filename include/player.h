#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include <stdbool.h>

void InitPlayer(void);

void UpdatePlayer(float dt, Vector2 bossPos, float bossRadius);
void DrawPlayer(void);
void DrawPlayerHp(void);

// util functions
void AddPlayerIFrames(float duration);
void HandleHit(void);

// getters
Vector2 GetPlayerPosition(void);
Vector2 GetPlayerMoveDirection(void);
bool CheckBossHit(void);
bool CheckPlayerHit(void);
float GetPlayerRadius(void);
int GetPlayerHp(void);

#endif
