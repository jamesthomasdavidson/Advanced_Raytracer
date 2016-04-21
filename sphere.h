#pragma once
#include "object.h"

class Sphere : public Object
{
public:
    vec3 Center;
    float Radius;

    Sphere(float c1, float c2, float c3, float r, int pix1, int pix2, int pix3, int Type)
    {
        SurfaceType = Type;
        pix = vec3(pix1, pix2, pix3);
        Center = vec3(c1,c2,c3);
        Radius = r;
    }

    virtual bool Intersect(vec3 Origin, vec3 Direction,float *t_out, vec3 *normal_out, vec3* tpix)
    {
        vec3 EO = Center - Origin;
        float v = dot(EO, Direction);
        float RadiusSquare = Radius * Radius;
        float EO_Square = dot(EO, EO);
        float discriminant = RadiusSquare - (EO_Square - v * v);

        float t = -1;
        if (discriminant > 0)
        {
            float d = sqrt(discriminant);
            t = v - d;
        }

        if (t > 0) {
            *t_out = t;
            vec3 IntersectionPoint = t*Direction;
            IntersectionPoint = IntersectionPoint + Origin;
            vec3 SurfaceNormal = IntersectionPoint - Center;
            (*normal_out) = normalize(SurfaceNormal);
            *tpix = pix;
            return true;
        }else{
            return false;
        }
    }
};
