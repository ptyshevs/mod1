#define sl 200
#define hf_sl 100
#define MAX_NEIGHBORS 26
#define MAX_PER_CELL 50
#define MAX_NONEMPTY_CELLS 1000


#define PARTICLE_MASS 1.0f
#define NEIGHBOR_RADIUS 1.5f

// like 60 FPS = 1/60 SPF
#define TIME_STEP 0.0166f
// increase to see how it would decrease relative velocity
#define DRAG_COEF 0.004f
#define RESTITUTION 0.6f
#define DAMPING 0.65f

//#define M_PI 3.1416 // M_PI is pre-defined
#define K_RADIUS 1.5f
#define K_DCONST = 45.0f / (M_PI * pow(r, 6))

#define PRESSURE_CONST 7.5f
#define TARGET_DENSITY 0.01f
#define NEGATIVE_PRESSURE_SCALE 0.5f
#define VISCOSITY 0.18f

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


typedef struct {
	unsigned int     n_cp;
	unsigned int     hmap_size;
	unsigned int     n_particles;
	unsigned int	n_non_empty_cells;
	unsigned int	non_empty_cells[MAX_NONEMPTY_CELLS];
} t_constants;


typedef struct {
	float x;
	float y;
	float z;
} t_cp;

typedef struct s_particle {
	float3 pos;
	float3 vel;
	float3 force;
	float density;
	float pressure;
	unsigned int n_neighbors;
	unsigned int neighbors[MAX_NEIGHBORS];

} t_particle;

typedef struct {
    unsigned int n_inside;
    unsigned int particles[MAX_PER_CELL];

} t_cell;

///////////////////////// PROTOTYPES
float k_distance(float3 a, float3 b);
float kernel_weight(float d);
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
__kernel void sim_update(__global t_constants *constants, __global t_cp *control_points, __global t_cell *hmap, __global t_particle *particles)
{
	float dist;
	unsigned int np_idx;
	size_t offset = get_global_id(0);
	__global t_particle *p = &particles[offset];
	p->n_neighbors = 0;
	float density_accum = 0;

	for (int offset_idx = 0; offset_idx < 27; ++offset_idx){
		if (p->n_neighbors == MAX_NEIGHBORS)
			break ;
		__global t_cell *cell = &hmap[hash(p->pos)];
		__constant int *cell_offset = offsets[offset_idx];
		float3 offset_pos = (float3)(p->pos.x + cell_offset[0], p->pos.y + cell_offset[1], p->pos.z + cell_offset[2]);
		if (out_of_bound(offset_pos))
			continue ;
		__global t_cell *neigh_cell = &hmap[hash(offset_pos)];
		for (unsigned int j = 0, k = neigh_cell->n_inside; j < k; ++j) {
			np_idx = neigh_cell->particles[j];
			if (offset != np_idx) {
				__global t_particle *np = &particles[np_idx];
				dist = k_distance(p->pos, np->pos);
				if (dist < NEIGHBOR_RADIUS && p->n_neighbors < MAX_NEIGHBORS) {
					p->neighbors[p->n_neighbors] = np_idx;
					p->n_neighbors += 1;
					density_accum += kernel_weight(dist);
					if (p->n_neighbors == MAX_NEIGHBORS) {
						break ;
					}
				}
			}
		}
	}

	// for (size_t i = 0, c = constants->n_particles; i < c; ++i) {
	// 	if (i == offset)
	// 		continue ;
	// 	__global t_particle *np = &particles[i];
	// 	if (p->n_neighbors == MAX_NEIGHBORS)
	// 		break ;
	// 	float dist = k_distance(p->pos, np->pos);
	// 	if (dist < NEIGHBOR_RADIUS) {
	// 		p->neighbors[p->n_neighbors] = i;
	// 		p->n_neighbors += 1;
	// 		density_accum += kernel_weight(dist);
	// 	}
	// }
	p->density = PARTICLE_MASS * (density_accum + kernel_weight(0));
	p->pressure = PRESSURE_CONST * (p->density - TARGET_DENSITY);
	p->force[0] = 0;
	p->force[1] = PARTICLE_MASS * gravity[1];
	p->force[2] = 0;
	p->force[0] -= DRAG_COEF * p->vel[0];
	p->force[1] -= DRAG_COEF * p->vel[1];
	p->force[2] -= DRAG_COEF * p->vel[2];
}
