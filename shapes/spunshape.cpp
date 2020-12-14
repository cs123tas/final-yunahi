#include "spunshape.h"
#include <iostream>
#include <math.h>
#include "Shape.h"


SpunShape::SpunShape()
{
    Shape();
    std::cout<<"not here"<<std::endl;

}

SpunShape::SpunShape(int p1, int p2, int p3){

    m_param1 = p1;
    m_param2 = p2;
    m_param3 = p3;
}


SpunShape::~SpunShape()
{
}

void SpunShape:: spin(glm::vec3(*point)(float p1, float p2, float p3, float param1, float param2, float param3),
              glm::vec3(*normal)(float p1, float p2, float p3, float param1, float param2, float param3),
              float dir){
    float ang = 4*acos(0.0)/m_param2;
    float rad = 0.5;
    float p3 = 0; //temp

    for (int p2 = 0; p2 < m_param2; p2++){
            for (int p1 = 0; p1 < m_param1;p1++){
                buildSquare(point(p1,p2,p3,m_param1,m_param2,m_param3),normal(p1,p2,p3,m_param1,m_param2,m_param3),
                            point(p1+1,p2,p3,m_param1,m_param2,m_param3),normal(p1+1,p2,p3,m_param1,m_param2,m_param3),
                            point(p1,p2+dir,p3,m_param1,m_param2,m_param3),normal(p1,p2+dir,p3,m_param1,m_param2,m_param3),
                            point(p1+1,p2+dir,p3,m_param1,m_param2,m_param3),normal(p1+1,p2+dir,p3,m_param1,m_param2,m_param3));
            }
    }

}


