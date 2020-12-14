#ifndef SPUNSHAPE_H
#define SPUNSHAPE_H
#include <glm/glm.hpp>
#include "GL/glew.h"
#include "Shape.h"

class SpunShape : public Shape
{
public:
    SpunShape();
    SpunShape(int p1, int p2, int p3);
    virtual ~SpunShape();
protected:
    int m_param1;
    int m_param2;
    int m_param3;
    void spin(glm::vec3(*point)(float p1, float p2, float p3, float param1, float param2, float param3),
                  glm::vec3(*normal)(float p1, float p2, float p3, float param1, float param2, float param3),
                  float dir);

};

#endif // SPUNSHAPE_H
