#ifndef ABILITY_H
#define ABILITY_H

#include <cooldowns.h>

typedef enum {
    //primary
    ABILITY_DASH,
    ABILITY_SHIELD,
    
    //secondary
    ABILITY_BOMB,
    ABILITY_SUPERCHARGE,
    ABILITY_HEALPOOL,
    
    ABILITY_COUNT
} AbilityID;

typedef enum {
    SLOT_PRIMARY,
    SLOT_SECONDARY,
    
    ABILITY_SLOT_COUNT
} AbilitySlot;

typedef enum {
    ABILITY_TYPE_PRIMARY,
    ABILITY_TYPE_SECONDARY
} AbilityType;

typedef struct {
    const char* Name;
    const char* Description;
    float Cooldown;
    CooldownID CooldownID;
    
    AbilityType Type;

    void (*Start)(void);
    void (*Update)(float dt);
    void (*Draw)(void);
} Ability;

typedef struct {
    Ability* ability;
    bool Active;
} EquippedSlot;

void  InitAbilities(void);

void  StartAbility(AbilitySlot slot);
void  DeactivateAbility(AbilitySlot slot);

void  UpdateAbilities(float dt);
void  DrawAbilities(void);

void  EquipAbility(AbilitySlot slot, AbilityID id);

void  SaveAbilities(const char* file);
void  LoadAbilities(const char* file);

#endif