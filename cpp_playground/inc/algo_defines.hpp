#ifndef ALGO_DEFINES_HPP
#define ALGO_DEFINES_HPP
/*
 * algo_defines.hpp is a single header containing all logic constants used in
 * simulation routines on CPU and GPU
 */

/*
 * number of grid nodes per axis
 */
# define sl 200
/*
 * half the number (used for centering in NDC)
 */
# define hf_sl 100
/*
 * Max amount of neighbors
 */
# define MAX_NEIGHBORS 15
/*
 * max amount of particles pointers stored in a single grid cell
 */
# define MAX_PER_CELL 30
/*
 * Step size along axis (used for control points border generation)
 */
# define CP_STEP 3.5f
/*
 * Control for particle neighbors list update frequency
 */
# define UPDATE_NEIGHBORS_EVERY_N_ITER 2
/*
 * Max amount of particles (used for single-shot memory allocation)
 */
# define MAX_PARTICLES 100000
/*
 * Used in simulation update. 1.0 for simplicity
 */
# define PARTICLE_MASS 1.0f
/*
 * Radius of search for particle neighbors
 */
# define NEIGHBOR_RADIUS 1.5f
/*
 * Viscosity constant (each particle defines it's own viscosity coef.)
 */
# define VISCOSITY 0.08f

/*
 * Constants responsible for emission mode
 */
#define EMITER_RAIN 1
#define EMITER_WAVE 2
#define EMITER_BOUNDARIES 4
#define EMITER_FLUSH 8

/*
 * Type of particle object
 */
#define P_DYNAMIC 1 // Dynamic particle - moves within space freely
#define P_STATIC  2 // Static particle - contraint to the initial position
#define P_SOLID   4 // not used currently

#endif
