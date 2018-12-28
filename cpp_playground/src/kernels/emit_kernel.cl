#define EMITER_RAIN 1
#define EMITER_WAVE 2
#define EMITER_UNDERGROUND 4

#define RAIN_PPI 4

#define sl 200
#define hf_sl 100
#define hf_hf_sl 50

/*
*/
typedef struct {
    int type;
    uint pps;
    uint seed;
} t_emiter;     

typedef struct {
    float x;
    float y;
    float z;
    int is_solid;
    float in_volume;
} t_cell;

/*
*/
void it_s_raining_man(__global t_cell *cell, size_t seed);

void havaji(__global t_cell *cell);

void underdog(__global t_cell *cell);

/*
*/

size_t place_in_corner_with_offset(__global t_cell *cell, size_t offset, size_t y_offset);

size_t to_address(float x, float y, float z);

size_t rand(size_t seed);

/*
*/
__kernel void emit_kernel(__global t_cell *cell,
                          __global t_emiter *emiter)
{
    const t_emiter _emiter = emiter[0];

    if (_emiter.type == EMITER_RAIN) {
        it_s_raining_man(cell, (size_t)_emiter.seed);
    } else if (_emiter.type == EMITER_WAVE) {
        havaji(cell);
    } else if (_emiter.type == EMITER_UNDERGROUND) {
        underdog(cell);
    }
}

void it_s_raining_man(__global t_cell *cell, size_t seed)
{
    for (size_t i = 0; i < RAIN_PPI; i++) {
        float foo = ((float)(seed % sl)) - hf_sl;
        float foo2 = ((float)(rand(seed) % sl)) - hf_sl;

        size_t offset = to_address((float)foo,
                                (float)hf_hf_sl - 1,
                                (float)foo2);
        cell[offset].in_volume += 1.0f;
    }
}

void havaji(__global t_cell *cell)
{
    for (int i = -hf_sl; i < hf_sl; i++) {
        size_t offset = to_address(-hf_sl, 1.0f, (float)i - 1);
        cell[offset].in_volume += 1.0f;
    }
}

void underdog(__global t_cell *cell)
{
    place_in_corner_with_offset(cell, 1, 1);
    place_in_corner_with_offset(cell, 2, 1);
}

size_t place_in_corner_with_offset(__global t_cell *cell, size_t offset, size_t y_offset)
{
    size_t _offset = to_address((float)hf_sl - offset, (float)y_offset, (float)hf_sl - offset);
    cell[_offset].in_volume += 1.0f;
    _offset = to_address((float)hf_sl - offset, (float)y_offset, (float)-hf_sl + offset);
    cell[_offset].in_volume += 1.0f;
    _offset = to_address((float)-hf_sl + offset, (float)y_offset, (float)-hf_sl + offset);
    cell[_offset].in_volume += 1.0f;
    _offset = to_address((float)-hf_sl + offset, (float)y_offset, (float)hf_sl - offset);
    cell[_offset].in_volume += 1.0f;
}


size_t to_address(float x, float y, float z)
{
    return ((x + hf_sl) * sl * (sl / 4) + y * sl + (z + hf_sl));
}

size_t rand(size_t seed)
{
    uint t = seed ^ (seed << 11);
    return (seed ^ (seed >> 19) ^ (t ^ (t >> 8)));
}
