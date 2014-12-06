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


//void drawprism(){
//    ModelPrism(12);
//}

//void DrawStage(){
//    float radius = 5.0;
//    for(int i=0; i<6; i++){
//        glPushMatrix();
//        glScalef(radius,0.3,radius);
//        glTranslatef(0., i*0.3, 0.);
//        ModelFrustum(8, 0.9);
//        glPopMatrix();
//        radius *= 0.8;
//    }
//}





void GlAll::redraw(GLenum drawMode)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();									// Reset The Current Modelview Matrix

    MoveControl();
    MoveEye();
    collisionDetector.CollisionProcess(objectList, eye);


    if (bWire) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    else { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

    if(drawMode == GL_RENDER){
        // crosshair
        DrawCrosshair();

        // lights
        for(int i=0;i<LIGHT_COUNT;i++){
            lights[i].DrawLight();
        }
    }


    for(int i=0; i<objectList.size(); i++){
        if(objectList.at(i)->GetObjectFrameworkName() == "imaxscreen")
            objectList.at(i)->SetTexture(texImax[static_cast<ImaxScreen*>(objectList.at(i))->GetFrameCount()]);

        objectList.at(i)->Draw(drawMode);
    }


    // global transformation
    glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
    glScalef(0.2, 0.2, 0.2);


    if(drawMode == GL_RENDER){
        DrawSky();
    }


    if (bAnim) fRotate    += 0.5f;

    // NOTE QOpenGLContext::swapBuffers() called with non-exposed window, behavior is undefined
    //glutSwapBuffers();
}


void GlAll::SetupScene()
{
    Girl* girl = new Girl;
    girl->SetPosition(0.,0.,0.);
    girl->SetObjectFrameworkName("girl");
    objectList.push_back(girl);

    MagicCube* magicCube = new MagicCube;
    magicCube->SetPosition(2.0,0.,0.);
    magicCube->SetTexture(texCube);
    magicCube->SetObjectFrameworkName("magiccube");
    objectList.push_back(magicCube);

    RingCouple* ringCouple = new RingCouple;
    ringCouple->SetPosition(0.,0.,5.0);
    ringCouple->SetObjectFrameworkName("ringcouple");
    objectList.push_back(ringCouple);

    ImaxScreen* imaxScreen = new ImaxScreen;
    imaxScreen->SetPosition(-4,0,2);
    imaxScreen->SetScale(5);
    imaxScreen->SetObjectFrameworkName("imaxscreen");
    objectList.push_back(imaxScreen);

    for(int i=0;i<6;i++){
        ModelPrism *poll = new ModelPrism;
        poll->SetSideCount(4);
        poll->SetPosition(10*cos(PI*2/6*i),0,10*sin(PI*2/6*i));
        poll->SetTexture(texPoll[i]);
        poll->SetDimension(1,1,1.5);
        poll->SetObjectFrameworkName("poll");
        objectList.push_back(poll);
    }

    // heaven gate
    {
        int heavenGateZ = 10;
        ModelCube* doorFrame[3];
        for(int i=0;i<3;i++){
            doorFrame[i] = new ModelCube;
            doorFrame[i]->SetObjectFrameworkName("doorframe");
            doorFrame[i]->SetTexture(texBalcony);
            doorFrame[i]->SetScale(3);
            objectList.push_back(doorFrame[i]);
        }

        doorFrame[0]->SetDimension(1.0,0.1,0.1);
        doorFrame[0]->SetPosition(0,0.45,heavenGateZ);
        doorFrame[1]->SetDimension(0.1,0.1,1.0);
        doorFrame[1]->SetPosition(-0.45,0,heavenGateZ);
        doorFrame[2]->SetDimension(0.1,0.1,1.0);
        doorFrame[2]->SetPosition(0.45,0,heavenGateZ);

//        HeavenGate* heavenGate = new HeavenGate;
//        heavenGate->SetPosition(0,0,10);
//        heavenGate->SetEnvelopingDimension(0,0,0);
//        heavenGate->SetObjectFrameworkName("heavengate");
//        objectList.push_back(heavenGate);
    }

    Tablelamp* tablelamp = new Tablelamp;
    tablelamp->SetPosition(0,0,-5);
    tablelamp->SetScale(2);
    tablelamp->SetEnvelopingDimension(2,2,2);
    tablelamp->SetObjectFrameworkName("tablelamp");
    //objectList.push_back(tablelamp);

}


void GlAll::DrawCrosshair()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-wWidth,wWidth,-wHeight,wHeight,-1.0,10);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();


    {
        glLineWidth(1);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH,GL_NICEST);

        glColor4f(0.7, 0.7, 0.7, 0.7);

        glBegin(GL_LINES);
        glVertex2f(-wHeight/50,0.);
        glVertex2f(wHeight/50,0.);
        glEnd();
        glBegin(GL_LINES);
        glVertex2f(0.,-wHeight/50);
        glVertex2f(0.,wHeight/50);
        glEnd();
    }


    // Making sure we can render 3d again
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glPopAttrib();
}



void GlAll::DrawSky()
{
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glBindTexture(GL_TEXTURE_2D, texNightSky); // 设置纹理
    GLUquadricObj *quadricObj;
    quadricObj = gluNewQuadric(); // 绘制球体
    gluQuadricNormals(quadricObj, GL_SMOOTH); // 产生光滑
    gluQuadricTexture(quadricObj, GL_TRUE); // 激活曲面纹理坐标参照
    gluSphere(quadricObj, 100.0f, 32, 16); // 绘制球体
    glPopAttrib();

    return;
}

void GlAll::Draw_Table() // This function draws a triangle with RGB colors
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
    //Draw_Leg();
    glScalef(1, 1, 3);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5, 1, 1.5);
    //Draw_Leg();
    glScalef(1, 1, 3);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5, -1, 1.5);
    //Draw_Leg();
    glScalef(1, 1, 3);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5, -1, 1.5);
    //Draw_Leg();
    glScalef(1, 1, 3);
    glutSolidCube(1.0);
    glPopMatrix();


    if (texBlend) {
        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE0);
        // restore texture attribute TEXTURE0
        glPopAttrib();
    }

}


