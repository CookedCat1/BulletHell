#ifndef BEAMS_H
#define BEAMS_H

#include "raylib.h"
#include <stdbool.h>

typedef struct {
    float Warning;
    float Fire;
    float Fade;
} BeamProfile;

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
    BeamProfile Profile;
    
    float FxTimer;
    float FxDuration;
} Beam;

void StartBeam(Beam* beam, Vector2 origin, float rotation, bool follow);
void UpdateBeam(Beam* beam, float dt);
void DrawBeam(Beam* beam);
void SpawnBeam(Vector2 origin, float rotation, bool follow);

void SetBeamProfile(int type);
void SetWarningStatus(bool enabled);

#endif