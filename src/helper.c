#include <raylib.h>
#include <raymath.h>

#include <config.h>

float ClampFloat(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float MinFloat(float value, float min) {
    if (value < min) return value;
    return min;
}

float MaxFloat(float value, float max) {
    if (value > max) return value;
    return max;
}

float GetCollisionCircles(Vector2 pos1, float radius1, Vector2 pos2, float radius2) {
    float dist = Vector2Distance(pos1, pos2);
    float totalRadius = radius1 + radius2;

    return MinFloat(totalRadius - dist, 0.0f);
}

void DrawTextRec(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint)
{
    int length = TextLength(text);

    float textOffsetY = 0;
    float textOffsetX = 0;

    float scaleFactor = fontSize/(float)font.baseSize;

    for (int i = 0; i < length; i++)
    {
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        if (codepoint == '\n')
        {
            textOffsetY += (font.baseSize + spacing)*scaleFactor;
            textOffsetX = 0;
        }
        else
        {
            if (textOffsetX + font.recs[index].width*scaleFactor > rec.width)
            {
                textOffsetY += (font.baseSize + spacing)*scaleFactor;
                textOffsetX = 0;
            }

            if (textOffsetY + font.baseSize*scaleFactor > rec.height) break;

            DrawTextCodepoint(font, codepoint,
                (Vector2){ rec.x + textOffsetX, rec.y + textOffsetY },
                fontSize, tint);

            textOffsetX += (font.recs[index].width + spacing)*scaleFactor;
        }

        i += (codepointByteCount - 1);
    }
}