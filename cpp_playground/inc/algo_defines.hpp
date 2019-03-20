#ifndef ALGO_DEFINES_HPP
#define ALGO_DEFINES_HPP

// number of points per axis
# define sl 200
// half the number (used for centering in NDC
# define hf_sl 100
# define MAX_NEIGHBORS 26
# define MAX_PER_CELL 50  // max amount of particles pointers stored in a single cell
# define MAX_NONEMPTY_CELLS 1000
# define UPDATE_NEIGHBORS_EVERY_N_ITER 10

#define EMITER_RAIN 1
#define EMITER_WAVE 2
#define EMITER_UNDERGROUND 4
#define EMITER_BOUNDARIES 8

#endif