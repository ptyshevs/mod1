#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "algo_defines.hpp"


#define MAX_NONEMPTY_CELLS 1000
#define NO_NEIGHBOR_PH 4294967294


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
#define VISCOSITY 0.05f

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
	unsigned int type;
	unsigned int n_neighbors;
	unsigned int neighbors[MAX_NEIGHBORS];

} t_particle;

typedef struct {
    unsigned int n_inside;
    unsigned int particles[MAX_PER_CELL];

} t_cell;

#endif
