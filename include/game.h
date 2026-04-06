#ifndef GAME_H
#define GAME_H

void InitGame(void);
void StartGame(void);

void UpdateGame(float dt);
void DrawGame(void);

Rectangle GetPlayArea(void);

#endif