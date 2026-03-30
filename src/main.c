#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raymath.h"
#include <stdbool.h>
#include <raygui.h>

#include <game.h>
#include <loadout.h>
#include <pauseMenu.h>
#include <mainMenu.h>
#include <settings.h>
#include <ability.h>

#include <snowParticles.h>

#include <config.h>
#include <helper.h>
#include <debug.h>

GameState CurrentGameState = Game_MainMenu;

bool Paused = false;

Vector2 GetVirtualMousePosition() {
    float screenWidth = GetScreenWidth();
    float screenHeight = GetScreenHeight();

    float scale = fminf(screenWidth / GAME_WIDTH,
                        screenHeight / GAME_HEIGHT);

    float destWidth = GAME_WIDTH * scale;
    float destHeight = GAME_HEIGHT * scale;

    float destX = (screenWidth - destWidth) / 2;
    float destY = (screenHeight - destHeight) / 2;
    
    SetMouseOffset(-destX, -destY);
    SetMouseScale(1.0f/scale, 1.0f/scale);

    Vector2 mouse = GetMousePosition();

    return mouse;
}

int main(void) {
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "Bullet hell");
    InitAudioDevice();
    SetTargetFPS(2400);
    
    // Inits
    InitGame();
    InitDebug();
    InitAbilities();
    InitLoadout();
    
    InitSettings();
    LoadSettings("saveData/settings.cfg");
    
    //raygui init
    GuiLoadStyle("assets/test.rgs");
    
    // Rendering Init
    RenderTexture2D GameTarget = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
    SetTextureFilter(GameTarget.texture, TEXTURE_FILTER_BILINEAR); 
    
    if (Settings.Fullscreen) ToggleBorderlessWindowed();
        
    //SetWindowSize(1820, 200);
    
    while (!WindowShouldClose()) {
        
        if (CurrentGameState == Game_Exit) break;
        
        double dt = GetFrameTime();
        
        if (IsKeyPressed(KEY_TAB)) {
            if (CurrentGameState == Game_Playing) {
                CurrentGameState = Game_Paused;
            } else {
                CurrentGameState = Game_Playing;
            }
        }
        
        if (IsKeyPressed(KEY_L)) {
            if (CurrentGameState == Game_Playing) {
                CurrentGameState = Game_Loadout;
            } else {
                CurrentGameState = Game_Playing;
            }
        }
        
        if (IsKeyPressed(KEY_M)) {
            CurrentGameState = Game_MainMenu;
        }
        
        if (IsKeyPressed(KEY_P)) Paused = !Paused;
        
        AddDebug(TextFormat("Paused: %d", Paused), Paused? RED : GREEN);
        
        // updates
        if (Settings.ShowSnowParticles) {
            UpdateSnowParticles(dt);
        }
        
        switch (CurrentGameState) {
            case Game_Playing:
                UpdateGame(dt);
                break;
                
            case Game_Paused:
                UpdatePauseMenu(dt, &CurrentGameState);
                break;
                
            case Game_Settings:
                UpdateSettingsMenu(dt, &CurrentGameState);
                break;
                
            case Game_Loadout:
                UpdateLoadout(dt);
                break;
                
            case Game_MainMenu:
                UpdateMainMenu(dt);
                break;
        }
        
        GetVirtualMousePosition();
        
        //game drawing
        BeginTextureMode(GameTarget);
            ClearBackground(BLACK);
            
            AddDebug(TextFormat("Show particles: %d", Settings.ShowSnowParticles), WHITE);
            if (Settings.ShowSnowParticles) {
                DrawSnowParticles();
            }
                    
            switch (CurrentGameState) {
                case Game_Playing:
                    DrawGame();
                    break;
                    
                case Game_Paused:                
                    DrawGame();
                    DrawPauseMenu(&CurrentGameState);
                    break;
                    
                case Game_Settings:
                    DrawGame();
                    DrawSettingsMenu(&CurrentGameState);
                    break;
                    
                case Game_Loadout:
                    DrawLoadout();
                    break;
                    
                case Game_MainMenu:
                    DrawMainMenu(&CurrentGameState);
                    break;
            }
            
            DrawDebug();
            
            Rectangle PlayArea = GetPlayArea();
            
            if (Settings.FPSCounter) DrawFPS(PlayArea.x + PlayArea.width - 85, PlayArea.y - 25);
            
            ClearDebug();
        EndTextureMode();
        
        // game render to scale
        BeginDrawing();
            ClearBackground(BLACK);

            float screenWidth = GetScreenWidth();
            float screenHeight = GetScreenHeight();

            float scale = fminf(screenWidth / GAME_WIDTH,
                                screenHeight / GAME_HEIGHT);

            float destWidth = GAME_WIDTH * scale;
            float destHeight = GAME_HEIGHT * scale;

            float destX = (screenWidth - destWidth) / 2;
            float destY = (screenHeight - destHeight) / 2;

            Rectangle source = {0, 0, GAME_WIDTH, -GAME_HEIGHT};
            Rectangle dest = {destX, destY, destWidth, destHeight};

            DrawTexturePro(
                GameTarget.texture,
                source,
                dest,
                (Vector2){0, 0},
                0.0f,
                WHITE
            );
        EndDrawing();
    }
    
    CleanupAbilities();
    CloseAudioDevice();
    SaveSettings("saveData/settings.cfg");
    
    CloseWindow();
    
    return 0;
}