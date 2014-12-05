#include "misc.h"

#include <GLUT/glut.h>

// drawBox(), glutWireCube() and glutSolidCube() are
// functions from the GLUT library(glut_shapes.c).
// They are modified for texture binding
void drawBox(GLfloat size, GLenum type)
{
    //======================
    // use to set multi-texture
    bool texBlend = false;
    //======================

    static GLfloat tex[2][2][2] =
    {
        { {0, 0},
            {1, 0}},
        { {0, 1},
            {1, 1}}
    };

    static GLfloat n[6][3] =
    {
        {-1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, -1.0}
    };
    static GLint faces[6][4] =
    {
        {0, 1, 2, 3},
        {3, 2, 6, 7},
        {7, 6, 5, 4},
        {4, 5, 1, 0},
        {5, 6, 2, 1},
        {7, 4, 0, 3}
    };
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;



    for (i = 5; i >= 0; i--) {
        glBegin(type);

        glNormal3fv(&n[i][0]);

        // when texture blend is enabled, use glMultiTexCoord2
        if(texBlend){
            glMultiTexCoord2f(GL_TEXTURE0,0.0,0.0);
            glMultiTexCoord2f(GL_TEXTURE1,0.0,0.0);
        }
        else{
            glTexCoord2f(tex[0][0][0], tex[0][0][1]);
        }
        glVertex3fv(&v[faces[i][0]][0]);

        if (texBlend) {
            glMultiTexCoord2f(GL_TEXTURE0,1.0,0.0);
            glMultiTexCoord2f(GL_TEXTURE1,1.0,0.0);
        }
        else{
            glTexCoord2f(tex[0][1][0], tex[0][1][1]);
        }
        glVertex3fv(&v[faces[i][1]][0]);

        if(texBlend){
            glMultiTexCoord2f(GL_TEXTURE0,1.0,1.0);
            glMultiTexCoord2f(GL_TEXTURE1,1.0,1.0);
        }
        else{
            glTexCoord2f(tex[1][1][0], tex[1][1][1]);
        }
        glVertex3fv(&v[faces[i][2]][0]);

        if (texBlend) {
            glMultiTexCoord2f(GL_TEXTURE0,0.0,1.0);
            glMultiTexCoord2f(GL_TEXTURE1,0.0,1.0);
        }
        else{
            glTexCoord2f(tex[1][0][0], tex[1][0][1]);
        }
        glVertex3fv(&v[faces[i][3]][0]);

        glEnd();
    }
}


// override the glutSolidCube() function in GLUT library
void glutSolidCube(GLdouble size)
{
    drawBox(size, GL_QUADS);
}
