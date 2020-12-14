#ifndef CUBE_H
#define CUBE_H

#include "Shape.h"

class Cube : public Shape
{
public:
    Cube();
    Cube(int param1);
    ~Cube();

private:
    int m_param1;
protected:
    void buildVertexData() override;


};

#endif // CUBE_H
