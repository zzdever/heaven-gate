#include "misc.h"
#include "glall.h"

#include <QThread>

#define TABLE 0
#define TEX 1



#if TEX




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





// drawBox(), glutWireCube() and glutSolidCube() are
// functions from the GLUT library(glut_shapes.c).
// They are modified for texture binding
void GlAll::drawBox(GLfloat size, GLenum type)
{
    static GLfloat tex[2][2][2] =
    {
        { {0, 0},
            {1, 0}},
        { {0, 1},
            {1, 1}}
    };

    static GLfloat n[6][3] =
    {
        {-1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, -1.0}
    };
    static GLint faces[6][4] =
    {
        {0, 1, 2, 3},
        {3, 2, 6, 7},
        {7, 6, 5, 4},
        {4, 5, 1, 0},
        {5, 6, 2, 1},
        {7, 4, 0, 3}
    };
    GLfloat v[8][3];
    GLint i;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;



    for (i = 5; i >= 0; i--) {
        glBegin(type);

        glNormal3fv(&n[i][0]);

        // when texture blend is enabled, use glMultiTexCoord2
        if(texBlend){
            glMultiTexCoord2f(GL_TEXTURE0,0.0,0.0);
            glMultiTexCoord2f(GL_TEXTURE1,0.0,0.0);
        }
        else{
            glTexCoord2f(tex[0][0][0], tex[0][0][1]);
        }
        glVertex3fv(&v[faces[i][0]][0]);

        if (texBlend) {
            glMultiTexCoord2f(GL_TEXTURE0,1.0,0.0);
            glMultiTexCoord2f(GL_TEXTURE1,1.0,0.0);
        }
        else{
            glTexCoord2f(tex[0][1][0], tex[0][1][1]);
        }
        glVertex3fv(&v[faces[i][1]][0]);

        if(texBlend){
            glMultiTexCoord2f(GL_TEXTURE0,1.0,1.0);
            glMultiTexCoord2f(GL_TEXTURE1,1.0,1.0);
        }
        else{
            glTexCoord2f(tex[1][1][0], tex[1][1][1]);
        }
        glVertex3fv(&v[faces[i][2]][0]);

        if (texBlend) {
            glMultiTexCoord2f(GL_TEXTURE0,0.0,1.0);
            glMultiTexCoord2f(GL_TEXTURE1,0.0,1.0);
        }
        else{
            glTexCoord2f(tex[1][0][0], tex[1][0][1]);
        }
        glVertex3fv(&v[faces[i][3]][0]);

        glEnd();
    }
}

// override the glutSolidCube() function in GLUT library
void GlAll::glutSolidCube(GLdouble size)
{
    drawBox(size, GL_QUADS);
}


void GlAll::DrawCone(float x, float y, float z)
{
    glutSolidCone(0.5, 0.7, 10, 10);
}


void GlAll::DrawSky()
{


    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    //glCullFace(GL_BACK);

    glPushMatrix();
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, texNightSky);

    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);


    GLfloat xequalzero[] = {1.0, 0.0, 0.0, 0.0};


    glTexGenfv(GL_S, GL_OBJECT_PLANE, xequalzero);
    glTexGenfv(GL_T, GL_OBJECT_PLANE, xequalzero);



    glEnable(GL_TEXTURE_GEN_S);//启用s坐标的纹理生成
    glEnable(GL_TEXTURE_GEN_T);//启用s坐标的纹理生成

    glNormal3f(0.f, 0.f, 0.f);
    //glutSolidSphere(100, 100, 100);
    gluSphere(gluNewQuadric(), 0.8, 100, 100);

    glPopMatrix();
    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);

    return;
}

void GlAll::Draw_Triangle() // This function draws a triangle with RGB colors
{
    glEnable(GL_TEXTURE_2D);
    if (texCus) {
        glBindTexture(GL_TEXTURE_2D, texCustom);    // use custom texture
    }
    else{
        glBindTexture(GL_TEXTURE_2D, texMonet);     // use Monet texture
    }
    glPushMatrix();
    glTranslatef(0, 0, 4+1);
    glRotatef(90, 1, 0, 0);
    glutSolidTeapot(1);
    glPopMatrix();


    glBindTexture(GL_TEXTURE_2D, texCrack);     // use Crack texture
    if(texBlend) {
        glActiveTexture(GL_TEXTURE0);
        // save texture attribute TEXTURE0
        glPushAttrib(GL_TEXTURE_BIT);

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);    // set to modulate texture


        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texSpot);

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);    // set to modulate texture

    }


    glPushMatrix();
    glTranslatef(0, 0, 3.5);
    glScalef(5, 4, 1);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5, 1, 1.5);
    Draw_Leg();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5, 1, 1.5);
    Draw_Leg();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5, -1, 1.5);
    Draw_Leg();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5, -1, 1.5);
    Draw_Leg();
    glPopMatrix();


    if (texBlend) {
        glActiveTexture(GL_TEXTURE1);
        glEnable(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE0);
        // restore texture attribute TEXTURE0
        glPopAttrib();
    }


}

void GlAll::Draw_Leg()
{
    glScalef(1, 1, 3);
    glutSolidCube(1.0);
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
        // move diffusion light to left
    case 'j':{
        light_pos[0]-=0.2f;
        break;
    }
        // move diffusion light to right
    case 'l':{
        light_pos[0]+=0.2f;
        break;
    }
        // move diffusion light back
    case 'i':{
        light_pos[1]+=0.2f;
        break;
    }
        // move diffusion light forward
    case 'k':{
        light_pos[1]-=0.2f;
        break;
    }
        // move diffusion light up
    case 'u':{
        light_pos[2]-=0.2f;
        break;
    }
        // move diffusion light down
    case 'n':{
        light_pos[2]+=0.2f;
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


void GlAll::redraw()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();									// Reset The Current Modelview Matrix


    MoveControl();
    MoveEye();


    if (bWire) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }




    glEnable(GL_DEPTH_TEST);



    //	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
    glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
    glRotatef(-90, 1, 0, 0);
    glScalef(0.2, 0.2, 0.2);

    glPushAttrib(GL_ALL_ATTRIB_BITS);   // save all attributes
    Draw_Triangle();						// Draw triangle

    DrawCone(0,0,0);
    glLoadIdentity();
    //glutSolidSphere(0.9, 300, 300);


    glPopAttrib();  // restore all attributes

    if (bAnim) fRotate    += 0.5f;

    // NOTE QOpenGLContext::swapBuffers() called with non-exposed window, behavior is undefined
    //glutSwapBuffers();
}



#endif








// Do not modify the following functions if not necessary

void GlAll::glFunctionInit()
{
    texCrack = load_texture(CRACK);
    texSpot = load_texture(SPOT);
    texMonet = load_texture(MONET);
    texCustom = GenerateTex();
    texNightSky = load_texture(NIGHTSKY);

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

