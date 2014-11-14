#include <QApplication>

#include "window.h"
#include <GLUT/glut.h>


int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(opengl);

    QApplication app(argc, argv);
    Window window;
    window.show();
    return app.exec();
}
