#include "Camera.hpp"
#include "Graphics.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

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

glm::mat2x3 Camera::getRay(const glm::ivec2 &screen_pos) const {
    Graphics *graphics = Graphics::getInstance();
    glm::vec2 window_size = graphics->getWindowSize();
    glm::vec2 win(screen_pos.x, window_size.y - screen_pos.y);
    glm::vec4 viewport(0.0f, 0.0f, window_size);
    glm::vec3 near_point = glm::unProject({win, 0.0f}, view_matrix, projection_matrix, viewport);
    glm::vec3 far_point = glm::unProject({win, 1.0f}, view_matrix, projection_matrix, viewport);
    return glm::mat2x3(near_point, far_point);
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
