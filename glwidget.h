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

    // size
    QSize minimumSizeHint() const;
    QSize sizeHint() const;
    void rotateBy(int xAngle, int yAngle, int zAngle);
    void setClearColor(const QColor &color);

signals:
    void clicked();

public slots:
    // key press event
    void keyPress(QKeyEvent *event);


protected:
    // opengl functions
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    // mouse events
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

    void changeTexture(int value){
        glAll.change_texture(value);
    }
    void moveX(int value){
        if(value == 0)
            return;
        static_cast<QSlider*>(sender())->setValue(0);
        glAll.move(value,0,0);
    }
    void moveY(int value){
        if(value == 0)
            return;
        static_cast<QSlider*>(sender())->setValue(0);
        glAll.move(0,value,0);
    }
    void moveZ(int value){
        if(value == 0)
            return;
        static_cast<QSlider*>(sender())->setValue(0);
        glAll.move(0,0,value);
    }
    void rotateX(int value){
        if(value == 0)
            return;
        static_cast<QSlider*>(sender())->setValue(0);
        glAll.rotate(value,0,0);
    }
    void rotateY(int value){
        if(value == 0)
            return;
        static_cast<QSlider*>(sender())->setValue(0);
        glAll.rotate(0,value,0);
    }
    void rotateZ(int value){
        if(value == 0)
            return;
        static_cast<QSlider*>(sender())->setValue(0);
        glAll.rotate(0,0,value);
    }
    void scale(int value){
        glAll.scale(value);
    }



};

#endif
