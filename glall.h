#ifndef GLALL_H
#define GLALL_H

#include <GLUT/glut.h>

#include "misc.h"
#include "globject.h"


//==========================================
// File path definition
// You may need to change this to compile
#define CRACK "/Users/ying/Crack.bmp"
#define SPOT "/Users/ying/Spot.bmp"
#define MONET "/Users/ying/Monet.bmp"
#define NIGHTSKY "/Users/ying/nightsky.bmp"
//==========================================

#define BMP_Header_Length 54

#define EYE_ROTATION_COEFFICIENT 0.05
#define EYE_STEP_COEFFICIENT 0.07
#define ZOOM_STEP_COEFFICIENT 0.7
#define ZOOM_THRESHOLD 0.000001



class GlAll{
public:
    GlAll(){;}
    ~GlAll(){;}


    float fTranslate;
    float fRotate;
    float fScale     = 1.0f;	// set inital scale value to 1.0f

    bool bPersp = true;
    bool bAnim = false;
    bool bWire = false;

    GLfloat light_pos[4] = {0.9, 0.9 , 1, 1};

    int wHeight = 0;
    int wWidth = 0;

    bool texBlend = false;
    bool texCus = false;


    GLuint texCrack;
    GLuint texSpot;
    GLuint texMonet;
    GLuint texCustom;
    GLuint texNightSky;


    float eye[3] = {0, 0, 8};
    float eye_center[3] = {0, 0, 0};
    float eye_theta[2] = {0, 0};


    float zoomAmount = 0.;
    float sideAmount = 0;
    float updownAmount = 0;






    void init();
    void idle();
    void redraw();
    void reshape(int width, int height);
    void updateView(int width, int height);
    void glFunctionInit();

    void key(unsigned char k);
    void mouse_press(int button,int state,int x,int y);
    void mouse_move(int dx, int dy);


private:
    GLuint load_texture(const char* file_name);
    GLuint GenerateTex();

    void MoveControl();
    void MoveEye();

    void drawBox(GLfloat size, GLenum type);

    void glutSolidCube(GLdouble size);
    void DrawCone(float x, float y, float z);
    void DrawSky();
    void Draw_Triangle();
    void Draw_Leg();




};






#endif // GLFUNCTION_H
