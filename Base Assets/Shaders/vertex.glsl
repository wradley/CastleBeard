#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aUV;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat4 uModel;

out vec3 vNormal;
out vec3 vFragPos;

void main() {
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0);

    // varients
    vNormal = aNormal;
    vFragPos = vec3(uModel * vec4(aPos, 1.0));
}
