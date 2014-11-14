#include "glfunction.h"

#define TABLE 0
#define TEX 1



#if TEX
//==========================================
// File path definition
// You may need to change this to compile
#define CRACK "/Users/ying/Crack.bmp"
#define SPOT "/Users/ying/Spot.bmp"
#define MONET "/Users/ying/Monet.bmp"
//==========================================

#define BMP_Header_Length 54

GLuint texCrack;
GLuint texSpot;
GLuint texMonet;
GLuint texCustom;


// judge whether n is a number of power of 2
int power_of_two(int n) {
    if( n <= 0 )
        return 0;

    return (n & (n-1)) == 0;
}

// open file_name, read in image data
// return a new texture ID
GLuint load_texture(const char* file_name) {
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


GLuint GenerateTex()
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



float fTranslate;
float fRotate;
float fScale     = 1.0f;	// set inital scale value to 1.0f

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

GLfloat light_pos[] = {0.9, 0.9 , 1, 1};

int wHeight = 0;
int wWidth = 0;

bool texBlend = false;
bool texCus = false;

void Draw_Leg();

// drawBox(), glutWireCube() and glutSolidCube() are
// functions from the GLUT library(glut_shapes.c).
// They are modified for texture binding
void drawBox(GLfloat size, GLenum type)
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
void glutSolidCube(GLdouble size)
{
    drawBox(size, GL_QUADS);
}



void Draw_Triangle() // This function draws a triangle with RGB colors
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

void Draw_Leg()
{
    glScalef(1, 1, 3);
    glutSolidCube(1.0);
}




float eye[] = {0, 0, 8};
float eye_center[] = {0, 0, 0};

void key(unsigned char k)
{
    switch(k)
    {
        case 27:
        case 'q': {exit(0); break; }
        case 'p': {bPersp = !bPersp; break; }

        case ' ': {bAnim = !bAnim; break;}
        case 'o': {bWire = !bWire; break;}

        case 'a': {
            eye[0] += 0.2f;
            eye_center[0] += 0.2f;
            break;
        }
        case 'd': {
            eye[0] -= 0.2f;
            eye_center[0] -= 0.2f;
            break;
        }
        case 'w': {
            eye[1] -= 0.2f;
            eye_center[1] -= 0.2f;
            break;
        }
        case 's': {
            eye[1] += 0.2f;
            eye_center[1] += 0.2f;
            break;
        }
        case 'z': {
            eye[2] -= 0.2f;
            eye_center[2] -= 0.2f;
            break;
        }
        case 'c': {
            eye[2] += 0.2f;
            eye_center[2] += 0.2f;
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
    }

    updateView(wWidth, wHeight);
}


void redraw()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();									// Reset The Current Modelview Matrix

    gluLookAt(eye[0], eye[1], eye[2],
              eye_center[0], eye_center[1], eye_center[2],
              0, 1, 0);				// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上

    if (bWire) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

    GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };

    // diffusion light settings
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
    glLightfv(GL_LIGHT0, GL_EMISSION, white);
    glEnable(GL_LIGHT0);
    // draw the diffusion light
    glPushMatrix();
    glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
    glMaterialfv(GL_FRONT, GL_EMISSION, white);
    glutSolidSphere(0.1, 300, 300);
    {
        float def_emi[] = {0.f, 0.f, 0.f, 1.f};
        glMaterialfv(GL_FRONT, GL_EMISSION, def_emi);
    }
    glPopMatrix();


    //	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
    glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
    glRotatef(-90, 1, 0, 0);
    glScalef(0.2, 0.2, 0.2);

    glPushAttrib(GL_ALL_ATTRIB_BITS);   // save all attributes
    Draw_Triangle();						// Draw triangle
    glPopAttrib();  // restore all attributes

    if (bAnim) fRotate    += 0.5f;
    glutSwapBuffers();
}



#endif


#if TABLE
float fTranslate;
float fRotate = 0.f;
float fScale     = 1.0f;	// set inital scale value to 1.0f

bool bPersp = false;
bool bAnim = false;
bool bWire = false;

int wHeight = 0;
int wWidth = 0;

float eye[] = {0, 0, 8};
float eye_center[] = {0, 0, 0};
float light_pos[] = {0.9, 0.9 , 1, 1};
GLfloat light_color[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat spot_angle = 0.f;

float mat_specular_tea[] = {0.f, 0.f, 0.f, 0.f};



void Draw_Leg()
{
    glScalef(1, 1, 3);
    glutSolidCube(1.0);
}


void Draw_Table()
{
    glPushMatrix();
    glTranslatef(0, 0, 4+1);
    glRotatef(90, 1, 0, 0);
    float mat_diffuse_tea[] = {0.85f, 0.65f, 0.2f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_tea);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular_tea);
    glutSolidTeapot(1);
    {
        float def_specular[] = {0.f, 0.f, 0.f, 0.f};
        glMaterialfv(GL_FRONT, GL_SPECULAR, def_specular);
    }
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, 3.5);
    glScalef(5, 4, 1);
    float mat_diffuse_sur[] = {1.0f, 0.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_sur);
    glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5, 1, 1.5);
    float mat_diffuse_leg1[] = {0.f, 1.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_leg1);
    Draw_Leg();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5, 1, 1.5);
    float mat_diffuse_leg2[] = {1.0f, 1.0f, 0.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_leg2);
    Draw_Leg();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.5, -1, 1.5);
    float mat_diffuse_leg3[] = {0.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_leg3);
    Draw_Leg();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.5, -1, 1.5);
    float mat_diffuse_leg4[] = {0.0f, 0.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse_leg4);
    Draw_Leg();
    glPopMatrix();

}






void redraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();									// Reset The Current Modelview Matrix

    gluLookAt(eye[0], eye[1], eye[2],
              eye_center[0], eye_center[1], eye_center[2],
              0, 1, 0);				// 场景（0，0，0）的视点中心 (0,5,50)，Y轴向上

    if (bWire) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_color);
    glLightfv(GL_LIGHT0, GL_EMISSION, light_color);
    glEnable(GL_LIGHT0);
    glPushMatrix();
    glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
    //glMaterialfv(GL_FRONT, GL_AMBIENT, white);
    glMaterialfv(GL_FRONT, GL_EMISSION, light_color);
    glutSolidSphere(0.1, 300, 300);
    {
        float def_emi[] = {0.f, 0.f, 0.f, 1.f};
        glMaterialfv(GL_FRONT, GL_EMISSION, def_emi);
    }
    glPopMatrix();



    //printf("%f,%f,%f\n",light_pos[0],light_pos[1],light_pos[2]);
    GLfloat spot_pos[] = {0.f, 3.0f, 0.0f, 0.5f};
    GLfloat spot_direction[] = {0.f, -1.0f, 0.0f};
    glLightfv(GL_LIGHT1, GL_POSITION, spot_pos);
    glLightfv(GL_LIGHT1, GL_AMBIENT, white);
    glLightfv(GL_LIGHT1, GL_EMISSION, white);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0f);
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF, spot_angle);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);
    //glEnable(GL_LIGHT1);




    GLfloat light1_ambient[]= { 0.2, 0.2, 0.2, 1.0 };
    GLfloat light1_diffuse[]= { 1.0, 0.0, 0.0, 1.0 };
    GLfloat light1_specular[] = { 1.0, 0.6, 0.6, 1.0 };



    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1, GL_SPECULAR,light1_specular);




    //	glTranslatef(0.0f, 0.0f,-6.0f);			// Place the triangle at Center
    glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
    glRotatef(-90, 1, 0, 0);
    glScalef(0.2, 0.2, 0.2);
    Draw_Table();						// Draw triangle

    if (bAnim) fRotate    += 0.5f;
    glutSwapBuffers();
}



void key(unsigned char k)
{
    switch(k)
    {
        case 27:
        case 'q': {exit(0); break; }
        case 'p': {bPersp = !bPersp; break; }

        case ' ': {bAnim = !bAnim; break;}
        case 'o': {bWire = !bWire; break;}

        case 'a': {
            eye[0] += 0.2f;
            eye_center[0] += 0.2f;
            break;
        }
        case 'd': {
            eye[0] -= 0.2f;
            eye_center[0] -= 0.2f;
            break;
        }
        case 'w': {
            eye[1] -= 0.2f;
            eye_center[1] -= 0.2f;
            break;
        }
        case 's': {
            eye[1] += 0.2f;
            eye_center[1] += 0.2f;
            break;
        }
        case 'z': {
            eye[2] -= 0.2f;
            eye_center[2] -= 0.2f;
            break;
        }
        case 'c': {
            eye[2] += 0.2f;
            eye_center[2] += 0.2f;
            break;
        }
        case 'j':{
            light_pos[0]-=0.2f;
            break;
        }
        case 'l':{
            light_pos[0]+=0.2f;
            break;
        }
        case 'i':{
            light_pos[1]+=0.2f;
            break;
        }
        case 'k':{
            light_pos[1]-=0.2f;
            break;
        }
        case 'u':{
            light_pos[2]-=0.2f;
            break;
        }
        case 'n':{
            light_pos[2]+=0.2f;
            break;
        }
        case 'r':{
            light_color[0] -= 0.05f;
            break;
        }
        case 'R':{
            light_color[0] += 0.05f;
            break;
        }
        case 'g':{
            light_color[1] -= 0.05f;
            break;
        }
        case 'G':{
            light_color[1] += 0.05f;
            break;
        }
        case 'b':{
            light_color[2] -= 0.05f;
            break;
        }
        case 'B':{
            light_color[2] += 0.05f;
            break;
        }
        case 't':{
            mat_specular_tea[0] += 0.1f;
            mat_specular_tea[1] += 0.1f;
            mat_specular_tea[2] += 0.1f;
            break;
        }
        case 'T':{
            mat_specular_tea[0] -= 0.1f;
            mat_specular_tea[1] -= 0.1f;
            mat_specular_tea[2] -= 0.1f;
            break;
        }
        case 'y':{
            spot_angle += 1.f;
            break;
        }
        case 'Y':{
            spot_angle -= 1.f;
            break;
        }
    }

    updateView(wWidth, wHeight);
}

#endif







// Do not modify the following functions if not necessary

void glFunctionInit()
{
    texCrack = load_texture(CRACK);
    texSpot = load_texture(SPOT);
    texMonet = load_texture(MONET);
    texCustom = GenerateTex();

    return;
}

void updateView(int width, int height)
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

void reshape(int width, int height)
{
    if (height==0)										// Prevent A Divide By Zero By
    {
        height=1;										// Making Height Equal One
    }

    wWidth = width;
    wHeight = height;

    updateView(wWidth, wHeight);
}

void idle()
{
    glutPostRedisplay();
}

