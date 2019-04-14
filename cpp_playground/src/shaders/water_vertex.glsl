#version 410 core
#define hf_sl 100
layout (location = 0) in vec4 in_position;
layout (location = 1) in vec4 in_velocity;
layout (location = 2) in vec4 in_force;
layout (location = 3) in float in_density;
layout (location = 4) in float in_pressure;
layout (location = 5) in float in_viscosity;
layout (location = 6) in uint in_ptype;
layout (location = 7) in uint in_id;
layout (location = 8) in uint in_n_neighbors;

out vec4 position;
out vec4 velocity;
out vec4 force;
out float density;
out float pressure;
out float viscosity;
out float ptype;
out float id;
out float n_neighbors;

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(in_position.x, in_position.y + 0.2f, in_position.z, 1.0f);
    position = vec4(in_position.xyz, 1.0f);
    velocity = in_velocity;
    force = in_force;
    density = in_density;
    pressure = in_pressure;
    viscosity = in_viscosity;
    ptype = float(in_ptype);
    id = float(in_id);
    n_neighbors = float(in_n_neighbors);
    gl_PointSize = clamp(1/gl_Position.z * 500, 2, 15);
}