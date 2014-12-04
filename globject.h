#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <string>

#include <GLUT/glut.h>

#include "objfile.h"

#include "misc.h"


class ObjectFramework
{
private:
    static int objectFrameworkCount;

public:
    ObjectFramework();
    ~ObjectFramework() {}

    int GetObjectFrameworkID(){ return objectFrameworkID; }

    void SetObjectFrameworkName(std::string name){ objectFrameworkName = name; }
    std::string GetObjectFrameworkName(){ return objectFrameworkName; }

    void SetDimension(GLfloat p_length, GLfloat p_width, GLfloat p_height);
    Dimension3f GetDimension() const;
    void SetEnvelopingDimension(GLfloat p_length, GLfloat p_width, GLfloat p_height);
    //Dimension3f GetEnvelopingDimension() { }

    void SetScale(GLfloat coefficient){ scaleCoefficient = coefficient; }
    GLfloat GetScale(){ return scaleCoefficient; }

    void SetPosition(GLfloat x, GLfloat y, GLfloat z);
    void MovePosition(GLfloat dx, GLfloat dy, GLfloat dz);

    Point3f GetPosition() const;

    void SetRotation(int drx, int dry, int drz);

    void SetTexture(GLuint tex){ texture = tex; }
    GLuint GetTexture() const{ return texture; }

    void SetMaterial(GLenum param, const GLfloat* params);

    void Select(){ isSelected = true; }
    void Unselect(){ isSelected = false; }

    //bool isInFramework(Point3f point);

    virtual void Draw(GLenum drawMode = GL_RENDER) = 0;

protected:
    int objectFrameworkID;
    std::string objectFrameworkName;

    void SetDrawEnv(GLenum drawMode = GL_RENDER);
    void UnsetDrawEnv();


private:
    bool isSelected;
    // envoloping cube dimension
    GLfloat en_length, en_width, en_height;

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


class Girl:public ObjectFramework
{
public:
    Girl(){
        //Select();
        isFileRead = false;
        SetScale(3.0);
        SetEnvelopingDimension(0.8, 0.2, 1.0);
    }
    ~Girl(){}

    void Draw(GLenum drawMode = GL_RENDER){
        if(isFileRead == false)
            girl.ReadFile(OBJ_FILE_GIRL);

        SetDrawEnv(drawMode);
        glTranslatef(0.,-0.5,-0.05);
        glScalef(0.5,0.5,0.5);
        girl.DrawModel();
        UnsetDrawEnv();
    }

private:
    ObjFileSupport girl;
    bool isFileRead;

};



class ModelCube:public ObjectFramework
{
public:
    ModelCube(){}
    ~ModelCube(){}

    void Draw(GLenum drawMode = GL_RENDER){
        SetDrawEnv(drawMode);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glutSolidCube(1.0);
        glPopMatrix();
        UnsetDrawEnv();
    }
};


class ModelSphere:public ObjectFramework
{
public:
    ModelSphere(){}
    ~ModelSphere(){}

    void Draw(GLenum drawMode = GL_RENDER){
        SetDrawEnv(drawMode);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glutSolidSphere(0.5, 60, 60);
        glPopMatrix();
        UnsetDrawEnv();
    }
};


class ModelCylinder:public ObjectFramework
{
public:
    ModelCylinder(){}
    ~ModelCylinder(){}

    void Draw(GLenum drawMode = GL_RENDER){
        SetDrawEnv(drawMode);
        const int grain = 60;
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glRotatef(-90,1,0,0);
        glTranslatef(0,0,-0.5);
        gluCylinder(gluNewQuadric(), 0.5, 0.5, 1.0, grain, grain);

        // top
        glBegin(GL_POLYGON);
        for(int i=0; i<grain; i++){
            glVertex3f(0.5*cos(PI*2/grain*i), 0.5*sin(PI*2/grain*i), 1.0);
        }
        glEnd();

        // bottom
        glBegin(GL_POLYGON);
        for(int i=0; i<grain; i++){
            glVertex3f(0.5*cos(PI*2/grain*i), 0.5*sin(PI*2/grain*i), 0.);
        }
        glEnd();

        glPopMatrix();
        UnsetDrawEnv();
    }
};

class ModelCone:public ObjectFramework
{
public:
    ModelCone(){}
    ~ModelCone(){}

    void Draw(GLenum drawMode = GL_RENDER){
        SetDrawEnv(drawMode);
        const int grain = 60;
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glRotatef(-90,1,0,0);
        glTranslatef(0,0,-0.5);
        glutSolidCone(0.5, 1.0, grain, grain);

        // bottom
        glBegin(GL_POLYGON);
        for(int i=0; i<grain; i++){
            glVertex3f(0.5*cos(PI*2/grain*i), 0.5*sin(PI*2/grain*i), 0.);
        }
        glEnd();

        glPopMatrix();
        UnsetDrawEnv();
    }
};



class ModelPrism:public ObjectFramework
{
public:
    ModelPrism(){ num = 7;}
    ~ModelPrism(){}

    void SetSideCount(int count){ num = count; }

    void Draw(GLenum drawMode = GL_RENDER){
        SetDrawEnv(drawMode);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        for(int i=0; i<num; i++){
            glBegin(GL_POLYGON);
            glVertex3f(0.5*cos(PI*2/num*i), 0.5, 0.5*sin(PI*2/num*i));
            glVertex3f(0.5*cos(PI*2/num*i), -0.5, 0.5*sin(PI*2/num*i));
            glVertex3f(0.5*cos(PI*2/num*(i+1)), -0.5, 0.5*sin(PI*2/num*(i+1)));
            glVertex3f(0.5*cos(PI*2/num*(i+1)), 0.5, 0.5*sin(PI*2/num*(i+1)));
            glEnd();
        }

        // top
        glBegin(GL_POLYGON);
        for(int i=0; i<num; i++){
            glVertex3f(0.5*cos(PI*2/num*i), 0.5, 0.5*sin(PI*2/num*i));
        }
        glEnd();

        // bottom
        glBegin(GL_POLYGON);
        for(int i=0; i<num; i++){
            glVertex3f(0.5*cos(PI*2/num*i), -0.5, 0.5*sin(PI*2/num*i));
        }
        glEnd();

        glPopMatrix();
        UnsetDrawEnv();
    }

private:
    int num;
};

class ModelFrustum:public ObjectFramework
{
public:
    ModelFrustum(){ num = 7; ratio = 0.5; }
    ~ModelFrustum(){}

    void SetSideCount(int count){ num = count; }
    void SetRatio(GLfloat r){ ratio = r; }

    void Draw(GLenum drawMode = GL_RENDER){
        SetDrawEnv(drawMode);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        if(ratio>1.0) ratio = 1.0;

        // sides
        for(int i=0; i<num; i++){
            glBegin(GL_POLYGON);
            glVertex3f(ratio*0.5*cos(PI*2/num*i), 0.5, ratio*0.5*sin(PI*2/num*i));
            glVertex3f(0.5*cos(PI*2/num*i), -0.5, 0.5*sin(PI*2/num*i));
            glVertex3f(0.5*cos(PI*2/num*(i+1)), -0.5, 0.5*sin(PI*2/num*(i+1)));
            glVertex3f(ratio*0.5*cos(PI*2/num*(i+1)), 0.5, ratio*0.5*sin(PI*2/num*(i+1)));
            glEnd();
        }

        // top
        glBegin(GL_POLYGON);
        for(int i=0; i<num; i++){
            glVertex3f(ratio*0.5*cos(PI*2/num*i), 0.5, ratio*0.5*sin(PI*2/num*i));
        }
        glEnd();

        // bottom
        glBegin(GL_POLYGON);
        for(int i=0; i<num; i++){
            glVertex3f(0.5*cos(PI*2/num*i), -0.5, 0.5*sin(PI*2/num*i));
        }
        glEnd();

        glPopMatrix();
        UnsetDrawEnv();
    }

private:
    int num;
    float ratio;
};




#endif // GLOBJECT_H
