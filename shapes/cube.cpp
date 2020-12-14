#include "cube.h"
#include <iostream>

Cube::Cube()
{
}

Cube::Cube(int param1) :
    m_param1(param1)
{
    /**
     * We build in vertex data for a cube, in this case they are handwritten.
     * You'll want to expand upon or refactor this code heavily to take advantage
     * of polymorphism, vector math/glm library, and utilize good software design
     *
     */
    buildVertexData();
    /** build the VAO so that the shape is ready to be drawn */
    buildVAO();
}

Cube::~Cube()
{
}


void Cube::buildVertexData(){
    m_vertexData.empty();
    for (int face = 0; face < 6; face++){
        for (int row = 0; row < m_param1; row ++){
            for (int col = 0; col < m_param1; col++){
                //set up values
                std::vector <int> faceOrd = {0,0,1,1,2,2};
                std::vector <int>rowOrd = {1,2,2,0,0,1};
                std::vector <int>colOrd = {2,1,0,2,1,0};
                std::vector <int>rowStep = {0,0,1,0,-1,1};
                std::vector <int>colStep = {0,1,-1,0,1,0};

                //set up initial coordinates
//                float coord[3];
                std::vector<float>coord = {0.f,0.f,0.f};
                coord[faceOrd[face]] = face % 2 == 0 ? -0.5: 0.5;
                coord[rowOrd[face]] = -0.5 + (float)row/m_param1;
                coord[colOrd[face]] = -0.5 + (float)col/m_param1;

                //set up normal
                std::vector<float> normal = {0.f,0.f,0.f};
                normal[faceOrd[face]] = face % 2 == 0 ? -1: 1;

                //insert
                for (int i = 0; i < 6; i++){
                    coord[rowOrd[face]] += (float)rowStep[i]/m_param1;
                    coord[colOrd[face]] += (float)colStep[i]/m_param1;

                    //uvs


                    std::vector<float> uv = {0.0,0.0};
                    if (face == 0){
                        uv[0] = coord[2] + 0.5;
                        uv[1] = -coord[1] + 0.5;
                        uv[0] = 0;
                        uv[1] = 0;
                    } else if (face == 1){
                        uv[0] = -coord[2] + 0.5;
                        uv[1] = -coord[1] + 0.5;

//                        uv[0] = 0;
//                        uv[1] = 0;
                    }else if (face == 2){
                        uv[0] = coord[0] + 0.5;
                        uv[1] = -coord[2] + 0.5;
                        uv[0] = 0;
                        uv[1] = 0;
                    }else if (face == 3){
                        uv[0] = coord[0] + 0.5;
                        uv[1] = coord[2] + 0.5;
                        uv[0] = 0;
                        uv[1] = 0;
                    }else if (face == 4){
                        uv[0] = -coord[0] + 0.5;
                        uv[1] = -coord[1] + 0.5;
                        uv[0] = 0;
                        uv[1] = 0;
                    }else if (face == 5){
                        uv[0] = coord[0] + 0.5;
                        uv[1] = -coord[1] + 0.5;
                        uv[0] = 0;
                        uv[1] = 0;
                    }
//                    if (face == 0){
//                        uv[0] = coord[2] + 0.5;
//                        uv[1] = -coord[1] + 0.5;
//                    } else if (face == 1){
//                        uv[0] = -coord[2] + 0.5;
//                        uv[1] = -coord[1] + 0.5;
//                    }else if (face == 2){
//                        uv[0] = coord[0] + 0.5;
//                        uv[1] = -coord[2] + 0.5;
//                    }else if (face == 3){
//                        uv[0] = coord[0] + 0.5;
//                        uv[1] = coord[2] + 0.5;
//                    }else if (face == 4){
//                        uv[0] = -coord[0] + 0.5;
//                        uv[1] = -coord[1] + 0.5;
//                    }else if (face == 5){
//                        uv[0] = coord[0] + 0.5;
//                        uv[1] = -coord[1] + 0.5;
//                    }
                    m_vertexData.insert(m_vertexData.end(),{coord[0],coord[1],coord[2],normal[0],normal[1],normal[2],
                                        uv[0],uv[1]});                }
            }
        }
    }
}
