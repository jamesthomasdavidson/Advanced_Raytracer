#pragma once
#include <math.h>
#include "Image.h"
#include <iostream>
using namespace std;
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;

typedef struct ObjectInfo
{
    vec3 Colour;
    vec3 Normal;
    vec3 IntersectDirection;
    float t;
}ObjectInfo;
vec3 PixToVec(Pixel px){return vec3(px.R, px.G, px.B);}

