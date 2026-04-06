#include <raylib.h>
#include <math.h>

#include <game.h>

static float Timer = 0.0f;

static Rectangle PlayArea;

void InitSpeedrunTimer(void) {
    PlayArea = GetPlayArea();
}

void UpdateSpeedrunTimer(float dt) {
    Timer += dt;
}

void DrawSpeedrunTimer(void) {
    int Mins = floor(Timer / 60);
    
    const char* TextTimer;
    
    if (Mins <= 0) {
        TextTimer = TextFormat("%06.3f", Timer);
    } else {
        TextTimer = TextFormat("%d:%06.3f", Mins, Timer - Mins * 60);
    }
     
    DrawText(TextTimer, PlayArea.x, PlayArea.y - 25, 20, WHITE);
}

void ResetTimer(void) {
    Timer = 0.0f;
}