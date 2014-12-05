#ifndef MISC_H
#define MISC_H

#include <QString>
#include <QDebug>

#include <iostream>
#include <math.h>

#include <GLUT/glut.h>


#define TEST 1

#define DEBUG(MESSAGE) {if(TEST){std::cout<<MESSAGE<<std::endl;}}
#define DEBUGS(MESSAGE) {if(TEST){std::cout<<MESSAGE;}}


//==========================================
// File path definition
// You may need to change this to compile
#define TEXTUREROOT "/Users/ying/Documents/DEV/qt_prj/gl/images"

#define CRACK (QString(TEXTUREROOT)+QString("/Crack.bmp")).toStdString().c_str()
#define SPOT (QString(TEXTUREROOT)+QString("/Spot.bmp")).toStdString().c_str()
#define MONET (QString(TEXTUREROOT)+QString("/Monet.bmp")).toStdString().c_str()
#define NIGHTSKY (QString(TEXTUREROOT)+QString("/starskyrecur.bmp")).toStdString().c_str()
#define BALCONY (QString(TEXTUREROOT)+QString("/balcony.bmp")).toStdString().c_str()
#define CUBE (QString(TEXTUREROOT)+QString("/texcube.bmp")).toStdString().c_str()

#define IMAX (QString(TEXTUREROOT)+QString("/teximax/")).toStdString().c_str()
#define IMAXFRAME 470

#define POLL (QString(TEXTUREROOT)+QString("/texpoll/")).toStdString().c_str()
#define POLLCOUNT 6

#define OBJ_FILE_GIRL (QString(TEXTUREROOT)+QString("/girl.obj")).toStdString().c_str()

//==========================================


#define PI 3.1415926

#define LIGHT_COUNT 1


struct Point3f{
    GLfloat x;
    GLfloat y;
    GLfloat z;
};

struct Dimension3f{
    GLfloat length;
    GLfloat width;
    GLfloat height;
};


enum LightParam{
    Ambient = 0,
    Diffuse = 1,
    Specular = 2,
    Position = 3
};


// override the glutSolidCube() function in GLUT library
void glutSolidCube(GLdouble size);


#endif // MISC_H
