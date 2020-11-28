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

struct clothParam{
    int dimension;
    float particleMass;
    int structuralStiffness;
    int shearStiffness;
    int bendStiffness;
    float damping;
    float viscous;
    int windVelocity;
    int windAngle;
};

class Cloth : public Shape
{
public:
    Cloth();
    Cloth(clothParam);
    ~Cloth();

    //fills m_position with the initial positions
    void initialPosition();

    //takes values in m_position and copies to m_vertexData with normals
    void setVertex();
    void setVertexHelper(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 normal);

private:
    clothParam m_params;
    std::vector<glm::vec3> m_position;

};

#endif // CLOTH_H
