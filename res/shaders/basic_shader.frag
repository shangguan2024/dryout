#version 460 core

// std430 layout
#define DIRECTIONAL 0
#define       POINT 1
#define        SPOT 2
struct Light {
    int type;
    vec3 color;             //                all
    float intensity;        //                all
    vec3 direction;         // directional & spot
    float range;            //       point & spot
    vec3 position;          //       point & spot
    float spot_inner_cone;  //               spot
    float spot_outer_cone;  //               spot
    float constant;         //       point & spot
    float linear;           //       point & spot
    float quadratic;        //       point & spot
};

layout(location = 0) in vec3 v_FragPos;
layout(location = 1) in vec3 v_Normal;
layout(location = 2) in vec4 v_Color;
layout(location = 3) in vec2 v_TexCoord;
layout(location = 4) flat in int v_TexIndex;

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_Textures[32];
uniform int u_LightCount;

#define LIGHT_BUFFER readonly buffer LightBuffer
layout(std430, binding = 0) LIGHT_BUFFER {
    Light lights[];
};

vec3 computeDirectionalLighting(Light l) {
    vec3 light_dir = normalize(-l.direction);
    float diff = max(dot(light_dir, v_Normal), 0.0);

    vec3 diffuse = diff * l.color * l.intensity;

    return diffuse;
}

vec3 computePointLighting(Light l) {
    vec3 light_vec = l.position - v_FragPos;
    float dist = length(light_vec);
    if(dist > l.range) {
        return vec3(0.0);
    }
    float attenuation = 1.0 / (l.constant + l.linear * dist + l.quadratic * dist * dist);
    float diff = max(dot(light_vec, v_Normal) / dist, 0.0);

    vec3 diffuse = diff * l.color * l.intensity * attenuation;

    return diffuse;
}

vec3 computeSpotLighting(Light l) {
    // TODO
    return vec3(0.0);
}

vec3 computeLighting() {
    vec3 result = vec3(0.1); // ambient

    for(int i = 0; i < u_LightCount; ++i) {
        Light l = lights[i];
        if(l.type == DIRECTIONAL) {
            result += computeDirectionalLighting(l);
        } else if(l.type == POINT) {
            result += computePointLighting(l);
        } else if(l.type == SPOT) {
            result += computeSpotLighting(l);
        } else {
            // Unknown type
        }
    }

    return result;
}

void main() {
    vec4 color = texture(u_Textures[v_TexIndex], v_TexCoord);
    color.xyz *= computeLighting();

    if(color.a < 0.1) {
        discard;
    }
    o_Color = color * v_Color;
}
