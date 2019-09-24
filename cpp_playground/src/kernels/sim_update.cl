#include "simulation.hpp"

__constant float3 gravity = (float3)(0.0f, -9.81f, 0.0f);
__constant float k_const = 315.0f / (64.0f * M_PI * 38.443359375);
__constant float k_dconst = 45.0f / (M_PI * 11.390625);




///////////////////////// PROTOTYPES
float k_distance(float3 a, float3 b);
float kernel_weight(float d);
unsigned int hash(float3 pos);

unsigned int hash(float3 pos) {
	return (unsigned int)((ceil(pos.x) + hf_sl) * (sl * (sl / 4.0)) + sl * ceil(pos.y) + (ceil(pos.z) + hf_sl));
}


float k_distance(float3 a, float3 b) {
	return length(a - b);
}

float kernel_weight(float d) {
	if (d >= K_RADIUS)
		return (0.0f);
	float x = K_RADIUS - d * d;
	return (k_const * x * x * x);
}

// step 1: find neighbors, compute density and pressure, and apply external forces
__kernel void sim_update(__global t_cell *hmap, __global t_particle *particles, __global t_constants *constants)
{
	float dist;
	size_t offset = get_global_id(0);
	if (offset >= constants->n_particles)
		return ;
	t_particle p = particles[offset];

	float density_accum = 0;
	for (unsigned int i = 0, c = p.n_neighbors; i < c; ++i) {
		if (p.neighbors[i] == NO_NEIGHBOR_PH)
			continue;
		dist = k_distance(p.pos, particles[p.neighbors[i]].pos);
		density_accum += kernel_weight(dist);
	}
	if (p.type == P_STATIC)
		p.density += 100000;  // to push avay dynamic partics from the static one
	p.density = PARTICLE_MASS * (density_accum + kernel_weight(0));
	p.pressure = PRESSURE_CONST * (p.density - TARGET_DENSITY);
	p.force = PARTICLE_MASS * (float3)(constants->gravity_x, constants->gravity_y, constants->gravity_z) - DRAG_COEF * p.vel;
	particles[offset] = p;
}
