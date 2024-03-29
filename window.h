#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

#include "misc.h"

class GLWidget;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    void setMainLayout(Window *parent);


protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void setCurrentGlWidget();
    void rotateOneStep();

private:
    enum { NumRows = 2, NumColumns = 3 };

    GLWidget *glWidgetMain;
    //GLWidget *glWidgets[NumRows][NumColumns];
    GLWidget *currentGlWidget;
};

#endif
