#include "gllight.h"

GlLight::GlLight()
{
}


//void GlLight::Draw()
//{
//    glEnable(GL_LIGHTING);
//    glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

//    GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };

//    // diffusion light settings
//    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
//    glLightfv(GL_LIGHT0, GL_AMBIENT, white);
//    glLightfv(GL_LIGHT0, GL_EMISSION, white);
//    glEnable(GL_LIGHT0);
//    // draw the diffusion light
//    glPushMatrix();
//    glTranslatef(light_pos[0], light_pos[1], light_pos[2]);
//    glMaterialfv(GL_FRONT, GL_EMISSION, white);
//    glutSolidSphere(0.1, 300, 300);
//    {
//        float def_emi[] = {0.f, 0.f, 0.f, 1.f};
//        glMaterialfv(GL_FRONT, GL_EMISSION, def_emi);
//    }
//    glPopMatrix();
//}
