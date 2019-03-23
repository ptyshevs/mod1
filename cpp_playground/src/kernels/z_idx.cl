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

__kernel void z_idx(__global t_constants *constants, __global t_cp *control_points, __global t_cell *hmap, __global t_particle *particles)
{
	size_t offset = get_global_id(0);
	__global t_particle *p = &particles[offset];
	uint3 pos = (uint3)(ceil(p->pos.x) + hf_sl, ceil(p->pos.y), ceil(p->pos.z) + hf_sl);
	// pos.x = (pos.x | (pos.x << 16)) & 0x030000FF;
	// pos.x = (pos.x | (pos.x <<  8)) & 0x0300F00F;
	// pos.x = (pos.x | (pos.x <<  4)) & 0x030C30C3;
	// pos.x = (pos.x | (pos.x <<  2)) & 0x09249249;

	// pos.y = (pos.y | (pos.y << 16)) & 0x030000FF;
	// pos.y = (pos.y | (pos.y <<  8)) & 0x0300F00F;
	// pos.y = (pos.y | (pos.y <<  4)) & 0x030C30C3;
	// pos.y = (pos.y | (pos.y <<  2)) & 0x09249249;

	// pos.z = (pos.z | (pos.z << 16)) & 0x030000FF;
	// pos.z = (pos.z | (pos.z <<  8)) & 0x0300F00F;
	// pos.z = (pos.z | (pos.z <<  4)) & 0x030C30C3;
	// pos.z = (pos.z | (pos.z <<  2)) & 0x09249249;
	p->z_idx = 10;
}
