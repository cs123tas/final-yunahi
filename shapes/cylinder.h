#ifndef CYLINDER_H
#define CYLINDER_H

#include "spunshape.h"

class Cylinder : public SpunShape
{
public:
    Cylinder();
    Cylinder(int param1, int param2);
    ~Cylinder();
    static glm::vec3 bottomCap( float p1, float p2, float p3,float param1, float param2,float param3);
    static glm::vec3 bottomCapNormal(float p1, float p2, float p3, float param1, float param2,float param3);


private:

protected:
    void buildVertexData() override;

    float perimeter(float x, float z);

    static glm::vec3 topCap( float p1, float p2, float p3, float param1, float param2,float param3);
    static glm::vec3 topCapNormal(float p1, float p2, float p3, float param1, float param2,float param3);
    static glm::vec3 wrap(float p1, float p2, float p3, float param1, float param2,float param3);
    static glm::vec3 wrapNormal(float p1, float p2, float p3, float param1, float param2,float param3);
};

#endif // CYLINDER_H
