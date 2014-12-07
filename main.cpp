#include <QApplication>

#include "window.h"
#include <GLUT/glut.h>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;
    window.show();
    return app.exec();
}
