#include "Shape.h"
#include "gl/datatype/VAO.h"
#include "gl/datatype/VBO.h"
#include "gl/datatype/VBOAttribMarker.h"
#include "gl/shaders/ShaderAttribLocations.h"
#include <iostream>
using namespace CS123::GL;


Shape::Shape() :
    m_VAO(nullptr)
{


}


Shape::~Shape()
{
}

void Shape::draw() {
    if (m_VAO) {
        m_VAO->bind();
        m_VAO->draw();
        m_VAO->unbind();
    }
}

void Shape::buildVAO() {
    const int numFloatsPerVertex = 8;

    const int numVertices = m_vertexData.size() / numFloatsPerVertex;

    std::vector<VBOAttribMarker> markers;
    markers.push_back(VBOAttribMarker(ShaderAttrib::POSITION, 3, 0));
    markers.push_back(VBOAttribMarker(ShaderAttrib::NORMAL, 3, 3*sizeof(float)));
    //texture

    markers.push_back(VBOAttribMarker(ShaderAttrib::TEXCOORD0, 2, 6*sizeof(float)));

    VBO vbo = VBO(m_vertexData.data(), m_vertexData.size(), markers);
    m_VAO = std::make_unique<VAO>(vbo, numVertices);
}
void Shape::buildVertexData(){

}
void Shape::buildSquare(glm::vec3 point1,glm::vec3 normal1,glm::vec3 point2,glm::vec3 normal2,
                           glm::vec3 point3,glm::vec3 normal3,glm::vec3 point4,glm::vec3 normal4){


    insertVec3(m_vertexData,point1);
    insertVec3(m_vertexData,normal1);
    insertVec3(m_vertexData,point2);
    insertVec3(m_vertexData,normal2);
    insertVec3(m_vertexData,point3);
    insertVec3(m_vertexData,normal3);

    insertVec3(m_vertexData,point3);
    insertVec3(m_vertexData,normal3);
    insertVec3(m_vertexData,point2);
    insertVec3(m_vertexData,normal2);
    insertVec3(m_vertexData,point4);
    insertVec3(m_vertexData,normal4);

}

void Shape::update(){

}
