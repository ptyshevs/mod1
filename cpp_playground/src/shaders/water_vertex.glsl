#version 410 core
#define hf_sl 100
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_velocity;
layout (location = 2) in vec3 in_force;
layout (location = 3) in float in_density;
layout (location = 4) in float in_pressure;
layout (location = 5) in uint in_n_neighbors;

out vec4 position;
out vec3 velocity;
out vec3 force;
out float density;
out float pressure;
flat out uint n_neighbors;

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(in_position.x, in_position.y + 0.2f, in_position.z, 1.0f);
    position = vec4(in_position, 1.0f);
    velocity = in_velocity;
    force = in_force;
    density = in_density;
    pressure = in_pressure;
    n_neighbors = in_n_neighbors;
}