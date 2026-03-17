#ifndef ABILITY.H
#define ABILITY.H

typedef struct {
    const char* Name;
    float Cooldown;
    const char* Description;
    
    void (*Start)(void);
    void (*Update)(float dt);
    void (*Draw)(void);
    
} Ability;

typedef enum {
    ABILITY_DASH,
    ABILITY_SHIELD,
    ABILITY_BOMB,
    ABILITY_SUPERCHARGE,

    ABILITY_COUNT

} AbilityID;

typedef enum {
    SLOT_PRIMARY,
    SLOT_SECONDARY,

    ABILITY_SLOT_COUNT
} AbilityType;

void InitAbilities(void);

void SaveAbilities(const char* file);
void LoadAbilities(const char* file);

void StartAbility(AbilityType type);
void UpdateAbilities(float dt);
void DrawAbilities(void);

#endif