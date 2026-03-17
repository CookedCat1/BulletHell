#include "player.h"
#include "raymath.h"
#include <math.h>
#include <reasings.h>

#include <config.h>
#include <settings.h>
#include <cooldowns.h>
#include <debug.h>
#include <game.h>
#include <helper.h>

#define MAX_BULLETS 512
#define MAX_TRAIL 12

//movement
static Vector2 PlayerPos;
static const float PlayerRadius = 12.0f;
static const float PlayerSpeed = 200.0f;

//player
static float MaxPlayerHp = 100.0f;
static float PlayerHp;
static float DisplayHp;
static bool RecentlyHit = false;
static float IFrameTime = 0.0f;

static float DamageTimer = 0; //dmg anim timer
static const float DmgAnimTime = 0.5f;

//shoot
static const float ShootCooldown = 0.25f;
static float ShootTimer = 0.0f;

//dash
static const float DashSpeed = 800.0f;
static const float DashDuration = 0.08f;
static const float DashCooldown = 0.5f;

static float DashTimer = 0.0f;
static float DashTimeLeft = 0.0f;
static bool IsDashing = false;
static Vector2 DashVelocity = {0};

static float TrailSpawnTimer = 0.0f;
static const float TrailSpawnInterval = 0.015f;

static bool RecentlyHitBoss = false;

static Rectangle PlayArea;

typedef struct {
    Vector2 Position;
    Vector2 Velocity;
    bool Active;
} Bullet;

typedef struct {
    Vector2 Pos;
    float Life;
} TrailPoint;

static Bullet Bullets[MAX_BULLETS];
static TrailPoint DashTrail[MAX_TRAIL];

CooldownID DashCD;
CooldownID ShootCD;

// ===== Public API =====

void InitPlayer(void) {
    PlayArea = GetPlayArea();
    
    DashCD = AddCooldown("Dash", true);
    ShootCD = AddCooldown("Shoot", true);
    
    PlayerHp = MaxPlayerHp;
    DisplayHp = PlayerHp;

    PlayerPos = (Vector2){ PlayArea.x + PLAY_WIDTH / 2.0f, PlayArea.y + PLAY_HEIGHT / 1.35f };
}

void UpdatePlayer(float dt, Vector2 bossPos, float bossRadius)
{
    // recently hit reset
    if (RecentlyHit) RecentlyHit = false;
    DamageTimer = MaxFloat(DamageTimer - dt, 0.0f);
    
    // Timers
    if (IFrameTime > 0.0f)
        IFrameTime -= dt;
    
    AddDebug(TextFormat("IFrameTime: %.2f", IFrameTime), YELLOW);

    // Movement
    Vector2 moveDir = GetPlayerMoveDirection();
    PlayerPos = Vector2Add(PlayerPos, Vector2Scale(moveDir, PlayerSpeed * dt));

    PlayerPos.x = ClampFloat(PlayerPos.x, PlayArea.x + PlayerRadius + 2, PlayArea.x + PLAY_WIDTH - PlayerRadius - 2);
    PlayerPos.y = ClampFloat(PlayerPos.y, PlayArea.y + PlayerRadius + 2, PlayArea.y + PLAY_HEIGHT - PlayerRadius - 2);

    // Dash
    if (IsKeyPressed(KEY_SPACE) && !OnCooldown(DashCD)) {
        Vector2 dir = GetPlayerMoveDirection();
        if (Vector2Length(dir) > 0) {
            DashVelocity = Vector2Scale(dir, DashSpeed);
            DashTimeLeft = DashDuration;
            IFrameTime += 0.25f;
            IsDashing = true;
            TriggerCooldown(DashCD, DashCooldown);
        }
    }

    if (IsDashing) {
        TrailSpawnTimer -= dt;
        DashTimeLeft -= dt;
        
        PlayerPos = Vector2Add(PlayerPos, Vector2Scale(DashVelocity, dt));

        //dash trails
        if (TrailSpawnTimer <= 0.0f) {
            TrailSpawnTimer = TrailSpawnInterval;

            for (int i = MAX_TRAIL - 1; i > 0; i--)
                DashTrail[i] = DashTrail[i - 1];

            DashTrail[0].Pos = PlayerPos;
            DashTrail[0].Life = 1.0f;
        }
        
        if (DashTimeLeft <= 0.0f) {
            IsDashing = false;
            DashVelocity = (Vector2){0};
        }
    }
    
    for (int i = 0; i < MAX_TRAIL; i++) {
    if (DashTrail[i].Life > 0.0f)
        DashTrail[i].Life -= dt * 4.0f;
    }

    // bullet shooting
    if ((IsMouseButtonDown(MOUSE_LEFT_BUTTON) || Settings.AutoShoot) && !OnCooldown(ShootCD)) {
        TriggerCooldown(ShootCD, ShootCooldown);

        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!Bullets[i].Active) {
                Bullets[i].Active = true;
                Bullets[i].Position = PlayerPos;
                Bullets[i].Velocity = (Vector2){0, -400};
                break;
            }
        }
    }

    // update bullets
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!Bullets[i].Active) continue;

        Bullets[i].Position = Vector2Add(Bullets[i].Position, Vector2Scale(Bullets[i].Velocity, dt));

        if (Bullets[i].Position.y < -10)
            Bullets[i].Active = false;

        // Boss collision
        if (CheckCollisionCircles(
                Bullets[i].Position, 4.0f,
                bossPos, bossRadius))
        {
            RecentlyHitBoss = true;            
            Bullets[i].Active = false;
        }
    }
}

void DrawPlayer(void)
{
    bool Flicker = (IFrameTime > 0.0f) && (((int)(GetTime() * 12) % 2) == 0);

    if (!Flicker)
        DrawCircleV(PlayerPos, PlayerRadius, GREEN);
    
    for (int i = 0; i < MAX_TRAIL; i++) {
        if (DashTrail[i].Life > 0.0f) {
            Color c = Fade(GREEN, DashTrail[i].Life * 0.4f);
            DrawCircleV(DashTrail[i].Pos, PlayerRadius, c);
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (Bullets[i].Active)
            DrawCircleV(Bullets[i].Position, 4, RED);
    }
}

void DrawPlayerHp(void) {
    Rectangle playArea = GetPlayArea();

    float hpRatio = PlayerHp / MaxPlayerHp;
    hpRatio = ClampFloat(hpRatio, 0.0f, 1.0f);
    
    Rectangle bar = {
        playArea.x - 230,
        playArea.y + playArea.height - 45,
        200,
        35
    };
    
    DrawRectangleRec(bar, RED);
    
    float padding = 3.0f;
    Rectangle fill = {
        bar.x + padding,
        bar.y + padding,
        (bar.width - padding * 2) * hpRatio,
        bar.height - padding * 2
    };
    
    if (DamageTimer > 0.0f)
    {
        float MaxScale = 4.5f;

        float t = DmgAnimTime - DamageTimer;
        float FxScale = EaseCubicOut(t, 1.0f, MaxScale - 1.0f, DmgAnimTime);

        float hpWidth = (bar.width - padding * 2) * (PlayerHp / MaxPlayerHp);
        float displayWidth = (bar.width - padding * 2) * (DisplayHp / MaxPlayerHp);

        float dmgWidth = displayWidth - hpWidth;

        float baseHeight = bar.height - padding * 2;
        float scaledHeight = baseHeight * FxScale;

        float yOffset = (scaledHeight - baseHeight) / 2;
        
        // i have no fucking clue how chat gpt cooked this shape math up

        Rectangle FxRec = {
            bar.x + padding + hpWidth,
            bar.y + padding - yOffset,
            dmgWidth,
            scaledHeight
        };

        float alpha = EaseCubicOut(t, 1.0f, 0.0f - 1.0f, DmgAnimTime);

        DrawRectangleRec(FxRec, Fade(GREEN, alpha));
    }

    DrawRectangleRec(fill, GREEN);
    
    DrawRectangleLinesEx(bar, 3.0f, WHITE);
    
    const char* hpText = TextFormat("%.0f/%.0f", PlayerHp, MaxPlayerHp);

    int fontSize = 20;
    int textWidth = MeasureText(hpText, fontSize);

    float textX = bar.x + (bar.width - textWidth) / 2.0f;
    float textY = bar.y + (bar.height - fontSize) / 2.0f;

    DrawText(hpText, textX, textY + 1, fontSize, Fade(WHITE, 0.95f));
}

// util functions

void AddPlayerIFrames(float duration) {
    IFrameTime += duration;
}

void HandleHit(void) { // HandleHit(float damage)
    if (IFrameTime <= 0.0f) {
        DisplayHp = PlayerHp;
        DamageTimer = DmgAnimTime;
        
        PlayerHp -= 20.0f;
        //ClampFloat(PlayerHp, 0.0f, MaxPlayerHp);
        
        IFrameTime += 1.5f;
        RecentlyHit = true;
    }
}

// getters
Vector2 GetPlayerMoveDirection(void) {
    Vector2 dir = {0, 0};

    if (IsKeyDown(KEY_W)) dir.y -= 1;
    if (IsKeyDown(KEY_S)) dir.y += 1;
    if (IsKeyDown(KEY_A)) dir.x -= 1;
    if (IsKeyDown(KEY_D)) dir.x += 1;

    if (Vector2Length(dir) > 0)
        dir = Vector2Normalize(dir);

    return dir;
}

bool CheckPlayerHit(void) {
    if (RecentlyHit) return true;
    return false;
}

bool CheckBossHit(void) {
    if (RecentlyHitBoss) {
        RecentlyHitBoss = false;
        return true;
    }
    
    return false;
}

int GetPlayerHp(void) {
    return PlayerHp;
}

Vector2 GetPlayerPosition(void) {
    return PlayerPos;
}

float GetPlayerRadius(void) {
    return PlayerRadius;
}
