#-------------------------------------------------
#
# Project created by QtCreator 2014-11-08T11:04:31
#
#-------------------------------------------------

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gl
TEMPLATE = app


HEADERS  += \
    window.h \
    glwidget.h \
    misc.h \
    globject.h \
    gllight.h \
    glall.h \
    objfile.h \
    nurbs.h \
    collision.h

SOURCES += main.cpp\
    window.cpp \
    glwidget.cpp \
    gldraw.cpp \
    glcontrol.cpp \
    globject.cpp \
    gllight.cpp \
    objfile.cpp \
    nurbs.cpp \
    misc.cpp \
    collision.cpp

FORMS    += mainwindow.ui

RESOURCES += \
    opengl.qrc

LIBS += -framework glut \
    -framework opengl
