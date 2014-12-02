#include "misc.h"
#include "glall.h"






void setMatirial(const GLfloat mat_diffuse[4], GLfloat mat_shininess)
{
    static const GLfloat mat_specular[] = {0.0f, 0.0f, 0.0f, 1.0f};
    static const GLfloat mat_emission[] = {0.0f, 0.0f, 0.0f, 1.0f};

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

    glMaterialf (GL_FRONT, GL_SHININESS, mat_shininess);

}


void DrawCube(){
    glutSolidCube(1.0);
}

void DrawSphere(){
    glutSolidSphere(0.5, 60, 60);
}

void DrawCylinder(){
    glPushMatrix();
    glRotatef(-90,1,0,0);
    glTranslatef(0,0,-0.5);
    gluCylinder(gluNewQuadric(), 0.5, 0.5, 1.0, 60, 60);
    glPopMatrix();
}

void DrawCone2(){
    glPushMatrix();
    glRotatef(-90,1,0,0);
    glTranslatef(0,0,-0.5);
    glutSolidCone(0.5, 1.0, 60, 60);
    glPopMatrix();
}

void GlAll::redraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();									// Reset The Current Modelview Matrix

    MoveControl();
    MoveEye();

    if (bWire) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }


    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);


    for(int i=0;i<LIGHT_COUNT;i++){
        lights[i].DrawLight();
    }


    tableLamp.DrawTableLamp();

    return;

    ObjectFramework cube(1,1,1);
    cube.SetPosition(0,0,2);
    cube.SetScale(0.5);
    cube.SetTexture(texCrack);
    cube.Draw(DrawCube);

    ObjectFramework cylinder(1,1,1);
    cylinder.SetPosition(-2,0,2);
    cylinder.Draw(DrawCylinder);

    ObjectFramework cone(1,1,1);
    cone.SetPosition(2,0,2);
    cone.SetScale(1.5);
    cone.Draw(DrawCone2);


    objfile.DrawModel();



    //	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
    glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
    glRotatef(-90, 1, 0, 0);
    glScalef(0.2, 0.2, 0.2);


    glPushAttrib(GL_ALL_ATTRIB_BITS);   // save all attributes
    Draw_Triangle();						// Draw triangle



    //DrawCone(0,0,0);
    //glLoadIdentity();
    //glutSolidSphere(0.9, 300, 300);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glBindTexture(GL_TEXTURE_2D, texNightSky); // 设置纹理
    GLUquadricObj *quadricObj;
    quadricObj = gluNewQuadric(); // 绘制球体
    gluQuadricNormals(quadricObj, GL_SMOOTH); // 产生光滑
    gluQuadricTexture(quadricObj, GL_TRUE); // 激活曲面纹理坐标参照
    gluSphere(quadricObj, 100.0f, 32, 16); // 绘制球体



    glPopAttrib();  // restore all attributes

    if (bAnim) fRotate    += 0.5f;

    // NOTE QOpenGLContext::swapBuffers() called with non-exposed window, behavior is undefined
    //glutSwapBuffers();
}









// drawBox(), glutWireCube() and glutSolidCube() are
// functions from the GLUT library(glut_shapes.c).
// They are modified for texture binding
void GlAll::drawBox(GLfloat size, GLenum type)
{
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
void GlAll::glutSolidCube(GLdouble size)
{
    drawBox(size, GL_QUADS);
}


void GlAll::DrawCone(float x, float y, float z)
{
    glutSolidCone(0.5, 0.7, 10, 10);
}


void GlAll::DrawSky()
{


    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    //glCullFace(GL_BACK);

    glPushMatrix();
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, texNightSky);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);


    GLfloat xequalzero[] = {1.0, 0.0, 0.0, 0.0};


    glTexGenfv(GL_S, GL_OBJECT_PLANE, xequalzero);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, xequalzero);



    glEnable(GL_TEXTURE_GEN_S);//启用s坐标的纹理生成
    glEnable(GL_TEXTURE_GEN_T);//启用s坐标的纹理生成

    glNormal3f(0.f, 0.f, 0.f);
    //glutSolidSphere(100, 100, 100);
    gluSphere(gluNewQuadric(), 0.8, 100, 100);

    glPopMatrix();
    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);

    return;
}

void GlAll::Draw_Triangle() // This function draws a triangle with RGB colors
{
    glEnable(GL_TEXTURE_2D);
    if (texCus) {
        glBindTexture(GL_TEXTURE_2D, texCustom);    // use custom texture
    }
    else{
        glBindTexture(GL_TEXTURE_2D, texMonet);     // use Monet texture
    }
    glPushMatrix();
    glTranslatef(0, 0, 4+1);
    glRotatef(90, 1, 0, 0);
    glutSolidTeapot(1);
    glPopMatrix();


    glBindTexture(GL_TEXTURE_2D, texCrack);     // use Crack texture
    if(texBlend) {
        glActiveTexture(GL_TEXTURE0);
        // save texture attribute TEXTURE0
        glPushAttrib(GL_TEXTURE_BIT);

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);    // set to modulate texture


        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texSpot);

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);    // set to modulate texture

    }


    glPushMatrix();
    glTranslatef(0, 0, 3.5);
    glScalef(5, 4, 1);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5, 1, 1.5);
    Draw_Leg();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5, 1, 1.5);
    Draw_Leg();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5, -1, 1.5);
    Draw_Leg();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5, -1, 1.5);
    Draw_Leg();
    glPopMatrix();


    if (texBlend) {
        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE0);
        // restore texture attribute TEXTURE0
        glPopAttrib();
    }


}

void GlAll::Draw_Leg()
{
    glScalef(1, 1, 3);
    glutSolidCube(1.0);
}


