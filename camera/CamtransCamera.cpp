/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>

CamtransCamera::CamtransCamera()
{
    // @TODO: [CAMTRANS] Fill this in...
    m_near = 1.f;
    m_far = 30.f;
    m_thetaH = 60.f;
    m_aspectRatio = 1.f;
    m_eye = glm::vec4(2,2,2,0);
    m_look = glm::vec4(-2,-2,-2,0);
    m_w = glm::vec4(2,2,2,0);
    m_up = glm::vec4(0,1,0,0);


}

void CamtransCamera::setAspectRatio(float a)
{
    m_aspectRatio = a;
    updateProjectionMatrix();

    // @TODO: [CAMTRANS] Fill this in...

}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
//    return glm::mat4x4();
    //scale and perspective
    return m_perspectiveTransformation * m_scaleMatrix;
//    return m_scaleMatrix * m_perspectiveTransformation;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
//    return glm::mat4x4();
    //translation and rotation
//    return m_translationMatrix * m_rotationMatrix;
    return m_rotationMatrix * m_translationMatrix;
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
//    return glm::mat4x4();
    return m_scaleMatrix;

}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
//    return glm::mat4x4();
    return m_perspectiveTransformation;
}

glm::vec4 CamtransCamera::getPosition() const {
    // @TODO: [CAMTRANS] Fill this in...
//    return glm::vec4();
    return m_eye;
}

glm::vec4 CamtransCamera::getLook() const {
    // @TODO: [CAMTRANS] Fill this in...
//    return glm::vec4();
    return -m_w;
}

glm::vec4 CamtransCamera::getUp() const {
    // @TODO: [CAMTRANS] Fill this in...
//    return glm::vec4();
    return m_up;
}

float CamtransCamera::getAspectRatio() const {
    // @TODO: [CAMTRANS] Fill this in...
//    return 0;
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const {
    // @TODO: [CAMTRANS] Fill this in...
//    return 0;
    return m_thetaH;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    // @TODO: [CAMTRANS] Fill this in...
    m_eye = eye;
    m_look = look;
    m_up = up;

    m_w = glm::normalize(-look);
    m_v = glm::normalize(m_up - glm::dot(m_up,m_w)*m_w);
    glm::vec3 u  = glm::cross(m_v.xyz(), m_w.xyz());
    m_u = glm::vec4(u.x,u.y,u.z,0);



    updateViewMatrix();
    updateProjectionMatrix();

}

void CamtransCamera::setHeightAngle(float h) {
    // @TODO: [CAMTRANS] Fill this in...
    m_thetaH = h;
    updateProjectionMatrix();

}

void CamtransCamera::translate(const glm::vec4 &v) {
    // @TODO: [CAMTRANS] Fill this in...
    m_eye.x+=v.x;
    m_eye.y+=v.y;
    m_eye.z+=v.z;

    updateViewMatrix();

}

void CamtransCamera::rotateU(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
//    . u = u0;
//    v = w0 ∗ sin(θ) + v0 ∗ cos(θ);
//    w = w0 ∗ cos(θ) − v0 ∗ sin(θ)
      glm::vec4 v = m_w * glm::sin(glm::radians(degrees)) + m_v * glm::cos(glm::radians(degrees));
      glm::vec4 w = m_w * glm::cos(glm::radians(degrees)) - m_v * glm::sin(glm::radians(degrees));
      m_v = v;
      m_w = w;
      updateViewMatrix();

}

void CamtransCamera::rotateV(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
//    . u = u0 ∗ cos(θ) − w0 ∗ sin(θ);
//    w = u0 ∗ sin(θ) + w0 ∗ cos(θ)
    glm::vec4 u = m_u * glm::cos(glm::radians(degrees)) - m_w * glm::sin(glm::radians(degrees));
    glm::vec4 w = m_u * glm::sin(glm::radians(degrees)) + m_w * glm::cos(glm::radians(degrees));
    m_u = u;
    m_w = w;
    updateViewMatrix();

}

void CamtransCamera::rotateW(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
//u = u0 ∗ cos(θ) − v0 ∗ sin(θ);
//    v = u0 ∗ sin(θ) + v0 ∗ cos(θ);
    glm::vec4 u = m_u * glm::cos(glm::radians(-degrees)) - m_v * glm::sin(glm::radians(-degrees));
    glm::vec4 v = m_u * glm::sin(glm::radians(-degrees)) + m_v * glm::cos(glm::radians(-degrees));
    m_u = u;
    m_v = v;
    updateViewMatrix();

}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    // @TODO: [CAMTRANS] Fill this in...
    m_near = nearPlane;
    m_far = farPlane;
    updateProjectionMatrix();

}

//task 1
glm::vec4 CamtransCamera::getU() const {
    // @TODO: [CAMTRANS] Fill this in...
//    return glm::vec4();
    return m_u;
}
glm::vec4 CamtransCamera::getV() const {
    // @TODO: [CAMTRANS] Fill this in...
//    return glm::vec4();
    return m_v;
}
glm::vec4 CamtransCamera::getW() const {
    // @TODO: [CAMTRANS] Fill this in...
//    return glm::vec4();
    return m_w;
}


//task 1
void CamtransCamera::updateProjectionMatrix(){
    updateScaleMatrix();
    updatePerspectiveMatrix();
}
void CamtransCamera::updatePerspectiveMatrix(){
    float c = -m_near/m_far;
    m_perspectiveTransformation = glm::transpose(glm::mat4(1,0,0,0,
                                                           0,1,0,0,
                                                           0,0,-1.f/(c+1.f),c/(c+1.f),
                                                           0,0,-1,0));
}
void CamtransCamera::updateScaleMatrix(){

    m_scaleMatrix = glm::transpose(glm::mat4(1.f/(m_far*tan(glm::radians(m_thetaH)/2)*m_aspectRatio),0,0,0,
                                             0,1.f/(m_far*tan(glm::radians(m_thetaH)/2)),0,0,
                                             0,0,1.f/m_far,0,
                                             0,0,0,1));
}



void CamtransCamera::updateViewMatrix(){
    updateRotationMatrix();
    updateTranslationMatrix();
}

void CamtransCamera::updateRotationMatrix(){
    m_rotationMatrix = glm::transpose(glm::mat4(m_u.x,m_u.y,m_u.z,0,
                                                m_v.x,m_v.y,m_v.z,0,
                                                m_w.x,m_w.y,m_w.z,0,
                                                0,0,0,1));
}
void CamtransCamera::updateTranslationMatrix(){
    m_translationMatrix = glm::transpose(glm::mat4(1,0,0,-m_eye.x,
                                                   0,1,0,-m_eye.y,
                                                   0,0,1,-m_eye.z,
                                                   0,0,0,1));
}

//is getposition() {return m_eye} correct?
//is -m_eye.x, -m_eye.y, -m_eye.z correct for updateTranslationMatrix()?
// CamtransCamera::CamtransCamera, is eye (2,2,2) and up (0,1,0)? keep in world view? no look vector?
//get projectiion     return m_perspectiveTransformation*m_scaleMatrix?
//get view
