#version 410 core
layout (location = 0) in vec3 position;
layout (location = 1) in float in_volume;

out vec4 pos;
out float volume;

uniform mat4 MVP;

void main() {
	vec4 p = vec4(position.x, position.y + in_volume + 1.0f, position.z, 1.0f);
    gl_Position = MVP * p;
    pos = p;
    volume = in_volume;
}