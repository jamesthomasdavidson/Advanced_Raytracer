#include "glm/glm.hpp"
using namespace glm;
#include "glm/gtc/matrix_transform.hpp"
using namespace glm;
#include "plane.h"
#include "lights.h"
#include "sphere.h"
#include "triangle.h"
#include <vector>

std::vector<Object *> ObjectList;
std::vector<Lights *> Light;
vec3 DiffuseColour(255, 128, 128);

#define AIR_REFRACTIVE_INDEX   1.00f
#define GLASS_REFRACTIVE_INDEX 200.0f
#define SHADOW_CONSTANT 0.95f
#define SAMPLE_SIZE 30

void SetColour(Pixel & px, vec3 CalculatedColor)
{
        if (CalculatedColor.x < 0) px.R = 0;
        else if (CalculatedColor.x > 255) px.R = 255;
        else px.R = (unsigned char)CalculatedColor.x;

        if (CalculatedColor.y < 0) px.G = 0;
        else if (CalculatedColor.y > 255) px.G = 255;
        else px.G = (unsigned char)CalculatedColor.y;

        if (CalculatedColor.z < 0) px.B = 0;
        else if (CalculatedColor.z > 255) px.B = 255;
        else px.B = (unsigned char)CalculatedColor.z;

        px.A = 255;
}

void InitializeClosestObjectStruct(ObjectInfo* info)
{
    info->t = 999999;
}

void InitializeObjects()
{
    /*                           Normal   | POP          | Color       */
    Triangle *Roof1       = new Triangle(vec3(0,512,512),vec3(0,0,0),vec3(0,512,0),vec3(10,  25,  40),vec3(0,1,0));//Green
    Triangle *Roof2       = new Triangle(vec3(0,0,512),vec3(0,0,0),vec3(0,512,512),vec3(10,  25, 40),vec3(0,1,0));//Green

    Triangle *BackWall1   = new Triangle(vec3(512,0,512),vec3(0,0,512),vec3(512,512,512),vec3(145, 65,  225),vec3(0,0,-1));//Purple
    Triangle *BackWall2   = new Triangle(vec3(0,0,512),vec3(0,512,512),vec3(512,512,512),vec3(145, 65,  225),vec3(0,0,-1));//Purple

    Triangle *Floor1      = new Triangle(vec3(512,512,0),vec3(512,0,0),vec3(512,512,512),vec3(120,  20,  30),vec3(0,-1,0));//Red
    Triangle *Floor2      = new Triangle(vec3(512,0,512),vec3(512,512,512),vec3(512,0,0),vec3(120,  20,  30),vec3(0,-1,0));//Red

    Triangle *LeftWall1   = new Triangle(vec3(512,0,0),vec3(0,0,0),vec3(512,0,512),vec3(50,  50,  14),vec3(-1,0,0));//yellow
    Triangle *LeftWall2   = new Triangle(vec3(0,0,0),vec3(0,0,512),vec3(512,0,512),vec3(50,  50,  14),vec3(-1,0,0));//yellow

    Triangle *RightWall1  = new Triangle(vec3(0,512,512),vec3(512,512,0),vec3(512,512,512),vec3(10,50,50),vec3(1,0,0));//cyan blue
    Triangle *RightWall2  = new Triangle(vec3(512,512,0),vec3(0,512,512),vec3(0,512,0),vec3(10,  50,  50),vec3(1,0,0));//cyan blue

    Triangle *FrontWall1  = new Triangle(vec3(0,0,512),vec3(0,0,0),vec3(0,512,0),vec3(90,50,50),vec3(1,0,0));//somethin
    Triangle *FrontWall2  = new Triangle(vec3(0,0,512),vec3(0,512,512),vec3(0,512,0),vec3(90,50,50),vec3(1,0,0));//somethin

    Triangle *Light1       = new Triangle(vec3(1,384,384),vec3(1,128,128),vec3(1,384,128),vec3(255,  255,  255),vec3(0,1,0));//WHITE
    Triangle *Light2       = new Triangle(vec3(1,128,384),vec3(1,128,128),vec3(1,384,384),vec3(255,  255,  255),vec3(0,1,0));//WHITE

    Plane *BackWall  = new Plane(0, 0,-1, 256, 256, 512, 60,  10,  14 );//Red
    Plane *FrontWall = new Plane(0, 0, 1, 256, 256,-512, 50, 65,  100); //Something
    Plane *Floor     = new Plane(0, 1, 0, 256, 0,   256, 10,  50,  50);//Cyan Blue
    Plane *Roof      = new Plane(0,-1, 0, 256, 512, 256, 145, 65,  225); //Purple
    Plane *LeftWall  = new Plane(1, 0, 0,   0, 256, 256, 25,  25,  7); //Yellow
    Plane *RightWall = new Plane(-1, 0, 0,512, 256, 256, 100,  250,  50); //Green

    Sphere *Ball1    = new Sphere(400,256,128,20, 200, 10, 100, 0);
    Sphere *Ball2    = new Sphere(256,256,200,100, 235, 160, 95, 1);
    Sphere *Ball3    = new Sphere(400,150,100,80, 235, 160, 95, 2);

    ObjectList.push_back(Ball3);
    ObjectList.push_back(Ball2);
    ObjectList.push_back(Ball1);

//    ObjectList.push_back(RightWall);
//    ObjectList.push_back(LeftWall);
//    ObjectList.push_back(Roof);
//    ObjectList.push_back(Floor);
//    ObjectList.push_back(FrontWall);
//    ObjectList.push_back(BackWall);

    ObjectList.push_back(Light2);
    ObjectList.push_back(Light1);
    ObjectList.push_back(FrontWall2);
    ObjectList.push_back(FrontWall1);
    ObjectList.push_back(RightWall2);
    ObjectList.push_back(RightWall1);
    ObjectList.push_back(LeftWall2);
    ObjectList.push_back(LeftWall1);
    ObjectList.push_back(Floor2);
    ObjectList.push_back(Floor1);
    ObjectList.push_back(BackWall2);
    ObjectList.push_back(BackWall1);
    ObjectList.push_back(Roof2);
    ObjectList.push_back(Roof1);

    //Light.push_back(new Lights(512, 256, 256));
    Light.push_back(new Lights(2, 256, -100));
}

void InitializeJittering(float ypos, float xpos, vec3 DirArray[4], vec3 Camera)
{
    float j = ypos - 0.25f + ((rand()%50)-25)/100.0f;
    float i = xpos - 0.25f + ((rand()%50)-25)/100.0f;
    vec3 PixelPos(j, i, 0);
    DirArray[0] = normalize(PixelPos - Camera);

    j = ypos - 0.25f + ((rand()%50)-25)/100.0f;
    i = xpos + 0.25f + ((rand()%50)-25)/100.0f;
    PixelPos = vec3(j, i, 0);
    DirArray[1] = normalize(PixelPos - Camera);

    j = ypos + 0.25f + ((rand()%50)-25)/100.0f;
    i = xpos + 0.25f + ((rand()%50)-25)/100.0f;
    PixelPos = vec3(j, i, 0);
    DirArray[2] = normalize(PixelPos - Camera);

    j = ypos + 0.25f + ((rand()%50)-25)/100.0f;
    i = xpos - 0.25f + ((rand()%50)-25)/100.0f;
    PixelPos = vec3(j, i, 0);
    DirArray[3] = normalize(PixelPos - Camera);
}

vec3 GetLightOffset(int i)
{
    int j = i/4;
    j = j%16;
    return(vec3(0, (i+(rand()%64) - 32)*1.0f, (j+(rand()%64)-32)*1.0f));
}

void InitializeLights(vec3 Intersection, int i)
{
    for(int k = 0; k < Light.size(); k++)
    {
        Light[k]->LightVector = Light[k]->Light + GetLightOffset(i) - Intersection;
        Light[k]->Length = length(Light[k]->LightVector);
        Light[k]->LightVector = normalize(Light[k]->LightVector);
    }
}

Object* GetClosestObject(ObjectInfo* info, vec3 Camera, vec3 Direction)
{
    Object* Ref = NULL;
    for(int k = 0; k < ObjectList.size(); ++k)
    {
        float t;
        float c = 0;
        vec3 Normal;
        vec3 TempPixel;

        Camera = Camera + 1/50.0f*normalize(Direction);

        if(ObjectList[k]->Intersect(Camera, Direction, &t, &Normal, &TempPixel))
        {
            if((info->t > t))
            {
                info->Colour = TempPixel;
                Ref = ObjectList[k];
                info->t = t;
                info->Normal = Normal;
            }
        }
    }
    return Ref;
}

Object* GetSameObject(ObjectInfo* info, vec3 Camera, vec3 Direction, Object* Ref1)
{
    Object* Ref = NULL;

    float t;
    vec3 Normal;
    vec3 TempPixel;

    Camera = Camera + 1/100000.0f*normalize(Direction);

    if(Ref1->Intersect(Camera, Direction, &t, &Normal, &TempPixel))
    {
        info->Colour = TempPixel;
        Ref = Ref1;
        info->t = t;
        info->Normal = Normal;

    }
    return Ref;
}

vec3 GetReflectionRay(vec3 Normal, vec3 IncomingRay)
{
    Normal = normalize(Normal);
    return IncomingRay - 2.0f*(dot(Normal, IncomingRay)*Normal);
}

vec3 GetRefractedRay(vec3 Normal, vec3 *Intersection, vec3 IncomingRay, Object* Ref1)
{
    Normal = normalize(Normal);

    vec3 RefractedRay = (AIR_REFRACTIVE_INDEX*(IncomingRay - Normal*(dot(IncomingRay, Normal))))
                                            /GLASS_REFRACTIVE_INDEX;

    float Root = (1 - (AIR_REFRACTIVE_INDEX*AIR_REFRACTIVE_INDEX*(1 - (dot(IncomingRay, Normal))*(dot(IncomingRay, Normal))))
    /(GLASS_REFRACTIVE_INDEX*GLASS_REFRACTIVE_INDEX));
    if(Root < 0)
    {
         cout<<"root is negative"<<endl;
    }
    RefractedRay = RefractedRay - Normal*(sqrt(Root));
    RefractedRay = normalize(RefractedRay);

    Object* Ref;
    ObjectInfo ClosestObject;
    Ref = GetSameObject(&ClosestObject,*Intersection, RefractedRay, Ref1);
    *Intersection = *Intersection + ClosestObject.t*RefractedRay;
    Normal = normalize(ClosestObject.Normal);


    vec3 ReturnRay = (GLASS_REFRACTIVE_INDEX*(RefractedRay - Normal*(dot(RefractedRay, Normal))))
                                            /AIR_REFRACTIVE_INDEX
      - Normal*(sqrt(1 - (GLASS_REFRACTIVE_INDEX*GLASS_REFRACTIVE_INDEX*(1 - (dot(RefractedRay, Normal))*(dot(RefractedRay, Normal))))
                                 /(AIR_REFRACTIVE_INDEX*AIR_REFRACTIVE_INDEX)));

    return ReturnRay;
}

vec3 GetPixelDiffuseColour(vec3 DiffuseColour, vec3 normal_min)
{
    vec3 PixelDiffuseColour(0,0,0);
    float DiffuseTerm = 0;
    int k;
    for(k = 0; k < Light.size(); k++)
    {
        DiffuseTerm += dot(Light[k]->LightVector, normal_min);
    }
    DiffuseTerm = DiffuseTerm/k;
    if (DiffuseTerm > 0)
    {
        PixelDiffuseColour = DiffuseTerm*DiffuseColour;
    }
    return PixelDiffuseColour;
}

vec3 ApplyShadows(vec3 PixelColour, int NumShadows)
{
    for(int q = 0; q < NumShadows; q++)
    {
        PixelColour = SHADOW_CONSTANT*PixelColour;
    }
    return PixelColour;
}

int NumberOfShadows(Object* Ref, vec3 Intersection)
{
    float NumShadows = 0;
    for(int i = 0; i < SAMPLE_SIZE; i++)
    {
        InitializeLights(Intersection, i);
        for(int k = 0; k < ObjectList.size(); ++k)
        {
            if(ObjectList[k] == Ref)
            {
                continue;
            }
            float t;
            vec3 Normal;
            vec3 TempPixel;
            bool i;

            for(int p = 0; p < Light.size(); p++)
            {
                Light[p]->eVec = Intersection + (2.0f*(Light[p]->LightVector));
                if((i = ObjectList[k]->Intersect(Light[p]->eVec, Light[p]->LightVector, &t, &Normal, &TempPixel)) && (t < Light[p]->Length) /*&& ObjectList[k]->SurfaceType != 2*/)
                {
                    Light[p]->IsBlocked = true;
                    if(ObjectList[k]->SurfaceType != 2)
                    {
                        NumShadows++;
                    }else{
                        NumShadows += 0.7f;
                    }
                    continue;
                }
            }
        }
        for(int k = 0; k < Light.size(); k++)
        {
            Light[k]->IsBlocked = false;
        }
    }

    return NumShadows;
}

vec3 getPixelColour(vec3 Camera, vec3 Direction)
{
    /* Init Ref & ObjectInfo struct */
    Object* Ref;
    ObjectInfo ClosestObject;
    InitializeClosestObjectStruct(&ClosestObject);

    /* Get Closest Object using Camera and Direction */
    Ref = GetClosestObject(&ClosestObject,Camera, Direction);

    vec3 PixelColour;
    if(Ref)
    {
        vec3 Intersection = Camera + ClosestObject.t*Direction;
        if(Ref->SurfaceType == 1)
        {
            vec3 ReflectionRay = GetReflectionRay(ClosestObject.Normal, Direction);
            return getPixelColour(Intersection, ReflectionRay);
        }else if(Ref->SurfaceType == 2){
            vec3 RefractedRay = GetRefractedRay(ClosestObject.Normal, &Intersection, Direction, Ref);
            return getPixelColour(Intersection, RefractedRay);
        }
        PixelColour = ClosestObject.Colour;
        PixelColour += GetPixelDiffuseColour(DiffuseColour, ClosestObject.Normal);
        int NumShadows = NumberOfShadows(Ref, Intersection);
        PixelColour = ApplyShadows(PixelColour, NumShadows);
    }else{
        PixelColour = vec3(0,0,0);
    }
    return PixelColour;
}

void RayTrace(Image * pImage)
{

    InitializeObjects();
    vec3 Camera(256,256, -400);
    vec3 Direction;
    Pixel px;

    /*These light colours will give the sphere a red appearance*/

    for(int i = 0; i < 512; ++i)
        for(int j = 0; j < 512; ++j){
            /*Step 1. Set up the ray*/
            vec3 PixelPosition(j, i, 0);
            vec3 PixelColour(0,0,0);
            vec3 AA[4];
            Direction = PixelPosition - Camera;
            Direction = normalize(Direction);
            InitializeJittering(j, i, AA, Camera);

            for(int i = 0; i < 4; i++)
            {
                PixelColour += getPixelColour(Camera, AA[i]);
            }
            PixelColour = PixelColour/4.0f;
            SetColour(px, PixelColour);
            (*pImage)(i,j) = px;
        }
}
