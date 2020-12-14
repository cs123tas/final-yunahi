#ifndef CLOTH2_H
#define CLOTH2_H

#include "Shape.h"


struct Particle{
    glm::vec3 force;
    glm::vec3 pos;
    glm::vec3 oldPos;
    glm::vec3 normal;
    glm::vec2 uv;
    bool pinned;
};

struct Spring{
    Particle& p1;
    Particle& p2;
    float restLength;
    Spring(Particle& one, Particle& two, float rl):
        p1(one),p2(two),restLength(rl){}

};

class Cloth2 : public Shape
{
public:
    Cloth2();
    Cloth2(int dimension, float stiffness, float windVelocity, float windAngle);
    ~Cloth2();

private:
    //parameters
    int m_dimension;
    float m_particleMass;
    float m_windVelocity;
    float m_windAngle;
    //new
    std::vector<Particle> m_particles;
    std::vector<Spring> m_springs;
    //override
    void update() override;


protected:
    void buildVertexData() override;
    void initialVertex();
    void addSprings(int row, int col);
    void addSpringsHelper(int row1, int col1, int row2, int col2, float restLength);
    void setVertex();
    void setVertexHelper(Particle p1, Particle p2, Particle p3, bool front);

    void updateVertex();
    void updateVertexHelper(int row);
    void calculateNormals();
    void calculateNormalsHelper(int row);
    void netForce(int row, int col);
    glm::vec3 gravityForce();
    glm::vec3 windForce(int row, int col);
    bool validIndex(int row, int col);

    void constraint();
    void constraintHelper(int start, int end);
    void collision();

};

#endif // CUBE_H
