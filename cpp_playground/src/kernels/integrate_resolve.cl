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
float surface_height(__global t_constants *constants, __global t_cp *control_points, float3 pos);
float3 surface_normal(__global t_constants *c, __global t_cp *cp, float3 pos);
float3 surface_collision(__global t_constants *constants, __global t_cp *control_points, float3 pos, float3 vel);




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


// idw interpolation, same as what was used to generate surface, to find surface height at point accurately

float surface_height(__global t_constants *constants, __global t_cp *control_points, float3 pos) {
	float num = 0;
	float denum = 0;
	float w;
	for (unsigned int i = 0, c = constants->n_cp; i < c; ++i) { // todo: how about using control points only on the neighbour cells in hmap? it has position pre-calculated already
		w = 0;
		float3 cppos = (float3)(control_points[i].x, control_points[i].y, control_points[i].z);
		float3 diff = pos - cppos;
		diff *= diff;
		float sqsum = diff.x + diff.z;
		w = 1.0f / (pow(sqsum, 1.3f) + 0.0001f);
		num += w * cppos.y;
		denum += w;
	}
	return (num / denum);
}

float3 surface_normal(__global t_constants *c, __global t_cp *cp, float3 pos) {
	float3 xleft = (float3)(pos.x + 0.5f, pos.y, pos.z);
	float3 xright = (float3)(pos.x - 0.5f, pos.y, pos.z);
	float3 zleft = (float3)(pos.x, pos.y, pos.z + 0.5f);
	float3 zright = (float3)(pos.x, pos.y, pos.z - 0.5f);
	float dx = surface_height(c, cp, xleft) - surface_height(c, cp, xright);
	float dz = surface_height(c, cp, zleft) - surface_height(c, cp, zright);
	return normalize((float3)(-dx, 1.0f, -dz));
}

float3 surface_collision(__global t_constants *constants, __global t_cp *control_points, float3 pos, float3 vel) {
	float3 normal = surface_normal(constants, control_points, pos);
	float vel_mag = length(vel);
	float3 vel_normalized = vel / vel_mag;
	float3 reflected = vel_normalized - (2 * dot(vel_normalized, normal) * normal);
	float3 renormalized = reflected * vel_mag;
	// Easy way
//	glm::vec3 frictioned = renormalized * RESTITUTION;
	// we prefer tangenial speed over
	float3 frictioned = renormalized - renormalized * RESTITUTION * fabs(dot(vel_normalized, normal));
	return frictioned;
	// p->vel[0] = frictioned.x;
	// p->vel[1] = frictioned.y;
	// p->vel[2] = frictioned.z;
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
	float3 pos = (float3)(p->pos[0], p->pos[1], p->pos[2]);
	float3 vel = (float3)(p->vel[0], p->vel[1], p->vel[2]);
	float h = surface_height(constants, control_points, pos);
	if (h > pos.y)
	{
		float3 new_vel = surface_collision(constants, control_points, pos, vel);
		p->vel[0] = new_vel.x;
		p->vel[1] = new_vel.y;
		p->vel[2] = new_vel.z;
		p->pos[1] = h;
	}
}
