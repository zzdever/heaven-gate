#ifndef GLLIGHT_H
#define GLLIGHT_H

#include <GLUT/glut.h>

#include "misc.h"

class GlLight
{
public:
    static int lightCount;
    GlLight();
    ~GlLight(){}

    void Switch();
    void DrawLight();
    void SetPosition(float x, float y, float z, float w=1.0);
    void MovePosition(float dx, float dy, float dz, float dw=0.);
    void SetColor(float r, float g, float b, float a);

    /*
    void SetAmbient(float r, float g, float b, float a);
    void SetDiffuse(float r, float g, float b, float a);
    void SetSpecular(float r, float g, float b, float a);
    */


private:
    int lightNum;   //GL_LIGHT*
    bool isOn;

    float position[4];

    float color[4];

    //GL_AMBIENT、GL_DIFFUSE、GL_SPECULAR
    /*
    float ambient[4];
    float diffuse[4];
    float specular[4];
    */


};

#endif // GLLIGHT_H
