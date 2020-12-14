#ifndef CLOTH_H
#define CLOTH_H

#include "Shape.h"



class Cloth : public Shape
{
public:
    Cloth();
    Cloth(int dimension, float particleMass, float windVelocity, float windAngle,
                 float stif1, float stif2, float stif3);
    ~Cloth();

private:
    int m_dimension;
    float m_particleMass;
    float m_windVelocity;
    float m_windAngle;
    float m_structuralStiffness;
    float m_shearStiffness;
    float m_bendStiffness;
    std::vector<glm::vec3> m_position;
    std::vector<glm::vec3> m_velocity;
    std::vector<glm::vec3> m_nextPosition;
    std::vector<glm::vec3> m_nextVelocity;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec2> m_uvs;


    void update() override;

protected:
    void buildVertexData() override;
    void initialVertex();
    void setVertex();
    void setVertexHelper(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
                         glm::vec3 n1, glm::vec3 n2, glm::vec3 n3,
                         glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3);
    void updateVertex();
    void calculateNormals();
    std::vector<glm::vec3> calculateNormalsHelper(int row);

     std::vector<std::vector<glm::vec3>> updateVertexHelper(int row);

     glm::vec3 netForce(int row, int col);
     bool validIndex(int row, int col);
     glm::vec3 springForce(glm::vec3 p, glm::vec3 q, float stiffness, float restLength);
     glm::vec3 dampingForce(glm::vec3 velocity);
     glm::vec3 gravityForce();
     glm::vec3 windForce(int row, int col);
     glm::vec3 structuralForce(int row, int col,
                              float stiffness, float restLength);
     glm::vec3 shearForce(int row, int col,
                         float stiffness, float restLength);
     glm::vec3 bendForce(int row, int col,
                        float stiffness, float restLength);


};

#endif // CUBE_H
