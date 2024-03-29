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
    mainLayout->addWidget(glWidgetMain,0,0,2,1);
    currentGlWidget = glWidgetMain;


    // object control group
    QGroupBox *objectGroup = new QGroupBox(parent);
    objectGroup->setFixedWidth(180);
    objectGroup->setTitle(tr("Object Control"));


    /*
    // object names
    QGroupBox *objectNameGroup = new QGroupBox(objectGroup);
    objectNameGroup->setTitle(tr("Objects"));
    QComboBox *objectName = new QComboBox(objectNameGroup);
    // TODO add items
    objectName->addItem("desk");
    //connect(objectName, SIGNAL(currentIndexChanged(int)), glWidgetMain, SLOT(SwitchObjectNum(int)));

    QHBoxLayout *objectNameGroupLayout = new QHBoxLayout(objectNameGroup);
    objectNameGroupLayout->addWidget(objectName);
    */



    // rotate, move, scale

    QGroupBox *objectPropertyGroup = new QGroupBox(objectGroup);
    objectPropertyGroup->setTitle(tr("Object Property"));

    /*
    QComboBox *objectTexture = new QComboBox(objectPropertyGroup);
    // TODO add items
    objectTexture->addItem("crack");
    connect(objectTexture, SIGNAL(currentIndexChanged(int)), glWidgetMain, SLOT(changeTexture(int)));
    */


    QSlider *moveXSlider = new QSlider(Qt::Horizontal, objectPropertyGroup);
    moveXSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    moveXSlider->setRange(-10,10);
    moveXSlider->setValue(0);
    moveXSlider->sliderReleased();
    // TODO slot should set back to 0
    connect(moveXSlider, SIGNAL(valueChanged(int)), glWidgetMain, SLOT(moveX(int)));


    QSlider *moveYSlider = new QSlider(Qt::Horizontal, objectPropertyGroup);
    moveYSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    moveYSlider->setRange(-10,10);
    moveYSlider->setValue(0);
    connect(moveYSlider, SIGNAL(valueChanged(int)), glWidgetMain, SLOT(moveY(int)));

    QSlider *moveZSlider = new QSlider(Qt::Horizontal, objectPropertyGroup);
    moveZSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    moveZSlider->setRange(-10,10);
    moveZSlider->setValue(0);
    connect(moveZSlider, SIGNAL(valueChanged(int)), glWidgetMain, SLOT(moveZ(int)));


    QSlider *rotateXSlider = new QSlider(Qt::Horizontal, objectPropertyGroup);
    rotateXSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    rotateXSlider->setRange(-10,10);
    rotateXSlider->setValue(0);
    connect(rotateXSlider, SIGNAL(valueChanged(int)), glWidgetMain, SLOT(rotateX(int)));

    QSlider *rotateYSlider = new QSlider(Qt::Horizontal, objectPropertyGroup);
    rotateYSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    rotateYSlider->setRange(-10,10);
    rotateYSlider->setValue(0);
    connect(rotateYSlider, SIGNAL(valueChanged(int)), glWidgetMain, SLOT(rotateY(int)));

    QSlider *rotateZSlider = new QSlider(Qt::Horizontal, objectPropertyGroup);
    rotateZSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    rotateZSlider->setRange(-10,10);
    rotateZSlider->setValue(0);
    connect(rotateZSlider, SIGNAL(valueChanged(int)), glWidgetMain, SLOT(rotateZ(int)));

    QSlider *scaleSlider = new QSlider(Qt::Horizontal, objectPropertyGroup);
    scaleSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    scaleSlider->setRange(0,500);
    scaleSlider->setValue(100);
    connect(scaleSlider, SIGNAL(valueChanged(int)), glWidgetMain, SLOT(scale(int)));


    QGridLayout *objectPropertyGroupLayout = new QGridLayout(objectPropertyGroup);
    objectPropertyGroupLayout->setColumnStretch(1,2);
    /*
    objectPropertyGroupLayout->addWidget(new QLabel("Tex"),0,0);
    objectPropertyGroupLayout->addWidget(objectTexture,0,1);
    */
    objectPropertyGroupLayout->addWidget(new QLabel("MoveX"), 1, 0);
    objectPropertyGroupLayout->addWidget(moveXSlider,1,1);
    objectPropertyGroupLayout->addWidget(new QLabel("MoveY"), 2, 0);
    objectPropertyGroupLayout->addWidget(moveYSlider,2,1);
    objectPropertyGroupLayout->addWidget(new QLabel("MoveZ"), 3, 0);
    objectPropertyGroupLayout->addWidget(moveZSlider,3,1);
    objectPropertyGroupLayout->addWidget(new QLabel("RotateX"), 4, 0);
    objectPropertyGroupLayout->addWidget(rotateXSlider,4,1);
    objectPropertyGroupLayout->addWidget(new QLabel("RotateY"), 5, 0);
    objectPropertyGroupLayout->addWidget(rotateYSlider,5,1);
    objectPropertyGroupLayout->addWidget(new QLabel("RotateZ"), 6, 0);
    objectPropertyGroupLayout->addWidget(rotateZSlider,6,1);
    objectPropertyGroupLayout->addWidget(new QLabel("Scale"), 7, 0);
    objectPropertyGroupLayout->addWidget(scaleSlider,7,1);



    //object control layout
    QVBoxLayout *objectGroupLayout = new QVBoxLayout(objectGroup);
    objectGroupLayout->setMargin(3);
    /*
    objectGroupLayout->addWidget(objectNameGroup);
    */
    objectGroupLayout->addWidget(objectPropertyGroup);




    QGroupBox *lightGroup = new QGroupBox(parent);
    lightGroup->setFixedWidth(180);
    lightGroup->setTitle(tr("Light Control"));


    // light number
    QGroupBox *lightNumGroup = new QGroupBox(lightGroup);
    lightNumGroup->setTitle(tr("Lights"));
    QComboBox *lightNum = new QComboBox(lightNumGroup);
    for (int i=0; i<LIGHT_COUNT; i++){
        lightNum->addItem(QString::number(i,10));
    }
    connect(lightNum, SIGNAL(currentIndexChanged(int)), glWidgetMain, SLOT(SwitchLightNum(int)));

    QPushButton *lightSwitch = new QPushButton(lightNumGroup);
    lightSwitch->setText("On/OFF");
    connect(lightSwitch, SIGNAL(clicked()), glWidgetMain, SLOT(SwitchLight()));

    QHBoxLayout *lightNumGroupLayout = new QHBoxLayout(lightNumGroup);
    lightNumGroupLayout->addWidget(lightNum);
    lightNumGroupLayout->addWidget(lightSwitch);



    //GL_AMBIENT、GL_DIFFUSE、GL_SPECULAR

    QGroupBox *lightColorGroup = new QGroupBox(lightGroup);
    lightColorGroup->setTitle(tr("Light Color"));

    /*
    QComboBox *colorType = new QComboBox(lightColorGroup);
    colorType->addItem(QString("Ambient"));
    colorType->addItem(QString("Diffuse"));
    colorType->addItem(QString("Specular"));
    connect(colorType, SIGNAL(currentIndexChanged(int)), glWidgetMain, SLOT(SwitchLightParam(int)));
    */

    QSlider *rColorSlider = new QSlider(Qt::Horizontal, lightColorGroup);
    rColorSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    rColorSlider->setRange(0,255);
    rColorSlider->setValue(255);
    connect(rColorSlider, SIGNAL(valueChanged(int)), glWidgetMain, SLOT(changeR(int)));


    QSlider *gColorSlider = new QSlider(Qt::Horizontal, lightColorGroup);
    gColorSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    gColorSlider->setRange(0,255);
    gColorSlider->setValue(255);
    connect(gColorSlider, SIGNAL(valueChanged(int)), glWidgetMain, SLOT(changeG(int)));

    QSlider *bColorSlider = new QSlider(Qt::Horizontal, lightColorGroup);
    bColorSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    bColorSlider->setRange(0,255);
    bColorSlider->setValue(255);
    connect(bColorSlider, SIGNAL(valueChanged(int)), glWidgetMain, SLOT(changeB(int)));


    QSlider *aColorSlider = new QSlider(Qt::Horizontal, lightColorGroup);
    aColorSlider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    aColorSlider->setRange(0,255);
    aColorSlider->setValue(255);
    connect(aColorSlider, SIGNAL(valueChanged(int)), glWidgetMain, SLOT(changeA(int)));


    QGridLayout *lightColorLayout = new QGridLayout(lightColorGroup);
    lightColorLayout->setColumnStretch(1, 2);
    /*
    lightColorLayout->addWidget(colorType,0,0,1,2);
    */
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


    mainLayout->addWidget(objectGroup,0,1);
    mainLayout->addWidget(lightGroup,1,1);

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

    setWindowTitle(tr("Heaven Gate"));
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
