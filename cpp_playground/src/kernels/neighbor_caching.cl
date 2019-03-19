#define sl 200
#define hf_sl 100
#define MAX_NEIGHBORS 26
#define MAX_PER_CELL 100
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


typedef struct {
	unsigned int     n_cp;
	unsigned int     hmap_size;
	unsigned int     n_particles;
	unsigned int	n_cells;
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
unsigned int hash(float3 pos);

unsigned int hash(float3 pos) {
	return (unsigned int)((ceil(pos.x) + hf_sl) * (sl * (sl / 4.0)) + sl * ceil(pos.y) + (ceil(pos.z) + hf_sl));
}

// step 1: find neighbors, compute density and pressure, and apply external forces
__kernel void neighbor_caching(__global t_constants *constants, __global t_cp *control_points, __global t_cell *hmap, __global t_particle *particles)
{
	size_t offset = get_global_id(0);
	if (offset >= constants->n_particles)
		return ;
	__global t_particle *p = &particles[offset];
	unsigned int h = hash(p->pos);
	if (h >= constants->n_cells)
		return ;
	__global t_cell *cell = &hmap[h];
	if (cell->n_inside < MAX_PER_CELL) {
		atomic_xchg(&cell->particles[cell->n_inside], offset);
		atomic_inc(&cell->n_inside);
	}
}
