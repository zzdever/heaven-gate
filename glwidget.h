#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QKeyEvent>

#include <QtWidgets>
#include <QGLWidget>
#include <GLUT/glut.h>

#include "misc.h"
#include "glall.h"


class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = 0, QGLWidget *shareWidget = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void rotateBy(int xAngle, int yAngle, int zAngle);
    void setClearColor(const QColor &color);

signals:
    void clicked();

public slots:
    void keyPress(QKeyEvent *event);


protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    void makeObject();

    QColor clearColor;
    QPoint lastPos;
    int xRot;
    int yRot;
    int zRot;
    GLuint textures[6];
    QVector<QVector3D> vertices;
    QVector<QVector2D> texCoords;

    bool isGLWidgetFocued;

    GlAll glAll;

    LightParam lightParam;
    int lightR;
    int lightG;
    int lightB;
    int lightA;

    void changeLightDriver();

public slots:
    void SwitchLight(){
        glAll.lights[glAll.lightUnderControl].Switch();
    }
    void SwitchLightNum(int num){
        glAll.lightUnderControl = num;
    }
    /*
    void SwitchLightParam(LightParam param){
        lightParam = param;
    }
    */
    void changeR(int value){
        lightR = value;
        changeLightDriver();
    }
    void changeG(int value){
        lightG = value;
        changeLightDriver();
    }
    void changeB(int value){
        lightB = value;
        changeLightDriver();
    }
    void changeA(int value){
        lightA = value;
        changeLightDriver();
    }



};

#endif
