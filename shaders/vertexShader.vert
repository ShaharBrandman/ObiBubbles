#version 330 core

layout (location = 0) in vec2 position;

uniform vec2 translation;
uniform float rotation;

void main() {
    // Apply translation
    vec2 translatedPosition = position + translation;
    
    // Apply rotation
    float c = cos(rotation);
    float s = sin(rotation);
    mat2 rotationMatrix = mat2(c, -s, s, c);
    vec2 rotatedPosition = rotationMatrix * translatedPosition;
    
    gl_Position = vec4(rotatedPosition, 0.0, 1.0);
}
