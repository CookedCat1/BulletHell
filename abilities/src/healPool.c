#include <raylib.h>

#include <ability.h>
#include <player.h>
#include <cooldowns.h>
#include <healPool.h>

static const float HealInterval = 0.1;
static const float HealAmount = 0.5f;
static float Timer = 0.0f;
static int HealTicks = 0;
static const int MaxHealTicks = 19;

CooldownID HealPoolCD;
static Ability HealPoolAbility;

void InitHealPool(void) {
    HealPoolCD = AddCooldown("Heal Pool", true);

    HealPoolAbility = (Ability) {
        .Name = "Heal Pool",
        .Description = "A healing pool that upon use,heals the player 0.5 hp every 0.1s                               Total healing : 10hp",
        .Cooldown = 10,
        .CooldownID = HealPoolCD,
        
        .Type = ABILITY_TYPE_SECONDARY,
        
        .Start = StartHealPool,
        .Update = UpdateHealPool,
        .Draw = DrawHealPool,
    };
}

void StartHealPool() {
    HealTicks = 0;
    Timer = 0.0f;
}

void UpdateHealPool(float dt) {
    Timer -= dt;
    
    if (Timer <= 0) {
        HandleHit(-HealAmount);
        
        Timer = HealInterval;
        HealTicks++;
    }
    
    if (HealTicks >= MaxHealTicks) {
        DeactivateAbility(SLOT_SECONDARY);
    }
}

void DrawHealPool() {

}

Ability GetHealPoolAbility() {
    return HealPoolAbility;
}