#include <raylib.h>
#include <raygui.h>
#include <raymath.h>
#include <math.h>

#include <mainMenu.h>
#include <gamestate.h>

#include <config.h>

#define MAX_TEXT_TRAIL 20

typedef struct {
    Vector2 Pos;
    float Life;
    Color Color;
} TextTrail;

static TextTrail Trail[MAX_TEXT_TRAIL];

//Text particles
static float TextTime = 0.0f;
static const float TextSpawnInterval = 0.15;
static float TextSpawnTimer = 0.0f;

Vector2 GetInfinityOffset(float t) {
    float x = sinf(t * 2.0f) * 40.0f;
    float y = sinf(t * 4.0f) * 20.0f;
    return (Vector2){x, y};
}

void UpdateMainMenu(float dt) {
    TextTime += dt;
    TextSpawnTimer -= dt; 
    
    //Text effect
    // shift trail
    if (TextSpawnTimer <= 0) {
        TextSpawnTimer = TextSpawnInterval;
        
        for (int i = MAX_TEXT_TRAIL - 1; i > 0; i--) {
            Trail[i] = Trail[i - 1];
        }
    }

    // new position
    Vector2 basePos = {
        GAME_WIDTH / 2,
        250
    };

    Vector2 offset = GetInfinityOffset(TextTime);

    Trail[0].Pos = Vector2Add(basePos, offset);
    Trail[0].Life = 1.0f;
    
    //RGB shifting color
    float t = TextTime * 2.0f;

    unsigned char r = (unsigned char)((sinf(t) * 0.5f + 0.5f) * 255);
    unsigned char g = (unsigned char)((sinf(t + 2.0f) * 0.5f + 0.5f) * 255);
    unsigned char b = (unsigned char)((sinf(t + 4.0f) * 0.5f + 0.5f) * 255);

    Trail[0].Color = (Color){ r, g, b, 255 };
    
    for (int i = 0; i < MAX_TEXT_TRAIL; i++) {
        if (Trail[i].Life > 0.0f) {
            Trail[i].Life -= dt * 1.5f;
        }
    }
}

void DrawMainMenu(GameState *State) {
    //rgb trail
    const char* GameName = "Bullet Hell game yea!!!!!!!!!!!";
    int fontSize = 52;

    for (int i = MAX_TEXT_TRAIL - 1; i >= 0; i--) {
        if (Trail[i].Life > 0.0f) {
            float alpha = Trail[i].Life;

            int textWidth = MeasureText(GameName, fontSize);
            
            DrawText(
                GameName,
                Trail[i].Pos.x - textWidth / 2,
                Trail[i].Pos.y,
                fontSize,
                Fade(Trail[i].Color, alpha * 0.6f)
            );
        }
    }
    
    //white Game Name
    Vector2 basePos = {
        GAME_WIDTH / 2,
        250
    };

    Vector2 offset = GetInfinityOffset(TextTime);
    Vector2 pos = Vector2Add(basePos, offset);

    int textWidth = MeasureText(GameName, fontSize);

    DrawText(
        GameName,
        pos.x - textWidth / 2,
        pos.y,
        fontSize,
        WHITE
    );
    
    //------------------------------------------------------------------
    //main buttons
    Rectangle panel = { 
        GAME_WIDTH /2 - 287, 
        GAME_HEIGHT /2 + 25, 
        575, 
        300 
    };
    
    if (GuiButton((Rectangle){panel.x, panel.y, 575, 75}, "Play"))
    {
        *State = Game_Playing;
    }
    
    if (GuiButton((Rectangle){panel.x, panel.y + 90, 575, 75}, "Loadout"))
    {
        *State = Game_Loadout;
    }

    if (GuiButton((Rectangle){panel.x, panel.y + 180, 575, 75}, "Settings"))
    {
        *State = Game_Settings;
    }

    if (GuiButton((Rectangle){panel.x, panel.y + 270, 575, 75}, "Quit"))
    {
        CloseWindow();
    }
    
    //Play
        //sandbox mode maybe
    //Loadout
    //Settings
    //Quit
}