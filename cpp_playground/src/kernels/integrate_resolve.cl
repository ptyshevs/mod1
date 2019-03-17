#define sl 200
#define hf_sl 100
#define MAX_NEIGHBORS 26
#define MAX_PER_CELL 100
#define NUM_PARTICLES 1000
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
} t_constants;


typedef struct {
	float x;
	float y;
	float z;
} t_cp;

typedef struct s_particle {
	float pos[3];
	float vel[3];
	float force[3];
	float density;
	float pressure;
	unsigned int n_neighbors;
	unsigned int neighbors[MAX_NEIGHBORS];

} t_particle;

typedef struct {
    float x;
    float y;
    float z;
    int is_solid;
    unsigned int n_inside;
    unsigned int particles[MAX_PER_CELL];

} t_cell;

///////////////////////// PROTOTYPES
float k_distance(float3 a, float3 b);
float kernel_weight(float d);
bool  bound(__global float *pos[3]);




float k_distance(float3 a, float3 b) {
	return length(a - b);
}



float kernel_weight(float d) {
	if (d >= K_RADIUS)
		return (0.0f);
	float x = K_RADIUS - d * d;
	return (k_const * x * x * x);
}

bool bound(__global float **pos) {
	float3 p = (float3)((*pos)[0], (*pos)[1], (*pos)[2]);
	bool out_of_bound = false;
	if (p.y < 0) {
		(*pos)[1] = 0;
		out_of_bound = true;
	}
	else if (p.y > hf_sl / 2.0f - 1) {
		(*pos)[1] = hf_sl / 2.0f - 1;
		out_of_bound = true;
	}

	if (p.z < -hf_sl) {
		(*pos)[2] = -hf_sl;
		out_of_bound = true;
	}
	else if (p.z > hf_sl - 1) {
		(*pos)[2] = hf_sl - 1;
		out_of_bound = true;
	}

	if (p.x < -hf_sl) {
		(*pos)[0] =  -hf_sl;
		out_of_bound = true;
	}
	else if (p.x > hf_sl - 1) {
		(*pos)[0] = hf_sl - 1;
		out_of_bound = true;
	}
	return (out_of_bound);
}

// step 2: accumulate forces from pressure and viscosity
__kernel void integrate_resolve(__global t_constants *constants, __global t_cp *control_points, __global t_cell *hmap, __global t_particle *particles)
{
	size_t offset = get_global_id(0);
	__global t_particle *p = &particles[offset];
	float3 val = (float3)(p->force[0], p->force[1], p->force[2]);
	val = TIME_STEP * val / PARTICLE_MASS;

	p->vel[0] += val[0];
	p->vel[1] += val[1];
	p->vel[2] += val[2];

	p->pos[0] += TIME_STEP * p->vel[0];
	p->pos[1] += TIME_STEP * p->vel[1];
	p->pos[2] += TIME_STEP * p->vel[2];
	// Collision resolution
	if (bound(&p->pos)) {
		p->vel[0] *= -DAMPING;
		p->vel[1] *= -DAMPING;
		p->vel[2] *= -DAMPING;
	}
	// No boundary crossing, maybe there's a surface?
	// float h = surface_height(control_points, p->pos);
	// if (h > p->pos[1])
	// {
	// 	surfaceCollision(p);
	// 	p->position[1] = h;
	// }
}
