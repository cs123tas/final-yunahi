#ifndef SHAPE_H
#define SHAPE_H

/** imports the OpenGL math library https://glm.g-truc.net/0.9.2/api/a00001.html */
#include <glm/glm.hpp>
#include "GL/glew.h"

#include<memory>
#include <vector>

/**
 *
 * inserts a glm::vec3 into a vector of floats
 * this will come in handy if you want to take advantage of vectors to build your shape
 * make sure to call reserve beforehand to speed this up
 */
inline void insertVec3(std::vector<float> &data, glm::vec3 v){
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

inline void insertVec2(std::vector<float> &data, glm::vec2 v){
    data.push_back(v.x);
    data.push_back(v.y);

}

namespace CS123 { namespace GL {
class VAO;
}}

class Shape
{
public:
    Shape();

    ~Shape();
    void draw();
    virtual void update();


protected:

    /** builds the VAO, pretty much the same as from lab 1 */
    void buildVAO();
    void buildSquare(glm::vec3 point1,glm::vec3 normal1,glm::vec3 point2,glm::vec3 normal2,
                     glm::vec3 point3,glm::vec3 normal3,glm::vec3 point4,glm::vec3 normal4);
    std::vector<GLfloat> m_vertexData;
    std::unique_ptr<CS123::GL::VAO> m_VAO;
    virtual void buildVertexData();



};

#endif // SHAPE_H
