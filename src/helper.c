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