#ifndef MISC_H
#define MISC_H


#include <iostream>
#include <math.h>


#define TEST 1

#define DEBUG(MESSAGE) {if(TEST){std::cout<<MESSAGE<<std::endl;}}
#define DEBUGS(MESSAGE) {if(TEST){std::cout<<MESSAGE;}}


#define PI 3.1415926


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




#endif // MISC_H
