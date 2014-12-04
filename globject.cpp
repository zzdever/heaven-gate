#include "globject.h"


int ObjectFramework::objectFrameworkCount = 0;


ObjectFramework::ObjectFramework()
{
    objectFrameworkID = objectFrameworkCount++;

    length = 1.0;
    width = 1.0;
    height = 1.0;
    scaleCoefficient = 1.0;

    position_x = 0.;
    position_y = 0.;
    position_z = 0.;
}


void ObjectFramework::SetDimension(GLfloat p_length, GLfloat p_width, GLfloat p_height)
{
    length = p_length;
    width = p_width;
    height = p_height;

    return;
}

GLfloat* ObjectFramework::GetDimension() const
{
    GLfloat dimension[3];
    dimension[0] = length;
    dimension[1] = width;
    dimension[2] = height;
    return dimension;
}

void ObjectFramework::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
    position_x = x;
    position_y = y;
    position_z = z;
}

void ObjectFramework::MovePosition(GLfloat dx, GLfloat dy, GLfloat dz)
{
    position_x += dx;
    position_y += dy;
    position_z += dz;
}

GLfloat* ObjectFramework::GetPosition() const
{
    GLfloat position[3];
    position[0] = position_x;
    position[1] = position_y;
    position[2] = position_z;
    return position;
}

void ObjectFramework::SetRotation(int rx, int ry, int rz)
{
    rotation_x = rx;
    rotation_y = ry;
    rotation_z = rz;
}

void ObjectFramework::SetDrawEnv(GLenum drawMode)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    //glScalef(scaleCoefficient, scaleCoefficient, scaleCoefficient);
    glScalef(length*scaleCoefficient, width*scaleCoefficient, height*scaleCoefficient);
    glTranslatef(position_x, position_y, position_z);

    glBindTexture(GL_TEXTURE_2D, texture);

    // draw the framework
    if(isSelected){
        glutWireCube(1.0);
    }

    if(drawMode == GL_SELECT)
        glLoadName(objectFrameworkID);
}
void ObjectFramework::UnsetDrawEnv()
{
    glPopAttrib();
    glPopMatrix();
}

