#ifndef OBJFILE_H
#define OBJFILE_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include <GLUT/glut.h>
#include "misc.h"

using namespace std;

class ObjFileSupport
{
public:
    ObjFileSupport(){
        scale = 500;
        v_num=0, vn_num=0, f_num=0;
        filename = "";
    }
    ~ObjFileSupport(){}

    void ReadFile(string filename);
    void DrawModel();

private:
    int scale;
    int v_num, vn_num, f_num;
    GLint drawlist;
    string filename;

    vector<vector<float> > v;
    vector<vector<float> > vn;
    vector<vector<int> > fv;
    vector<vector<int> > fn;
    vector<float> point;
    vector<int> index1;
    vector<int> index2;

    GLint GenDrawList();
};

#endif // OBJFILE_H
