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
__kernel void neighbor_caching(__global t_cell *hmap, __global t_particle *particles)
{
	size_t offset = get_global_id(0);
	unsigned int h = hash(particles[offset].pos);
	t_cell cell = hmap[h];
	if (cell.n_inside < MAX_PER_CELL) {
		atomic_xchg(&hmap[h].particles[cell.n_inside], offset);
		atomic_inc(&hmap[h].n_inside);
	}
}
