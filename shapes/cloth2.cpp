#include "cloth2.h"
#include <iostream>
#include <QCoreApplication>
#include <QtConcurrent/QtConcurrent>
#include <QThread>
Cloth2::Cloth2()
{
}

Cloth2::Cloth2(int dimension, float stiffness, float windVelocity, float windAngle):
    m_dimension(dimension),
    m_particleMass(1),
    m_windVelocity(windVelocity),
    m_windAngle(windAngle)



{

    buildVertexData();
    initialVertex();
    setVertex();
    buildVAO();
}

Cloth2::~Cloth2()
{
}


void Cloth2::buildVertexData(){
    m_vertexData.empty();
}

void Cloth2::initialVertex(){

    m_particles.clear();
    m_springs.clear();
    for (int row = 0; row < m_dimension; row ++){
        for (int col = 0; col < m_dimension; col++){
            float x = (float(col)/(m_dimension - 1)) - 0.5;
            float y = 0.5 - (float(row)/(m_dimension - 1));
            Particle p;
            p.pos = glm::vec3(x,y,0);
            p.oldPos = glm::vec3(x,y,0);
            p.normal = glm::vec3(0,0,-1);
            p.force = glm::vec3(0,0,0);
            p.uv = glm::vec2(x+0.5,y+0.5);
            if (row == 0 && (col == 0 || col == m_dimension - 1))
                p.pinned = true;
            else
                p.pinned = false;
            m_particles.push_back(p);

        }
    }
    for (int row = 0; row < m_dimension; row++){
        for (int col = 0; col < m_dimension; col++){
            addSprings(row, col);
        }
    }
}

void Cloth2::addSprings(int row, int col){

    float structuralLength =  (1.f/(m_dimension - 1));
    float shearLength = (sqrt(2)/(m_dimension - 1));

    //structural
    addSpringsHelper(row, col, row, col + 1, structuralLength);
    addSpringsHelper(row, col, row + 1, col, structuralLength);

    //shear
    addSpringsHelper(row, col, row + 1, col + 1, shearLength);
    addSpringsHelper(row, col + 1, row + 1, col, shearLength);


    addSpringsHelper(row, col, row, col + 2, structuralLength * 2);
    addSpringsHelper(row, col, row + 2 , col, structuralLength * 2);

    //shear
    addSpringsHelper(row, col, row + 2, col + 2, shearLength * 2);
    addSpringsHelper(row, col + 2, row + 2, col, shearLength * 2);




}

void Cloth2::addSpringsHelper(int row1, int col1, int row2, int col2, float restLength){
    if (validIndex(row1,col1) && validIndex(row2,col2)){
        Spring s(m_particles[row1 * m_dimension + col1],
                m_particles[row2 * m_dimension + col2],
                glm::distance(m_particles[row1 * m_dimension + col1].pos, m_particles[row2 * m_dimension + col2].pos));
        m_springs.push_back(s);
    }
}

void Cloth2::setVertex(){
    m_vertexData.clear();

    for (int row = 0; row < m_dimension - 1; row ++){
        for (int col = 0; col < m_dimension - 1; col++ ){

            Particle topLeft = m_particles.at(row * m_dimension + col);
            Particle topRight = m_particles.at(row * m_dimension + (col + 1));
            Particle bottomLeft = m_particles.at((row + 1) * m_dimension + col);
            Particle bottomRight = m_particles.at((row + 1) * m_dimension + (col + 1));


            setVertexHelper(bottomLeft,topRight,topLeft,true);
            setVertexHelper(bottomRight,topRight,bottomLeft,true);
            setVertexHelper(bottomLeft,topLeft,topRight,false);
            setVertexHelper(bottomRight,bottomLeft,topRight,false);
        }
    }
}

void Cloth2::setVertexHelper(Particle p1, Particle p2, Particle p3, bool front){



    //		Vec3 v1 = pos2-pos1;
    //		Vec3 v2 = pos3-pos1;

    //		return v1.cross(v2);

    glm::vec3 v1 = p2.pos - p1.pos;
    glm::vec3 v2 = p3.pos - p1.pos;
    glm::vec3 n = glm::normalize(glm::cross(v1,v2));

    insertVec3(m_vertexData,p1.pos);
    insertVec3(m_vertexData,n);
    if (front)
        insertVec2(m_vertexData,p1.uv);
    else
        insertVec2(m_vertexData,glm::vec2(0,0));


    insertVec3(m_vertexData,p2.pos);
    insertVec3(m_vertexData,n);
    if (front)
        insertVec2(m_vertexData,p2.uv);
    else
        insertVec2(m_vertexData,glm::vec2(0,0));


    insertVec3(m_vertexData,p3.pos);
    insertVec3(m_vertexData,n);
    if (front)
        insertVec2(m_vertexData,p3.uv);
    else
        insertVec2(m_vertexData,glm::vec2(0,0));

}

void Cloth2::update(){

    updateVertex();
    setVertex();
    buildVAO();
}

void Cloth2::updateVertex(){


    constraint();

    std::vector<QFuture<void>> threads;

    for (int row = 0; row < m_dimension; row++){
        threads.push_back(QtConcurrent::run(this,&Cloth2::updateVertexHelper,row));
    }

    for (int i = 0; i < threads.size();i++){
        threads[i].waitForFinished();
    }
    calculateNormals();
    collision();

}

void Cloth2::updateVertexHelper(int row){
    std::vector<std::vector<glm::vec3>> result;
    std::vector<glm::vec3> nextVelocity;
    std::vector<glm::vec3> nextPosition;

    float step = 0.01;
    float damping = 0.01;
    for (int col = 0; col < m_dimension; col++){
        if (!m_particles[row * m_dimension + col].pinned){
            netForce(row,col);
            glm::vec3 temp = m_particles[row*m_dimension+col].pos;
            m_particles[row*m_dimension+col].pos = m_particles[row*m_dimension+col].pos +
                    (m_particles[row*m_dimension+col].pos - m_particles[row*m_dimension+col].oldPos) * (1.0f - damping) +
                    (m_particles[row * m_dimension + col].force / m_particleMass) * step;
            m_particles[row * m_dimension + col].oldPos = temp;
            m_particles[row * m_dimension + col].force = glm::vec3(0,0,0);


        }
    }
}

void Cloth2::constraint(){


    for (int i = 0; i < 15; i ++){
        int size = m_springs.size()/5;
        for (int i = 0; i < 6; i++){
            QtConcurrent::run(this,&Cloth2::constraintHelper,i*size, (i+1)*size);

        }


    }
}

void Cloth2::constraintHelper(int start, int end){
    for (int springCount = start; springCount < std::min<int>(end,m_springs.size()); springCount++){
        glm::vec3 vec = m_springs[springCount].p2.pos - m_springs[springCount].p1.pos;
        float dist = glm::distance(m_springs[springCount].p2.pos,m_springs[springCount].p1.pos);
        glm::vec3 correct = vec * (1 - m_springs[springCount].restLength/dist);
        if (!m_springs[springCount].p1.pinned && !m_springs[springCount].p2.pinned){
            correct *= 0.5;
        }
        if (!m_springs[springCount].p1.pinned)
            m_springs[springCount].p1.pos += correct;
        if (!m_springs[springCount].p2.pinned)
            m_springs[springCount].p2.pos -= correct;
    }
}

void Cloth2::calculateNormals(){
    std::vector<QFuture<void>> threads;

    for (int row = 0; row < m_dimension; row++){
        threads.push_back(QtConcurrent::run(this,&Cloth2::calculateNormalsHelper,row));
    }
    for (int i = 0; i < threads.size();i++){
        threads[i].waitForFinished();

    }
}

void Cloth2::calculateNormalsHelper(int row){
    std::vector<int> dx {1,1,0,-1,-1,0};
    std::vector<int> dy {0,1,1,0,-1,-1};
    std::vector<glm::vec3> result;
    for (int col = 0; col < m_dimension; col++){

        glm::vec3 p = m_particles[row*m_dimension+col].pos;
        std::vector<glm::vec3> normals;
        for (int t = 0; t < 6; t++){
            int row1 = row + dy[t];
            int col1 = col + dx[t];
            int row2 = row + dy[(t+1)%6];
            int col2 = col + dx[(t+1)%6];
            if (validIndex(row1,col1)&&validIndex(row2,col2)){
                glm::vec3 e1 = m_particles[row1 * m_dimension + col1].pos - p;
                glm::vec3 e2 = m_particles[row2 * m_dimension + col2].pos - p;
                normals.push_back(glm::normalize(glm::cross(e1,e2)));
            }
        }
        glm::vec3 normalsSum;
        for (int t = 0; t < normals.size(); t++){
            normalsSum += normals[t];
        }
        m_particles[row * m_dimension + col].normal = glm::normalize(normalsSum);
    }
}

void Cloth2::netForce(int row, int col){


    m_particles[row * m_dimension + col].force = glm::vec3 (0,0,0);
    m_particles[row * m_dimension + col].force += gravityForce();
    m_particles[row * m_dimension + col].force += windForce(row, col);

    return;
}

glm::vec3 Cloth2::gravityForce(){
    return glm::vec3 (0,-0.2,0);

}

glm::vec3 Cloth2::windForce(int row, int col){

    float radian = m_windAngle*M_PI/180.f;
    float unknownVal = 0.1 * m_windVelocity;
    glm::vec3 direction = glm::vec3(sin(radian), 0, cos(radian));
    glm::vec3 force = m_particles[row * m_dimension + col].normal *glm::dot(m_particles[row * m_dimension + col].normal,direction) * unknownVal;

    return force;

}

bool Cloth2::validIndex(int row, int col){
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

void Cloth2::collision(){
    for (int i = 0; i < m_particles.size(); i++){
        if (m_particles[i].pos.z < 0){
            m_particles[i].pos.z = 0;
        }
    }
}






