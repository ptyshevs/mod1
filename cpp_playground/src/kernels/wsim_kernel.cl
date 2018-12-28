#define sl 200
#define hf_sl 100
#define eps 0.002f
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
    // bool  early_exit = true;
    const t_cell cell = prev_state[offset];
    float v = cell.V;
    float dv = 0.0f;
    if (cell.is_solid)
        return ;

    float in_down = 0.0f;
    float out_down = 0.0f;
    if (cell.y > 0) // Flow up
    {
        float out_up = excess(v);
        t_cell below = prev_state[to_address(cell.x, cell.y - 1, cell.z)];
        float in_up = excess(below.V);
        dv += in_up - out_up;
        if (!below.is_solid)
            out_down += clamp(v, VMIN, free(below.V));
    }
    if (cell.y < hf_sl / 2)
    {
        t_cell above = prev_state[to_address(cell.x, cell.y + 1, cell.z)];
        in_down += clamp(above.V, VMIN, free(v));
    }
    if (fabs(in_down - out_down) > eps)
    {
        next_state[offset].V = v + dv + (in_down - out_down);
        return ;
    }
    float in_sides = 0.0f;
    float out_sides = 0.0f;
    float N = 1.0f;
    if (cell.x > -(hf_sl))
    {
        t_cell left = prev_state[to_address(cell.x - 1, cell.y, cell.z)];
        if (!left.is_solid)
            N += 1;
    }
    if (cell.x < (hf_sl - 1))
    {
        t_cell right = prev_state[to_address(cell.x + 1, cell.y, cell.z)];
        if (!right.is_solid)
            N += 1;
    }
    if (cell.z > -hf_sl)
    {
        t_cell back = prev_state[to_address(cell.x, cell.y, cell.z - 1)];
        if (!back.is_solid)
            N += 1;
    }
    if (cell.z < (hf_sl - 1))
    {
        t_cell front = prev_state[to_address(cell.x, cell.y, cell.z + 1)];
        if (!front.is_solid)
            N += 1;
    }

    if (cell.x > -hf_sl)
    {
        t_cell left = prev_state[to_address(cell.x - 1, cell.y, cell.z)];
        if (!left.is_solid)
        {
            in_sides += diff(&left, &cell, N);
            out_sides += diff(&cell, &left, N);
        }
    }
    if (cell.x < (hf_sl - 1)) 
    {
        t_cell right = prev_state[to_address(cell.x + 1, cell.y, cell.z)];
        if (!right.is_solid)
        {
            in_sides += diff(&right, &cell, N);
            out_sides += diff(&cell, &right, N);
        }
    }
    if (cell.z > -hf_sl)
    {
        t_cell back = prev_state[to_address(cell.x, cell.y, cell.z - 1)];
        if (!back.is_solid)
        {
            in_sides += diff(&back, &cell, N);
            out_sides += diff(&cell, &back, N);
        }

    }
    if (cell.z < (hf_sl - 1))
    {
        t_cell front = prev_state[to_address(cell.x, cell.y, cell.z + 1)];
        if (!front.is_solid)
        {
            in_sides += diff(&front, &cell, N);
            out_sides += diff(&cell, &front, N);
        }
    }
    dv += (in_sides - out_sides);
    // dv += in_down - out_down;
    // flow down
    if (fabs(dv) > eps)
        next_state[offset].V = v + dv;
    else
        next_state[offset].V = v;
}
