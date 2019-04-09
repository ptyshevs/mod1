#ifndef ALGO_DEFINES_HPP
#define ALGO_DEFINES_HPP

// number of points per axis
# define sl 200
// half the number (used for centering in NDC
# define hf_sl 100
# define MAX_NEIGHBORS 40
# define MAX_PER_CELL 30  // max amount of particles pointers stored in a single cell
# define CP_STEP 3.5f
# define UPDATE_NEIGHBORS_EVERY_N_ITER 1
# define MAX_PARTICLES 100000

# define PARTICLE_MASS 1.0f
# define NEIGHBOR_RADIUS 1.5f
# define VISCOSITY 0.08f

#define EMITER_RAIN 1
#define EMITER_WAVE 2
#define EMITER_BOUNDARIES 4
#define EMITER_FLUSH 8

#define P_DYNAMIC 1
#define P_STATIC  2
#define P_SOLID   4
//#define P_SOLID   8 // Not working

#endif
