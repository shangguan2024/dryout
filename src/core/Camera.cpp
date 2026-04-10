#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace dryout {

Camera::Camera()
    : fovy(glm::radians(45.0f)), aspect(4.0f / 3.0f), near(g_near), far(g_far),
      eye(0.0f, 0.0f, 100.0f), center(0.0f, 0.0f, 0.0f), up(0.0f, 1.0f, 0.0f) {
    updateProjectionMatrix();
    updateViewMatrix();
}

glm::mat4 Camera::getViewProjectionMatrix() const {
    return projection_matrix * view_matrix;
}

void Camera::setFovyByDegrees(float degrees) {
    fovy = glm::radians(degrees);
    updateProjectionMatrix();
}

void Camera::setFovyRadians(float radians) {
    fovy = radians;
    updateProjectionMatrix();
}

void Camera::setAspect(float aspect) {
    this->aspect = aspect;
    updateProjectionMatrix();
}

void Camera::setNear(float near) {
    this->near = near;
    updateProjectionMatrix();
}

void Camera::setFar(float far) {
    this->far = far;
    updateProjectionMatrix();
}

void Camera::setEye(glm::vec3 eye) {
    this->eye = eye;
    updateViewMatrix();
}

void Camera::setCenter(glm::vec3 center) {
    this->center = center;
    updateViewMatrix();
}

void Camera::setUp(glm::vec3 up) {
    this->up = up;
    updateViewMatrix();
}

void Camera::updateProjectionMatrix() {
    projection_matrix = glm::perspective(fovy, aspect, near, far);
}

void Camera::updateViewMatrix() {
    view_matrix = glm::lookAt(eye, center, up);
}

} // namespace dryout
