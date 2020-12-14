#include "cloth.h"
#include <iostream>
#include <QCoreApplication>
#include <QtConcurrent/QtConcurrent>
#include <QThread>

Cloth::Cloth()
{
}

Cloth::Cloth(int dimension, float particleMass, float windVelocity, float windAngle,
             float stif1, float stif2, float stif3):
    m_dimension(dimension),
    m_particleMass(800.f/pow(dimension,2)),
    m_windVelocity(windVelocity),
    m_windAngle(windAngle),
    m_structuralStiffness(26),
    m_shearStiffness(31),
    m_bendStiffness(24)


{

    buildVertexData();
    initialVertex();
    setVertex();
    buildVAO();
}

Cloth::~Cloth()
{
}


void Cloth::buildVertexData(){
    m_vertexData.empty();
}

void Cloth::initialVertex(){
    m_position.clear();
    m_velocity.clear();
    for (int row = 0; row < m_dimension; row ++){
        for (int col = 0; col < m_dimension; col++){
            float x = (float(col)/(m_dimension - 1)) - 0.5;
            float y = 0.5 - (float(row)/(m_dimension - 1));
            m_position.push_back(glm::vec3(x,y,0));
            m_velocity.push_back(glm::vec3(0,0,0));
            m_normals.push_back(glm::vec3(0,0,-1));
            m_uvs.push_back(glm::vec2(x+0.5,y+0.5));
        }
    }
}

void Cloth::setVertex(){
    m_vertexData.clear();
    glm::vec3 frontNormal(0,0,-1);
    glm::vec3 backNormal(0,0,1);

    for (int row = 0; row < m_dimension - 1; row ++){
        for (int col = 0; col < m_dimension - 1; col++ ){
            glm::vec3 topLeft = m_position.at(row * m_dimension + col);
            glm::vec3 topRight = m_position.at(row * m_dimension + (col + 1));
            glm::vec3 bottomLeft = m_position.at((row + 1) * m_dimension + col);
            glm::vec3 bottomRight = m_position.at((row + 1) * m_dimension + (col + 1));

            glm::vec3 topLeftNormal = m_normals.at(row * m_dimension + col);
            glm::vec3 topRightNormal = m_normals.at(row * m_dimension + (col + 1));
            glm::vec3 bottomLeftNormal = m_normals.at((row + 1) * m_dimension + col);
            glm::vec3 bottomRightNormal = m_normals.at((row + 1) * m_dimension + (col + 1));

            glm::vec2 topLeftUv = m_uvs.at(row * m_dimension + col);
            glm::vec2 topRightUv = m_uvs.at(row * m_dimension + (col + 1));
            glm::vec2 bottomLeftUv = m_uvs.at((row + 1) * m_dimension + col);
            glm::vec2 bottomRightUv = m_uvs.at((row + 1) * m_dimension + (col + 1));

            setVertexHelper(bottomLeft,topRight,topLeft,
                            bottomLeftNormal,topRightNormal,topLeftNormal,
                            bottomLeftUv,topRightUv,topLeftUv);
            setVertexHelper(bottomRight,topRight,bottomLeft,
                            bottomRightNormal,topRightNormal,bottomLeftNormal,
                            bottomRightUv,topRightUv,bottomLeftUv);

            setVertexHelper(bottomLeft,topLeft,topRight,
                            -bottomLeftNormal,-topLeftNormal,-topRightNormal,
                            bottomLeftUv,topLeftUv,topRightUv);
            setVertexHelper(bottomRight,bottomLeft,topRight,
                            -bottomRightNormal,-bottomLeftNormal,-topRightNormal,
                            bottomRightUv,bottomLeftUv,topRightUv);
        }
    }
}

void Cloth::setVertexHelper(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
                            glm::vec3 n1, glm::vec3 n2, glm::vec3 n3,
                            glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3){


    insertVec3(m_vertexData,p1);
    insertVec3(m_vertexData,-n1);
    insertVec2(m_vertexData,uv1);

    insertVec3(m_vertexData,p2);
    insertVec3(m_vertexData,-n2);
    insertVec2(m_vertexData,uv2);


    insertVec3(m_vertexData,p3);
    insertVec3(m_vertexData,-n3);
    insertVec2(m_vertexData,uv3);


}

void Cloth::update(){
    updateVertex();
    setVertex();
    buildVAO();
}

std::vector<std::vector<glm::vec3>> Cloth::updateVertexHelper(int row){
    std::vector<std::vector<glm::vec3>> result;
    std::vector<glm::vec3> nextVelocity;
    std::vector<glm::vec3> nextPosition;


    float step = 0.005;
    for (int col = 0; col < m_dimension; col++){
        glm::vec3 acceleration = netForce(row,col)/(m_particleMass*0.1f);
        glm::vec3 nextVel = glm::vec3(m_velocity[row*m_dimension+col] + acceleration * step);
        glm::vec3 nextPos = glm::vec3(m_position[row*m_dimension+col] + nextVel * step);
        if (nextPos.z < 0.f){
            nextPos.z = 0.f;
            nextVel.x = 0.f;
            nextVel.y = 0.f;
            nextVel.z = 0.f;

        }
        nextVelocity.push_back(nextVel);
        nextPosition.push_back(nextPos);

    }
    result.push_back(nextPosition);
    result.push_back(nextVelocity);

    return result;
}

void Cloth::calculateNormals(){
    //fix later
    m_normals.clear();
    std::vector<QFuture<std::vector<glm::vec3>>> threads;

    for (int row = 0; row < m_dimension; row++){
        threads.push_back(QtConcurrent::run(this,&Cloth::calculateNormalsHelper,row));
    }
    for (int i = 0; i < threads.size();i++){
        std::vector<glm::vec3>ret = threads[i].result();
        m_normals.insert(m_normals.end(), ret.begin(), ret.end());

    }
}

std::vector<glm::vec3> Cloth::calculateNormalsHelper(int row){
    std::vector<int> dx {1,1,0,-1,-1,0};
    std::vector<int> dy {0,1,1,0,-1,-1};
    std::vector<glm::vec3> result;
    for (int col = 0; col < m_dimension; col++){

        glm::vec3 p = m_position[row * m_dimension + col];
        std::vector<glm::vec3> normals;
        for (int t = 0; t < 6; t++){
            int row1 = row + dy[t];
            int col1 = col + dx[t];

            int row2 = row + dy[(t+1)%6];
            int col2 = col + dx[(t+1)%6];
            if (validIndex(row1,col1)&&validIndex(row2,col2)){
                glm::vec3 e1 = m_position[row1 * m_dimension + col1] - p;
                glm::vec3 e2 = m_position[row2 * m_dimension + col2] - p;
                normals.push_back(glm::normalize(glm::cross(e1,e2)));
            }
        }
        glm::vec3 normalsSum;
        for (int t = 0; t < normals.size(); t++){
            normalsSum += normals[t];
        }
        result.push_back(glm::normalize(normalsSum));

    }
    return result;
}


void Cloth::updateVertex(){


    calculateNormals();
    std::vector<QFuture<std::vector<std::vector<glm::vec3>>>> threads;

    for (int row = 0; row < m_dimension; row++){
        threads.push_back(QtConcurrent::run(this,&Cloth::updateVertexHelper,row));
    }

    for (int i = 0; i < threads.size();i++){
        std::vector<std::vector<glm::vec3>>ret = threads[i].result();
        m_nextPosition.insert(m_nextPosition.end(), ret[0].begin(), ret[0].end());
        m_nextVelocity.insert(m_nextVelocity.end(), ret[1].begin(), ret[1].end());

    }
    m_nextVelocity[0] = glm::vec3(0,0,0);
    m_nextPosition[0] = glm::vec3(-0.5,0.5,0);
    m_nextVelocity[m_dimension - 1] = glm::vec3(0,0,0);
    m_nextPosition[m_dimension - 1] = glm::vec3(0.5,0.5,0);

    //transfer and clear
    m_position = m_nextPosition;
    m_velocity = m_nextVelocity;
    m_nextPosition.clear();
    m_nextVelocity.clear();


}



glm::vec3 Cloth::netForce(int row, int col){
    glm::vec3 netForce(0,0,0);


    float rs =  (1.f/(m_dimension - 1));
    float rsh = (sqrt(2)/(m_dimension - 1));
    float rb= (2.f/(m_dimension - 1));

    netForce += structuralForce(row,col,m_structuralStiffness,rs);
    netForce += shearForce(row,col, m_shearStiffness,rsh);
    netForce += bendForce(row,col, m_bendStiffness,rb);


    netForce += gravityForce();
    netForce += windForce(row, col);
    netForce += dampingForce(m_velocity[row * m_dimension + col]);

    return netForce;
}

bool Cloth::validIndex(int row, int col){
    if (row >=m_dimension)
        return false;
    if (row < 0)
        return false;
    if (col >= m_dimension)
        return false;
    if (col < 0)
        return false;
    return true;
}

glm::vec3 Cloth::springForce(glm::vec3 p, glm::vec3 q, float stiffness, float restLength){
    glm::vec3 e = p - q;
    float length = glm::length(e);
    return glm::vec3(((stiffness)  *100* (restLength - length) * 1.f/length) * e);
}

glm::vec3 Cloth::dampingForce(glm::vec3 velocity){
    float damping = 1;
    return -damping * 0.1f * velocity;
}

glm::vec3 Cloth::gravityForce(){
    float g = -9.8;
    return glm::vec3 (0,(m_particleMass*0.1f) * g,0);
}

glm::vec3 Cloth::windForce(int row, int col){

    float radian = m_windAngle*M_PI/180.f;
    float unknownVal = 0.1 * m_windVelocity;
    glm::vec3 direction = glm::vec3(sin(radian), 0, cos(radian));
    glm::vec3 force = m_normals.at(row * m_dimension + col) *glm::dot(m_normals.at(row * m_dimension + col),direction) * unknownVal;

//    return glm::vec3(unknownVal * sin(radian), 0, unknownVal * cos(radian));
    return force;

}

glm::vec3 Cloth::structuralForce(int row, int col,
                                 float stiffness, float restLength){
    glm::vec3 force(0,0,0);
    if (validIndex(row + 1, col))
        force += springForce(m_position[row*m_dimension+col],m_position[(row + 1) * m_dimension + (col)],
                                stiffness,restLength);
    if (validIndex(row - 1, col))
        force += springForce(m_position[row*m_dimension+col],m_position[(row - 1) * m_dimension + (col)],
                                stiffness,restLength);
    if (validIndex(row, col + 1))
        force += springForce(m_position[row*m_dimension+col],m_position[(row) * m_dimension + (col + 1)],
                                stiffness,restLength);
    if (validIndex(row, col - 1))
        force += springForce(m_position[row*m_dimension+col],m_position[(row) * m_dimension + (col - 1)],
                                stiffness,restLength);
    return force;
}

glm::vec3 Cloth::shearForce(int row, int col, float stiffness, float restLength){
    glm::vec3 force(0,0,0);

    if (validIndex(row + 1, col + 1))
        force += springForce(m_position[row*m_dimension+col],m_position[(row + 1)*m_dimension+(col + 1)],
                                stiffness,restLength);
    if (validIndex(row + 1, col - 1))
        force += springForce(m_position[row*m_dimension+col],m_position[(row + 1)*m_dimension+(col - 1)],
                                stiffness,restLength);
    if (validIndex(row - 1, col + 1))
        force += springForce(m_position[row*m_dimension+col],m_position[(row - 1)*m_dimension+(col + 1)],
                                stiffness,restLength);
    if (validIndex(row - 1, col - 1))
        force += springForce(m_position[row*m_dimension+col],m_position[(row - 1)*m_dimension+(col - 1)],
                                stiffness,restLength);
    return force;
}

glm::vec3 Cloth::bendForce(int row, int col, float stiffness, float restLength){
    glm::vec3 force(0,0,0);

    if (validIndex(row + 2, col))
        force += springForce(m_position[row*m_dimension+col],m_position[(row + 2)*m_dimension+(col)],
                                stiffness,restLength);
    if (validIndex(row - 2, col))
        force += springForce(m_position[row*m_dimension+col],m_position[(row - 2)*m_dimension+(col)],
                                stiffness,restLength);
    if (validIndex(row, col + 2))
        force += springForce(m_position[row*m_dimension+col],m_position[(row)*m_dimension+(col + 2)],
                                stiffness,restLength);
    if (validIndex(row, col - 2))
        force += springForce(m_position[row*m_dimension+col],m_position[(row)*m_dimension+(col - 2)],
                                stiffness,restLength);
    return force;

}



