#include "misc.h"
#include "glall.h"

#include <QThread>


// do something after selection
void GlAll::ProcessHits(GLint hits, GLuint buffer[])
{
    GLuint names, *ptr, minZ, *ptrNames, numberOfNames;

    DEBUG("hits = "<<hits);

    ptr = (GLuint *) buffer;
    minZ = 0xffffffff;
    // choose the nearest object
    for (int i = 0; i < (int)hits; i++) {
        names = *ptr;
        ptr++;
        if (*ptr < minZ) {
            numberOfNames = names;
            minZ = *ptr;
            ptrNames = ptr+2;
        }
        ptr += names+2;
    }

    DEBUG("The closest hit names are ");
    ptr = ptrNames;

    // find the object ID
    for (int j = 0; j < (int)numberOfNames; j++, ptr++) {
         DEBUG(*ptr);
         for(int k=0; k<(int)objectList.size(); k++){
             if(objectList.at(k)->GetObjectFrameworkID() == (int)*ptr){
                 objectList.at(k)->Select();
                 selectedObject = objectList.at(k);
             }
             else
                 objectList.at(k)->Unselect();
         }
     }
}


void GlAll::SelectObject(GLint x, GLint y)
{
    // buffer to store results
    GLuint selectBuff[32]={0};
    GLint hits, viewport[4];

    // get viewport
    glGetIntegerv(GL_VIEWPORT, viewport);
    // initialize buffer
    glSelectBuffer(32, selectBuff);
    // enter select mode
    glRenderMode(GL_SELECT);

    // initialize name stack
    glInitNames();
    // push a name 0
    glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    x = viewport[2]/2;
    y = viewport[3]/2;
    gluPickMatrix(x,
                  y,                // selection position
                  5,5,              // selection port 5*5
                  viewport          // viewport information
                  );


    float whRatio = (GLfloat)wWidth/(GLfloat)wHeight;
    gluPerspective(45.0f, whRatio,0.1f,100.0f);

    // redraw
    redraw(GL_SELECT);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // return to render mode and get hit count
    hits = glRenderMode(GL_RENDER);

    if(hits > 0){
        // do processing
        ProcessHits(hits, selectBuff);
    }
    else{
        // unselect all if nothing is selected
        for(int k=0; k<(int)objectList.size(); k++){
            objectList.at(k)->Unselect();
        }
        selectedObject = NULL;
    }

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


void GlAll::TextureColorkey(GLubyte r, GLubyte g, GLubyte b, GLubyte threshold)
{
    GLint width, height;
    GLubyte* pixels = 0;
    // get texture information
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    // allocate space
    pixels = (GLubyte*)malloc(width*height*4);
    if( pixels == 0 )
        return;

    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);
    // modify the alpha channel
    {
        GLint i;
        GLint count = width * height;
        for(i=0; i<count; ++i){
            if( abs(pixels[i*4] - b) <= threshold
             && abs(pixels[i*4+1] - g) <= threshold
             && abs(pixels[i*4+2] - r) <= threshold )
                pixels[i*4+3] = 0;
            else
                pixels[i*4+3] = 255;
        }
    }
    // set texture pixels
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);
    free(pixels);
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

    // left
    case 'a': {
        sideAmount = -EYE_STEP_COEFFICIENT;
        break;
    }
    // right
    case 'd': {
        sideAmount = EYE_STEP_COEFFICIENT;
        break;
    }
    // forward
    case 'w': {
        zoomAmount = EYE_STEP_COEFFICIENT;
        break;
    }
    // backward
    case 's': {
        zoomAmount = -EYE_STEP_COEFFICIENT;
        break;
    }
    // up
    case 'z': {
        updownAmount = EYE_STEP_COEFFICIENT;
        break;
    }
    // down
    case 'c': {
        updownAmount = -EYE_STEP_COEFFICIENT;
        break;
    }
    // whether to enable texture blending
    case 'b':{
        texBlend = !texBlend;
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
        if(selectedObject == NULL)
            break;

        Point3f objp = selectedObject->GetPosition();
        Dimension3f objd = selectedObject->GetDimension();
        updownAmount = objp.y - eye[1];
        // (1-0.8) because of smooth move
        zoomAmount = (1-0.8)*(sqrt(pow(objp.x-eye[0],2) + pow(objp.z-eye[2],2))
                - 2*(wHeight<wWidth ? objd.height : (objd.length<objd.width ? objd.length : objd.width))*cos(45.0/2/180*PI) );

        break;
    }
    // pan
    case 'P':{
        if(isCameraOrbit)
            break;
        isCameraPan = !isCameraPan;
        break;
    }
    // orbit
    case 'O':{
        if(selectedObject == NULL || isCameraPan){
            isCameraOrbit = false;
            break;
        }
        isCameraOrbit = !isCameraOrbit;
        if(isCameraOrbit){
            Point3f objp = selectedObject->GetPosition();
            // calculate orbit radius
            orbitRadius = sqrt(pow(objp.x-eye[0],2) + pow(objp.z-eye[2],2));
        }
        break;
    }
    }

    updateView(wWidth, wHeight);
}


// move mouse to move eye center
void GlAll::mouse_move(int dx, int dy){

    eye_center[0] += 1.0*dx/wWidth;
    eye_center[1] -= 1.0*dy/wHeight;

    // direction in x-z plane
    eye_theta[0] += 1.0*dx;
    if(eye_theta[0]*EYE_ROTATION_COEFFICIENT < -360 || eye_theta[0]*EYE_ROTATION_COEFFICIENT > 360)
        eye_theta[0] = 0;

    // direction in vertical plane
    eye_theta[1] += 1.0*dy;
    if(eye_theta[1]*EYE_ROTATION_COEFFICIENT < -90)
        eye_theta[1] = -90*1.0/EYE_ROTATION_COEFFICIENT;
    if(eye_theta[1]*EYE_ROTATION_COEFFICIENT > 90)
        eye_theta[1] = 90*1.0/EYE_ROTATION_COEFFICIENT;

    updateView(wWidth, wHeight);
}


void GlAll::MoveControl()
{
    // pan control
    if(isCameraPan){
        eye_theta[0] += 2.0;
    }

    // orbit control
    if(isCameraOrbit && selectedObject!=NULL){
        Point3f objp = selectedObject->GetPosition();
        eye[0] = objp.x - orbitRadius*sin(eye_theta[0]*EYE_ROTATION_COEFFICIENT/180*PI);
        eye[2] = objp.z + orbitRadius*cos(eye_theta[0]*EYE_ROTATION_COEFFICIENT/180*PI);
        eye_theta[0] += 10.0;
    }
    else{
        isCameraOrbit = false;
    }


    // step forward and backward
    if(zoomAmount > ZOOM_THRESHOLD || zoomAmount < -ZOOM_THRESHOLD){
        zoomAmount = zoomAmount*0.8;
        eye[0] += zoomAmount*(eye_center[0] - eye[0]);
        eye[2] += zoomAmount*(eye_center[2] - eye[2]);
        if(zoomAmount < ZOOM_THRESHOLD && zoomAmount > -ZOOM_THRESHOLD) {
            zoomAmount = 0.f;
        }
        QThread::msleep(10);
    }

    // step side
    if(sideAmount > ZOOM_THRESHOLD || sideAmount < -ZOOM_THRESHOLD){
        sideAmount = sideAmount*0.8;
        eye[0] -= sideAmount*(eye_center[2] - eye[2]);
        eye[2] -= sideAmount*(-1)*(eye_center[0] - eye[0]);
        if(sideAmount < ZOOM_THRESHOLD && sideAmount > -ZOOM_THRESHOLD) {
            sideAmount = 0.f;
        }
        QThread::msleep(10);
    }

    // step up and down
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
    // calculate eye center coordinates
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

    // load textures
    texCrack = load_texture(CRACK);
    texSpot = load_texture(SPOT);
    texMonet = load_texture(MONET);
    texCustom = GenerateTex();
    texNightSky = load_texture(NIGHTSKY);
    texBalcony = load_texture(BALCONY);
    texCube = load_texture(CUBE);
    glBindTexture(GL_TEXTURE_2D, texBalcony);
    TextureColorkey(255,255,255,20);
    for(int i=0; i<IMAXFRAME; i++){
        texImax[i] = load_texture((QString(IMAX)+QString("teximax-")+QString::number(i,10)+QString(".bmp")).toStdString().c_str());
    }
    for(int i=0; i<POLLCOUNT; i++){
        texPoll[i] = load_texture((QString(POLL)+QString("poll-")+QString::number(i,10)+QString(".bmp")).toStdString().c_str());
    }



    eye[0] = 0;
    eye[1] = 0;
    eye[2] = 30;

    eye_center[0] = 0;
    eye_center[1] = 0;
    eye_center[2] = 0;

    eye_theta[0] = 0;
    eye_theta[1] = 0;


    zoomAmount = 0.;
    sideAmount = 0;
    updownAmount = 0;
    isCameraPan = false;
    isCameraOrbit = false;

    selectedObject = NULL;


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

    SetupScene();

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


vector<GLdouble> GlAll::screen2world(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    // get matrices
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;

    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
    // unproject to get the world coordinates
    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    vector<GLdouble> v;
    v.push_back(posX);
    v.push_back(posY);
    v.push_back(posZ);

    //GVector v(4, posX, posY, posZ, 1.0);
    return v;
}

