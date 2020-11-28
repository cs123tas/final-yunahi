#ifndef CLOTH_H
#define CLOTH_H

#include "Shape.h"

// hand-written cube points and normals.. if only there were a way to do this procedurally
#define CLOTH_DATA_POSITIONS {\
        -0.5f,-0.5f,-0.5f, \
        -1.f, 0.f, 0.f, \
        -0.5f,-0.5f, 0.5f,\
        -1.f, 0.f, 0.f, \
        -0.5f, 0.5f, 0.5f, \
        -1.f, 0.f, 0.f, \
        0.5f, 0.5f,-0.5f, \
        0.f, 0.f, -1.f, \
        -0.5f,-0.5f,-0.5f,\
        0.f, 0.f, -1.f, \
        -0.5f, 0.5f,-0.5f, \
        0.f, 0.f, -1.f, \
        0.5f,-0.5f, 0.5f, \
        0.f, -1.f, 0.f, \
        -0.5f,-0.5f,-0.5f, \
        0.f, -1.f, 0.f, \
        0.5f,-0.5f,-0.5f, \
        0.f, -1.f, 0.f, \
        0.5f, 0.5f,-0.5f, \
        0.f, 0.f, -1.f, \
        0.5f,-0.5f,-0.5f, \
        0.f, 0.f, -1.f, \
        -0.5f,-0.5f,-0.5f, \
        0.f, 0.f, -1.f, \
        -0.5f,-0.5f,-0.5f, \
        -1.f, 0.f, 0.f, \
        -0.5f, 0.5f, 0.5f,\
        -1.f, 0.f, 0.f, \
        -0.5f, 0.5f,-0.5f,\
        -1.f, 0.f, 0.f, \
        0.5f,-0.5f, 0.5f,\
        0.f, -1.f, 0.f, \
        -0.5f,-0.5f, 0.5f,\
        0.f, -1.f, 0.f, \
        -0.5f,-0.5f,-0.5f,\
        0.f, -1.f, 0.f, \
        -0.5f, 0.5f, 0.5f,\
        0.f, 0.f, 1.f, \
        -0.5f,-0.5f, 0.5f,\
        0.f, 0.f, 1.f, \
        0.5f,-0.5f, 0.5f,\
        0.f, 0.f, 1.f, \
        0.5f, 0.5f, 0.5f,\
        1.f, 0.f, 0.f, \
        0.5f,-0.5f,-0.5f,\
        1.f, 0.f, 0.f, \
        0.5f, 0.5f,-0.5f,\
        1.f, 0.f, 0.f, \
        0.5f,-0.5f,-0.5f,\
        1.f, 0.f, 0.f, \
        0.5f, 0.5f, 0.5f,\
        1.f, 0.f, 0.f, \
        0.5f,-0.5f, 0.5f,\
        1.f, 0.f, 0.f, \
        0.5f, 0.5f, 0.5f,\
        0.f, 1.f, 0.f, \
        0.5f, 0.5f,-0.5f,\
        0.f, 1.f, 0.f, \
        -0.5f, 0.5f,-0.5f,\
        0.f, 1.f, 0.f, \
        0.5f, 0.5f, 0.5f,\
        0.f, 1.f, 0.f, \
        -0.5f, 0.5f,-0.5f,\
        0.f, 1.f, 0.f, \
        -0.5f, 0.5f, 0.5f,\
        0.f, 1.f, 0.f, \
        0.5f, 0.5f, 0.5f,\
        0.f, 0.f, 1.f, \
        -0.5f, 0.5f, 0.5f,\
        0.f, 0.f, 1.f, \
        0.5f,-0.5f, 0.5f, \
        0.f, 0.f, 1.f}

class Cloth : public Shape
{
public:
    Cloth();
    Cloth(int param1, int param2);
    ~Cloth();

private:
    int m_param1;
    int m_param2;
};

#endif // CLOTH_H
