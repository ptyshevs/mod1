#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in float in_volume;

out vec4 pos;

out vec4 world_normal;

out float volume;

out vec2 uv;

uniform mat4 MVP;

uniform sampler2D normal;

void main() {
    world_normal = normalize(MVP * texture(normal, uv));

    gl_Position = MVP * vec4(position, 1.0f);
    pos = vec4(position, 1.0f);

    float u = (pos.x + 200.0f) / 400.0f;
    float v = (pos.z + 200.0f) / 400.0f;
    uv = vec2(u, v);

    volume = in_volume;
}