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
#include <ability.h>

#define MAX_BULLETS 512

//movement
static Vector2 PlayerPos;
static const float PlayerRadius = 12.0f;
static const float PlayerSpeed = 200.0f;
static Vector2 Velocity = {0};

//player
static float MaxPlayerHp = 100.0f;
static float PlayerHp;
static float DisplayHp;

static const float MaxOverHealth = 20.0f;
static float OverHealth;
static float DisplayOverHealth;
bool ShatteredOverHealth;

static bool RecentlyHit = false;
static bool PlayerDead = false;
static float IFrameTime = 0.0f;

static float DamageTimer = 0; //dmg anim timer
static const float DmgAnimTime = 0.5f;

//shoot
static const float ShootCooldown = 0.25f;
static float ShootTimer = 0.0f;

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

CooldownID ShootCD;

//sounds
static Sound OverHpShatterSound;
static Sound HitSound;
static Sound CritHpSound;
static bool PlayedCritSound = false;

//textures
static Texture2D ViginetteTexture;

// ===== Public API =====

void InitPlayer(void) {
    PlayArea = GetPlayArea();
    
    ShootCD = AddCooldown("Shoot", false);

    HitSound = LoadSound("assets/SFX/HitSFX.mp3");
    CritHpSound = LoadSound("assets/SFX/CriticalHpSFX.mp3");
    ViginetteTexture = LoadTexture("assets/Textures/Viginette.png");
    OverHpShatterSound = LoadSound("assets/SFX/OverhealthShatter.mp3");
}

void StartPlayer() {
    PlayerHp = MaxPlayerHp;
    OverHealth = 0.0f;
    DisplayHp = PlayerHp;
    DisplayOverHealth = OverHealth;
    
    IFrameTime = 0.0f
    
    RecentlyHit = false;
    ShatteredOverHealth = false;
    PlayerDead = false;

    PlayerPos = (Vector2){ PlayArea.x + PLAY_WIDTH / 2.0f, PlayArea.y + PLAY_HEIGHT / 1.35f };
    
    //bullet removal
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (Bullets[i].Active) {
            Bullets[i].Active = false;
            Bullets[i].Position = (Vector2){-10, -10};
            break;
        }
    }
}

void UpdatePlayer(float dt, Vector2 bossPos, float bossRadius) {    
    // recently hit reset
    if (RecentlyHit) RecentlyHit = false;
    DamageTimer = MaxFloat(DamageTimer - dt, 0.0f);
    
    // Timers
    if (IFrameTime > 0.0f)
        IFrameTime -= dt;
    
    AddDebug(TextFormat("IFrameTime: %.2f", IFrameTime), YELLOW);

    // Movement
    if (!PlayerDead) {
        Vector2 moveDir = GetPlayerMoveDirection();
        PlayerPos = Vector2Add(PlayerPos, Vector2Scale(moveDir, PlayerSpeed * dt));   
    }
    
    PlayerPos = Vector2Add(PlayerPos, Vector2Scale(Velocity, dt));
    
    //velocity reset
    Velocity = (Vector2) {0};

    PlayerPos.x = ClampFloat(PlayerPos.x, PlayArea.x + PlayerRadius + 2, PlayArea.x + PLAY_WIDTH - PlayerRadius - 2);
    PlayerPos.y = ClampFloat(PlayerPos.y, PlayArea.y + PlayerRadius + 2, PlayArea.y + PLAY_HEIGHT - PlayerRadius - 2);

    // PrimaryKeybind
    if (!PlayerDead) {
        if (IsKeyPressed(KEY_SPACE)) StartAbility(SLOT_PRIMARY);
        if (IsKeyPressed(KEY_E)) StartAbility(SLOT_SECONDARY);
    }

    // bullet shooting
    if ((IsMouseButtonDown(MOUSE_LEFT_BUTTON) || Settings.AutoShoot) && !OnCooldown(ShootCD) && !PlayerDead) {
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

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (Bullets[i].Active)
            DrawCircleV(Bullets[i].Position, 4, RED);
    }
}

void DrawPlayerHp(void) {
    Rectangle playArea = GetPlayArea();
    
    AddDebug(TextFormat("Overhealth: %.1f", OverHealth), VIOLET);

    float hpRatio = PlayerHp / MaxPlayerHp;
    hpRatio = ClampFloat(hpRatio, 0.0f, 1.0f);
    
    float OverRatio = OverHealth / MaxPlayerHp;
    OverRatio = ClampFloat(OverRatio, 0.0f, 1.0f);
    
    Rectangle bar = {
        playArea.x - 230,
        playArea.y + playArea.height - 45,
        200,
        35
    };
    
    DrawRectangleRec(bar, RED);
    
    float padding = 3.0f;
    
    float innerWidth = bar.width - padding * 2;
    float innerHeight = bar.height - padding * 2;
    
    float hpWidth = innerWidth * hpRatio;
    
    Rectangle hpFill = {
        bar.x + padding,
        bar.y + padding,
        hpWidth,
        innerHeight
    };
    
    float OverWidth = innerWidth * OverRatio;

    Rectangle OverFill = hpFill;
    OverFill.width = OverWidth;
    
    //regular hp dmg effect
    if (DamageTimer > 0.0f) {
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
    
    //overhp dmg effect
    if (ShatteredOverHealth) {
        float t = DmgAnimTime - DamageTimer;
        float scale = EaseCubicOut(t, 1.0f, 3.5f, DmgAnimTime);

        float scaledHeight = innerHeight * scale;
        float yOffset = (scaledHeight - innerHeight) / 2;

        Rectangle shatterRect = {
            bar.x + padding,
            bar.y + padding - yOffset,
            innerWidth,
            scaledHeight
        };

        float alpha = EaseCubicOut(t, 0.8f, -0.8f, DmgAnimTime);

        DrawRectangleRec(shatterRect, Fade(VIOLET, alpha));
        
        PlaySound(OverHpShatterSound);
        ShatteredOverHealth = false;
    }
    
    //main hp bar
    //-----------------------------------------------------------------------
    
    DrawRectangleRec(hpFill, GREEN);
    
    if (OverHealth > 0.0f) {        
        //Color purple = (Color){180, 80, 255, 255};
        DrawRectangleRec(OverFill, Fade(VIOLET, 0.9f));
    }
    
    DrawRectangleLinesEx(bar, 3.0f, WHITE);
    
    const char* hpText;
    
    if (OverHealth > 0.0f) {
        hpText = TextFormat("%.1f/%.1f (+%.1f)", PlayerHp, MaxPlayerHp, OverHealth);
    } else {
        hpText = TextFormat("%.1f/%.1f", PlayerHp, MaxPlayerHp);
    }

    int fontSize = 20;
    int textWidth = MeasureText(hpText, fontSize);

    float textX = bar.x + (bar.width - textWidth) / 2.0f;
    float textY = bar.y + (bar.height - fontSize) / 2.0f;

    DrawText(hpText, textX, textY + 1, fontSize, Fade(WHITE, 0.95f));
        
    // crit hp red overlay
    //-----------------------------------------------------------------------
    
    if (PlayerHp <= 10.0f) {
        if (!PlayedCritSound) {
            PlaySound(CritHpSound);
            PlayedCritSound = true;
        }

        DrawTexturePro(
            ViginetteTexture,
            (Rectangle){0, 0, ViginetteTexture.width, ViginetteTexture.height},
            (Rectangle){0, 0, GAME_WIDTH, GAME_HEIGHT},
            (Vector2){0, 0},
            0.0f,
            Fade(RED, 0.4)
        );
    }
}

// util functions

void AddPlayerIFrames(float duration) {
    IFrameTime += duration;
}

void AddPlayerVelocity(Vector2 v) {
    Velocity = Vector2Add(Velocity, v);
}

void HandleHit(float damage) {
    if (PlayerDead) return;
    
    //healing
    if (damage < 0) {
        if (PlayerHp - damage > 10.0f && PlayedCritSound) PlayedCritSound = false;
        
        if (PlayerHp - damage > MaxPlayerHp) {
            float LeftOver = PlayerHp - damage - MaxPlayerHp;
            
            PlayerHp -= damage + LeftOver;
            OverHealth += LeftOver;
            
        } else PlayerHp -= damage;
        
        PlayerHp = ClampFloat(PlayerHp, 0.0f, MaxPlayerHp);
        OverHealth = ClampFloat(OverHealth, 0.0f, MaxOverHealth);
        
        return;
    }

    //damaging
    if (IFrameTime <= 0.0f) {
        DisplayHp = PlayerHp;
        DamageTimer = DmgAnimTime;
        
        if (OverHealth > 0.0f) {
            OverHealth -= damage;
            
            if (OverHealth <= 0) ShatteredOverHealth = true;
            
        } else {
            PlayerHp -= damage;
        }
        
        PlayerHp = ClampFloat(PlayerHp, 0.0f, MaxPlayerHp);
        OverHealth = ClampFloat(OverHealth, 0.0f, MaxOverHealth);
        
        IFrameTime += 1.5f;
        RecentlyHit = true;
        PlaySound(HitSound);
        
        if (PlayerHp <= 0.0f) PlayerDead = true;
    }
}

void CleanupPlayer(void) {
    UnloadTexture(ViginetteTexture);
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

bool IsPlayerDead(void) {
    return PlayerDead;
}