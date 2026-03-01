#include "raylib.h"
#include <stdlib.h>
#include <math.h>
#include "raymath.h"
#include <stdbool.h>
#include <raygui.h>

#include <game.h>
#include <menu.h>
// #include <settings.h>

#include <config.h>
#include <helper.h>
#include <debug.h>

GameState CurrentGameState = Game_Playing;

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
    SetTargetFPS(2400);
    
    // Inits
    InitGame();
    InitDebug();
    
    //raygui init
    GuiLoadStyle("assets/test.rgs");
    
    // Rendering Init
    RenderTexture2D GameTarget = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
    SetTextureFilter(GameTarget.texture, TEXTURE_FILTER_BILINEAR); 
    
    ToggleBorderlessWindowed();
    //ToggleFullscreen();
        
    //SetWindowSize(1820, 200);
   
    while (!WindowShouldClose()) {
        double dt = GetFrameTime();
        
        if (IsKeyPressed(KEY_TAB)) {
            if (CurrentGameState == Game_Playing) {
                CurrentGameState = Game_Paused;
            } else {
                CurrentGameState = Game_Playing;
            }
        }
        
        if (IsKeyPressed(KEY_P)) Paused = !Paused;
        
        AddDebug(TextFormat("Paused: %d", Paused), Paused? RED : GREEN);
        
        // updates
        switch (CurrentGameState) {
            case Game_Playing:
                UpdateGame(dt);
                
                break;
                
            case Game_Paused:
                UpdatePauseMenu(dt, &CurrentGameState);
                break;
        }
        
        GetVirtualMousePosition();
        
        //game drawing
        BeginTextureMode(GameTarget);
            ClearBackground(BLACK);
                    
            switch (CurrentGameState) {
                case Game_Playing:
                    DrawGame();
                    break;
                    
                case Game_Paused:                
                    DrawGame();
                    DrawPauseMenu(&CurrentGameState);
                    break;
            }
            
            DrawDebug();
            
            Vector2 FpsPos = {930, 10};
            DrawFPS(FpsPos.x, FpsPos.y);
            DrawText(TextFormat("Screen Resolution: %dx%d", GetScreenWidth(), GetScreenHeight()), 930, 30, 18, GREEN);
            
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
    CloseWindow();
    return 0;
}