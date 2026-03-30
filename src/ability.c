#include <raylib.h>
#include <stddef.h>
#include <stdio.h>

#include <ability.h>
#include <cooldowns.h>

//ability includes
#include <dash.h>
#include <healPool.h>

static EquippedSlot Slots[ABILITY_SLOT_COUNT];
static Ability AllAbilities[ABILITY_COUNT];
static AbilityID EquippedIDs[ABILITY_SLOT_COUNT];

void InitAbilities(void) {
    InitDash();
    InitHealPool();
    
    //Primary
    AllAbilities[ABILITY_DASH] = GetDashAbility();
    AllAbilities[ABILITY_SHIELD] = (Ability){
        .Name        = "Shield",
        .Cooldown    = 3.0f,
        .Description = "shiel",
        
        .Type = ABILITY_TYPE_PRIMARY,
        .BackDropColor = BLUE,
        
        .Icon = LoadTexture("assets/Textures/ShieldIcon.png"),
        
        .Start       = NULL,
        .Update      = NULL,
        .Draw        = NULL
    };
    
    //Secondary
    AllAbilities[ABILITY_BOMB] = (Ability){
        .Name        = "Bomb",
        .Cooldown    = 5.0f,
        .Description = "a bomb that is thrown at the nigga boss and it goes boom",
        
        .Type = ABILITY_TYPE_SECONDARY,
        .BackDropColor = ORANGE,
        
        .Icon = LoadTexture("assets/Textures/BombIcon.png"),
        
        .Start       = NULL,
        .Update      = NULL,
        .Draw        = NULL
    };
    AllAbilities[ABILITY_SUPERCHARGE] = (Ability){
        .Name        = "Supercharge",
        .Cooldown    = 8.0f,
        .Description = "bullets go ratatatata vroooooom",
        
        .Type = ABILITY_TYPE_SECONDARY,
        .BackDropColor = RED,
        
        .Start       = NULL,
        .Update      = NULL,
        .Draw        = NULL
    };
    
    AllAbilities[ABILITY_HEALPOOL] = GetHealPoolAbility();

    EquippedIDs[SLOT_PRIMARY] = ABILITY_DASH;
    EquippedIDs[SLOT_SECONDARY] = ABILITY_HEALPOOL;

    for (int i = 0; i < ABILITY_SLOT_COUNT; i++) {
        Slots[i].ability = &AllAbilities[EquippedIDs[i]];
        Slots[i].Active = false;
    }
}

void StartAbility(AbilitySlot slot) {
    EquippedSlot* s = &Slots[slot];
    
    if (!s->ability) return;
    Ability* ability = s->ability;
    
    if (ability->CooldownID && OnCooldown(ability->CooldownID))  return;

    s->Active = true;
    TriggerCooldown(ability->CooldownID, ability->Cooldown);

    if (ability->Start) ability->Start();
}

void DeactivateAbility(AbilitySlot slot) {
    Slots[slot].Active = false;
}

void UpdateAbilities(float dt) {
    for (int i = 0; i < ABILITY_SLOT_COUNT; i++) {
        EquippedSlot* slot = &Slots[i];

        if (slot->Active && slot->ability->Update) slot->ability->Update(dt);
    }
}

void DrawAbilities(void) {
    for (int i = 0; i < ABILITY_SLOT_COUNT; i++) {
        EquippedSlot* slot = &Slots[i];
        
        if (slot->Active && slot->ability->Draw) slot->ability->Draw();
    }
}

void EquipAbility(AbilitySlot slot, AbilityID id) {
    Ability* ability = &AllAbilities[id];
    
    if ((slot == SLOT_PRIMARY && ability->Type != ABILITY_TYPE_PRIMARY) 
       || (slot == SLOT_SECONDARY && ability->Type != ABILITY_TYPE_SECONDARY)) return;
    
    EquippedIDs[slot] = id;
    Slots[slot].ability = ability;
    Slots[slot].Active = false;
}

void SaveAbilities(const char* file) {
    FILE* f = fopen(file, "w");
    if (!f) return;
    fprintf(f, "%d %d", EquippedIDs[SLOT_PRIMARY], EquippedIDs[SLOT_SECONDARY]);
    fclose(f);
}

void LoadAbilities(const char* file) {
    FILE* f = fopen(file, "r");
    if (!f) return;
    fscanf(f, "%d %d", &EquippedIDs[SLOT_PRIMARY], &EquippedIDs[SLOT_SECONDARY]);
    fclose(f);

    for (int i = 0; i < ABILITY_SLOT_COUNT; i++) {
        Slots[i].ability = &AllAbilities[EquippedIDs[i]];
        Slots[i].Active = false;
    }
}

void CleanupAbilities() {
    CleanupHealPool();
    CleanupDash();
    
    UnloadTexture(AllAbilities[ABILITY_BOMB].Icon);
    UnloadTexture(AllAbilities[ABILITY_SHIELD].Icon);
}

//getters
Ability* GetAbilityByID(AbilityID id) {
    return &AllAbilities[id];
}

Ability* GetAllAbilities() {
    return AllAbilities;
}

int GetAbilityCount() {
    return ABILITY_COUNT;
}

bool IsAbilityEquipped(AbilityID id) {
    return EquippedIDs[SLOT_PRIMARY] == id || EquippedIDs[SLOT_SECONDARY] == id;
}