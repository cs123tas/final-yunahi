#include "cylinder.h"
#include <iostream>
#include <math.h>


Cylinder::Cylinder()
{
    std::cout<<"not here"<<std::endl;
}

Cylinder::Cylinder(int param1, int param2) :
    SpunShape(std::max(1,param1),std::max(3,param2),0)

{
    buildVertexData();
    buildVAO();
}

Cylinder::~Cylinder()
{
}


void Cylinder::buildVertexData(){

    m_vertexData.clear();
    spin(topCap,topCapNormal,-1);
    std::vector<GLfloat> topCap = m_vertexData;
    m_vertexData.clear();
    spin(bottomCap,bottomCapNormal,1);
    std::vector<GLfloat> bottomCap = m_vertexData;
    m_vertexData.clear();
    spin(wrap,wrapNormal,1);
    std::vector<GLfloat> wrap = m_vertexData;
    m_vertexData.clear();



    for (int i = 0; i < topCap.size() - 5; i+=6){
        m_vertexData.push_back(topCap.at(i));
        m_vertexData.push_back(topCap.at(i+1));
        m_vertexData.push_back(topCap.at(i+2));
        m_vertexData.push_back(topCap.at(i+3));
        m_vertexData.push_back(topCap.at(i+4));
        m_vertexData.push_back(topCap.at(i+5));
        m_vertexData.push_back(0);
        m_vertexData.push_back(0);
//        m_vertexData.push_back(topCap.at(i) + 0.5);
//        m_vertexData.push_back(topCap.at(i+2) + 0.5);
    }

    for (int i = 0; i < bottomCap.size() - 5; i+=6){
        m_vertexData.push_back(bottomCap.at(i));
        m_vertexData.push_back(bottomCap.at(i+1));
        m_vertexData.push_back(bottomCap.at(i+2));
        m_vertexData.push_back(bottomCap.at(i+3));
        m_vertexData.push_back(bottomCap.at(i+4));
        m_vertexData.push_back(bottomCap.at(i+5));
        m_vertexData.push_back(0);
        m_vertexData.push_back(0);
//        m_vertexData.push_back(bottomCap.at(i) + 0.5);
//        m_vertexData.push_back(-bottomCap.at(i+2) + 0.5);
    }

    for (int i = 0; i < wrap.size() - 5; i+=6){
        m_vertexData.push_back(wrap.at(i));
        m_vertexData.push_back(wrap.at(i+1));
        m_vertexData.push_back(wrap.at(i+2));
        m_vertexData.push_back(wrap.at(i+3));
        m_vertexData.push_back(wrap.at(i+4));
        m_vertexData.push_back(wrap.at(i+5));
        m_vertexData.push_back(perimeter(wrap.at(i),wrap.at(i+2)));
        m_vertexData.push_back(-wrap.at(i+1) + 0.5);
    }

}

float Cylinder::perimeter(float x, float z){
    float theta = atan2(z,x);
    if (theta < 0)
        return (-theta/(2*M_PI));
    else
        return (1 - (theta/(2*M_PI)));
}

glm::vec3 Cylinder::topCap(float p1, float p2, float p3, float param1, float param2,float param3){
    float ang = 4*acos(0.0)/param2;
    float rad = 0.5;
    return glm::vec3(rad * cos(ang*p2) * p1/param1,
                     0.5,
                     rad*sin(ang*p2)* p1/param1);
}
glm::vec3 Cylinder::topCapNormal( float p1, float p2, float p3, float param1, float param2,float param3){
    return glm::vec3(0,1,0);
}
glm::vec3 Cylinder::bottomCap(float p1, float p2, float p3, float param1, float param2,float param3){
    float ang = 4*acos(0.0)/param2;
    float rad = 0.5;
    return glm::vec3(rad * cos(ang*p2) * p1/param1,
                     -0.5,
                     rad*sin(ang*p2)* p1/param1);
}
glm::vec3 Cylinder::bottomCapNormal(float p1, float p2, float p3, float param1, float param2,float param3){
    return glm::vec3(0,-1,0);
}
glm::vec3 Cylinder::wrap(float p1, float p2, float p3, float param1, float param2,float param3){
    float ang = 4*acos(0.0)/param2;
    float rad = 0.5;
    return glm::vec3(rad * cos(ang*p2),
                     -0.5 + p1/param1,
                     rad*sin(ang*p2));
}
glm::vec3 Cylinder::wrapNormal(float p1, float p2, float p3, float param1, float param2,float param3){
    float ang = 4*acos(0.0)/param2;
    float rad = 0.5;
    return glm::vec3(cos(ang*p2),
                     0,
                     sin(ang*p2));
}


