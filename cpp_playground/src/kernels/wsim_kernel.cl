#define sl 200
#define hf_sl 100
#define eps 0.0001f
#define VMIN 0.0f
#define VMAX 1.0f
#define dt 1.0f  // 1 for single rain-drops, lower to have tails
#define dt_side 0.25f  // viscosity

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
float   excess(float v);
float   free(float v);


// float   clamp(float n, float min, float max);
/*
*/

size_t to_address(float x, float y, float z)
{
    return ((x + hf_sl) * sl * (sl / 4) + y * sl + (z + hf_sl));
}


float   excess(float v)
{
    return (max(v - VMAX, VMIN));
}

float   free(float v)
{
    return (max(VMAX - v, VMIN));
}

float   diff(const t_cell *one, const t_cell *two, float N)
{
    return (clamp((one->V - two->V) / N, VMIN, one->V / N));
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
    float v_new = 0.0f;
    if (cell.is_solid)
        return ;
    /*
    GRAVITY
    */
    if (v > VMAX)  // pressure out
        v_new -= (v - VMAX) * dt;
    if (cell.y > 0)
    {
        t_cell below = prev_state[to_address(cell.x, cell.y - 1.0f, cell.z)];
        if (!below.is_solid && below.V < VMAX)
        {
            // flow to below
            v_new -= v * dt;
        }
        if (!below.is_solid && below.V > VMAX) // pressure in
            v_new += (below.V - VMAX) * dt;
        if (below.is_solid || below.V >= VMAX)
            early_exit = false;
    }
    if (cell.y < (hf_sl / 2) - 1)
    {
        t_cell above = prev_state[to_address(cell.x, cell.y + 1.0f, cell.z)];
        if (v < VMAX)
            v_new += (above.V) * dt;
    }
    /*
    END GRAVITY
    */

    /*
    SIDEWAYS
    */
    if (early_exit) // free fall
    {
        next_state[offset].V = v + v_new;
        return ;
    }

    float N = 1.0f;
    if (cell.x > -hf_sl)
        N += !prev_state[to_address(cell.x - 1, cell.y, cell.z)].is_solid;
    if (cell.x < (hf_sl - 1))
        N += !prev_state[to_address(cell.x + 1, cell.y, cell.z)].is_solid;
    if (cell.z > -hf_sl)
        N += !prev_state[to_address(cell.x, cell.y, cell.z - 1)].is_solid;
    if (cell.z < (hf_sl - 1))
        N += !prev_state[to_address(cell.x, cell.y, cell.z + 1)].is_solid;
    if (cell.x > -hf_sl) // left neighbour
    {
        t_cell left = prev_state[to_address(cell.x - 1, cell.y, cell.z)];
        if (!left.is_solid)
            v_new += clamp(left.V - v, -VMAX, VMAX) / N;
    }
    if (cell.x < (hf_sl - 1))
    {
        t_cell right = prev_state[to_address(cell.x + 1, cell.y, cell.z)];
        if (!right.is_solid)
            v_new += clamp(right.V - v, -VMAX, VMAX) / N;
    }
    if (cell.z > -hf_sl)
    {
        t_cell back = prev_state[to_address(cell.x, cell.y, cell.z - 1)];
        if (!back.is_solid)
            v_new += clamp(back.V - v, -VMAX, VMAX) / N;
    }
    if (cell.z < (hf_sl - 1))
    {
        t_cell front = prev_state[to_address(cell.x, cell.y, cell.z + 1)];
        if (!front.is_solid)
            v_new += clamp(front.V - v, -VMAX, VMAX) / N;
    }
    /*
    END SIDEWAYS
    */
    // clamp prevents "exploding"
    if (fabs(v_new) > eps)
        next_state[offset].V = v + clamp(v_new, -VMAX, VMAX);
    else
        next_state[offset].V = v;
}
