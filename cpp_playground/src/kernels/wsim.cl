#define sl 200
#define hf_sl 100
#define dt 0.25f
#define c2 1.0f

/*
*/
typedef struct {
    float x;
    float y;
    float z;
    float V;
} t_cell;

/*
*/
size_t to_address(float x, float z);
/*
*/

size_t to_address(float x, float z)
{
    return ((x + hf_sl) * sl + (z + hf_sl));
}


__kernel void wsim_kernel(__global t_cell *prev_state,
                          __global t_cell *next_state)
{
    size_t offset = get_global_id(0);
    t_cell cell = prev_state[offset];
    float Vleft = 0.0f;
    float Vright = 0.0f;
    float Vfront = 0.0f;
    float Vback = 0.0f;
    if (cell.x > -hf_sl)
    {
        t_cell left = prev_state[to_address(cell.x - 1, cell.z)];
        Vleft = ( left.V) - (cell.V);
    }
    if (cell.x < (hf_sl - 1))
    {
        t_cell right = prev_state[to_address(cell.x + 1, cell.z)];
        Vright = ( right.V) - (cell.V);
    }
    if (cell.z > -hf_sl)
    {
        t_cell back = prev_state[to_address(cell.x, cell.z - 1)];
        Vback = ( back.V) - ( cell.V);
    }
    if (cell.z < (hf_sl - 1))
    {
        t_cell front = prev_state[to_address(cell.x, cell.z + 1)];
        Vfront = ( front.V) - ( cell.V);
    }
	next_state[offset].V = prev_state[offset].V + (Vleft + Vright + Vfront + Vback) * dt;
}
