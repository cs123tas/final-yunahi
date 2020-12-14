#ifndef SCENE_H
#define SCENE_H

#include "CS123SceneData.h"

class Camera;
class CS123ISceneParser;

// Scene global color coefficients
struct nodeInfo  {
    glm::mat4 transformation;
    CS123SceneMaterial material;
    PrimitiveType type;
};

/**
 * @class Scene
 *
 * @brief This is the base class for all scenes. Modify this class if you want to provide
 * common functionality to all your scenes.
 */
class Scene {
public:
    Scene();
    Scene(Scene &scene);
    virtual ~Scene();

    virtual void settingsChanged() {}

    static void parse(Scene *sceneToFill, CS123ISceneParser *parser);
    virtual void updateScene();
protected:

    // Adds a primitive to the scene.
    virtual void addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix);

    // Adds a light to the scene.
    virtual void addLight(const CS123SceneLightData &sceneLight);

    // Sets the global data for the scene.
    virtual void setGlobal(const CS123SceneGlobalData &global);

    void addPrimitiveHelper(CS123SceneNode *node,glm::mat4 parentMatrix);

    std::vector<nodeInfo> m_primitiveList;
    std::vector<CS123SceneLightData> m_lightList;
    CS123SceneGlobalData m_global;



};




#endif // SCENE_H
