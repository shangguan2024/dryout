#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in vec2 a_TexCoord;
layout(location = 4) in int a_TexIndex;

layout(location = 0) out vec3 v_FragPos;
layout(location = 1) out vec3 v_Normal;
layout(location = 2) out vec4 v_Color;
layout(location = 3) out vec2 v_TexCoord;
layout(location = 4) flat out int v_TexIndex;

uniform mat4 u_ViewProjectionMatrix;

void main() {
    gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
    v_FragPos = a_Position;
    v_Normal = a_Normal;
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
}
