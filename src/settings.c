#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <raygui.h>

#include <settings.h>
#include <config.h>
#include <debug.h>
#include <gamestate.h>

GameSettings Settings;

void SaveSettings(const char* file) {
    FILE* f = fopen(file, "w");
    printf("Saving settings \n");
    printf("%d \n", f);
    if (!f) {
        printf("Failed to open settings file for writing\n");
        return;
    }

    fprintf(f, "AutoShoot=%d\n", Settings.AutoShoot);
    fprintf(f, "ScreenShake=%d\n", Settings.ScreenShake);
    fprintf(f, "ShowSnowParticles=%d\n", Settings.ShowSnowParticles);
    
    fprintf(f, "Fullscreen=%d\n", Settings.Fullscreen);
    fprintf(f, "VSync=%d\n", Settings.VSync);

    fprintf(f, "MasterVolume=%f\n", Settings.MasterVolume);
    fprintf(f, "MusicVolume=%f\n", Settings.MusicVolume);
    fprintf(f, "SfxVolume=%f\n", Settings.SfxVolume);

    fclose(f);

    printf("Settings saved\n");
}

void LoadSettings(const char* file)
{
    printf("inside load function \n");
    FILE* f = fopen(file, "r");
    printf("%d \n", f);
    if (!f) {
        printf("No settings file found, using defaults\n");
        return;
    }

    char line[128];

    while (fgets(line, sizeof(line), f)) {
        char key[64];
        char value[64];

        if (sscanf(line, "%63[^=]=%63s", key, value) == 2) {

            // ints / bools
            if (strcmp(key, "AutoShoot") == 0)
                Settings.AutoShoot = atoi(value);

            else if (strcmp(key, "ScreenShake") == 0)
                Settings.ScreenShake = atoi(value);

            else if (strcmp(key, "ShowSnowParticles") == 0)
                Settings.ShowSnowParticles = atoi(value);

            else if (strcmp(key, "Fullscreen") == 0)
                Settings.Fullscreen = atoi(value);

            else if (strcmp(key, "VSync") == 0)
                Settings.VSync = atoi(value);

            // floats
            else if (strcmp(key, "MasterVolume") == 0)
                Settings.MasterVolume = (float)atof(value);

            else if (strcmp(key, "MusicVolume") == 0)
                Settings.MusicVolume = (float)atof(value);

            else if (strcmp(key, "SfxVolume") == 0)
                Settings.SfxVolume = (float)atof(value);
        }
    }

    fclose(f);

    printf("Settings loaded\n");
}

void InitSettings(void)
{
    //gameplay
    Settings.AutoShoot = true;
    Settings.Fullscreen = false;
    Settings.ShowSnowParticles = true;
    
    //graphics
    Settings.ScreenShake = true;
    Settings.VSync = false;

    Settings.MasterVolume = 1.0f;
    Settings.MusicVolume = 1.0f;
    Settings.SfxVolume = 1.0f;
}

void UpdateSettingsMenu(float dt, GameState *State) {
    
}

/* void DrawSettingsMenu(GameState *State) {
    Rectangle panel = { 
        GAME_WIDTH /2 - 300, 
        GAME_HEIGHT /2 - 225, 
        600, 
        450 
    };

    GuiPanel(panel, "Settings");

    /* if (GuiCheckBox((Rectangle){panel.x + 50, panel.y + 60, 350, 50}, "Resume"))
    {
        *State = Game_Playing;
    }
} */

static Vector2 scroll = {0};

void DrawSettingsMenu(GameState *State)
{
    Rectangle panel = { 
        GAME_WIDTH /2 - 250, 
        GAME_HEIGHT /2 - 225, 
        500, 
        450 
    };
    Rectangle content = {0, 0, 330, 700};

    GuiWindowBox(panel, "Settings");
    panel.y += 24;
    panel.height = 426;
    GuiScrollPanel(panel, NULL, content, &scroll, NULL);

    BeginScissorMode(panel.x, panel.y, panel.width, panel.height);

    float x = panel.x + 15;
    float y = panel.y + scroll.y + 15;

    // ===== Gameplay =====
    GuiGroupBox((Rectangle) {x, y, 200, 120}, "Gameplay");   
    y += 15;

    GuiCheckBox((Rectangle){x + 10, y, 20, 20}, "Auto Shoot", &Settings.AutoShoot);
    y += 35;

    GuiCheckBox((Rectangle){x + 10, y, 20, 20}, "Screen Shake", &Settings.ScreenShake);
    y += 35;
    
    GuiCheckBox((Rectangle){x + 10, y, 20, 20}, "Show background particles", &Settings.ShowSnowParticles);
    y += 60;

    // ===== Graphics =====
    GuiGroupBox((Rectangle) {x, y, 200, 85}, "Graphics");   
    y += 15;

    bool oldFullscreen = Settings.Fullscreen;
    bool oldVSync = Settings.VSync;
    GuiCheckBox((Rectangle){x+ 10, y, 20, 20}, "Fullscreen", &Settings.Fullscreen);
    
    if (oldFullscreen != Settings.Fullscreen) {
        ToggleBorderlessWindowed();
    }
    y += 35;

    GuiCheckBox((Rectangle){x + 10, y, 20, 20}, "VSync", &Settings.VSync);
    
    if (oldVSync != Settings.VSync) {
        if (Settings.VSync) SetWindowState(FLAG_VSYNC_HINT);
        else ClearWindowState(FLAG_VSYNC_HINT);
    }
    y += 60;

    // ===== Audio =====
    GuiGroupBox((Rectangle) {x, y, 300, 130}, "Audio");   
    y += 20;

    GuiSliderBar(
        (Rectangle){x + 10, y, 200, 20},
        NULL,
        "Master",
        &Settings.MasterVolume,
        0.0f,
        1.0f
    );
    y += 35;

    GuiSliderBar(
        (Rectangle){x + 10, y, 200, 20},
        NULL,
        "Music",
        &Settings.MusicVolume,
        0.0f,
        1.0f
    );
    y += 35;

    GuiSliderBar(
        (Rectangle){x + 10, y, 200, 20},
        NULL,
        "SFX",
        &Settings.SfxVolume,
        0.0f,
        1.0f
    );

    EndScissorMode();
}