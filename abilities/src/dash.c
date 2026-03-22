#include <raylib.h>
#include <raymath.h>

#include <dash.h>
#include <player.h>
#include <cooldowns.h>
#include <ability.h>

#include <debug.h>
#include <helper.h>

#define MAX_TRAIL 12

typedef struct {
    Vector2 Pos;
    float   Life;
} TrailParticle;

//consts
static const float DashSpeed = 800.0f;
static const float DashDuration = 0.08f;
static const float DashCooldown = 0.5f;

//dash
static float DashTimer = 0.0f;
static bool IsDashing = false;
static Vector2 DashVelocity = {0};

//trail
static float TrailSpawnTimer = 0.0f;
static const float TrailSpawnInterval = 0.015f;
static TrailParticle DashTrail[MAX_TRAIL];

static float TrailYieldTimer = 0.0f;

//cooldown
CooldownID DashCD;

static Ability DashAbility;

void InitDash() {
    DashCD = AddCooldown("Dash", true);
    
    DashAbility = (Ability){
        .Name = "Dash",
        .Description = "IFrame dash",
        .Cooldown = DashCooldown,
        .CooldownID = DashCD,
        
        .Type = ABILITY_TYPE_PRIMARY,
        
        .Start = StartDash,
        .Update = UpdateDash,
        .Draw = DrawDash
    };
}

void StartDash(void) {
    Vector2 Direction = GetPlayerMoveDirection();
    
    if (Vector2Length(Direction) <= 0) return;

    DashVelocity = Vector2Scale(Direction, DashSpeed);
    DashTimer = DashDuration;
    IsDashing = true;
    
    for (int i = 0; i < MAX_TRAIL; i++)
    DashTrail[i].Life = 0.0f;

    AddPlayerIFrames(0.25f);
}

void UpdateDash(float dt) {
    if (IsDashing) {
        TrailSpawnTimer -= dt;
        DashTimer = MaxFloat(DashTimer - dt, 0.0f);
        
        AddPlayerVelocity(DashVelocity);

        //dash trails
        if (TrailSpawnTimer <= 0.0f) {
            TrailSpawnTimer = TrailSpawnInterval;

            for (int i = MAX_TRAIL - 1; i > 0; i--)
                DashTrail[i] = DashTrail[i - 1];

            DashTrail[0].Pos = GetPlayerPosition();
            DashTrail[0].Life = 1.0f;
        }
        
        if (DashTimer <= 0.0f) {
            IsDashing = false;
            DashVelocity = (Vector2){0};
            
            TrailYieldTimer = .5f;
        }
    } else {
        if (TrailYieldTimer > 0.0f) {
            TrailYieldTimer -= dt;
        } else {
            TrailYieldTimer = 0.0f;
            DashTimer = 0.0f;
            
            DeactivateAbility(SLOT_PRIMARY);
        }
    }
    
    for (int i = 0; i < MAX_TRAIL; i++) {
        if (DashTrail[i].Life > 0.0f) DashTrail[i].Life -= dt * 4.0f;
    }
}

void DrawDash() {
    for (int i = 0; i < MAX_TRAIL; i++) {
        if (DashTrail[i].Life > 0.0f) {
            Color c = Fade(GREEN, DashTrail[i].Life * 0.4f);
            DrawCircleV(DashTrail[i].Pos, GetPlayerRadius(), c);
        }
    }
}

Ability GetDashAbility(void) {
    return DashAbility;
}