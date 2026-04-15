#version 430 core

layout(location = 0) out vec4 o_Color;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

void main() {
    vec4 texColor = v_Color;

    int index = int(v_TexIndex);
    texColor *= texture(u_Textures[index], v_TexCoord);

    // Safe:
    // switch (index) {
    // case 0:
    //     texColor *= texture(u_Textures[0], v_TexCoord);
    //     break;
    // case 1:
    //     texColor *= texture(u_Textures[1], v_TexCoord);
    //     break;
    // case 2:
    //     texColor *= texture(u_Textures[2], v_TexCoord);
    //     break;
    // case 3:
    //     texColor *= texture(u_Textures[3], v_TexCoord);
    //     break;
    // case 4:
    //     texColor *= texture(u_Textures[4], v_TexCoord);
    //     break;
    // case 5:
    //     texColor *= texture(u_Textures[5], v_TexCoord);
    //     break;
    // case 6:
    //     texColor *= texture(u_Textures[6], v_TexCoord);
    //     break;
    // case 7:
    //     texColor *= texture(u_Textures[7], v_TexCoord);
    //     break;
    // default:
    //     texColor *= texture(u_Textures[0], v_TexCoord);
    //     break;
    // }

    if(texColor.a < 0.1) {
        discard;
    }
    o_Color = texColor;
}
