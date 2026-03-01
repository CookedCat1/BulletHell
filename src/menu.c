#include "raylib.h"
#include <stdbool.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

#include <config.h>
#include <helper.h>
#include <gamestate.h>

void UpdatePauseMenu(float dt, GameState *State) {

}

void DrawPauseMenu(GameState *State) {
    Rectangle panel = { 
        GAME_WIDTH /2 - 225, 
        GAME_HEIGHT /2 - 150, 
        450, 
        300 
    };

    GuiPanel(panel, "Paused");

    if (GuiButton((Rectangle){panel.x + 50, panel.y + 60, 350, 50}, "Resume"))
    {
        *State = Game_Playing;
    }

    if (GuiButton((Rectangle){panel.x + 50, panel.y + 120, 350, 50}, "Settings"))
    {
        ToggleBorderlessWindowed();
        //*State = Game_Settings;
    }

    if (GuiButton((Rectangle){panel.x + 50, panel.y + 180, 350, 50}, "Quit"))
    {
        CloseWindow();
    }
}