#include <raylib.h>

#include <ability.h>

static Ability AllAbilities[ABILITY_COUNT];
static Ability* EquippedAbilities[ABILITY_SLOT_COUNT];
static AbilityID EquippedIDs[ABILITY_SLOT_COUNT];

// ABILITY FUNCTIONS
//dash
void StartDash(void) {
    
};

void UpdateDash(float dt) {
    
};

void DrawDash() {
    
};

// bomb
void StartBomb() {
};
void UpdateBomb(float dt) {
};
void DrawBomb() {
};

void InitAbilities() {
    AllAbilities[ABILITY_DASH] = (Ability){
        .Name = "Dash",
        .Cooldown = 1.0f,
        .Description = "IFrame dash",
        
        .Start = StartDash,
        .Update = UpdateDash,
        .Draw = DrawDash
    };

    AllAbilities[ABILITY_SHIELD] = (Ability){
        .Name = "Shield",
        .Cooldown = 3.0f,
        .Description = "",
        
        .Start = NULL,
        .Update = NULL,
        .Draw = NULL
    };

    AllAbilities[ABILITY_BOMB] = (Ability){
        .Name = "Bomb",
        .Cooldown = 5.0f,
        .Description = "",
        
        .Start = StartBomb,
        .Update = UpdateBomb,
        .Draw = DrawBomb
    };

    AllAbilities[ABILITY_SUPERCHARGE] = (Ability){
        .Name = "Supercharge",
        .Cooldown = 8.0f,
        .Description = "",
        
        .Start = NULL,
        .Update = NULL,
        .Draw = NULL
    };

    EquippedIDs[SLOT_PRIMARY] = ABILITY_DASH;
    EquippedIDs[SLOT_SECONDARY] = ABILITY_BOMB;

    EquippedAbilities[SLOT_PRIMARY] = &AllAbilities[EquippedIDs[SLOT_PRIMARY]];
    EquippedAbilities[SLOT_SECONDARY] = &AllAbilities[EquippedIDs[SLOT_SECONDARY]];
};

void SaveAbilities(const char* file) {
    FILE* f = fopen(file, "w");
    if (!f) return;
    
    fprintf(f, "%d %d",
        EquippedIDs[SLOT_PRIMARY],
        EquippedIDs[SLOT_SECONDARY]
    );
    
    fclose(f);
;

void LoadAbilities(const char* file) {
    FILE* f = fopen(file, "r");
    if (!f) return;
    
    fscanf(f, "%d %d",
        &EquippedIDs[SLOT_PRIMARY],
        &EquippedIDs[SLOT_SECONDARY]
    );

    fclose(f);

    EquippedAbilities[SLOT_PRIMARY] = &AllAbilities[EquippedIDs[SLOT_PRIMARY]];
    EquippedAbilities[SLOT_SECONDARY] = &AllAbilities[EquippedIDs[SLOT_SECONDARY]];
};

void StartAbility(AbilitySlot slot) {
    Ability* ability = EquippedAbilities[slot];

    if (!ability) return;

    if (ability->Start)
        ability->Start();
};

void UpdateAbilities(float dt) {
    for (int i = 0; i < ABILITY_SLOT_COUNT; i++) {
        Ability* ability = EquippedAbilities[i];

        if (!ability) continue;

        if (ability->Update)
            ability->Update(dt);
    }
};

void DrawAbilities(void) {
    for (int i = 0; i < ABILITY_SLOT_COUNT; i++) {
        Ability* ability = EquippedAbilities[i];

        if (!ability) continue;

        if (ability->Draw)
            ability->Draw();
    }
};

Ability GetAbilityData(const char* name) {
    //return the ability
};