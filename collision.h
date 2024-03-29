#ifndef COLLISION_H
#define COLLISION_H

#include <string>

#include "misc.h"
#include "globject.h"

class CollisionDetector
{
public:
    CollisionDetector();
    CollisionDetector(float x, float y, float z);

    void CollisionProcess(vector<ObjectFramework*> objectList, float eye[]);

private:
    float original[3];
};

#endif // COLLISION_H
