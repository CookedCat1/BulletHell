#include <raylib.h>

#include <ability.h>

static AbilityID SelectedAbility;

void UpdateLoadout(float dt) {

}

void DrawLoadout() {
    
    Rectangle TopBox = {100, 100, 500, 350};
    
    DrawRectangleLinesEx(TopBox, 3, WHITE);
    
    TopBox.y = 500;
    DrawRectangleLinesEx(TopBox, 3, WHITE);
}