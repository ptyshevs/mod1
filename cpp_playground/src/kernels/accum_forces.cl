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
#define VISCOSITY 0.1f

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
	float3 pos;
	float3 vel;
	float3 force;
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
float k_weight(float d);
float k_first_derivative(float d);
float k_second_derivative(float d);



float k_distance(float3 a, float3 b) {
	return length(a - b);
}



float k_weight(float d) {
	if (d >= K_RADIUS)
		return (0.0f);
	float x = K_RADIUS - d * d;
	return (k_const * x * x * x);
}



float k_first_derivative(float d) {
	if (d >= K_RADIUS)
		return (0.0f);
	float x = K_RADIUS - d;
	return (-k_dconst * x * x);
}

float k_second_derivative(float d)
{
	if (d >= K_RADIUS)
		return (0.0f);
	return (k_dconst * (K_RADIUS - d));
}



// step 2: accumulate forces from pressure and viscosity
__kernel void accum_forces(__global t_constants *constants, __global t_cp *control_points, __global t_cell *hmap, __global t_particle *particles)
{
	size_t offset = get_global_id(0);
	__global t_particle *p = &particles[offset];
	for (unsigned int i=0; i < p->n_neighbors; ++i) {
		__global t_particle *np = &particles[p->neighbors[i]];

		float dist = k_distance(p->pos, np->pos);
		if (dist > 0) {
			float3 dir = (p->pos - np->pos) / dist; // normed vector
			float3 val = PARTICLE_MASS * (p->pressure + np->pressure) /
						(2 * p->density * np->density) * k_first_derivative(dist) * dir;

			p->force[0] -= val[0];
			p->force[1] -= val[1];
			p->force[2] -= val[2];

			float3 pvel = (float3)(p->vel[0], p->vel[1], p->vel[2]);
			float3 npvel = (float3)(np->vel[0], np->vel[1], np->vel[2]);
			val = VISCOSITY * (npvel - pvel) / np->density * k_second_derivative(dist);

			p->force[0] += val[0];
			p->force[1] += val[1];
			p->force[2] += val[2];
		}
	}
}
