#pragma once
#include "raylib.h"

class RayDefs
{
public:
    static Rectangle GetRectangle(float x, float y, float width, float height);
    static Vector2 GetVector2(float x, float y);
    static Vector4 GetVector4(float x, float y, float z, float w);
    static Vector2 GetCenteredVec2(float x, float y, int screenWidth, int screenHeight);
    static Vector4 GetCenteredVec4(float x, float y, float z, float w, int screenWidth, int screenHeight);
    static Vector2 GetUncenteredVec2(float x, float y, int screenWidth, int screenHeight);
    static float ReverseFloat(float number);
    static float SubtractFloat(float number, float subtractAmount);
    static float FRandom(float LO, float HI);
    static int RoundToTen(float num);
};