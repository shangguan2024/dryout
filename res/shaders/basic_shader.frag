#version 460 core

// std430 layout
struct Light {
    int type;
    vec3 color;
    float intensity;
    vec3 direction;         // directional & spot
    float range;            //       point & spot
    vec3 position;          //       point & spot
    float spot_inner_cone;
    float spot_outer_cone;
    float constant;
    float linear;
    float quadratic;
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

void main() {
    vec4 color = texture(u_Textures[v_TexIndex], v_TexCoord);

    // test
    float ambient = 0.1;
    float diffuse = 0.0;
    for(int i = 0; i < u_LightCount; ++i) {
        vec3 lightPos = lights[i].position;
        diffuse += max(dot(normalize(lightPos - v_FragPos), normalize(v_Normal)) * 0.66, 0.0);
    }
    color.xyz *= (ambient + diffuse);

    if(color.a < 0.1) {
        discard;
    }
    o_Color = color * v_Color;
}
