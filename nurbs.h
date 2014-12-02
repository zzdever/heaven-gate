#ifndef NURBS_H
#define NURBS_H

#include <GLUT/GLUT.h>
#include <math.h>

#include "misc.h"


class Nurbs
{
public:
    Nurbs();
    ~Nurbs(){}

    void DrawTableLamp();

private:
    GLfloat spin_x;/*整个图形绕x轴旋转角度*/
    GLfloat spin_y;/*整个图形绕y轴旋转角度*/
    GLfloat spin_z;/*整个图形绕z轴旋转角度*/

    GLUnurbsObj *pNurb_base;/*NURBS对象指针*/
    GLUnurbsObj *pNurb_pipe;
    GLUnurbsObj *pNurb_lamp;



    //台灯底座的控制点
    GLfloat ctrlPoints_base[4][4][3]={
        {{ -3.0f, -3.0f, -3.0f },
            { -4.5f, -2.0f, -3.0f },
            { -4.5f, 2.0f, -3.0f },
            { -3.0f, 3.0f, -3.0f }},

        {{ -1.0f, -4.5f, -3.0f },
            { -1.0f, -1.0f, 1.0f },
            { -1.0f, 1.0f, 1.0f },
            { -1.0f, 4.5f, -3.0f }},

        {{ 1.0f, -4.5f, -3.0f },
            { 1.0f, -1.0f, 1.0f },
            { 1.0f, 1.0f, 1.0f },
            { 1.0f, 4.5f, -3.0f }},

        {{ 3.0f, -3.0f, -3.0f },
            { 4.5f, -2.0f, -3.0f },
            { 4.5f, 2.0f, -3.0f },
            { 3.0f, 3.0f, -3.0f }}};

    //台灯弯曲管的控制点
    GLfloat ctrlPoints_pipe[5][5][3] = {
        {{-4.5f, 0.0f, -1.0f},
            {-4.5f, 0.0f, 3.0f},
            {-3.0f, 0.0f, 6.0f},
            {1.0f, 0.0f, 9.0f},
            {6.0f, 0.0f, 11.0f}},

        {{-3.5f, -2.0f, -1.0f},
            {-3.5f, -2.0f, 4.0f},
            {-2.0f, -2.0f, 7.0f},
            {1.5f, -2.0f, 8.0f},
            {6.0f, -2.0f, 10.0f}},

        {{-2.5f, 0.0f, -1.0f},
            {-2.5f, 0.0f, 4.0f},
            {-1.0f, 0.0f, 6.0f},
            {2.0f, 0.0f, 7.0f},
            {6.0f, 0.0f, 9.0f}},

        {{-3.5f, 2.0f, -1.0f},
            {-3.5f, 2.0f, 4.0f},
            {-2.0f, 2.0f, 7.0f},
            {1.5f, 2.0f, 8.0f},
            {6.0f, 2.0f, 10.0f}},

        {{-4.5f, 0.0f, -1.0f},
            {-4.5f, 0.0f, 3.0f},
            {-3.0f, 0.0f, 6.0f},
            {1.0f, 0.0f, 9.0f},
            {6.0f, 0.0f, 11.0f}},
    };


    //灯罩的控制点
    GLfloat ctrlPoints_lamp[4][6][3] = {
        {{8.0f, -5.0f, 9.0f},
            {8.0f, -3.0f, 9.0f},
            {8.0f, -1.0f, 9.0f},
            {8.0f, 1.0f, 9.0f},
            {8.0f, 3.0f, 9.0f},
            {8.0f, 5.0f, 9.0f}},

        {{9.0f, -7.0f, 9.0f},
            {9.0f, -6.0f, 11.0f},
            {9.05f, -5.0f, 13.0f},
            {9.0f, 5.0f, 13.0f},
            {9.0f, 6.0f, 11.0f},
            {9.0f, 7.0f, 9.0f}},

        {{10.0f, -7.0f, 9.0f},
            {10.0f, -6.0f, 11.0f},
            {10.0f, -5.0f, 13.0f},
            {10.0f, 5.0f, 13.0f},
            {10.0f, 6.0f, 11.0f},
            {10.0f, 7.0f, 9.0f}},

        {{11.0f, -5.0f, 9.0f},
            {11.0f, -3.0f, 9.0f},
            {11.0f, -1.0f, 9.0f},
            {11.0f, 1.0f, 9.0f},
            {11.0f, 3.0f, 9.0f},
            {11.0f, 5.0f, 9.0f}}
    };


    /*u方向的结点序列，u_knot数=u方向控制网格数+u方向阶数*/
    GLfloat u_knots_base[8] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat u_knots_pipe[10] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat u_knots_lamp[8] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    /*v方向的结点序列，v_knot数=v方向控制网格数+v方向阶数*/
    GLfloat v_knots_base[8] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat v_knots_pipe[10] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat v_knots_lamp[12] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};


    void DrawPoints();
    void Draw_base();


};

#endif // NURBS_H
