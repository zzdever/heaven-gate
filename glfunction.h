#ifndef GLFUNCTION_H
#define GLFUNCTION_H

#include "glwidget.h"

void idle();
void reshape(int width, int height);
void updateView(int width, int height);
void key(unsigned char k);
void redraw();

void glFunctionInit();


#endif // GLFUNCTION_H
