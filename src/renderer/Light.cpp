#include "Light.hpp"

#include <glm/glm.hpp>

namespace dryout {

Light Light::createDirectionalLight(const glm::vec3 &direction, const glm::vec3 &color,
                                    float intensity) {
    Light light;
    light.type = LightType::DIRECTIONAL;
    light.color = color;
    light.intensity = intensity;
    light.direction = glm::normalize(direction);
    return light;
}

Light Light::createPointLight(const glm::vec3 &position, const glm::vec3 &color, float range,
                              float intensity, float constant, float linear, float quadratic) {
    Light light;
    light.type = LightType::POINT;
    light.color = color;
    light.intensity = intensity;
    light.position = position;
    light.range = range;
    light.constant = constant;
    light.linear = linear;
    light.quadratic = quadratic;
    return light;
}

Light Light::createSpotLight(const glm::vec3 &position, const glm::vec3 &direction,
                             const glm::vec3 &color, float range, float spot_inner_cone_angle,
                             float spot_outer_cone_angle, float intensity, float constant,
                             float linear, float quadratic) {
    Light light;
    light.type = LightType::SPOT;
    light.color = color;
    light.intensity = intensity;
    light.position = position;
    light.direction = glm::normalize(direction);
    light.range = range;
    light.spot_inner_cone = glm::cos(spot_inner_cone_angle);
    light.spot_outer_cone = glm::cos(spot_outer_cone_angle);
    light.constant = constant;
    light.linear = linear;
    light.quadratic = quadratic;
    return light;
}

Light::Light()
    : type(LightType::UNKNOWN), color(1.0f), intensity(1.0f), direction(0.0f, -1.0f, 0.0f),
      range(0.0f), position(0.0f), spot_inner_cone(0.0f), spot_outer_cone(0.0f), constant(1.0f),
      linear(0.0f), quadratic(0.0f) {}

} // namespace dryout
