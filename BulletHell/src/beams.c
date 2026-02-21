#include "beams.h"
#include "player.h"
#include <math.h>
#include <raymath.h>

#include <config.h>
#include <player.h>

typedef struct {
    float Warning;
    float Fire;
    float Fade;
} BeamProfile;

static BeamProfile FastProfile = {0.35f, 0.15f, 0.2f};
static BeamProfile DefaultProfile = {0.75f, 0.3f, 0.5f};
static BeamProfile SlowProfile = {1.2f, 0.5f, 0.55f};

static BeamProfile CurrentProfile = {0.75f, 0.3f, 0.5f};

static bool ShowWarning = true;

extern const int ScreenWidth;
extern const int ScreenHeight;

extern Beam BossBeams[];

void StartBeam(Beam* beam, Vector2 origin, float rotation, bool follow) {
    beam->State = ShowWarning? BeamWarning : BeamFiring;
    beam->Rotation = rotation;
    beam->Position = origin;
    beam->Following = follow;
    
    beam->Width = 40;
    beam->Length = 2300.0f;
    
    beam->Timer = ShowWarning? CurrentProfile.Warning : CurrentProfile.Fire; // warning duration
    beam->Alpha = 1.0f;
    
    beam->CheckedHit = false;
}

void UpdateBeam(Beam* beam, float dt) {
    if (beam->State == BeamInactive) return;
    
    Vector2 PlayerPos = GetPlayerPosition();

    beam->Timer -= dt;

    switch (beam->State) {
        case BeamWarning:
            if (beam->Timer <= 0.0f) {
                beam->State = BeamFiring;
                beam->Timer = CurrentProfile.Fire;
            }
            
            if (beam->Timer >= CurrentProfile.Fire && beam->Following) {
                beam->Position = PlayerPos;
            } else if (beam->Timer < CurrentProfile.Fire && beam->Following) {
                beam->Following = false;
            } 
            
            break;

        case BeamFiring:
            if (beam->Timer <= 0.0f) {
                beam->State = BeamFading;
                beam->Timer = CurrentProfile.Fade;
            }
            
            if (!beam->CheckedHit) {
                Vector2 player = GetPlayerPosition();
                Vector2 diff = Vector2Subtract(player, beam->Position);

                // Rotate player into beam's local space
                float angleRad = -beam->Rotation * DEG2RAD;

                float localX = diff.x * cosf(angleRad) - diff.y * sinf(angleRad);
                float localY = diff.x * sinf(angleRad) + diff.y * cosf(angleRad);

                if (localX >= -beam->Length / 2 && localX <=  beam->Length / 2 && fabsf(localY) <= beam->Width / 2) {
                    HandleHit();
                }
                
                beam->CheckedHit = true;
            }
            
            break;

        case BeamFading:
            beam->Alpha = beam->Timer / CurrentProfile.Fade;
            if (beam->Timer <= 0.0f) {
                beam->State = BeamInactive;
            }
            break;

        default: break;
    }
}

void DrawBeam(Beam* beam) {
    if (beam->State == BeamInactive) return;
    
    Color color;

    if (beam->State == BeamWarning) {
        bool flash = ((int)(GetTime() * 8) % 2) == 0;
        color = flash ? RED : WHITE;
        color.a = 180;
    } else {
        color = WHITE;
        color.a = (unsigned char)(255 * beam->Alpha);
    }
    
    Rectangle fxRect;
    
    Rectangle rect = {
        beam->Position.x, 
        beam->Position.y, 
        beam->Length,
        beam->Width,
    };
    
    Vector2 origin = {
        beam->Length / 2,
        beam->Width / 2
    };
    
    if (beam->State == BeamFiring) {
        float MaxFxScale = 2.25f;
        
        float FireProgress = 1.0f - (beam->Timer / 0.3f);
        float Curved = 1.0f - powf(1.0f - FireProgress, 3.0f);
        float FxScale = 1.0f + Curved * (MaxFxScale - 1.0f);

        Rectangle fxRect = {
            beam->Position.x,
            beam->Position.y,
            beam->Length,
            beam->Width * FxScale
        };

        Vector2 fxOrigin = {
            beam->Length / 2,
            (beam->Width * FxScale) / 2
        };

        DrawRectanglePro(
            fxRect,
            fxOrigin,
            beam->Rotation,
            Fade(WHITE, powf(1.0f - FireProgress, 2.0f) * 0.4f)
        );
    }
    
    DrawRectanglePro(rect, origin, beam->Rotation, color);
}

void SpawnBeam(Vector2 origin, float rotation, bool follow) {
    for (int i=0; i < MAX_BEAMS; i++) {
        if (BossBeams[i].State != BeamInactive) continue;
        StartBeam(&BossBeams[i], origin, rotation, follow);
        break;
    }
}

void SetBeamProfile(int type) {
    switch (type)
    {
        case 0: CurrentProfile = FastProfile;    break;
        case 1: CurrentProfile = DefaultProfile; break;
        case 2: CurrentProfile = SlowProfile;    break;
        default: CurrentProfile = DefaultProfile;
    }
}

void SetWarningStatus(bool enabled) {
    ShowWarning = enabled;
}