#include "CameraManager.hpp"
#include "Graphics.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <algorithm>

namespace dryout {

// Camera

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

// CameraManager

CameraManager *CameraManager::instance = nullptr;

CameraManager *CameraManager::getInstance() {
    if (instance == nullptr) {
        instance = new CameraManager();
    }
    return instance;
}

void CameraManager::registerCamera(Camera *camera, bool active) {
    if (std::find(cameras.begin(), cameras.end(), camera) == cameras.end()) {
        cameras.push_back(camera); // Not thread-safe
        if (active) {
            active_camera_index = cameras.size() - 1;
        }
    } else {
        std::cout << "CameraManager::registerCamera: camera already registered" << std::endl;
    }
}

void CameraManager::unregisterCamera(Camera *camera) {
    if (std::find(cameras.begin(), cameras.end(), camera) != cameras.end()) {
        cameras.erase(std::find(cameras.begin(), cameras.end(), camera));
    } else {
        std::cout << "CameraManager::unregisterCamera: camera not registered" << std::endl;
    }
}

void CameraManager::setActiveCamera(Camera *camera) {
    for (int i = 0; i < cameras.size(); i++) {
        if (cameras[i] == camera) {
            active_camera_index = i;
            return;
        }
    }
    std::cout << "CameraManager::setActiveCamera: camera not registered" << std::endl;
}

Camera *CameraManager::getActiveCamera() {
    if (active_camera_index == -1) {
        std::cout << "CameraManager::getActiveCamera: no active camera" << std::endl;
        return nullptr;
    }
    return cameras[active_camera_index];
}

CameraManager::CameraManager() : active_camera_index(-1) {
    // TODO
}

CameraManager::~CameraManager() {
    // TODO
}

} // namespace dryout
