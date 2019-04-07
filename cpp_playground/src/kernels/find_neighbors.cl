#include "simulation.hpp"

__constant float3 gravity = (float3)(0.0f, -9.81f, 0.0f);
// pow(K_RADIUS, 9)
__constant float k_const = 315.0f / (64.0f * M_PI * 38.443359375);
__constant float k_dconst = 45.0f / (M_PI * 11.390625);
__constant int offsets[27][3] = {
							{0, 0, 0},
							{-1, 0, 0},
							{0, -1, 0},
							{0, 0, -1},
							{0, 0, 1},
							{0, 1, 0},
							{1, 0, 0},
							{-1, -1, 0},
							{-1, -1, 1},
							{-1, 0, -1},
							{-1, 0, 1},
							{-1, 1, -1},
							{-1, 1, 0},
							{-1, 1, 1},
							{0, -1, -1},
							{0, -1, 1},
							{0, 1, -1},
							{0, 1, 1},
							{1, -1, -1},
							{1, -1, 0},
							{1, -1, 1},
							{1, 0, -1},
							{1, 0, 1},
							{1, 1, -1},
							{1, 1, 0},
							{1, 1, 1},
							{-1, -1, -1}};

///////////////////////// PROTOTYPES
unsigned int hash(float3 pos);
bool out_of_bound(float3 pos);


unsigned int hash(float3 pos) {
	return (unsigned int)((ceil(pos.x) + hf_sl) * (sl * (sl / 4.0)) + sl * ceil(pos.y) + (ceil(pos.z) + hf_sl));
}

bool out_of_bound(float3 pos) {
	if (pos.y < 0) {
		return (true);
	}
	else if (pos.y > hf_sl / 2.0f - 1) {
		return (true);
	}

	if (pos.z < -hf_sl) {
		return (true);
	}
	else if (pos.z > hf_sl - 1) {
		return (true);
	}

	if (pos.x < -hf_sl) {
		return (true);
	}
	else if (pos.x > hf_sl - 1) {
		return (true);
	}
	return (false);
}

// step 1: find neighbors
__kernel void find_neighbors(__global t_cell *hmap, __global t_particle *particles, __global t_constants *constants)
{
	size_t offset = get_global_id(0);
	if (offset >= constants->n_particles)
		return ;
	t_particle p = particles[offset];
	p.n_neighbors = 0;
	#pragma unroll
	for (unsigned int i = 0; i < MAX_NEIGHBORS; ++i) {
		p.neighbors[i] = NO_NEIGHBOR_PH;
	}
	unsigned int np_idx;
	float dist;
	#pragma unroll
	for (int offset_idx = 0; offset_idx < 27; ++offset_idx){
		if (p.n_neighbors >= MAX_NEIGHBORS)
			break ;
		__constant int *cell_offset = offsets[offset_idx];
		float3 offset_pos = (float3)(p.pos.x + cell_offset[0], p.pos.y + cell_offset[1], p.pos.z + cell_offset[2]);
		if (out_of_bound(offset_pos))
			continue ;
		t_cell neigh_cell = hmap[hash(offset_pos)];
		for (unsigned int j = 0, k = neigh_cell.n_inside; j < k; ++j) {
			np_idx = neigh_cell.particles[j];
			if (offset != np_idx) {
				dist = length(p.pos - particles[np_idx].pos);
				if (dist < NEIGHBOR_RADIUS && p.n_neighbors < MAX_NEIGHBORS) {
					p.neighbors[p.n_neighbors] = np_idx;
					p.n_neighbors += 1;
					if (p.n_neighbors >= MAX_NEIGHBORS) {
						break ;
					}
				}
			}
		}
	}
	particles[offset] = p;
}