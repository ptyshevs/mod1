/*
*/
typedef struct {
    float x;
    float y;
    float z;
    int is_solid;
    float in_volume;
} t_cell;

/*
*/
/*
*/

__kernel void wsim_kernel(__global t_cell *prev_state,
                          __global t_cell *next_state)
{
    size_t offset = get_global_id(0);

    const t_cell prev_cell = prev_state[offset];

    next_state[offset].in_volume = prev_cell.in_volume;
}
