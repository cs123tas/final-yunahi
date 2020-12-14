#include "Scene.h"
#include "Camera.h"
#include "CS123ISceneParser.h"

#include "glm/gtx/transform.hpp"
#include<iostream>

Scene::Scene()
{
}

Scene::Scene(Scene &scene)
{
    // We need to set the global constants to one when we duplicate a scene,
    // otherwise the global constants will be double counted (squared)
    CS123SceneGlobalData global = { 1, 1, 1, 1};
    setGlobal(global);

    // TODO [INTERSECT]
    // Make sure to copy over the lights and the scenegraph from the old scene,
    // as well as any other member variables your new scene will need.

    m_lightList = scene.m_lightList;
    m_primitiveList = scene.m_primitiveList;
}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser) {
    // TODO: load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and
    //   finishParsing()

    //set global data
    CS123SceneGlobalData data;
     parser->getGlobalData(data);
    sceneToFill->setGlobal(data);

     //addLights
     int numLights = parser->getNumLights();

     for (int i = 0; i < numLights; i++){
         CS123SceneLightData data;
         parser->getLightData(i,data);
         sceneToFill->addLight(data);
     }

     //addprimitives
     sceneToFill->addPrimitiveHelper(parser->getRootNode(),glm::mat4(1,0,0,0,
                                                                     0,1,0,0,
                                                                     0,0,1,0,
                                                                     0,0,0,1));



}

//depth first traversal
void Scene::addPrimitiveHelper(CS123SceneNode *node, glm::mat4 parentMatrix){

    //iterate through transformations of that node and build a cumulative matrix,
    //multiplying it with the parent matrix
    for (int t_i = 0; t_i < node->transformations.size();t_i ++){
        CS123SceneTransformation *transformation = node->transformations[t_i];

        switch (node->transformations[t_i]->type) {
        case TRANSFORMATION_TRANSLATE:
            parentMatrix *= glm::translate(transformation->translate);
            break;
        case TRANSFORMATION_SCALE:
            parentMatrix *= glm::scale(transformation->scale);
            break;
        case TRANSFORMATION_ROTATE:
            parentMatrix *= glm::rotate(transformation->angle,transformation->rotate);
            break;
        case TRANSFORMATION_MATRIX:
            parentMatrix *= transformation->matrix;
            break;
        }
    }


    //add all primitives to list with calculated matrix
    for (int p_i = 0; p_i < node->primitives.size(); p_i++){
        addPrimitive(*node->primitives[p_i], parentMatrix);
    }

    //recurse for children nodes, passing the current matrix as parent matrix
    for (int c_1 = 0; c_1<node->children.size();c_1++){
        addPrimitiveHelper(node->children[c_1],parentMatrix);
    }

}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix) {
    nodeInfo node;
    node.type = scenePrimitive.type;
    node.material = scenePrimitive.material;
    node.transformation = matrix;
    m_primitiveList.push_back(node);
}

void Scene::addLight(const CS123SceneLightData &sceneLight) {
    m_lightList.push_back(sceneLight);

}

void Scene::setGlobal(const CS123SceneGlobalData &global) {
    m_global = global;
}

void Scene::updateScene(){

}

