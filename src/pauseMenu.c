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
        GAME_HEIGHT /2 - 162, 
        450, 
        325 
    };

    GuiPanel(panel, "Paused");

    if (GuiButton((Rectangle){panel.x + 50, panel.y + 60, 350, 50}, "Resume"))
    {
        *State = Game_Playing;
    }

    if (GuiButton((Rectangle){panel.x + 50, panel.y + 120, 350, 50}, "Settings"))
    {
        *State = Game_Settings;
    }
    
    if (GuiButton((Rectangle){panel.x + 50, panel.y + 180, 350, 50}, "Return to menu")) {
        *State = Game_MainMenu;
    }

    if (GuiButton((Rectangle){panel.x + 50, panel.y + 240, 350, 50}, "Quit"))
    {
        *State = Game_Exit;
    }
}