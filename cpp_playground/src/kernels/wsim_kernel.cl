#define sl 200
#define hf_sl 100
#define eps 0.0001f
#define VMAX 1.0f
#define dt 1.0f  // 1 for single rain-drops, lower to have tails
#define dt_side 0.3f  // viscosity

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
    bool  early_exit = true;
    const t_cell cell = prev_state[offset];
    float v = cell.V;
    float v_in = 0;
    float v_out = 0;
    if (cell.is_solid)
        return ;
    /*
    GRAVITY
    */
    if (v > VMAX)  // pressure out
        v_out += (v - VMAX) * dt;
    if (cell.y > 0)
    {
        t_cell below = prev_state[to_address(cell.x, cell.y - 1.0f, cell.z)];
        if (!below.is_solid && below.V < VMAX)
        {
            // flow to below
            v_out += v * dt;
        }
        if (!below.is_solid && below.V > VMAX) // pressure in
            v_in += (below.V - VMAX) * dt;
        if (below.is_solid)
            early_exit = false;
    }
    if (cell.y < hf_sl / 2)
    {
        t_cell above = prev_state[to_address(cell.x, cell.y + 1.0f, cell.z)];
        if (above.V > v)
            v_in += (above.V - v) * dt;
    }
    /*
    END GRAVITY
    */

    /*
    SIDEWAYS
    */
    if (early_exit) // free fall
    {
        next_state[offset].V = v + (v_in - v_out);
        return ;
    }
    if (cell.x > -hf_sl) // left neighbour
    {
        t_cell left = prev_state[to_address(cell.x - 1, cell.y, cell.z)];
        if (left.V < v)
            v_out += (v - left.V) * dt_side;
        if (left.V > v)
            v_in += (left.V - v) * dt_side;
    }
    if (cell.x < hf_sl)
    {
        t_cell right = prev_state[to_address(cell.x + 1, cell.y, cell.z)];
        if (right.V < v)
            v_out += (v - right.V) * dt_side;
        if (right.V > v)
            v_in += (right.V - v) * dt_side;
    }
    if (cell.z > -hf_sl)
    {
        t_cell back = prev_state[to_address(cell.x, cell.y, cell.z - 1)];
        if (back.V < v)
            v_out += (v - back.V) * dt_side;
        if (back.V > v)
            v_in += (back.V - v) * dt_side;
    }
    if (cell.z < hf_sl)
    {
        t_cell front = prev_state[to_address(cell.x, cell.y, cell.z + 1)];
        if (front.V < v)
            v_out += (v - front.V) * dt_side;
        if (front.V > v)
            v_in += (front.V - v) * dt_side;
    }
    /*
    END SIDEWAYS
    */
    // clamp prevents "exploding"
    if (v_in > eps || v_out > eps)
        next_state[offset].V = clamp(v + (v_in - v_out), 0.0f, VMAX);
}
