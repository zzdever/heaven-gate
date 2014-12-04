#include "nurbs.h"


Nurbs::Nurbs(){

    spin_x = 0;/*整个图形绕x轴旋转角度*/
    spin_y = 0;/*整个图形绕y轴旋转角度*/
    spin_z = 0;/*整个图形绕z轴旋转角度*/

    drawlist = -1;
}


void Nurbs::Draw_base()
{
    glScaled(1, 1, 2);
    glutSolidCube(1.0);
}

/*显示NURB控制点*/
void Nurbs::DrawPoints(void)
{
    int i,j;

    glPointSize(5.0f);
    glColor3ub(255,0,0);/*红色*/

    glBegin(GL_POINTS);
    for(i = 0; i < 5; i++)
        for(j = 0; j < 7; j++)
            glVertex3fv(ctrlPoints_lamp[i][j]);
    glEnd();
}






//用于绘制台灯
GLint Nurbs::GenDrawList()
{
    GLint lid = 0;
    /*
    lid = glGenLists(1);
    glNewList(lid, GL_COMPILE);
    */

    glPushAttrib(GL_ALL_ATTRIB_BITS);

    //glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glScalef(0.05, 0.05, 0.05);

    /*特殊键控制网格旋转*/
    glRotatef(spin_x, 1.0, 0.0, 0.0);//绕x轴旋转
    glRotatef(spin_y, 0.0, 1.0, 0.0);//绕y轴旋转
    glRotatef(spin_z, 0.0, 0.0, 1.0);//绕z轴旋转

    gluBeginSurface(pNurb_base);
    float mat_diffuse_base[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float mat_specular_base[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_base);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_base);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,50);
    glColor3ub(0,255,20);
    gluNurbsSurface(pNurb_base, 8, u_knots_base, 8, v_knots_base, 4 * 3, 3, &ctrlPoints_base[0][0][0], 4, 4, GL_MAP2_VERTEX_3);//[4][4][3]
    gluEndSurface(pNurb_base);
    glPopMatrix();

    gluBeginSurface(pNurb_lamp);
    float mat_diffuse_lamp[] = {0.0f, 1.0f, 0.2f, 1.0f};
    float mat_specular_lamp[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_lamp);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_lamp);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,50);
    glColor3ub(0,255,20);
    gluNurbsSurface(pNurb_lamp, 8, u_knots_lamp, 12, v_knots_lamp, 6 * 3, 3, &ctrlPoints_lamp[0][0][0], 4, 6, GL_MAP2_VERTEX_3);//[4][6][3]
    gluEndSurface(pNurb_lamp);
    glPopMatrix();

    gluBeginSurface(pNurb_pipe);
    float mat_diffuse_pipe[] = {0.7f, 0.7f, 0.7f, 1.0f};
    float mat_specular_pipe[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_pipe);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular_pipe);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,50);
    glColor3ub(192,192,192);
    gluNurbsSurface(pNurb_pipe, 10, u_knots_pipe, 10, v_knots_pipe, 5 * 3, 3, &ctrlPoints_pipe[0][0][0], 5, 5, GL_MAP2_VERTEX_3);//[5][5][3]
    gluEndSurface(pNurb_pipe);
    glPopMatrix();

    float mat_diffuse_cube[] = {0.0f, 1.0f, 0.2f, 1.0f};
    float mat_specular_cube[] = {0.8f, 0.8f, 0.8f, 1.0f};
    glPushMatrix();
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse_cube);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_cube);
    glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,50);
    glTranslatef(-4, 0, -2);
    glRotatef(90, 0, 0, 1);
    glColor3ub(0,255,20);
    Draw_base();
    glPopMatrix();

    float R = 4.2;
    double n = 50;
    glPushMatrix();
    glColor3ub(0,255,20);
    glTranslatef(0, 0, -3);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0, 0.0);
    for (int i = 0; i < 50; i++) {
        glVertex2f(R * cos(2 * PI / n * i), R * sin(2 * PI / n * i));
    }
    glEnd();
    glPopMatrix();

    R = 1;
    glPushMatrix();
    glColor3ub(0,255,20);
    glTranslatef(6, 0, 10);
    glRotatef(90, 0, 1, 0);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0, 0.0);
    for (int i = 0; i < 50; i++) {
        glVertex2f(R * cos(2 * PI / n * i), R * sin(2 * PI / n * i));
    }
    glEnd();
    glPopMatrix();

    GLUquadricObj *cylinder;
    cylinder = gluNewQuadric();
    glPushMatrix();
    glTranslatef(6, 0, 10);
    glRotatef(90, 0, 1, 0);
    gluCylinder(cylinder, 1.0f, 1.0f, 2.6f, 32, 32);
    glPopMatrix();

    //    DrawPoints();
    glPopMatrix();/*重置模型视图矩阵*/

    glPopAttrib();

    /*
    glEndList();
    */
    return lid;
}


void Nurbs::DrawTableLamp()
{
    /*
    if(drawlist<0){
        drawlist = GenDrawList();
    }
    */
    GenDrawList();
    //glCallList(drawlist);

    return;
}



void Nurbs::SetupRC()
{
    /*开启颜色跟踪*/
    glEnable(GL_COLOR_MATERIAL);
    /*自动生成法线*/
    glEnable(GL_AUTO_NORMAL);

    /*设置Nurbs对象*/
    pNurb_base = gluNewNurbsRenderer();
    pNurb_pipe = gluNewNurbsRenderer();
    pNurb_lamp = gluNewNurbsRenderer();


    /*设置NURBS的属性*/
    gluNurbsProperty(pNurb_base, GLU_SAMPLING_TOLERANCE, 25.0f);/*设置采样容差，定义线框的精细程度*/
    gluNurbsProperty(pNurb_pipe, GLU_SAMPLING_TOLERANCE, 25.0f);/*设置采样容差，定义线框的精细程度*/
    gluNurbsProperty(pNurb_lamp, GLU_SAMPLING_TOLERANCE, 25.0f);/*设置采样容差，定义线框的精细程度*/
}



/*NURBS错误回调函数*/
//void CALLBACK NurbsErrorHandler(GLenum nErrorCode)
//{
//    char cMessage[64];

//    /*提取错误信息*/
//    strcpy(cMessage,"NURBS error occured: ");
//    strcat(cMessage, (char *)gluErrorString(nErrorCode));

//    /*显示错误信息*/
//    glutSetWindowTitle(cMessage);
//}
