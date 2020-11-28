#include "cloth.h"
#include <iostream>

Cloth::Cloth()
{
}

Cloth::Cloth(clothParam params) :
    m_params(params)
{
    /**
     * We build in vertex data for a cube, in this case they are handwritten.
     * You'll want to expand upon or refactor this code heavily to take advantage
     * of polymorphism, vector math/glm library, and utilize good software design
     *
     */

    initialPosition();
    setVertex();



    /** build the VAO so that the shape is ready to be drawn */
    buildVAO();
}

Cloth::~Cloth()
{
}
void Cloth::initialPosition(){
    m_position.clear();
    for (int row = 0; row < m_params.dimension; row ++){
        for (int col = 0; col < m_params.dimension; col++){
            float x = (float(col)/(m_params.dimension - 1)) - 0.5;
            float y = 0.5 - (float(row)/(m_params.dimension - 1));
            m_position.push_back(glm::vec3(x,y,0));
        }
    }
}

void Cloth::setVertex(){
    m_vertexData.clear();
    glm::vec3 frontNormal(0,0,-1);
    glm::vec3 backNormal(0,0,1);

    for (int row = 0; row < m_params.dimension - 1; row ++){
        for (int col = 0; col < m_params.dimension - 1; col++ ){
            glm::vec3 topLeft = m_position.at(row * m_params.dimension + col);
            glm::vec3 topRight = m_position.at(row * m_params.dimension + (col + 1));
            glm::vec3 bottomLeft = m_position.at((row + 1) * m_params.dimension + col);
            glm::vec3 bottomRight = m_position.at((row + 1) * m_params.dimension + (col + 1));
            setVertexHelper(bottomLeft,topRight,topLeft,frontNormal);
            setVertexHelper(bottomRight,topRight,bottomLeft,frontNormal);
            setVertexHelper(bottomLeft,topLeft,topRight,backNormal);
            setVertexHelper(bottomRight,bottomLeft,topRight,backNormal);
        }
    }
}

void Cloth::setVertexHelper(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 normal){
    insertVec3(m_vertexData,p1);
    insertVec3(m_vertexData,normal);
    insertVec3(m_vertexData,p2);
    insertVec3(m_vertexData,normal);
    insertVec3(m_vertexData,p3);
    insertVec3(m_vertexData,normal);
}

