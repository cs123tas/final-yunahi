#ifndef CLOTH_H
#define CLOTH_H

#include "Shape.h"
#include <QImage>
#include "gl/textures/Texture2D.h"

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

struct restLength{
    float structure;
    float shear;
    float bend;
};

class Cloth : public Shape
{
public:
    Cloth();
    Cloth(clothParam, int);
    ~Cloth();


    //fills m_position with the initial positions and velocity
    void initialVertex();

    //sets m_restLength
    void setRestLength();

    //takes values in m_position and copies to m_vertexData with normals
    void setVertex();
//    void setVertexHelper(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 normal);
    void setVertexHelper(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 normal,
                                glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3);

    //given two particles positions p and q connected by a spring with
    //stiffness K and restLength L,returns the spring force on p
    glm::vec3 springForce(glm::vec3 p, glm::vec3 q, float stiffness, float restLength);
    glm::vec3 dampingForce(glm::vec3 velocity);
    glm::vec3 viscousForce(glm::vec3 velocity);
    glm::vec3 gravityForce();
    glm::vec3 windForce();
    glm::vec3 netForce(int row, int col);

    //utils
    glm::vec3 getIndexAt(int row, int col,std::vector<glm::vec3> vector);
    bool validIndex(int row, int col);
    CS123::GL::Texture2D loadTexture();
    void timeInvariant();

    //updates
    void update();

    virtual void restartAnimationCloth() override;

private:
    clothParam m_params;
    int m_tempTime;
    std::vector<glm::vec3> m_position;
    std::vector<glm::vec3> m_velocity;
    restLength m_restLength;
    std::vector<glm::vec3> m_nextPosition;
    std::vector<glm::vec3> m_nextVelocity;
    float m_time;
    float m_step;
    std::vector<glm::vec2> m_uv;
    glm::vec3 m_wind;
    glm::vec3 m_gravity;


};

#endif // CLOTH_H
