#ifndef GAMESTATE_H
#define GAMESTATE_H

typedef enum {
    Game_Playing,
    Game_Paused,
    Game_Settings,
    Game_MainMenu,
    Game_Loadout,
    Game_Exit,
} GameState;

#endif