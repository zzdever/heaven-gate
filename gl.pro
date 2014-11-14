#-------------------------------------------------
#
# Project created by QtCreator 2014-11-08T11:04:31
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gl
TEMPLATE = app


SOURCES += main.cpp\
    window.cpp \
    glwidget.cpp \
    glfunction.cpp

HEADERS  += \
    window.h \
    glwidget.h \
    glfunction.h

FORMS    += mainwindow.ui

RESOURCES += \
    opengl.qrc

LIBS+= -framework glut
