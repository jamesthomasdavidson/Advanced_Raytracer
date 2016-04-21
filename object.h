#pragma once
#include "common.h"

class Object
{
public:
    int SurfaceType;
    vec3 pix;
    virtual bool Intersect(vec3 Origin, vec3 Direction, float * t, vec3 * normal, vec3 *tpix) = 0;
};
