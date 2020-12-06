#include "cloth.h"
#include <iostream>
#include<QGLWidget>
#include "gl/textures/Texture2D.h"
#include "gl/textures/TextureParametersBuilder.h"
#include "gl/textures/TextureParameters.h"
#include <QDebug>
#include <QThread>
#include <QString>
#include <QFuture>
#include <QApplication>
#include <QtConcurrent/QtConcurrent>

Cloth::Cloth()
{
}


Cloth::Cloth(clothParam params,int time) :
    m_params(params),
    m_tempTime(time)

{
    /**
     * We build in vertex data for a cube, in this case they are handwritten.
     * You'll want to expand upon or refactor this code heavily to take advantage
     * of polymorphism, vector math/glm library, and utilize good software design
     *
     */
    m_step = 0.005;

    initialVertex();
    setRestLength();
//    loadTexture();


    setVertex();
    /** build the VAO so that the shape is ready to be drawn */
    buildVAO();
}

Cloth::~Cloth()
{
}
void Cloth::initialVertex(){
    m_position.clear();
    m_velocity.clear();
    m_uv.clear();
    for (int row = 0; row < m_params.dimension; row ++){
        for (int col = 0; col < m_params.dimension; col++){
            float x = (float(col)/(m_params.dimension - 1)) - 0.5;
            float y = 0.5 - (float(row)/(m_params.dimension - 1));
            m_position.push_back(glm::vec3(x,y,0));
            m_velocity.push_back(glm::vec3(0,0,0));
            m_uv.push_back(glm::vec2(x+0.5,y+0.5));
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

            //ERROR
            glm::vec2 topLeftUv = m_uv.at(row * m_params.dimension + col);
            glm::vec2 topRightUv = m_uv.at(row * m_params.dimension + (col + 1));
            glm::vec2 bottomLeftUv = m_uv.at((row + 1) * m_params.dimension + col);
            glm::vec2 bottomRightUv = m_uv.at((row + 1) * m_params.dimension + (col + 1));

            setVertexHelper(bottomLeft,topRight,topLeft,frontNormal,
                            bottomLeftUv,topRightUv,topLeftUv);
            setVertexHelper(bottomRight,topRight,bottomLeft,frontNormal,
                            bottomRightUv,topRightUv,bottomLeftUv);
            setVertexHelper(bottomLeft,topLeft,topRight,backNormal,
                            bottomLeftUv,topLeftUv,topRightUv);
            setVertexHelper(bottomRight,bottomLeft,topRight,backNormal,
                            bottomRightUv,bottomLeftUv,topRightUv);
        }
    }
}

void Cloth::timeInvariant(){
    m_wind = windForce();
    m_gravity = gravityForce();
}

void Cloth::setVertexHelper(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 normal,
                            glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3){

    //textureHelp1

    insertVec3(m_vertexData,p1);
    insertVec3(m_vertexData,normal);
//    insertVec2(m_vertexData,uv1);

    insertVec3(m_vertexData,p2);
    insertVec3(m_vertexData,normal);
//    insertVec2(m_vertexData,uv2);

    insertVec3(m_vertexData,p3);
    insertVec3(m_vertexData,normal);
//    insertVec2(m_vertexData,uv3);

}

void Cloth::setRestLength(){
    m_restLength.structure =  (1.f/(m_params.dimension - 1));
    m_restLength.shear = (sqrt(2)/(m_params.dimension - 1));
    m_restLength.bend = (2.f/(m_params.dimension - 1));
}

glm::vec3 Cloth::springForce(glm::vec3 p, glm::vec3 q, float stiffness, float restLength){
//    return glm::vec3(stiffness * 1000 * (restLength - glm::distance(p,q)) * glm::normalize(p-q));
    glm::vec3 e = p - q;
    float length = glm::length(e);
    return glm::vec3(((stiffness)  *100* (restLength - length) * 1.f/length) * e);

    //check if this is correct
}

glm::vec3 Cloth::dampingForce(glm::vec3 velocity){
    return -m_params.damping * 0.1f * velocity;
}

glm::vec3 Cloth::viscousForce(glm::vec3 velocity){
    glm::vec3 u (0,0,1.0);
    return m_params.viscous*0.1f * (float(m_params.dimension) * (u - velocity)) * float(m_params.dimension);
}

glm::vec3 Cloth::gravityForce(){
    float g = -9.8;
    return glm::vec3 (0,(m_params.particleMass*0.1) * g,0);

}

glm::vec3 Cloth::windForce(){
    float radian = m_params.windAngle*M_PI/180.f;
    float unknownVal = 0.12 * 0.5 * 1.2 * pow(m_params.windVelocity,2);

    return glm::vec3(unknownVal * sin(radian), 0, unknownVal * cos(radian));
    //what is the unknown val?
}
glm::vec3 Cloth::netForce(int row, int col){
    glm::vec3 netForce(0,0,0);
    glm::vec3 p = getIndexAt(row,col,m_position);

    //structural
    if (validIndex(row + 1, col))
        netForce += springForce(p,getIndexAt(row + 1, col,m_position),
                                m_params.structuralStiffness,m_restLength.structure);
    if (validIndex(row - 1, col))
        netForce += springForce(p,getIndexAt(row - 1, col,m_position),
                                m_params.structuralStiffness,m_restLength.structure);
    if (validIndex(row, col + 1))
        netForce += springForce(p,getIndexAt(row, col + 1,m_position),
                                m_params.structuralStiffness,m_restLength.structure);
    if (validIndex(row, col - 1))
        netForce += springForce(p,getIndexAt(row, col - 1,m_position),
                                m_params.structuralStiffness,m_restLength.structure);

    //shear
    if (validIndex(row + 1, col + 1))
        netForce += springForce(p,getIndexAt(row + 1, col + 1,m_position),
                                m_params.shearStiffness,m_restLength.shear);
    if (validIndex(row + 1, col - 1))
        netForce += springForce(p,getIndexAt(row + 1, col - 1,m_position),
                                m_params.shearStiffness,m_restLength.shear);
    if (validIndex(row - 1, col + 1))
        netForce += springForce(p,getIndexAt(row - 1, col + 1,m_position),
                                m_params.shearStiffness,m_restLength.shear);
    if (validIndex(row - 1, col - 1))
        netForce += springForce(p,getIndexAt(row - 1, col - 1,m_position),
                                m_params.shearStiffness,m_restLength.shear);



    //bend
    if (validIndex(row + 2, col))
        netForce += springForce(p,getIndexAt(row + 2, col,m_position),
                                m_params.bendStiffness,m_restLength.bend);
    if (validIndex(row - 2, col))
        netForce += springForce(p,getIndexAt(row - 2, col,m_position),
                                m_params.bendStiffness,m_restLength.bend);
    if (validIndex(row, col + 2))
        netForce += springForce(p,getIndexAt(row, col + 2,m_position),
                                m_params.bendStiffness,m_restLength.bend);
    if (validIndex(row, col - 2))
        netForce += springForce(p,getIndexAt(row, col - 2,m_position),
                                m_params.bendStiffness,m_restLength.bend);


    //other forces
    glm::vec3 velocity = getIndexAt(row, col,m_velocity);
    netForce += dampingForce(velocity) + viscousForce(velocity)+ gravityForce() + windForce();


    return netForce;
}

glm::vec3 Cloth::getIndexAt(int row, int col,std::vector<glm::vec3> vector){
    return vector.at(row * m_params.dimension + col);
}

bool Cloth::validIndex(int row, int col){
    if (row >= m_params.dimension)
        return false;
    if (row < 0)
        return false;
    if (col >= m_params.dimension)
        return false;
    if (col < 0)
        return false;
    return true;
}



void Cloth::update(){
    for (int row = 0; row < m_params.dimension; row++){
        for (int col = 0; col < m_params.dimension; col++){
            glm::vec3 acceleration = netForce(row,col)/(m_params.particleMass*0.1f);
            glm::vec3 nextVelocity = getIndexAt(row,col,m_velocity) + acceleration * m_step;
            glm::vec3 nextPosition = getIndexAt(row,col,m_position) + nextVelocity * m_step;

            if (row == 0 && col == 0){
                nextVelocity = glm::vec3(0,0,0);
                nextPosition = glm::vec3(-0.5,0.5,0);
            }
            if (row == 0 && col == m_params.dimension -1){
                nextVelocity = glm::vec3(0,0,0);
                nextPosition = glm::vec3(0.5,0.5,0);
            }
            m_nextVelocity.push_back(nextVelocity);
            m_nextPosition.push_back(nextPosition);

        }
    }
    //transfer and clear
    m_position = m_nextPosition;
    m_velocity = m_nextVelocity;
    m_nextPosition.clear();
    m_nextVelocity.clear();

}

void Cloth::restartAnimationCloth(){
    update();
    setVertex();
    buildVAO();
}

CS123::GL::Texture2D Cloth::loadTexture(){
//textureHelp3
//    QString filenameQ = QString::fromStdString("/Users/yuna.hiraide/Desktop/texture.png");
//    QImage image = QImage(filenameQ);
//    QImage fImage = QGLWidget::convertToGLFormat(image);

//    CS123::GL::Texture2D texture(fImage.bits(), fImage.width(), fImage.height());    CS123::GL::TextureParametersBuilder builder;
//    builder.setFilter(CS123::GL::TextureParameters::FILTER_METHOD::LINEAR);
//    builder.setWrap(CS123::GL::TextureParameters::WRAP_METHOD::REPEAT);
//    CS123::GL::TextureParameters parameters = builder.build();
//    parameters.applyTo(texture);
//    return texture;
}

