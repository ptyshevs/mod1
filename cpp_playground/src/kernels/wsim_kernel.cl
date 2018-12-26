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
size_t to_address(size_t x, size_t y, size_t z);
/*
*/

// const size_t sl = 200;
// const size_t hf_sl = 100;

size_t to_address(size_t x, size_t y, size_t z)
{
    size_t sl = 200;
    size_t hf_sl = 100;

    return ((x + hf_sl) * sl * (sl / 4) + y * sl + (z + hf_sl));
}



__kernel void wsim_kernel(__global t_cell *prev_state,
                          __global t_cell *next_state)
{
    size_t offset = get_global_id(0);
    size_t sl = 200;
    size_t hf_sl = 100;
    float   eps = 0.001f;

    const t_cell prev_cell = prev_state[offset];
    size_t v = prev_cell.in_volume;
    if (prev_cell.is_solid)
        return ;
    if (prev_cell.in_volume > 1.0f) // excess
        v = 1.0f; // assume to pass all the excess above
    if (prev_cell.y > 0 && prev_cell.in_volume > eps)
    {
        size_t below_idx = to_address((size_t)prev_cell.x, (size_t)prev_cell.y - 1, (size_t)prev_cell.z);
        if (!prev_state[below_idx].is_solid && prev_state[below_idx].in_volume < eps)
            next_state[offset].in_volume -= 0.01f;
            // next_state[offset].in_volume -= 1 - prev_state[below_idx].in_volume;
    }
    if (prev_cell.y < hf_sl / 2)
    {
        size_t aidx = to_address((size_t)prev_cell.x, (size_t)prev_cell.y + 1, (size_t)prev_cell.z);
        if (prev_cell.in_volume < 1 && prev_state[aidx].in_volume > eps)
            next_state[offset].in_volume += 0.01f;
            // next_state[offset].in_volume -= 1 - prev_state[below_idx].in_volume;
    }
        // t_cell above = to_address(prev_state, prev_cell.x, clamp(prev_cell.y + 1, 0, hf_sl / 2 - 1), prev_cell.z);
    // if (above.in_volume > 0 && prev_cell.in_volume < 1)
    //     next_state[offset].in_volume += (1 - prev_cell.in_volume) - above.in_volume;
    // next_state[offset].in_volume = prev_cell.in_volume + 0.005f;
}
