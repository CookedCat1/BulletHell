#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <raygui.h>

#include <settings.h>
#include <config.h>
#include <debug.h>
#include <gamestate.h>

GameSettings Settings;

void SaveSettings(const char* file)
{
    FILE* f = fopen(file, "w");
    if (!f) return;

    fprintf(f,
        "%d %d\n"
        "%d %d\n"
        "%f %f %f\n",
        Settings.AutoShoot,
        Settings.ScreenShake,
        Settings.Fullscreen,
        Settings.VSync,
        Settings.MasterVolume,
        Settings.MusicVolume,
        Settings.SfxVolume
    );

    fclose(f);
}

void LoadSettings(const char* file)
{
    FILE* f = fopen(file, "r");
    if (!f) return;

    fscanf(f,
        "%d %d"
        "%d %d"
        "%f %f %f",
        &Settings.AutoShoot,
        &Settings.ScreenShake,
        &Settings.Fullscreen,
        &Settings.VSync,
        &Settings.MasterVolume,
        &Settings.MusicVolume,
        &Settings.SfxVolume
    );

    fclose(f);
}

void InitSettings(void)
{
    //gameplay
    Settings.AutoShoot = true;
    Settings.Fullscreen = false;
    
    //graphics
    Settings.ScreenShake = true;

    //Settings.MasterVolume = 1.0f;
    //Settings.MusicVolume = 1.0f;
    //Settings.SfxVolume = 1.0f;
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
    GuiGroupBox((Rectangle) {x, y, 200, 85}, "Gameplay");   
    y += 15;

    GuiCheckBox((Rectangle){x + 10, y, 20, 20}, "Auto Shoot", &Settings.AutoShoot);
    y += 35;

    GuiCheckBox((Rectangle){x + 10, y, 20, 20}, "Screen Shake", &Settings.ScreenShake);
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