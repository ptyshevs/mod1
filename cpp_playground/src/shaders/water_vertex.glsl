#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in int  is_solid;
layout (location = 2) in float in_volume;

out vec4 pos;
out float volume;

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(position, 1.0f);
    pos = vec4(position, 1.0f);
    volume = in_volume;
}