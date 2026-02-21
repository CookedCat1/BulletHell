#ifndef HELPER_H
#define HELPER_H

float ClampFloat(float value, float min, float max);
float MinFloat(float value, float min);
float MaxFloat(float value, float max);

float GetCollisionCircles(Vector2 pos1, float radius1, Vector2 pos2, float radius2);

#endif