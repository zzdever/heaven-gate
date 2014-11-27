#ifndef GLOBJECT_H
#define GLOBJECT_H


#include "glall.h"

class ObjectFramework
{
public:
    ObjectFramework(){
        m_width = 1.0;
        m_length = 1.0;
        m_height = 1.0;
    }

    ObjectFramework(float width, float length, float height){
        m_width = width;
        m_length = length;
        m_height = height;
    }

    ~ObjectFramework();


    bool isInFramework(Point3f point);

private:
    float m_width, m_length, m_height;

    float height;
};


class GlObject:public ObjectFramework
{
public:
    GlObject();
    GlObject(ObjectFramework framework);


    void Draw(){
        glutSolidSphere(0.9,100,100);
    }

private:
    GLuint texture;

};

#endif // GLOBJECT_H
