#ifndef GLALL_H
#define GLALL_H


#include <vector>

#include "globject.h"
#include "gllight.h"
#include "misc.h"
#include "nurbs.h"
#include "collision.h"

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
    // main draw function
    void redraw(GLenum drawMode=GL_RENDER);
    // call when window is shaped
    void reshape(int width, int height);
    // update view when eye is moves
    void updateView(int width, int height);
    // initialize all drawing environment
    void glAllInit();

    // accept keyboard events
    void key(unsigned char k);
    // accept mouse press events
    void mouse_press(int button,int state,int x,int y);
    // accept mouse move events
    void mouse_move(int dx, int dy);

    // select an object
    void SelectObject(GLint x, GLint y);
    // do something after an object is selected
    void ProcessHits(GLint hits, GLuint buffer[]);
    // map screen coordinate to the world coordinate
    std::vector<GLdouble> screen2world(int x, int y);

    // control light (call by GUI module)
    void change_light(int num, float value[4], LightParam param = Diffuse);

    // move object (call by GUI module)
    void move(int x, int y, int z){
        DEBUG("move "<<x<<" "<<y<<" "<<z);
        if(selectedObject == NULL) return;
        selectedObject->MovePosition(x/100.0,y/100.0,z/100.0);
    }
    // rotate object (call by GUI module)
    void rotate(int x, int y, int z){
        DEBUG("rotate "<<x<<" "<<y<<" "<<z);
        if(selectedObject == NULL) return;
        selectedObject->SetRotation(x/5.0,y/5.0,z/5.0);
    }
    // scale object (call by GUI module)
    void scale(int s) {
        DEBUG("scale "<<s);
        if(selectedObject == NULL) return;
        selectedObject->SetScale(s/100.0);
    }
    // change texture (call by GUI module)
    void change_texture(int value){
        value;
    }

private:

    float fTranslate;
    float fRotate;
    float fScale;

    bool bPersp;
    bool bAnim;
    bool bWire;

    // window dimension
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
    GLuint texBalcony;
    GLuint texCube;
    GLuint texImax[IMAXFRAME];
    GLuint texPoll[POLLCOUNT];

    // view control
    float eye[3];
    float eye_center[3];
    float eye_theta[2];

    // move control
    float zoomAmount;
    float sideAmount;
    float updownAmount;
    bool isCameraPan;
    bool isCameraOrbit;
    float orbitRadius;

    // object list
    vector<ObjectFramework*> objectList;
    ObjectFramework* selectedObject;

    // collision detector
    CollisionDetector collisionDetector;

    // texture processing
    GLuint load_texture(const char* file_name);
    void TextureColorkey(GLubyte r, GLubyte g, GLubyte b, GLubyte threshold);
    GLuint GenerateTex();

    // move smooth
    void MoveControl();
    // move eye
    void MoveEye();

    // setup the scene (define all objects)
    void SetupScene();

    // draw the crosshair
    void DrawCrosshair();
    // draw sky
    void DrawSky();
    // draw a table
    void Draw_Table();

};






#endif // GLFUNCTION_H
