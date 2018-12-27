#define sl 200
#define hf_sl 100
#define eps 0.0001f
#define VMAX 1.0f
#define dt 1.0f

/*
*/
typedef struct {
    float x;
    float y;
    float z;
    int is_solid;
    float V;
} t_cell;

/*
*/
size_t to_address(float x, float y, float z);
// float   clamp(float n, float min, float max);
/*
*/

size_t to_address(float x, float y, float z)
{
    return ((x + hf_sl) * sl * (sl / 4) + y * sl + (z + hf_sl));
}



__kernel void wsim_kernel(__global t_cell *prev_state,
                          __global t_cell *next_state)
{
    size_t offset = get_global_id(0);

    const t_cell prev_cell = prev_state[offset];
    float v = prev_cell.V;
    float v_in = 0;
    float v_out = 0;
    if (prev_cell.is_solid)
        return ;
    // if (prev_cell.V > 1.0f) // excess
    //     v_out = prev_cell.V - 1.0f; // assume to pass all the excess above
    // if (prev_cell.V <= eps)
    //     next_state[offset].V = 0.0f;
    if (prev_cell.y > 0 && v > 0)
    {
        size_t below_idx = to_address(prev_cell.x, prev_cell.y - 1, prev_cell.z);
        if (!prev_state[below_idx].is_solid && prev_state[below_idx].V < v)
            v_out += (v - prev_state[below_idx].V) * dt;
    }
    // if (prev_cell.y < hf_sl / 2)
    // {
    //     size_t aidx = to_address(prev_cell.x, prev_cell.y + 1, prev_cell.z);
    //     if (prev_state[aidx].V > v)
    //         v_in += (prev_state[aidx].V - v) * dt;
    // }
    if (fabs(v_in - v_out) != 0)
    {
        next_state[offset].V += v_in - v_out;
    }

    // if (next_state[offset].V <= eps)
        // next_state[offset].V = 0.0f;
        // t_cell above = to_address(prev_state, prev_cell.x, clamp(prev_cell.y + 1, 0, hf_sl / 2 - 1), prev_cell.z);
    // if (above.in_volume > 0 && prev_cell.in_volume < 1)
    //     next_state[offset].in_volume += (1 - prev_cell.in_volume) - above.in_volume;
    // next_state[offset].in_volume = prev_cell.in_volume + 0.005f;
}
