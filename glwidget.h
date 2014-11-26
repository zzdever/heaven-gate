#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QKeyEvent>

#include <QtWidgets>
#include <QGLWidget>
#include <GLUT/glut.h>

#include "misc.h"


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


};

#endif
