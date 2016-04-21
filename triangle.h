#pragma once
#include "object.h"

class Triangle : public Object
{
public:
    vec3 Normal;
    vec3 v0;
    vec3 v1;
    vec3 v2;

    Triangle(vec3 p0, vec3 p1, vec3 p2, vec3 C, vec3 N)
    {
        v0 = p0;
        v1 = p1;
        v2 = p2;
        Normal = N;
        pix = C;
    }


    virtual bool Intersect(vec3 Origin, vec3 Direction,float *t_out, vec3 *normal_out, vec3* tpix)
    {
        vec3 e1 = v1 - v0;
        vec3 e2 = v2 - v0;
        vec3 P = cross(Direction, e2);
        float a = dot(e1, P);

        if (a > -1E-5f && a < 1E-5f) return false;
        float f = 1 / a;
        vec3 s = Origin - v0;
        float u = f * dot(s, P);

        if (u < 0.0f || u > 1.0f) return false;
        vec3 q = cross(s, e1);
        float v = f * dot(Direction, q);
        if (v < 0.0f || u + v > 1.0f) return false;

        if (f * dot(e2, q) > 0){
            *t_out = f * dot(e2, q);
            *tpix = pix;
            vec3 Normal = cross(e1,e2);
            *normal_out = normalize(Normal);
            return true;
        }
        return false;
    }
};
