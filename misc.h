#ifndef MISC_H
#define MISC_H


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
#define CRACK "/Users/ying/Crack.bmp"
#define SPOT "/Users/ying/Spot.bmp"
#define MONET "/Users/ying/Monet.bmp"
#define NIGHTSKY "/Users/ying/starskyrecur.bmp"

#define OBJ_FILE_GIRL "/Users/ying/Documents/DEV/qt_prj/gl/girl.obj"

//==========================================


#define PI 3.1415926

#define LIGHT_COUNT 1


struct Point3f{
    float x;
    float y;
    float z;
};

enum ObjectType{
    CUBE = 1,
    SPHERE = 2,
    COLUMN = 3,
    CONE = 4,

};

enum LightParam{
    Ambient = 0,
    Diffuse = 1,
    Specular = 2,
    Position = 3
};



#endif // MISC_H
