#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>

namespace dryout {

constexpr float g_near = 0.1f;
constexpr float g_far = 1000.0f;

class Camera {
  public:
    Camera();
    ~Camera() = default;

    glm::mat4 getViewProjectionMatrix() const;

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

} // namespace dryout

#endif // CAMERA_HPP
