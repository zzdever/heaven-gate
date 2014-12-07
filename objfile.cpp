#include "objfile.h"


void ObjFileSupport::ReadFile(string filename) {

    if(this->filename == filename){
        return;
    }
    else{
        this->filename = filename;
    }

    GLfloat x, y, z;
    string type;
    string buffer;

    // file path
    ifstream fin(filename.c_str());
    if (!fin){
        cout<<"file not found: "<<filename<<endl;
        return;
    }

    string newLine;
    while (getline(fin, newLine)) {
        if (newLine[0] == 'v') {
            if (newLine[1] == 'n') {        // vn x y z
                istringstream stream(newLine);
                stream >> type >> x >> y >> z;
                point.push_back(x);
                point.push_back(y);
                point.push_back(z);
                vn.push_back(point);
                //                cout << "vn[" << vn_num+1 << "] = (" << vn.at(vn_num).at(0) << ", "
                //                     <<  vn.at(vn_num).at(1) << ", " << vn.at(vn_num).at(2) << ")" << endl;
                vn_num++;
                for (int i=0; i<3; i++) {
                    point.pop_back();
                }
            }
            else {
                if (newLine[1] != 't') {    // v x y z, ignore vt
                    istringstream stream(newLine);
                    stream >> type >> x >> y >> z;
                    point.push_back(x);
                    point.push_back(y);
                    point.push_back(z);
                    v.push_back(point);
                    //                    cout << "v[" << v_num+1 << "] = (" << v.at(v_num).at(0) << ", "
                    //                         <<  v.at(v_num).at(1) << ", " << v.at(v_num).at(2) << ")" << endl;
                    v_num++;
                    for (int i=0; i<3; i++) {
                        point.pop_back();
                    }
                }
            }
        }
        else {
            if (newLine[0] == 'f') {        // f
                istringstream stream(newLine);
                stream >> type;             // eliminate f
                for (int i=0; i<3; i++) {
                    stream >> buffer;
                    int k;
                    int num1=0, num2=0;
                    for (k=0; buffer[k]!='/'; k++)
                        num1 = num1*10 + buffer[k] - 48;
                    index1.push_back(num1);
                    for (k++; buffer[k]!='/'; k++)  {
                        ;                   // ignore ft
                    }
                    for (k++; k<buffer.size(); k++)
                        num2 = num2*10 + buffer[k] - 48;
                    index2.push_back(num2);
                }
                fv.push_back(index1);
                fn.push_back(index2);
                //                cout << "fv[" << f_num+1 << "] = (" << fv.at(f_num).at(0) << " "
                //                <<  fv.at(f_num).at(1) << " " << fv.at(f_num).at(2) << ")" << " ";
                //                cout << "fn[" << f_num+1 << "] = (" << fn.at(f_num).at(0) << " "
                //                <<  fn.at(f_num).at(1) << " " << fn.at(f_num).at(2) << ")" << endl;
                f_num++;
                for (int i=0; i<3; i++) {
                    index1.pop_back();
                    index2.pop_back();
                }
            }
            else {                          // # || g || s || use...
                ;
                //                cout << newLine << endl;
            }
        }
    }
    fin.close();

    drawlist = GenDrawList();

    DEBUG("obj file read done, draw list gen done");
}


void ObjFileSupport::DrawModel(){
    glCallList(drawlist);
    return;
}


GLint ObjFileSupport::GenDrawList(){
    GLint lid = 0;
    lid = glGenLists(1);
    glNewList(lid, GL_COMPILE);

    glPushMatrix();

//    GLfloat color[] = {0, 0, 0, 1.0};
//    glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,color);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, color);


    // draw vertices
    for (int i=0; i<f_num; i++) {
        glBegin(GL_TRIANGLES);
        glColor3f(0.78125, 0.70588, 0.49804);

        glNormal3f(vn.at(fn.at(i).at(0) -1).at(0), vn.at(fn.at(i).at(0) -1).at(1), vn.at(fn.at(i).at(0) -1).at(2));
        glVertex3f(v.at(fv.at(i).at(0) -1).at(0)/scale, v.at(fv.at(i).at(0) -1).at(1)/scale, v.at(fv.at(i).at(0) -1).at(2)/scale);

        glNormal3f(vn.at(fn.at(i).at(1) -1).at(0), vn.at(fn.at(i).at(1) -1).at(1), vn.at(fn.at(i).at(1) -1).at(2));
        glVertex3f(v.at(fv.at(i).at(1) -1).at(0)/scale, v.at(fv.at(i).at(1) -1).at(1)/scale, v.at(fv.at(i).at(1) -1).at(2)/scale);

        glNormal3f(vn.at(fn.at(i).at(2) -1).at(0), vn.at(fn.at(i).at(2) -1).at(1), vn.at(fn.at(i).at(2) -1).at(2));
        glVertex3f(v.at(fv.at(i).at(2) -1).at(0)/scale, v.at(fv.at(i).at(2) -1).at(1)/scale, v.at(fv.at(i).at(2) -1).at(2)/scale);
        glEnd();
    }


    glPopMatrix();

    glEndList();

    return lid;

}
