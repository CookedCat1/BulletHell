#include "raylib.h"
#include <stdio.h>
#include <string.h>

#define DEBUG_MAX_LINES 64
#define DEBUG_MAX_LENGTH 256
#define DEBUG_FONT_SIZE 14
#define DEBUG_PADDING 10
#define DEBUG_LINE_SPACING 2

typedef struct {
    char Text[DEBUG_MAX_LENGTH];
    Color TextColor;
} DebugLine;

static DebugLine DebugLines[DEBUG_MAX_LINES];
static int DebugCount = 0;
static bool ShouldDrawDebug = true;

void InitDebug(void)
{
    DebugCount = 0;
}

void ClearDebug(void)
{
    DebugCount = 0;
}

void AddDebug(const char* text, Color color) {
    if (DebugCount >= DEBUG_MAX_LINES)
        return;

    strncpy(DebugLines[DebugCount].Text, text, DEBUG_MAX_LENGTH - 1);
    DebugLines[DebugCount].Text[DEBUG_MAX_LENGTH - 1] = '\0';
    DebugLines[DebugCount].TextColor = color;
    DebugCount++;
}

void DrawDebug(void) {
    if (!ShouldDrawDebug) return;
    
    int ScreenWidth = GetScreenWidth();
    int ScreenHeight = GetScreenHeight();

    int Y = ScreenHeight - 50 - DEBUG_PADDING - DEBUG_FONT_SIZE;

    for (int i = 0; i < DebugCount; i++)
    {
        DrawText(
            DebugLines[i].Text,
            DEBUG_PADDING,
            Y,
            DEBUG_FONT_SIZE,
            DebugLines[i].TextColor
        );

        Y -= (DEBUG_FONT_SIZE + DEBUG_LINE_SPACING);
    }
}

void ToggleDebug() {
    ShouldDrawDebug = !ShouldDrawDebug;
}