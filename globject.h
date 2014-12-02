#ifndef GLOBJECT_H
#define GLOBJECT_H


#include "glall.h"


class ObjectFramework
{
public:
    ObjectFramework(){
        length = 1.0;
        width = 1.0;
        height = 1.0;
        scaleCoefficient = 1.0;
    }

    ObjectFramework(GLfloat p_length, GLfloat p_width, GLfloat p_height){
        length = p_length;
        width = p_width;
        height = p_height;
        scaleCoefficient = 1.0;
    }

    ~ObjectFramework() {}

    GLfloat *GetDimension() const{
        GLfloat dimension[3];
        dimension[0] = length;
        dimension[1] = width;
        dimension[2] = height;
        return dimension;
    }

    void SetScale(GLfloat coefficient){ scaleCoefficient = coefficient; }
    GLfloat GetScale(){ return scaleCoefficient; }


    void SetPosition(GLfloat x, GLfloat y, GLfloat z){
        position_x = x;
        position_y = y;
        position_z = z;
    }
    void MovePosition(GLfloat dx, GLfloat dy, GLfloat dz){
        position_x += dx;
        position_y += dy;
        position_z += dz;
    }

    GLfloat* GetPosition() const{
        GLfloat position[3];
        position[0] = position_x;
        position[1] = position_y;
        position[2] = position_z;
        return position;
    }

    void SetRotation(int rx, int ry, int rz){
        rotation_x = rx;
        rotation_y = ry;
        rotation_z = rz;
    }

    void SetTexture(GLuint tex){ texture = tex; }

    GLuint GetTexture() const{ return texture; }

    void SetMaterial(GLenum param, const GLfloat* params);


    void Draw(void (*drawfunc)()) {
        glPushMatrix();
        glPushAttrib(GL_ALL_ATTRIB_BITS);


        glScalef(scaleCoefficient, scaleCoefficient, scaleCoefficient);
        glTranslatef(position_x, position_y, position_z);

        glBindTexture(GL_TEXTURE_2D, texture);

        (*drawfunc)();

        // draw the framework
        glScalef(length, width, height);
        glutWireCube(1.0);

        glPopAttrib();
        glPopMatrix();
    }


    //bool isInFramework(Point3f point);

private:
    // width:z, length:x, height:y
    GLfloat length, width, height;
    GLfloat scaleCoefficient;

    GLfloat position_x, position_y, position_z;

    int rotation_x, rotation_y, rotation_z;

    GLuint texture;

    //GL_AMBIENT、GL_DIFFUSE、GL_SPECULAR
    //GL_SHININESS 0~128
    //GL_EMISSION
    GLfloat materialAmbient[4], materialDiffuse[4], materialSpecular[4];
    GLfloat materialShiness;
    GLfloat materialEmission[4];

};


//class GlObject:public ObjectFramework
//{
//public:
//    GlObject();
//    GlObject(ObjectFramework framework);


//    void Draw(){
//        glutSolidSphere(0.9,100,100);
//    }

//private:
//    GLuint texture;

//};

#endif // GLOBJECT_H
