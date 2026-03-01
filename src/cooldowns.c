#include <cooldowns.h>
#include <raylib.h>
#include <config.h>

typedef struct {
    const char* Name;
    bool Draw;
    float Timer;
    float Duration;
} Cooldown;

#define MAX_COOLDOWNS 16
static Cooldown Cooldowns[MAX_COOLDOWNS];
static int CooldownCount = 0;

CooldownID AddCooldown(const char* name, bool draw) {
    if (CooldownCount >= MAX_COOLDOWNS)
        return -1;

    Cooldowns[CooldownCount] = (Cooldown){
        .Name = name,
        .Draw = draw,
        .Timer = 0,
        .Duration = 1
    };

    return CooldownCount++;
}

void TriggerCooldown(CooldownID id, float duration) {
    Cooldowns[id].Duration = duration;
    Cooldowns[id].Timer = duration;
}

void UpdateCooldowns(float dt) {
    for (int i = 0; i < CooldownCount; i++) {
        if (Cooldowns[i].Timer > 0)
            Cooldowns[i].Timer -= dt;
    }
}

void DrawCooldowns(void) {
    Vector2 start = { GAME_WIDTH - 75, 70 };
    int visualIndex = 0;
    
    DrawText("Cooldowns", start.x - 9, start.y - 30, 15, WHITE);

    for (int i = 0; i < CooldownCount; i++) {
        if (!Cooldowns[i].Draw)
            continue;

        Rectangle outline = {
            start.x,
            start.y + visualIndex * 90,
            50,
            50
        };

        DrawRectangleLinesEx(outline, 3, WHITE);

        float percent = GetCooldownPercent(i);
        float fillHeight = percent * 44;

        DrawRectangle(
            outline.x + 3,
            outline.y + 4 + (44 - fillHeight),
            44,
            fillHeight,
            RED
        );

        DrawText(Cooldowns[i].Name,
                 outline.x,
                 outline.y + 60,
                 14,
                 WHITE);

        visualIndex++;
    }
}

bool OnCooldown(CooldownID id) {
    return Cooldowns[id].Timer > 0;
}

float GetCooldownPercent(CooldownID id) {
    if (Cooldowns[id].Duration <= 0) return 0;

    float ratio = Cooldowns[id].Timer / Cooldowns[id].Duration;
    if (ratio < 0) ratio = 0;
    if (ratio > 1) ratio = 1;

    return ratio;
}
