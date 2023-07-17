#version 330 core

layout (location = 0) in vec3 firstPos;

void main() {
    gl_Position = vec4(firstPos.x, firstPos.y, firstPos.z, 1.0);
}