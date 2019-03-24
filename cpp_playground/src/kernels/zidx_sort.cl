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

__kernel void zidx_sort(__global unsigned int *indices, __global t_constants *constants, __global t_particle *particles)
{
	size_t offset = get_global_id(0);
	__global t_particle *p = &particles[offset];
	unsigned int idx = 0;
	for (unsigned int i = 0, c = constants->n_particles; i < c; ++i) {
		if (particles[i].z_idx < p->z_idx)
			++idx;
	}
	indices[offset] = idx;
}
