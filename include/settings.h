#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>
#include <gamestate.h>

typedef struct
{
    bool AutoShoot;
    bool Fullscreen;
    bool ShowSnowParticles;
    bool FPSCounter;
    
    bool ScreenShake;
    bool VSync;
    
    float MasterVolume;
    float MusicVolume;
    float SfxVolume;

} GameSettings;

extern GameSettings Settings;

void SaveSettings(const char* file);
void LoadSettings(const char* file);

void InitSettings(void);
void UpdateSettingsMenu(float dt, GameState *State);
void DrawSettingsMenu(GameState *State);

#endif