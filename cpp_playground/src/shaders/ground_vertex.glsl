#version 410 core

layout (location = 0) in vec3 position;

out vec4 pos;

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(position, 1.0f);
    pos = gl_Position;
}