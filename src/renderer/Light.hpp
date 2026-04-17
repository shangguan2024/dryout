#ifndef LIGHT_HPP
#define LIGHT_HPP

#include <glm/glm.hpp>

namespace dryout {

enum class LightType : int { UNKNOWN = -1, DIRECTIONAL = 0, POINT = 1, SPOT = 2 };

// std430 layout
struct Light {
    LightType type;
    char _padding[12]; // align to 16 bytes
    glm::vec3 color;
    float intensity;
    glm::vec3 direction;   // directional & spot
    float range;           //       point & spot
    glm::vec3 position;    //       point & spot
    float spot_inner_cone; //               spot
    float spot_outer_cone; //               spot
    float constant;        //       point & spot
    float linear;          //       point & spot
    float quadratic;       //       point & spot

    //    attenuation = 1.0 / (constant + linear * dist + quadratic * dist * dist)
    //    spot_factor = dot(-light_dir, spot_dir)
    // spot_intensity = clamp((spot_factor - outer_cone) / (inner_cone - outer_cone), 0.0, 1.0)

    static Light createDirectionalLight(const glm::vec3 &direction, const glm::vec3 &color,
                                        float intensity);
    static Light createPointLight(const glm::vec3 &position, const glm::vec3 &color, float range,
                                  float intensity, float constant, float linear, float quadratic);
    static Light createSpotLight(const glm::vec3 &position, const glm::vec3 &direction,
                                 const glm::vec3 &color, float range,
                                 float spot_inner_cone_angle, // in radians
                                 float spot_outer_cone_angle, // in radians
                                 float intensity, float constant, float linear, float quadratic);

    Light();
};

} // namespace dryout

#endif // LIGHT_HPP
