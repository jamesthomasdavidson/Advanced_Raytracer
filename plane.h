#pragma once
#include "object.h"

class Plane : public Object
{
public:
    vec3 Normal;
    vec3 Point;

    Plane(float a, float b, float c, float p1, float p2, float p3, float pix1, float pix2, float pix3)
    {
        SurfaceType = 0;
        Normal = normalize(vec3(a,b,c));
        Point = vec3(p1,p2,p3);
        pix = vec3(pix1, pix2, pix3);
    }
    float CalculateT(vec3 Origin, vec3 Direction){
         return ((dot(Point, Normal) - dot(Normal, Origin))/
                (dot(Normal, Direction)));
    }

    virtual bool Intersect(vec3 Origin, vec3 Direction, float * t, vec3 * normal, vec3 * tpix)
    {
        if(CalculateT(Origin, Direction) >= 0)
        {
            *tpix = pix;
            *t = CalculateT(Origin, Direction);
            (*normal) = Normal;
            return true;
        }
        return false;
    }
};
