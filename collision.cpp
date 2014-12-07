#include "collision.h"

CollisionDetector::CollisionDetector()
{
    original[0] = 0;
    original[1] = 0;
    original[2] = 30;
}

CollisionDetector::CollisionDetector(float x, float y, float z)
{
    original[0] = x;
    original[1] = y;
    original[2] = z;
}


void CollisionDetector::CollisionProcess(vector<ObjectFramework*> objectList, float eye[])
{
    for(int i = 0;i<objectList.size();i++)
        {
            Dimension3f dimension = objectList.at(i)->GetEnvelopingDimension();
            // scale
            float scale = objectList.at(i)->GetScale();
            dimension.height *= scale;
            dimension.width *= scale;
            dimension.length *= scale;
            //长方体框架中心
            Point3f point = objectList.at(i)->GetPosition();
//            qDebug()<<(objectList.at(i)->GetObjectFrameworkName()).c_str()<<"eye:"<<eye[0]<<eye[1]<<eye[2];
//            qDebug()<<(objectList.at(i)->GetObjectFrameworkName()).c_str()<<"dimension"<<dimension.length<<dimension.width<<dimension.height;
//            qDebug()<<(objectList.at(i)->GetObjectFrameworkName()).c_str()<<"point"<<point.x<<point.y<<point.z;

            // determine whether there exist a collision
            if(eye[0]<point.x+dimension.length/2&&eye[0]>point.x-dimension.length/2)
                if(eye[1]<point.y+dimension.width/2&&eye[1]>point.y-dimension.width/2)
                    if(eye[2]<point.z+dimension.height/2&&eye[2]>point.z-dimension.height/2)
                    {
                        //根据坐标，发现碰撞，接下来进行处理
                        DEBUG("collision");
                        if(original[0]>point.x+dimension.length/2)
                        {
                            float proportion = (eye[0] - (point.x+dimension.length/2))/(point.x+dimension.length/2-original[0]);
                            //计算垂直距离比例
                            float eye_y = eye[1] + proportion*(original[1]-eye[1])/(proportion+1);
                            float eye_z = eye[2] + proportion*(original[2]-eye[2])/(proportion+1);
                            //计算交点的坐标，由于是平面，所以坐标只有两个
                            if(eye_z<point.z+dimension.height/2&&eye_z>point.z-dimension.height/2)
                                if(eye_y<point.y+dimension.width/2&&eye_y>point.y-dimension.width/2)
                                {
                                    //如果交点在长方体的平面内，则将此点定位新视点
                                    eye[0] = point.x+dimension.length/2;
                                    eye[1] = eye_y;
                                    eye[2] = eye_z;
                                    //由于只可能有一个交点，故此处可直接跳出
                                    break;
                                }
                        }
                        //以下五个条件分支与上面类似，分别检测剩余五个面
                        else if(original[0]<point.x-dimension.length/2)
                        {
                            float proportion = (eye[0] - (point.x-dimension.length/2))/(point.x-dimension.length/2-original[0]);
                            float eye_y = eye[1] + proportion*(original[1]-eye[1])/(proportion+1);
                            float eye_z = eye[2] + proportion*(original[2]-eye[2])/(proportion+1);
                            if(eye_z<point.z+dimension.height/2&&eye_z>point.z-dimension.height/2)
                                if(eye_y<point.y+dimension.width/2&&eye_y>point.y-dimension.width/2)
                                {
                                    eye[0] = point.x-dimension.length/2;
                                    eye[1] = eye_y;
                                    eye[2] = eye_z;
                                    break;
                                }
                        }
                        if(original[1]>point.y+dimension.width/2)
                        {
                            float proportion = (eye[1] - (point.y+dimension.width/2))/(point.y+dimension.width/2-original[1]);
                            float eye_x = eye[0] + proportion*(original[0]-eye[0])/(proportion+1);
                            float eye_z = eye[2] + proportion*(original[2]-eye[2])/(proportion+1);
                            if(eye_z<point.z+dimension.height/2&&eye_z>point.z-dimension.height/2)
                                if(eye_x<point.x+dimension.length/2&&eye_x>point.x-dimension.length/2)
                                {
                                    eye[1] = point.y+dimension.width/2;
                                    eye[0] = eye_x;
                                    eye[2] = eye_z;
                                    break;
                                }
                        }
                        else if(original[1]<point.y-dimension.width/2)
                        {
                            float proportion = (eye[1] - (point.y-dimension.width/2))/(point.y-dimension.width/2-original[1]);
                            float eye_x = eye[0] + proportion*(original[0]-eye[0])/(proportion+1);
                            float eye_z = eye[2] + proportion*(original[2]-eye[2])/(proportion+1);
                            if(eye_z<point.z+dimension.height/2&&eye_z>point.z-dimension.height/2)
                                if(eye_x<point.x+dimension.length/2&&eye_x>point.x-dimension.length/2)
                                {
                                    eye[1] = point.y-dimension.width/2;
                                    eye[0] = eye_x;
                                    eye[2] = eye_z;
                                    break;
                                }
                        }
                        if(original[2]>point.z+dimension.height/2)
                        {
                            float proportion = (eye[2] - (point.z+dimension.height/2))/(point.z+dimension.height/2-original[2]);
                            float eye_y = eye[1] + proportion*(original[1]-eye[1])/(proportion+1);
                            float eye_x = eye[0] + proportion*(original[0]-eye[0])/(proportion+1);
                            if(eye_x<point.x+dimension.length/2&&eye_x>point.x-dimension.length/2)
                                if(eye_y<point.y+dimension.width/2&&eye_y>point.y-dimension.width/2)
                                {
                                    eye[2] = point.z+dimension.height/2;
                                    eye[1] = eye_y;
                                    eye[0] = eye_x;
                                    break;
                                }
                        }
                        else if(original[2]<point.z-dimension.height/2)
                        {
                            float proportion = (eye[2] - (point.z-dimension.height/2))/(point.z-dimension.height/2-original[2]);
                            float eye_y = eye[1] + proportion*(original[1]-eye[1])/(proportion+1);
                            float eye_x = eye[0] + proportion*(original[0]-eye[0])/(proportion+1);
                            if(eye_x<point.x+dimension.length/2&&eye_x>point.x-dimension.length/2)
                                if(eye_y<point.y+dimension.width/2&&eye_y>point.y-dimension.width/2)
                                {
                                    eye[2] = point.z-dimension.height/2;
                                    eye[1] = eye_y;
                                    eye[0] = eye_x;
                                    break;
                                }
                        }
                    }
        }

}
