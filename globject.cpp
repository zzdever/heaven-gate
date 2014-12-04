#include "globject.h"


int ObjectFramework::objectFrameworkCount = 100; // count from 100


ObjectFramework::ObjectFramework()
{
    objectFrameworkID = objectFrameworkCount++;
    objectFrameworkName = "";

    length = 1.0;
    width = 1.0;
    height = 1.0;
    scaleCoefficient = 1.0;

    position_x = 0.;
    position_y = 0.;
    position_z = 0.;

    rotation_x = 0.;
    rotation_y = 0.;
    rotation_z = 0.;

    en_length = 1.0;
    en_width = 1.0;
    en_height = 1.0;
}

void ObjectFramework::SetEnvelopingDimension(GLfloat p_length, GLfloat p_width, GLfloat p_height){
    en_length = p_length;
    en_width = p_width;
    en_height = p_height;
}

void ObjectFramework::SetDimension(GLfloat p_length, GLfloat p_width, GLfloat p_height)
{
    length = p_length;
    width = p_width;
    height = p_height;

    return;
}


Dimension3f ObjectFramework::GetDimension() const
{
    Dimension3f dimension;
    dimension.length = length;
    dimension.width = width;
    dimension.height = height;
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


Point3f ObjectFramework::GetPosition() const
{
    Point3f point;
    point.x = position_x;
    point.y = position_y;
    point.z = position_z;
    return point;
}


void ObjectFramework::SetRotation(int drx, int dry, int drz)
{
    drx == 0 ? : rotation_x += drx;
    dry == 0 ? : rotation_y += dry;
    drz == 0 ? : rotation_z += drz;
}

void ObjectFramework::SetDrawEnv(GLenum drawMode)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);


    // move
    glTranslatef(position_x, position_y, position_z);

    // rotate
    glRotatef(rotation_x,1.0,0.,0.);
    glRotatef(rotation_y,0.,1.0,0.);
    glRotatef(rotation_z,0.,0.,1.0);


    //scale
    glScalef(length*scaleCoefficient, height*scaleCoefficient, width*scaleCoefficient);

    // texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // draw the framework
    if(isSelected){
        glPushMatrix();
        glScalef(en_length+0.05, en_height+0.05, en_width+0.05);
        glutWireCube(1.0);
        glPopMatrix();
    }

    if(drawMode == GL_SELECT)
        glLoadName(objectFrameworkID);
}
void ObjectFramework::UnsetDrawEnv()
{
    glPopAttrib();
    glPopMatrix();
}

