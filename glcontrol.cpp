#include "misc.h"
#include "glall.h"

#include <QThread>


void GlAll::draw(GLenum model)
{
    if(model==GL_SELECT){
        glColor3f(1.0,0.0,0.0);
        glLoadName(100);
        glPushMatrix();
        glTranslatef(-5, 0.0, 10.0);
        glBegin(GL_QUADS);
        glVertex3f(-1, -1, 0);
        glVertex3f( 1, -1, 0);
        glVertex3f( 1, 1, 0);
        glVertex3f(-1, 1, 0);
        glEnd();
        glPopMatrix();

        glColor3f(0.0,0.0,1.0);
        glLoadName(101);
        glPushMatrix();
        glTranslatef(5, 0.0, -10.0);
        glBegin(GL_QUADS);
        glVertex3f(-1, -1, 0);
        glVertex3f( 1, -1, 0);
        glVertex3f( 1, 1, 0);
        glVertex3f(-1, 1, 0);
        glEnd();
        glPopMatrix();
        glPopName();
    }
    else{
        glColor3f(1.0,0.0,0.0);
        glPushMatrix();
        glTranslatef(-5, 0.0, -5.0);
        glBegin(GL_QUADS);
        glVertex3f(-1, -1, 0);
        glVertex3f( 1, -1, 0);
        glVertex3f( 1, 1, 0);
        glVertex3f(-1, 1, 0);
        glEnd();
        glPopMatrix();

        glColor3f(0.0,0.0,1.0);
        glPushMatrix();
        glTranslatef(5, 0.0, -10.0);
        glBegin(GL_QUADS);
        glVertex3f(-1, -1, 0);
        glVertex3f( 1, -1, 0);
        glVertex3f( 1, 1, 0);
        glVertex3f(-1, 1, 0);
        glEnd();
        glPopMatrix();
    }
}



void GlAll::SelectObject(GLint x, GLint y)

{

    GLuint selectBuff[32]={0};//创建一个保存选择结果的数组
    GLint hits, viewport[4];


    glGetIntegerv(GL_VIEWPORT, viewport); //获得viewport
    glSelectBuffer(64, selectBuff); //告诉OpenGL初始化  selectbuffer
    glRenderMode(GL_SELECT);    //进入选择模式

    glInitNames();  //初始化名字栈
    glPushName(0);  //在名字栈中放入一个初始化名字，这里为‘0’


    glMatrixMode(GL_PROJECTION);    //进入投影阶段准备拾取
    glPushMatrix();     //保存以前的投影矩阵
    glLoadIdentity();   //载入单位矩阵


    float m[16];
    glGetFloatv(GL_PROJECTION_MATRIX, m);

    gluPickMatrix( x,           // 设定我们选择框的大小，建立拾取矩阵，就是上面的公式
                  viewport[3]-y,    // viewport[3]保存的是窗口的高度，窗口坐标转换为OpenGL坐标
                  2,2,              // 选择框的大小为2，2
                  viewport          // 视口信息，包括视口的起始位置和大小
                  );


    glGetFloatv(GL_PROJECTION_MATRIX, m);
    glOrtho(-10, 10, -10, 10, -10, 10);     //拾取矩阵乘以投影矩阵，这样就可以让选择框放大为和视体一样大
    glGetFloatv(GL_PROJECTION_MATRIX, m);


    draw(GL_SELECT);    // 该函数中渲染物体，并且给物体设定名字


    glMatrixMode(GL_PROJECTION);
    glPopMatrix();  // 返回正常的投影变换
    glGetFloatv(GL_PROJECTION_MATRIX, m);

    hits = glRenderMode(GL_RENDER); // 从选择模式返回正常模式,该函数返回选择到对象的个数
    if(hits > 0)
        std::cout<<hits<<"ssssssssssssssssssssssssss selected"<<std::endl;

        //processSelect(selectBuff);  //  选择结果处理

}




// judge whether n is a number of power of 2
int power_of_two(int n) {
    if( n <= 0 )
        return 0;

    return (n & (n-1)) == 0;
}

// open file_name, read in image data
// return a new texture ID
GLuint GlAll::load_texture(const char* file_name) {
    GLint width, height, total_bytes;
    GLubyte* pixels = 0;
    GLint last_texture_ID;
    GLuint texture_ID = 0;

    // open BMP image file
    FILE* pFile = fopen(file_name, "rb");
    if( pFile == 0 ){
        return 0;
    }


    // read BMP file width and height
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&width, 4, 1, pFile);
    fread(&height, 4, 1, pFile);
    fseek(pFile, BMP_Header_Length, SEEK_SET);

    // calculate the total bytes of data
    {
        GLint line_bytes = width * 3;
        while( line_bytes % 4 != 0 )
            ++line_bytes;
        total_bytes = line_bytes * height;
    }

    // allocate memory
    pixels = (GLubyte*)malloc(total_bytes);
    if( pixels == 0 )
    {
        fclose(pFile);
        return 0;
    }

    // read image data
    if( fread(pixels, total_bytes, 1, pFile) <= 0 ) {
        free(pixels);
        fclose(pFile);
        return 0;
    }



    // scale the image if it's not aligned, or too large
    {
        GLint max;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
        if( !power_of_two(width) || !power_of_two(height) || width > max || height > max )
        {
            // dimension after scaling
            const GLint new_width = 256;
            const GLint new_height = 256;
            GLint new_line_bytes, new_total_bytes;
            GLubyte* new_pixels = 0;

            // calculate the total bytes of one line
            new_line_bytes = new_width * 3;
            while( new_line_bytes % 4 != 0 )
                ++new_line_bytes;
            new_total_bytes = new_line_bytes * new_height;
            // allocate memory
            new_pixels = (GLubyte*)malloc(new_total_bytes);
            if( new_pixels == 0 )
            {
                free(pixels);
                fclose(pFile);
                return 0;
            }

            // scale
            gluScaleImage(GL_RGB, width, height, GL_UNSIGNED_BYTE, pixels,
                          new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

            free(pixels);
            pixels = new_pixels;
            width = new_width;
            height = new_height;
        }
    }



    // allocate a new texture id
    glGenTextures(1, &texture_ID);
    if( texture_ID == 0 ) {
        free(pixels);
        fclose(pFile);
        return 0;
    }


    // set the attributes of the texture
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, last_texture_ID);


    free(pixels);
    return texture_ID;
}


GLuint GlAll::GenerateTex()
{
    GLint width, height, total_bytes;

    GLint last_texture_ID;
    GLuint texture_ID = 0;
    GLubyte *pixels = 0;

    width = 64;
    height = 64;
    total_bytes = 3*width*height;
    // allocate memory
    pixels = (GLubyte *)malloc(total_bytes);
    if( pixels == 0 )
    {
        return 0;
    }

    int pixelPosition = 0;
    int blocks = 8;
    GLubyte r, g, b;
    // write color data
    for (int i=0; i<width; i++) {
        for (int j=0; j<height; j++) {
            if((i/(width/blocks))%2 == (j/(width/blocks))%2){   // this condition is the key to generate the tiling pattern
                // color dark purple
                r = 22;
                g = 24;
                b = 50;
            }
            else{
                // color white
                r = 236;
                g = 237;
                b = 218;
            }

            // write r,g,b channel
            pixels[pixelPosition] = b;
            pixelPosition++;
            pixels[pixelPosition] = g;
            pixelPosition++;
            pixels[pixelPosition] = r;
            pixelPosition++;
        }
    }

    // allocate a new texture id
    glGenTextures(1, &texture_ID);
    if( texture_ID == 0 ) {
        free(pixels);
        return 0;
    }


    // set the attributes of the texture
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, last_texture_ID);


    free(pixels);
    return texture_ID;
}









void GlAll::key(unsigned char k)
{
    switch(k){
    case 27:
    case 'q': {exit(0); break; }
    case 'p': {bPersp = !bPersp; break; }

    case ' ': {bAnim = !bAnim; break;}
    case 'o': {bWire = !bWire; break;}

    case 'a': {
        sideAmount = -EYE_STEP_COEFFICIENT;
        break;
    }
    case 'd': {
        sideAmount = EYE_STEP_COEFFICIENT;
        break;
    }
    case 'w': {
        zoomAmount = EYE_STEP_COEFFICIENT;
        break;
    }
    case 's': {
        zoomAmount = -EYE_STEP_COEFFICIENT;
        break;
    }
    case 'z': {
        updownAmount = EYE_STEP_COEFFICIENT;
        break;
    }
    case 'c': {
        updownAmount = -EYE_STEP_COEFFICIENT;
        break;
    }
        // whether to enable texture blending
    case 'b':{
        texBlend = !texBlend;
        break;
    }
    case 'B':{
        break;
    }
        // whether to use custom texture
    case 't':{
        texCus = !texCus;
        break;
    }

    // zoom in
    case '+':{
        zoomAmount = ZOOM_STEP_COEFFICIENT;
        break;
    }
    // zoon out
    case '-':{
        zoomAmount = -ZOOM_STEP_COEFFICIENT;
        break;
    }
    // zoom to fit
    case '0':{
        break;
    }
    // pan
    case 'P':{
        break;
    }
    // orbit
    case 'O':{
        break;
    }
    }

    updateView(wWidth, wHeight);
}










void GlAll::mouse_move(int dx, int dy){

    eye_center[0] += 1.0*dx/wWidth;
    eye_center[1] -= 1.0*dy/wHeight;

    eye_theta[0] += 1.0*dx;
    if(eye_theta[0]*EYE_ROTATION_COEFFICIENT < -360 || eye_theta[0]*EYE_ROTATION_COEFFICIENT > 360)
        eye_theta[0] = 0;

    eye_theta[1] += 1.0*dy;
    if(eye_theta[1]*EYE_ROTATION_COEFFICIENT < -90)
        eye_theta[1] = -90*1.0/EYE_ROTATION_COEFFICIENT;
    if(eye_theta[1]*EYE_ROTATION_COEFFICIENT > 90)
        eye_theta[1] = 90*1.0/EYE_ROTATION_COEFFICIENT;

    updateView(wWidth, wHeight);
}


void GlAll::MoveControl()
{
    if(zoomAmount > ZOOM_THRESHOLD || zoomAmount < -ZOOM_THRESHOLD){
        zoomAmount = zoomAmount*0.8;
        eye[0] += zoomAmount*(eye_center[0] - eye[0]);
        eye[2] += zoomAmount*(eye_center[2] - eye[2]);
        if(zoomAmount < ZOOM_THRESHOLD && zoomAmount > -ZOOM_THRESHOLD) {
            zoomAmount = 0.f;
        }
        QThread::msleep(10);
    }

    if(sideAmount > ZOOM_THRESHOLD || sideAmount < -ZOOM_THRESHOLD){
        sideAmount = sideAmount*0.8;
        eye[0] -= sideAmount*(eye_center[2] - eye[2]);
        eye[2] -= sideAmount*(-1)*(eye_center[0] - eye[0]);
        if(sideAmount < ZOOM_THRESHOLD && sideAmount > -ZOOM_THRESHOLD) {
            sideAmount = 0.f;
        }
        QThread::msleep(10);
    }

    if(updownAmount > ZOOM_THRESHOLD || updownAmount < -ZOOM_THRESHOLD){
        updownAmount = updownAmount*0.8;
        eye[1] += updownAmount;
        if(updownAmount < ZOOM_THRESHOLD && updownAmount > -ZOOM_THRESHOLD) {
            updownAmount = 0.f;
        }
        QThread::msleep(10);
    }




}


void GlAll::MoveEye()
{

    eye_center[0] = eye[0] + sin(eye_theta[0]*EYE_ROTATION_COEFFICIENT/180*PI);
    eye_center[1] = eye[1] - tan(eye_theta[1]*EYE_ROTATION_COEFFICIENT/180*PI);
    eye_center[2] = eye[2] - cos(eye_theta[0]*EYE_ROTATION_COEFFICIENT/180*PI);

    gluLookAt(eye[0], eye[1], eye[2],
              eye_center[0], eye_center[1], eye_center[2],
              0, 1, 0);				// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上

}



void GlAll::change_light(int num, float value[], LightParam param)
{
    Q_UNUSED(param);

    lights[num].SetColor(value[0], value[1], value[2], value[3]);

    /*
    switch (param) {
    case Ambient:
        lights[num].SetAmbient(value[0], value[1], value[2], value[3]);
        break;
    case Diffuse:
        lights[num].SetDiffuse(value[0], value[1], value[2], value[3]);
        break;
    case Specular:
        lights[num].SetSpecular(value[0], value[1], value[2], value[3]);
        break;
    case Position:
        lights[num].SetPosition(value[0], value[1], value[2]);
        break;
    default:
        return;
        break;
    }
    */
}






// Do not modify the following functions if not necessary

void GlAll::glAllInit()
{
    lightUnderControl = 0;

    fScale = 1.0f;

    bPersp = true;
    bAnim = false;
    bWire = false;


    wHeight = 0;
    wWidth = 0;

    texBlend = false;
    texCus = false;

    texCrack = load_texture(CRACK);
    texSpot = load_texture(SPOT);
    texMonet = load_texture(MONET);
    texCustom = GenerateTex();
    texNightSky = load_texture(NIGHTSKY);


    eye[0] = 0;
    eye[1] = 0;
    eye[2] = 8;

    eye_center[0] = 0;
    eye_center[1] = 0;
    eye_center[2] = 0;

    eye_theta[0] = 0;
    eye_theta[1] = 0;


    zoomAmount = 0.;
    sideAmount = 0;
    updownAmount = 0;


    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);



//    glEnable(GL_POINT_SMOOTH);
//    glEnable(GL_LINE_SMOOTH);
//    glEnable(GL_POLYGON_SMOOTH);

//    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
//    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
//    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);

    objfile.ReadFile("/Users/ying/Documents/DEV/qt_prj/gl/girl.obj");

    tableLamp.SetupRC();

    return;
}

void GlAll::updateView(int width, int height)
{
    glViewport(0,0,width,height);						// Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
    glLoadIdentity();									// Reset The Projection Matrix


    float whRatio = (GLfloat)width/(GLfloat)height;
    if (bPersp) {
        gluPerspective(45.0f, whRatio,0.1f,100.0f);
    } else {
        glOrtho(-width/200.0, width/200.0, -height/200.0, height/200.0, -100.0, 100.0);
        //glOrtho(-3 ,3, -3, 3,-100,100);
    }


    glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void GlAll::reshape(int width, int height)
{
    if (height==0)										// Prevent A Divide By Zero By
    {
        height=1;										// Making Height Equal One
    }

    wWidth = width;
    wHeight = height;

    updateView(wWidth, wHeight);
}

void GlAll::idle()
{
    glutPostRedisplay();
}

