#version 460 core

layout(location = 0) in vec3 v_FragPos;
layout(location = 1) in vec3 v_Normal;
layout(location = 2) in vec4 v_Color;
layout(location = 3) in vec2 v_TexCoord;
layout(location = 4) flat in int v_TexIndex;

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_Textures[32];
// TODO: uniform lights

void main() {
    vec4 texColor = texture(u_Textures[v_TexIndex], v_TexCoord);

    // test
    vec3 lightPos = vec3(0.0, 0.0, 200.0);
    float ambient = 0.1;
    float diffuse = max(dot(normalize(lightPos - v_FragPos), normalize(v_Normal)), 0.0);

    if(texColor.a < 0.1) {
        discard;
    }
    o_Color = vec4((ambient + diffuse) * texColor.xyz, texColor.a) * v_Color;
}
