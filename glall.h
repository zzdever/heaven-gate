#ifndef GLALL_H
#define GLALL_H


#include <vector>

#include "globject.h"
#include "gllight.h"
#include "misc.h"
#include "nurbs.h"


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
    void redraw(GLenum drawMode=GL_RENDER);
    void reshape(int width, int height);
    void updateView(int width, int height);
    void glAllInit();

    void key(unsigned char k);
    void mouse_press(int button,int state,int x,int y);
    void mouse_move(int dx, int dy);

    void SelectObject(GLint x, GLint y);
    void ProcessHits(GLint hits, GLuint buffer[]);
    std::vector<GLdouble> screen2world(int x, int y);


    void change_light(int num, float value[4], LightParam param = Diffuse);

    void move(int x, int y, int z){
        DEBUG("move"<<x<<" "<<y<<" "<<z);
        if(selectedObject == NULL) return;
        selectedObject->MovePosition(x/100.0,y/100.0,z/100.0);
    }
    void rotate(int x, int y, int z){
        DEBUG("rotate"<<x<<" "<<y<<" "<<z);
        if(selectedObject == NULL) return;
        selectedObject->SetRotation(x/5.0,y/5.0,z/5.0);
    }
    void scale(int s) {
        DEBUG("scale"<<s);
        if(selectedObject == NULL) return;
        selectedObject->SetScale(s/100.0);
    }
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


    vector<ObjectFramework*> objectList;
    ObjectFramework* selectedObject;
    //Girl* girl;

    ModelCube cube;

    Nurbs tableLamp;


    GLuint load_texture(const char* file_name);
    void TextureColorkey(GLubyte r, GLubyte g, GLubyte b, GLubyte threshold);
    GLuint GenerateTex();

    void MoveControl();
    void MoveEye();

    void SetupScene();

    void DrawCrosshair();
    void drawBox(GLfloat size, GLenum type);
    void glutSolidCube(GLdouble size);
    void DrawSky();
    void Draw_Triangle();
    void Draw_Leg();

    void Radio();

};






#endif // GLFUNCTION_H
