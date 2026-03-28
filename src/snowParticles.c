#include <raylib.h>
#include <raymath.h>

#include <snowParticles.h>
#include <config.h>

#define MAX_SNOW_PARTICLES 60

typedef struct {
    Vector2 Pos;
    float Speed;

    float Size;
    float Life;

    float Rotation;
    float RotSpeed;
} SnowParticle;

static SnowParticle Particles[MAX_SNOW_PARTICLES];

static const float ParticleSpawnInterval = 0.55f;
static float ParticleSpawnTimer = 0.0f;

static void SpawnParticle() {
    for (int i = 0; i < MAX_SNOW_PARTICLES; i++) {
        if (Particles[i].Life <= 0.0f) {
            Particles[i].Pos = (Vector2){
                GetRandomValue(0, GAME_WIDTH),
                -10
            };

            Particles[i].Speed = GetRandomValue(20, 60);

            Particles[i].Size = GetRandomValue(8, 16);
            Particles[i].Life = 1.0f;

            Particles[i].Rotation = GetRandomValue(0, 360);
            Particles[i].RotSpeed = GetRandomValue(-90, 90);

            break;
        }
    }
}

void UpdateSnowParticles(float dt) {
    ParticleSpawnTimer -= dt;
    
    if (ParticleSpawnTimer <= 0.0f) {
        ParticleSpawnTimer = ParticleSpawnInterval;
        SpawnParticle();
    }

    for (int i = 0; i < MAX_SNOW_PARTICLES; i++) {
        if (Particles[i].Life > 0.0f) {
            Particles[i].Pos.y += Particles[i].Speed * dt;
            Particles[i].Pos.x += sinf(Particles[i].Pos.y * 0.05f) * 10.0f * dt;

            Particles[i].Rotation += Particles[i].RotSpeed * dt;

            Particles[i].Size -= dt * 0.5f;
            Particles[i].Life -= dt * 0.01f;

            // kill if too small or off screen
            if (Particles[i].Size <= 0.0f || Particles[i].Pos.y > GAME_HEIGHT + 10) {
                Particles[i].Life = 0.0f;
            }
        }
    }
}

void DrawSnowParticles() {
    for (int i = 0; i < MAX_SNOW_PARTICLES; i++) {
        if (Particles[i].Life > 0.0f) {
            float alpha = Particles[i].Life;

            Color c = Fade(WHITE, alpha * 0.4f);

            Rectangle rect = {
                Particles[i].Pos.x,
                Particles[i].Pos.y,
                Particles[i].Size,
                Particles[i].Size
            };

            DrawRectanglePro(
                rect,
                (Vector2){rect.width / 2, rect.height / 2},
                Particles[i].Rotation,
                c
            );
        }
    }
}