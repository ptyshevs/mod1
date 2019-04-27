#include "simulation.hpp"

__constant float3 gravity = (float3)(0.0f, -9.81f, 0.0f);
__constant float k_const = 315.0f / (64.0f * M_PI * 38.443359375);
__constant float k_dconst = 45.0f / (M_PI * 11.390625);


///////////////////////// PROTOTYPES
unsigned int hash(float3 pos);

unsigned int hash(float3 pos) {
	return (unsigned int)((ceil(pos.x) + hf_sl) * (sl * (sl / 4.0)) + sl * ceil(pos.y) + (ceil(pos.z) + hf_sl));
}

// step 1: find neighbors, compute density and pressure, and apply external forces
__kernel void clear_caching(__global t_cell *hmap)
{
	size_t offset = get_global_id(0);
	t_cell cell = hmap[offset];
	#pragma unroll
	for (unsigned int i = 0, c = cell.n_inside; i < c; ++i)
		cell.particles[i] = 0;
	cell.n_inside = 0;
	hmap[offset] = cell;
}
