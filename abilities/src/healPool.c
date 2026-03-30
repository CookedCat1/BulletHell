#include <raylib.h>
#include <reasings.h>

#include <ability.h>
#include <player.h>
#include <cooldowns.h>
#include <healPool.h>

#define MAX_PARTICLES 10

typedef struct {
    Vector2 Pos;
    float Life;
} HealParticle;

static const float HealInterval = 0.1;
static const float HealAmount = 0.5f;
static float Timer = 0.0f;
static int HealTicks = 0;
static const int MaxHealTicks = 20;

//yielding
bool YieldActive = false;
float YieldTimer = 0.0f;

//particle
static float ParticleSpawnTimer = 0.0f;
static const float ParticleSpawnInterval = 0.1f;
static HealParticle Particles[MAX_PARTICLES];

//pulse
static float PulseTimer = 0.0f;
static const float PulseDuration = 0.25f;

static bool ShowPulse = false;
static Vector2 PulsePos = {0};

CooldownID HealPoolCD;
static Ability HealPoolAbility;

void InitHealPool(void) {
    HealPoolCD = AddCooldown("Heal Pool", true);

    HealPoolAbility = (Ability) {
        .Name = "Heal Pool",
        .Description = "A healing pool that upon use,heals the player 0.5 hp every0.1s                               Total healing : 10hp",
        .Cooldown = 8.5,
        .CooldownID = HealPoolCD,
        
        .Type = ABILITY_TYPE_SECONDARY,
        
        .Icon = LoadTexture("assets/Textures/HealPoolIcon.png"),
        .BackDropColor = GREEN,
        
        .Start = StartHealPool,
        .Update = UpdateHealPool,
        .Draw = DrawHealPool,
    };
}

void StartHealPool() {
    HealTicks = 0;
    Timer = 0.0f;
    
    YieldActive = false;
    
    // pulse
    PulseTimer = PulseDuration;
    ShowPulse = true;
    PulsePos = GetPlayerPosition();
    
    for (int i = 0; i < MAX_PARTICLES; i++) {
        Particles[i].Life = 0.0f;
    }
}

void UpdateHealPool(float dt) {
    if (!YieldActive) {
        Timer -= dt;
    
        if (Timer <= 0) {
            HandleHit(-HealAmount);
            
            Timer = HealInterval;
            HealTicks++;
        }
        
        //new particles
        ParticleSpawnTimer -= dt;
        
        if (ParticleSpawnTimer <= 0) {
            ParticleSpawnTimer = ParticleSpawnInterval;
            
            for (int i = MAX_PARTICLES - 1; i > 0; i--) {
                Particles[i] = Particles[i - 1];
            }

            // spawn new particle
            Vector2 playerPos = GetPlayerPosition();

            Particles[0].Pos = (Vector2){
                playerPos.x + GetRandomValue(-20, 20),
                playerPos.y + GetRandomValue(-20, 20)
            };

            Particles[0].Life = 1.0f;
        }
        
        //pulse
        if (ShowPulse) {
            PulseTimer -= dt;

            if (PulseTimer <= 0.0f) {
                PulseTimer = 0.0f;
                ShowPulse = false;
            }
        }
        
        if (HealTicks >= MaxHealTicks) {
            YieldActive = true;
            YieldTimer = 1.0f;
        }
    }
    
    //update particles
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (Particles[i].Life > 0.0f) {
            Particles[i].Life -= dt * 2.0f;

            // float upward
            Particles[i].Pos.y -= 30.0f * dt;
        }
    }
    
    if (YieldActive) {
        YieldTimer -= dt;
        
        if (YieldTimer <= 0.0f) DeactivateAbility(SLOT_SECONDARY);
    }
}

void DrawHealPool() {
    //pulse    
    if (ShowPulse) {
        float t = 1.0f - (PulseTimer / PulseDuration); // 0 → 1

        // easing
        float radius = EaseCubicOut(t, 0.0f, 120.0f, 1.0f);
        float alpha  = 1.0f - EaseQuadOut(t, 0.0f, 1.0f, 1.0f);

        Color c = Fade(GREEN, alpha * 0.4f);

        DrawCircleV(PulsePos, radius, c);
    }

    //particles
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (Particles[i].Life > 0.0f) {
            Color c = Fade(GREEN, Particles[i].Life);
            int size = 24 + (int)(Particles[i].Life * 8);
            
            DrawText("+",
                Particles[i].Pos.x,
                Particles[i].Pos.y,
                size,
                c
            );
        }
    }
}

Ability GetHealPoolAbility() {
    return HealPoolAbility;
}

void CleanupHealPool() {
    UnloadTexture(HealPoolAbility.Icon);
}