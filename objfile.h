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
        scale = 100;
        v_num=0, vn_num=0, f_num=0;
    }
    ~ObjFileSupport(){}

    void ReadFile(string filename);
    GLint GenDrawList();

    void DrawModel();

private:
    int scale;
    int v_num, vn_num, f_num;
    GLint drawlist;

    vector<vector<float> > v;
    vector<vector<float> > vn;
    vector<vector<int> > fv;
    vector<vector<int> > fn;
    vector<float> point;
    vector<int> index1;
    vector<int> index2;
};

#endif // OBJFILE_H
