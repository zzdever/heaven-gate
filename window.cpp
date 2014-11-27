#include <QtWidgets>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QSlider>

#include "misc.h"
#include "glwidget.h"
#include "window.h"


void Window::setMainLayout(Window* parent)
{
    QGridLayout *mainLayout = new QGridLayout;


    QColor clearColor;
    clearColor.setRgb(125,127,25);
    glWidgetMain = new GLWidget(0,0);
    glWidgetMain->setClearColor(clearColor);
    mainLayout->addWidget(glWidgetMain, 0, 0);
    currentGlWidget = glWidgetMain;


    QGroupBox *lightGroup = new QGroupBox(parent);
    lightGroup->setFixedWidth(180);
    lightGroup->setTitle(tr("Light Control"));


    // light number
    QGroupBox *lightNumGroup = new QGroupBox(lightGroup);
    QComboBox *lightNum = new QComboBox(lightGroup);
    for (int i=0; i<8; i++){
        lightNum->addItem(QString::number(i,10));
    }
    lightNumGroup->setTitle(tr("Light Number"));
    QVBoxLayout *lightNumLayout = new QVBoxLayout(lightNumGroup);
    lightNumLayout->addWidget(lightNum);


    //GL_AMBIENT、GL_DIFFUSE、GL_SPECULAR

    QGroupBox *lightColorGroup = new QGroupBox(lightGroup);
    lightColorGroup->setTitle(tr("Light Color"));

    QComboBox *colorType = new QComboBox(lightColorGroup);
    colorType->addItem(QString("Ambient"));
    colorType->addItem(QString("Diffuse"));
    colorType->addItem(QString("Specular"));

    QSlider *rColorSlider = new QSlider(Qt::Horizontal, lightColorGroup);
    rColorSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    rColorSlider->setRange(0,255);
    rColorSlider->setValue(255);

    QSlider *gColorSlider = new QSlider(Qt::Horizontal, lightColorGroup);
    gColorSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    gColorSlider->setRange(0,255);
    gColorSlider->setValue(255);

    QSlider *bColorSlider = new QSlider(Qt::Horizontal, lightColorGroup);
    bColorSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    bColorSlider->setRange(0,255);
    bColorSlider->setValue(255);

    QSlider *aColorSlider = new QSlider(Qt::Horizontal, lightColorGroup);
    aColorSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    aColorSlider->setRange(0,255);
    aColorSlider->setValue(255);


    QGridLayout *lightColorLayout = new QGridLayout(lightColorGroup);
    lightColorLayout->setColumnStretch(1, 2);
    lightColorLayout->addWidget(colorType,0,0,1,2);
    lightColorLayout->addWidget(new QLabel("R"), 1, 0);
    lightColorLayout->addWidget(rColorSlider, 1, 1);
    lightColorLayout->addWidget(new QLabel("G"), 2, 0);
    lightColorLayout->addWidget(gColorSlider, 2, 1);
    lightColorLayout->addWidget(new QLabel("B"), 3, 0);
    lightColorLayout->addWidget(bColorSlider, 3, 1);
    lightColorLayout->addWidget(new QLabel("A"), 4, 0);
    lightColorLayout->addWidget(aColorSlider, 4, 1);




    //light control layout
    QVBoxLayout *lightGroupLayout = new QVBoxLayout(lightGroup);
    lightGroupLayout->setMargin(3);
    lightGroupLayout->addWidget(lightNumGroup);
    lightGroupLayout->addWidget(lightColorGroup);
    lightGroupLayout->addStretch(1);


    mainLayout->addWidget(lightGroup, 0, 1);

    setLayout(mainLayout);

}

Window::Window()
{


    setMainLayout(this);



//    for (int i = 0; i < NumRows; ++i) {
//        for (int j = 0; j < NumColumns; ++j) {
//            QColor clearColor;
//            clearColor.setHsv(((i * NumColumns) + j) * 255
//                              / (NumRows * NumColumns - 1),
//                              255, 63);

//            glWidgets[i][j] = new GLWidget(0, 0);
//            glWidgets[i][j]->setClearColor(clearColor);
//            glWidgets[i][j]->rotateBy(+42 * 16, +42 * 16, -21 * 16);
//            mainLayout->addWidget(glWidgets[i][j], i, j);

//            connect(glWidgets[i][j], SIGNAL(clicked()),
//                    this, SLOT(setCurrentGlWidget()));
//        }
//    }

    //currentGlWidget = glWidgets[0][0];

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(rotateOneStep()));
    timer->start(20);

    setWindowTitle(tr("Textures"));
}

void Window::setCurrentGlWidget()
{
    currentGlWidget = qobject_cast<GLWidget *>(sender());
}

void Window::rotateOneStep()
{
    if (currentGlWidget)
        currentGlWidget->rotateBy(+2 * 16, +2 * 16, -1 * 16);
}

void Window::keyPressEvent(QKeyEvent *event)
{
    currentGlWidget->keyPress(event);
}
