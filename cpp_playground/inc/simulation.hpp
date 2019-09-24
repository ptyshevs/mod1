#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include "algo_defines.hpp"

/*
 * Placeholder for absence of neighbor
 */
#define NO_NEIGHBOR_PH 4294967294

/*
 * 60 FPS = 1/60 SPF
 */
#define TIME_STEP 0.0166f
/*
 * Constraint on particle velocity while moving in the medium (air)
 * increase to see how it would decrease relative velocity
 */
#define DRAG_COEF 0.004f
/*
 * Fraction of velocity restored after particle is refrected along the surface
 * normal during collision
 */
#define RESTITUTION 0.6f
/*
 * Velocity fraction after collision (unused)
 */
#define DAMPING 0.65f
/*
 * Radius of kernel support
 */
#define K_RADIUS 1.5f
/*
 * Part of calculation used for first and second derivative of a Poly3 kernel
 * (unused, defined instead as const variables in each kernel)
 */
#define K_DCONST = 45.0f / (M_PI * pow(r, 6))

/*
 * Part of equation of state proposed from MCG03
 */
#define PRESSURE_CONST 7.5f
/*
 * Part of equation of state proposed from MCG
 */
#define TARGET_DENSITY 0.01f
/*
 * Negative pressure clamping constant (currently unused)
 */
#define NEGATIVE_PRESSURE_SCALE 0.5f

/*
 * Set of constants passed to kernels that need them:
 */
typedef struct {
	unsigned int     n_cp;               // # of control points
	unsigned int     hmap_size;          // size of heightmap (vector of cells)
	unsigned int     n_particles;        // # of "active" particles
	float            gravity_x;
	float            gravity_y;
	float            gravity_z;
	float            g1;
	float            g2;
	float            g3;

} t_constants;

/*
 * Control point
 */
typedef struct {
	float x;
	float y;
	float z;
} t_cp;

/*
 * Particle structure
 */
typedef struct s_particle {
	float3 pos;                              // position
	float3 vel;                              // velocity
	float3 force;
	float density;
	float pressure;
	float viscosity;
	unsigned int type;                      // dynamic, static, or solid?
	unsigned int id;                        // id of particle (or object to which particle belongs)
	unsigned int n_neighbors;               // # of active neighbors
	unsigned int neighbors[MAX_NEIGHBORS];  // array of neighbors indices

} t_particle;

/*
 * Cell structure
 */
typedef struct {
    unsigned int n_inside;                    // number of particles inside
    unsigned int particles[MAX_PER_CELL];     // indices of particles inside

} t_cell;

#endif
