#ifndef GLALL_H
#define GLALL_H

#include <GLUT/glut.h>

#include "misc.h"
#include "globject.h"
#include "gllight.h"


//==========================================
// File path definition
// You may need to change this to compile
#define CRACK "/Users/ying/Crack.bmp"
#define SPOT "/Users/ying/Spot.bmp"
#define MONET "/Users/ying/Monet.bmp"
#define NIGHTSKY "/Users/ying/starskyrecur.bmp"
//==========================================

#define BMP_Header_Length 54


#define EYE_ROTATION_COEFFICIENT 0.05
#define EYE_STEP_COEFFICIENT 0.07
#define ZOOM_STEP_COEFFICIENT 0.7
#define ZOOM_THRESHOLD 0.000001



class GlAll{
public:
    GlAll(){}
    ~GlAll(){}

    // light control
    GlLight lights[LIGHT_COUNT];
    int lightUnderControl;


public:
    void init();
    void idle();
    void redraw();
    void reshape(int width, int height);
    void updateView(int width, int height);
    void glAllInit();

    void key(unsigned char k);
    void mouse_press(int button,int state,int x,int y);
    void mouse_move(int dx, int dy);

    void change_light(int num, float value[4], LightParam param = Diffuse);

private:

    float fTranslate;
    float fRotate;
    float fScale;

    bool bPersp;
    bool bAnim;
    bool bWire;

    int wHeight;
    int wWidth;


    // texture
    bool texBlend;
    bool texCus;

    GLuint texCrack;
    GLuint texSpot;
    GLuint texMonet;
    GLuint texCustom;
    GLuint texNightSky;

    // view control
    float eye[3];
    float eye_center[3];
    float eye_theta[2];

    // move control
    float zoomAmount;
    float sideAmount;
    float updownAmount;


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

    void Radio();

};






#endif // GLFUNCTION_H
