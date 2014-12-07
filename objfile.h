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
    int scale;  // 由于不同文件顶点坐标范围不同，在绘制时用于缩放模型大小
    int v_num, vn_num, f_num;   // 记录顶点、法向和三角形面片的数量
    GLint drawlist;
    string filename;

    vector<vector<float> > v;   // 用二维向量保存顶点坐标
    vector<vector<float> > vn;  // 用二维向量保存顶点法向
    vector<vector<int> > fv;    // 用二维数组保存三角形面片顶点的索引
    vector<vector<int> > fn;    // 用二维数组保存三角形面片顶点法向的索引
    vector<float> point;    // 一维向量用于保存坐标和法向的信息
    vector<int> index1; // 一维向量用于保存三角形面片顶点坐标索引
    vector<int> index2; // 一维向量用于保存三角形面片顶点法向索引

    GLint GenDrawList();
};

#endif // OBJFILE_H
