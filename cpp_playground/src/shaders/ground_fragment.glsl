#version 410 core

in vec4 pos;

in vec4 world_normal;

in vec2 uv;

uniform sampler2D albedo;

uniform sampler2D ao;
uniform sampler2D rough;

out vec4 color;

const float gamma = 1/2.2;
const vec4 light_position = vec4(0.0f, 40.0f, 0.0f, 1.0f);

void main() {
    vec4 light_ray = normalize(light_position - pos);
    vec4 diffuse = max(dot(light_ray, world_normal), 0.0f) * texture(rough, uv);
    vec4 ambient = texture(albedo, uv);

    color = mix(texture(albedo, uv) * diffuse, ambient, 1.0f);
}
