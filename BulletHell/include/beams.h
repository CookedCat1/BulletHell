#ifndef BEAMS_H
#define BEAMS_H

#include "raylib.h"
#include <stdbool.h>

typedef enum {
    BeamInactive,
    BeamWarning,
    BeamFiring,
    BeamFading
} BeamState;

typedef struct {
    BeamState State;
    
    bool Following;
    bool CheckedHit;
    
    float WarningDuration;
    float FireDuration;
    float FadeDuration;
    
    Vector2 Position;
    float Rotation;
    float Width;
    float Length;
    
    float Timer;
    float Alpha;
} Beam;

void StartBeam(Beam* beam, Vector2 origin, float rotation, bool follow);
void UpdateBeam(Beam* beam, float dt);
void DrawBeam(Beam* beam);
void SpawnBeam(Vector2 origin, float rotation, bool follow);

void SetBeamProfile(int type);
void SetWarningStatus(bool enabled);

#endif