#include <QtWidgets>
#include <QtOpenGL>
#include <QMatrix>
#include <QThread>

#include "misc.h"
#include "glall.h"
#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent, QGLWidget *shareWidget)
    : QGLWidget(parent, shareWidget)
{
    clearColor = Qt::black;
    xRot = 0;
    yRot = 0;
    zRot = 0;

    isGLWidgetFocued = false;

    this->setMouseTracking(false);
}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(600, 600);
}

void GLWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
    xRot += xAngle;
    yRot += yAngle;
    zRot += zAngle;
    updateGL();
}

void GLWidget::setClearColor(const QColor &color)
{
    clearColor = color;
    updateGL();
}

void GLWidget::initializeGL()
{
    //makeObject();

    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);


    glAll.glFunctionInit();

}


void GLWidget::keyPress(QKeyEvent *event)
{
    if(event->modifiers() == (Qt::ControlModifier | Qt::AltModifier)){
        isGLWidgetFocued = false;
        this->setMouseTracking(false);
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        QCursor::setPos(QWidget::mapToGlobal(QPoint(this->width()/2, this->height()/2)));

        DEBUG("gl widget focus lost");
        return;
    }

    if(event->modifiers() == (Qt::ControlModifier)){
        switch (event->key()) {
        case '=':{
            glAll.key('+');
            DEBUG("zoom in");
            break;
        }

        case '-':{
            glAll.key('-');
            DEBUG("zoom out");
            break;
        }

        case '0':{
            glAll.key('0');
            DEBUG("zoom to fit");
            break;
        }

        case 'P':{
            glAll.key('P');
            DEBUG("pan");
            break;
        }

        case 'O':{
            glAll.key('O');
            DEBUG("orbit");
            break;
        }

        default:{
            DEBUG(event->key());
            break;
        }
        }

        return;
    }


    unsigned char k = event->key();;
    if(k<='Z'&&k>='A'){
        if(event->modifiers() == Qt::ShiftModifier)
            ;
        else{
            k = k - 'A' + 'a';
        }
    }

    if(k == 'g'){
        QString fileName = QFileDialog::getSaveFileName(this,
                                   tr("Save Screenshot"), QDir::homePath());
        if (!fileName.isEmpty())
        {
            this->paintGL();
            glFlush();
            QImage image = this->grabFrameBuffer();
            image.save(fileName,"JPG");

            return;
        }
    }
    else{
        glAll.key(k);
    }




    return;
}

void GLWidget::paintGL()
{
    glAll.redraw();
    return;


    qglClearColor(clearColor);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -10.0f);
    glRotatef(xRot / 16.0f, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot / 16.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(zRot / 16.0f, 0.0f, 0.0f, 1.0f);

    glVertexPointer(3, GL_FLOAT, 0, vertices.constData());
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords.constData());
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    for (int i = 0; i < 6; ++i) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }
}

void GLWidget::resizeGL(int width, int height)
{
    glAll.reshape(this->width(), this->height());
    return;


    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);

    glMatrixMode(GL_MODELVIEW);

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();

    isGLWidgetFocued = true;
    this->setMouseTracking(true);
    QApplication::setOverrideCursor(Qt::BlankCursor);
    QApplication::setOverrideCursor(Qt::BlankCursor);

    DEBUG("gl widget focus got");

}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    //int dx = event->x() - lastPos.x();
    //int dy = event->y() - lastPos.y();
    //lastPos = event->pos();

    int dx = event->x() - this->width()/2;
    int dy = event->y() - this->height()/2;

    DEBUG("mouse position: "<<event->x()<<","<<event->y());

    glAll.mouse_move(dx, dy);

    if(isGLWidgetFocued){
        QCursor::setPos(QWidget::mapToGlobal(QPoint(this->width()/2, this->height()/2)));
        this->setCursor(Qt::BlankCursor);
    }

    return;



    if (event->buttons() & Qt::LeftButton) {
        rotateBy(8 * dy, 8 * dx, 0);
    } else if (event->buttons() & Qt::RightButton) {
        rotateBy(8 * dy, 0, 8 * dx);
    }
}

void GLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
    emit clicked();
}

void GLWidget::makeObject()
{
    static const int coords[6][4][3] = {
        { { +1, -1, -1 }, { -1, -1, -1 }, { -1, +1, -1 }, { +1, +1, -1 } },
        { { +1, +1, -1 }, { -1, +1, -1 }, { -1, +1, +1 }, { +1, +1, +1 } },
        { { +1, -1, +1 }, { +1, -1, -1 }, { +1, +1, -1 }, { +1, +1, +1 } },
        { { -1, -1, -1 }, { -1, -1, +1 }, { -1, +1, +1 }, { -1, +1, -1 } },
        { { +1, -1, +1 }, { -1, -1, +1 }, { -1, -1, -1 }, { +1, -1, -1 } },
        { { -1, -1, +1 }, { +1, -1, +1 }, { +1, +1, +1 }, { -1, +1, +1 } }
    };

    for (int j=0; j < 6; ++j) {
        textures[j] = bindTexture
            (QPixmap(QString(":/images/side%1.png").arg(j + 1)), GL_TEXTURE_2D);
    }

    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 4; ++j) {
            texCoords.append
                (QVector2D(j == 0 || j == 3, j == 0 || j == 1));
            vertices.append
                (QVector3D(0.2 * coords[i][j][0], 0.2 * coords[i][j][1],
                           0.2 * coords[i][j][2]));
        }
    }
}
