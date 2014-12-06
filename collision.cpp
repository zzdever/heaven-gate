#include "collision.h"

CollisionDetector::CollisionDetector()
{
    for(int i = 0;i<3;i++)
    {
        original[i] = 0;
    }
}

CollisionDetector::CollisionDetector(float start_point[])
{
    for(int i = 0;i<3;i++)
    {
        original[i] = start_point[i];
    }
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
            Point3f point = objectList.at(i)->GetPosition();
//            qDebug()<<(objectList.at(i)->GetObjectFrameworkName()).c_str()<<"eye:"<<eye[0]<<eye[1]<<eye[2];
//            qDebug()<<(objectList.at(i)->GetObjectFrameworkName()).c_str()<<"dimension"<<dimension.length<<dimension.width<<dimension.height;
//            qDebug()<<(objectList.at(i)->GetObjectFrameworkName()).c_str()<<"point"<<point.x<<point.y<<point.z;

            if(eye[0]<point.x+dimension.length/2&&eye[0]>point.x-dimension.length/2)
                if(eye[1]<point.y+dimension.width/2&&eye[1]>point.y-dimension.width/2)
                    if(eye[2]<point.z+dimension.height/2&&eye[2]>point.z-dimension.height/2)
                    {
                        cout<<"collision"<<endl;
                        if(original[0]>point.x+dimension.length/2)
                        {
                            float proportion = (eye[0] - (point.x+dimension.length/2))/(point.x+dimension.length/2-original[0]);
                            float eye_y = eye[1] + proportion*(original[1]-eye[1])/(proportion+1);
                            float eye_z = eye[2] + proportion*(original[2]-eye[2])/(proportion+1);
                            if(eye_z<point.z+dimension.height/2&&eye_z>point.z-dimension.height/2)
                                if(eye_y<point.y+dimension.width/2&&eye_y>point.y-dimension.width/2)
                                {
                                    eye[0] = point.x+dimension.length/2;
                                    eye[1] = eye_y;
                                    eye[2] = eye_z;
                                    break;
                                }
                        }
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
