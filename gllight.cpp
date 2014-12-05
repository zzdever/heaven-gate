
#include "gllight.h"

int GlLight::lightCount = 0;

GlLight::GlLight()
{
    lightNum = GL_LIGHT0 + (lightCount++);

    SetPosition(1.5*cos(PI*360/LIGHT_COUNT*lightCount/180), 1.3,
                1.5*sin(PI*360/LIGHT_COUNT*lightCount/180), 1.0);

    SetColor(1.0,1.0,1.0,1.0);

    /*
    ambient[0] = 1.0;
    ambient[1] = 1.0;
    ambient[2] = 1.0;
    ambient[3] = 1.0;

    diffuse[0] = 1.0;
    diffuse[1] = 1.0;
    diffuse[2] = 1.0;
    diffuse[3] = 1.0;

    specular[0] = 1.0;
    specular[1] = 1.0;
    specular[2] = 1.0;
    specular[3] = 1.0;
    */

    isOn = true;
}

void GlLight::Switch()
{
    if(isOn){
        glDisable(lightNum);
        isOn = false;
    }
    else{
        glEnable(lightNum);
        isOn = true;
    }
}

void GlLight::DrawLight()
{
    if(!isOn){
        return;
    }
    else{
        glEnable(lightNum);
    }

    glPushMatrix();

    glTranslatef(position[0], position[1], position[2]);

    glPushAttrib(GL_ALL_ATTRIB_BITS);
    //glMaterialfv(GL_FRONT, GL_EMISSION, color);
    glMaterialfv(GL_FRONT, GL_AMBIENT, color);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
    glMaterialfv(GL_FRONT, GL_SPECULAR, color);
    glDisable(GL_TEXTURE_2D);
    glutSolidSphere(0.05, 100, 100);
    glEnable(GL_TEXTURE_2D);
    glPopAttrib();

    glPopMatrix();

}

void GlLight::SetPosition(float x, float y, float z, float w)
{
    position[0] = x;
    position[1] = y;
    position[2] = z;
    position[3] = w;

    glLightfv(lightNum, GL_POSITION, position);

}

void GlLight::MovePosition(float dx, float dy, float dz, float dw)
{
    SetPosition(position[0]+dx, position[1]+dy, position[2]+dz, position[3]+dw);
}


void GlLight::SetColor(float r, float g, float b, float a)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;

    glLightfv(lightNum, GL_AMBIENT, color);
    glLightfv(lightNum, GL_DIFFUSE, color);
    glLightfv(lightNum, GL_SPECULAR, color);
}


/*
void GlLight::SetAmbient(float r, float g, float b, float a)
{
    ambient[0] = r;
    ambient[1] = g;
    ambient[2] = b;
    ambient[3] = a;
}

void GlLight::SetDiffuse(float r, float g, float b, float a)
{
    diffuse[0] = r;
    diffuse[1] = g;
    diffuse[2] = b;
    diffuse[3] = a;
}

void GlLight::SetSpecular(float r, float g, float b, float a)
{
    specular[0] = r;
    specular[1] = g;
    specular[2] = b;
    specular[3] = a;
}
*/
