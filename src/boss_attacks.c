#include <raylib.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <raymath.h>

#include "boss_attacks.h"
#include "boss.h"
#include "beams.h"
#include <config.h>
#include <debug.h>
#include <player.h>
#include <helper.h>

extern int ScreenWidth;
extern int ScreenHeight;

//  INTERNAL ATTACK SYSTEM TYPES

typedef void (*AttackStartFunc)(void);
typedef void (*AttackUpdateFunc)(float);
typedef void (*AttackDrawFunc) (void);

typedef struct {
    AttackStartFunc Start;
    AttackUpdateFunc Update;
    AttackDrawFunc Draw;
} BossAttackEntry;

//  INTERNAL STATE

static int CurrentAttackIndex = -1;

//  FORWARD DECLARATIONS OF ATTACKS

// ---- Cross Beam ----
static void CrossAttack_Start(void);
static void CrossAttack_Update(float dt);

static void SpinAttack_Start(void);
static void SpinAttack_Update(float dt);
static void SpinAttack_Draw(void);

static void HorizontalBeams1_Start(void);
static void HorizontalBeams1_Update(float dt);

//  ATTACK TABLE

static BossAttackEntry AttackTable[] = {
    //{ CrossAttack_Start, CrossAttack_Update, NULL },
    //{SpinAttack_Start, SpinAttack_Update, SpinAttack_Draw},
    {HorizontalBeams1_Start, HorizontalBeams1_Update, NULL},
};

static const int AttackCount = sizeof(AttackTable) / sizeof(BossAttackEntry);

//  PUBLIC FUNCTIONS

int GetAttackCount(void) {
    return AttackCount;
}

void StartRandomAttack(void) {
    if (AttackCount <= 0) return;

    CurrentAttackIndex = GetRandomValue(0, AttackCount - 1);

    AttackTable[CurrentAttackIndex].Start();
}

void UpdateCurrentAttack(float dt)
{
    if (CurrentAttackIndex < 0) return;

    AttackTable[CurrentAttackIndex].Update(dt);
}

void DrawCurrentAttack(void) {
    if (CurrentAttackIndex < 0) return;
    
    if (AttackTable[CurrentAttackIndex].Draw != NULL) AttackTable[CurrentAttackIndex].Draw();
}

static Vector2 Center = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

//  CROSS BEAM ATTACK

static int ShotsFired = 0;
static float Timer = 0.0f;

static void DecreaseTimer(float dt) {
    Timer -= dt;
    Timer = MinFloat(Timer, 0.0f);
}

static const int   CrossShotsMax = 6;
static const float CrossInterval = 0.4f;

static void CrossAttack_Start(void) {
    Timer = 0.0f;
    ShotsFired = 0;
}

static void CrossAttack_Update(float dt) {
    DecreaseTimer(dt);

    if (Timer <= 0.0f) {
        Vector2 bossPos = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}; //GetBossPos();

        float baseRotation = ShotsFired * 15.0f; // rotates slightly each shot

        // Spawn 4-beam cross
        for (int i = 0; i < 2; i++)
        {
            float angle = baseRotation + i * 90.0f;
            SpawnBeam(bossPos, angle, false);
        }

        ShotsFired++;
        Timer = CrossInterval;
    }

    if (ShotsFired >= CrossShotsMax)
    {
        CurrentAttackIndex = -1;
        EndBossAttack();   // tells boss to return to idle
    }
}

//  SPIN BEAM ATTACK

static const int   SpinBeamsMax = 49;
static const float SpinInterval = 0.05f;

static float DeadZoneRadius = 100.0f;

static void SpinAttack_Start(void) {
    Timer = 0.0f;
    ShotsFired = 0;
    
    SetBeamProfile(0);
}

static void SpinAttack_Update(float dt) {
    DecreaseTimer(dt);
   
   SetWarningStatus(false);

    if (Timer <= 0.0f) {
        Vector2 bossPos = Center; //GetBossPos();

        float angle = ShotsFired * 7.5f; // rotates slightly each shot

        SpawnBeam(bossPos, angle, false);

        ShotsFired++;
        Timer = SpinInterval;
    }
    
    float PlayerRadius = GetPlayerRadius();

    float collision = GetCollisionCircles(Center, DeadZoneRadius, GetPlayerPosition(), PlayerRadius);
    collision = MaxFloat(collision, GetPlayerRadius());
    
    if (collision / PlayerRadius >= 1.0f) HandleHit();

    if (ShotsFired >= SpinBeamsMax) {
        SetBeamProfile(1);
        SetWarningStatus(true);
        
        CurrentAttackIndex = -1;
        EndBossAttack();   // tells boss to return to idle
    }
}

static void SpinAttack_Draw(void) {
    DrawCircleV(Center, DeadZoneRadius, Fade(RED, 0.5f));
}

// Horizontal beam attack 1

static float HB1_Interval = 0.4f;
static int HB1_Max = 10;

Vector2 Left = {100, SCREEN_HEIGHT / 2};
Vector2 Right = {SCREEN_WIDTH - 100, SCREEN_HEIGHT / 2};

static void HorizontalBeams1_Start() {
    Timer = 0.0f;
    ShotsFired = 0;
    
    SetBeamProfile(0);
}

static void HorizontalBeams1_Update(float dt) {
    DecreaseTimer(dt);
    
    if (Timer <= 0.0f) {
        SpawnBeam(GetPlayerPosition(), GetRandomValue(-10, 10), true);
        Timer = HB1_Interval;
        
        ShotsFired++;
    }
    
    if (ShotsFired >= HB1_Max) {
        SetBeamProfile(1);
        SetWarningStatus(true);
        
        CurrentAttackIndex = -1;
        EndBossAttack();
    }
}

static void HorizontalBeams1_Draw() {
    
}