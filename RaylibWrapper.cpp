#include "RaylibWrapper.h"
#include <cstdlib>
#include <cmath>

Rectangle RayDefs::GetRectangle(float x, float y, float width, float height)
{
    Rectangle rect = {x, y, width, height};
    return rect;
}
Vector2 RayDefs::GetVector2(float x, float y)
{
    Vector2 vec2 = { x, y };
    return vec2;
}

Vector4 RayDefs::GetVector4(float x, float y, float z, float w)
{
    return {x, y, z, w};
}

Vector2 RayDefs::GetCenteredVec2(float x, float y, int screenWidth, int screenHeight)
{
    Vector2 centeredVector2 = GetVector2(x += screenWidth, y -= screenHeight);
    return centeredVector2;
}

Vector4 RayDefs::GetCenteredVec4(float x, float y, float z, float w, int screenWidth, int screenHeight)
{
    Vector4 centeredVec4 = GetVector4(x+=screenWidth, y+=screenWidth, z-=screenHeight, w-=screenHeight);
    return centeredVec4;
}

Vector2 RayDefs::GetUncenteredVec2(float x, float y, int screenWidth, int screenHeight)
{
    Vector2 vec2 = GetVector2(x -= screenWidth, y += screenHeight);
    return vec2;
}

float RayDefs::ReverseFloat(float number)
{
    float numberToReturn = -number;
    return numberToReturn;
}

float RayDefs::SubtractFloat(float number, float subtractAmount)
{
    float subtractedNumber = (number -= subtractAmount);
    return subtractedNumber;
}

float RayDefs::FRandom(float LO, float HI)
{
    return LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
}

int RayDefs::RoundToTen(float num)
{
    float dec, numcpy;
    numcpy = num;
    dec = num - floor(num);
    num = numcpy;
    int n = floor(num);

    if (n % 10 < 5)
        n = (n / 10) * 10;
    else if (n % 10 == 5)
        if(dec > 0)
            n = (((n + 10) / 10) * 10);
        else
            n = (n / 10) * 10;
    else
        n = (((n + 10) / 10) * 10);    

    return n;
}
