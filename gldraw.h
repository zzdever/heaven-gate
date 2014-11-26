#ifndef GLFUNCTION_H
#define GLFUNCTION_H

#include "misc.h"

#define PI 3.1415926

void init();
void idle();
void redraw();
void reshape(int width, int height);
void updateView(int width, int height);
void glFunctionInit();

void key(unsigned char k);
void mouse_press(int button,int state,int x,int y);
void mouse_move(int dx, int dy);




#endif // GLFUNCTION_H
