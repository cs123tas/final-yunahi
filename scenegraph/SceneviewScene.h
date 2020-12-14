#ifndef SCENEVIEWSCENE_H
#define SCENEVIEWSCENE_H

#include "OpenGLScene.h"
#include "shapes/Shape.h"
#include <map>
#include <QCoreApplication>
#include "gl/textures/Texture2D.h"

#include <memory>


namespace CS123 { namespace GL {

    class Shader;
    class CS123Shader;
    class Texture2D;
}}

/**
 *
 * @class SceneviewScene
 *
 * A complex scene consisting of multiple objects. Students will implement this class in the
 * Sceneview assignment.
 *
 * Here you will implement your scene graph. The structure is up to you - feel free to create new
 * classes and data structures as you see fit. We are providing this SceneviewScene class for you
 * to use as a stencil if you like.
 *
 * Keep in mind that you'll also be rendering entire scenes in the next two assignments, Intersect
 * and Ray. The difference between this assignment and those that follow is here, we are using
 * OpenGL to do the rendering. In Intersect and Ray, you will be responsible for that.
 */
class SceneviewScene : public OpenGLScene {
public:
    SceneviewScene();
    virtual ~SceneviewScene();

    virtual void render(SupportCanvas3D *context) override;
    virtual void settingsChanged() override;

    // Use this method to set an internal selection, based on the (x, y) position of the mouse
    // pointer.  This will be used during the "modeler" lab, so don't worry about it for now.
    void setSelection(int x, int y);



private:


private:

    void loadPhongShader();
    void loadWireframeShader();
    void loadNormalsShader();
    void loadNormalsArrowShader();

    void setSceneUniforms(SupportCanvas3D *context);\
    void setMatrixUniforms(CS123::GL::Shader *shader, SupportCanvas3D *context);
    void setLights();
    void renderGeometry();
    void updateScene() override;

    std::unique_ptr<CS123::GL::CS123Shader> m_phongShader;
    std::unique_ptr<CS123::GL::Shader> m_wireframeShader;
    std::unique_ptr<CS123::GL::Shader> m_normalsShader;
    std::unique_ptr<CS123::GL::Shader> m_normalsArrowShader;

    CS123SceneMaterial m_material;
    std::map<PrimitiveType,std::unique_ptr<Shape>> m_shapes;
    std::unique_ptr<Shape> m_cloth;

    void renderGeometryAsWireframe();
    void renderGeometryAsArrows ();

    void loadTexture();
    std::map<std::string,QImage> m_texture;

    void loadTextures();
    bool m_loadTextures;
    std::map<std::string, CS123::GL::Texture2D> m_textures;



};

#endif // SCENEVIEWSCENE_H
