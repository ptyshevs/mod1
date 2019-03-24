#include "simulation.hpp"

__constant float3 gravity = (float3)(0.0f, -9.81f, 0.0f);
// pow(K_RADIUS, 9)
__constant float k_const = 315.0f / (64.0f * M_PI * 38.443359375);
__constant float k_dconst = 45.0f / (M_PI * 11.390625);


///////////////////////// PROTOTYPES
unsigned int hash(float3 pos);

unsigned int hash(float3 pos) {
	return (unsigned int)((ceil(pos.x) + hf_sl) * (sl * (sl / 4.0)) + sl * ceil(pos.y) + (ceil(pos.z) + hf_sl));
}

// step 1: find neighbors, compute density and pressure, and apply external forces
__kernel void zidx_swap(__global unsigned int *indices, __global t_constants *constants, __global t_particle *particles_from, __global t_particle *particles_to)
{
	size_t offset = get_global_id(0);
	__global t_particle *p = &particles_from[offset];
	unsigned int idx_to = indices[offset];
	__global t_particle *p_to = &particles_to[idx_to];
	p_to->pos = p->pos;
	p_to->vel = p->vel;
	p_to->force = p->force;
	p_to->z_idx = p->z_idx;
}
