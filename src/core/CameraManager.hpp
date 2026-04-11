#ifndef CAMERA_MANAGER_HPP
#define CAMERA_MANAGER_HPP

#include <glm/glm.hpp>
#include <vector>

namespace dryout {

constexpr float g_near = 0.1f;
constexpr float g_far = 1000.0f;

class Camera {
  public:
    Camera();
    ~Camera() = default;

    glm::mat4 getViewProjectionMatrix() const;
    glm::mat2x3 getRay(const glm::ivec2 &screen_pos) const; // [near_point, far_point]

    void setFovyByDegrees(float degrees);
    void setFovyRadians(float radians);
    void setAspect(float aspect);
    void setNear(float near);
    void setFar(float far);

    void setEye(glm::vec3 eye);
    void setCenter(glm::vec3 center);
    void setUp(glm::vec3 up);

  private:
    void updateProjectionMatrix();
    void updateViewMatrix();

    float fovy, aspect, near, far;
    glm::vec3 eye, center, up;

    glm::mat4 projection_matrix, view_matrix;
};

class CameraManager {
  public:
    static CameraManager *getInstance();

    void registerCamera(Camera *camera, bool active = false);
    void unregisterCamera(Camera *camera);

    void setActiveCamera(Camera *camera);
    Camera *getActiveCamera();

  private:
    CameraManager();
    ~CameraManager();

    CameraManager(const CameraManager &) = delete;
    CameraManager &operator=(const CameraManager &) = delete;
    CameraManager(CameraManager &&) = delete;
    CameraManager &operator=(CameraManager &&) = delete;

    static CameraManager *instance;
    std::vector<Camera *> cameras;
    int active_camera_index;
};

} // namespace dryout

#endif // CAMERA_MANAGER_HPP
