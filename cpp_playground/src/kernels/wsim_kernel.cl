#define MAX_NEIGHBORS 10
#define MAX_PER_CELL 100

//typedef struct {
//	float x;
//	float y;
//	float z;
//} t_cp;

typedef struct s_particle {
	float3 pos;
	float3 vel;
	float3 force;
	float density;
	float pressure;
	unsigned long long  n_neighbors;
	unsigned long long   neighbors[MAX_NEIGHBORS];
} t_particle;

//typedef struct {
//    float x;
//    float y;
//    float z;
//    int is_solid;
//    unsigned long long n_inside;
//    t_particle *particles[MAX_PER_CELL];
//
//} t_cell;


__kernel void wsim_kernel(__global t_particle *particles)
{
    size_t offset = get_global_id(0);
    t_particle p = particles[offset];
	particles[offset].vel.x = 300;
	particles[offset].vel.y = 300;
}
