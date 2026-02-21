#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include <stdbool.h>

void InitPlayer(void);

void UpdatePlayer(float dt, Vector2 bossPos, float bossRadius);
void DrawPlayer(void);

void HandleHit(void);

bool CheckBossHit(void);
bool CheckPlayerHit(void);

int GetPlayerLives(void);
Vector2 GetPlayerPosition(void);
Vector2 GetPlayerMoveDirection(void);
float GetPlayerRadius(void);

#endif
