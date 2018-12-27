#define sl 200
#define hf_sl 100
#define eps 0.0001f
#define VMAX 1.0f
#define dt 0.3f

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

    const t_cell cell = prev_state[offset];
    float v = cell.V;
    float v_in = 0;
    float v_out = 0;
    if (cell.is_solid)
        return ;
    if (v > VMAX)
        v_out += (v - VMAX) * dt;
    if (cell.y > 0)
    {
        t_cell below = prev_state[to_address(cell.x, cell.y - 1.0f, cell.z)];
        if (!below.is_solid && below.V < VMAX)
        {
            // flow to below
            v_out += v * dt;
        }
        if (!below.is_solid && below.V > VMAX)
            v_in += (below.V - VMAX) * dt;
    }
    if (cell.y < hf_sl / 2)
    {
        t_cell above = prev_state[to_address(cell.x, cell.y + 1.0f, cell.z)];
        if (above.V > 0 && v < VMAX)
            v_in += above.V * dt;
        // if (above.V > 0)
        // {
        //     v_in += 1 * dt;
        // }
    }

    if (v_in > eps || v_out > eps)
    {
        next_state[offset].V = v + (v_in - v_out);
        // next_state[offset].V = clamp(next_state[offset].V, 0.0f, VMAX);
    }
}
