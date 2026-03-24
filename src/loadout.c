#include <raylib.h>

#include <ability.h>

#include <debug.h>
#include <helper.h>

static AbilityID SelectedAbility = -1;
Ability* abilities;

extern Vector2 GetVirtualMousePosition();

void InitLoadout() {
    abilities = GetAllAbilities();
}

void UpdateLoadout(float dt) {

}

void DrawLoadout() {
    //mouse
    Vector2 MousePos = GetVirtualMousePosition();
    
    Rectangle TopBox = {100, 75, 950, 350};
    const int Padding = 25;
    const int PrimaryYOffset = TopBox.y + Padding;
    
    //primary box
    DrawText("Primary", TopBox.x, TopBox.y - 35, 30, WHITE);
    DrawRectangleLinesEx(TopBox, 3, WHITE);
    
    //secondary box
    TopBox.y = 500;
    
    DrawText("Secondary", TopBox.x, TopBox.y - 35, 30, WHITE);
    DrawRectangleLinesEx(TopBox, 3, WHITE);

    //ability population primary
    int PrimaryXOffset = TopBox.x + Padding;
    int SecondaryXOffset = TopBox.x + Padding;
    
    for (AbilityID i=0; i < ABILITY_COUNT; i++) {
        Ability* ability = &abilities[i];
        
        Color BorderColor = WHITE;
        Rectangle AbilityButton = {0, 0, 150, 150};
        
        if (ability->Type == ABILITY_TYPE_PRIMARY) {
            AbilityButton.x = PrimaryXOffset;
            AbilityButton.y = PrimaryYOffset;
            
            PrimaryXOffset += AbilityButton.width + Padding;
            
        } else if (ability->Type == ABILITY_TYPE_SECONDARY) {
            AbilityButton.x = SecondaryXOffset;
            AbilityButton.y = TopBox.y + Padding;
            
            SecondaryXOffset += AbilityButton.width + Padding;
        } else continue;
        
        if (CheckCollisionPointRec(MousePos, AbilityButton)) {
            BorderColor = (Color){119,119,119,255};
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                BorderColor = GREEN;
                SelectedAbility = i;
            }
        } 
        
        if (i == SelectedAbility) BorderColor = GREEN;
        
        DrawRectangleLinesEx(AbilityButton, 2, BorderColor);
    }
    
    //info side panel
    Rectangle SideBar = {1100, 75, 400, 775};
    
    DrawRectangleLinesEx(SideBar, 3, WHITE);
    
    if (SelectedAbility != -1) {
        
        DrawRectangleLinesEx((Rectangle){1200, 125, 200, 200}, 3, WHITE);
        
        Ability* CurrentAbility = GetAbilityByID(SelectedAbility);
        const char* TypeText = CurrentAbility->Type == ABILITY_TYPE_PRIMARY ? "Primary" : "Secondary";

        int TypeFontSize = 30;
        int TypeTextWidth = MeasureText(TypeText, TypeFontSize);

        float textX = 1200 + (200 - TypeTextWidth) / 2.0f;
        
        DrawText(TypeText, textX, 340, TypeFontSize, WHITE);
        
        Rectangle TextRect = {
            SideBar.x + 20,
            380,
            SideBar.width - 40,
            200
        };

        DrawTextRec(
            GetFontDefault(),
            CurrentAbility->Description,
            TextRect,
            20,     // font size
            2,      // spacing
            true,   // word wrap
            LIGHTGRAY
        );
        
        Rectangle EquipButton = {SideBar.x - 125 + SideBar.width / 2,
                                 SideBar.y + SideBar.height - 50 - 50,
                                 250, 60};
        
        bool Equipped = IsAbilityEquipped(SelectedAbility);
        
        DrawRectangleRec(EquipButton, Equipped ? RED : GREEN);
        DrawRectangleLinesEx(EquipButton, 3, WHITE);
        
        const char* EquipText = Equipped ? "Unequip" : "Equip";
        int FontSize = 25; 
        
        int EquipTextWidth = MeasureText(EquipText, FontSize);
        float EquipTextX = EquipButton.x + (EquipButton.width - EquipTextWidth) / 2.0f;
        float EquipTextY = EquipButton.y  + (EquipButton.height - FontSize) / 2.0f;
        
        DrawText(EquipText, EquipTextX, EquipTextY, FontSize, WHITE);
    }
}