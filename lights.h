#pragma once
#include "common.h"

class Lights
{
public:
    vec3 Light;
    vec3 p1;
    vec3 p2;
    vec3 p3;
    vec3 p4;
    vec3 LightVector;
    vec3 eVec;
    float Length;
    bool IsBlocked;
    Lights(float x, float y, float z)
    {
        Light = vec3(x,y,z);
        IsBlocked = false;
    }
};

