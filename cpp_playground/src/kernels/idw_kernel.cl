


__kernel void idw_kernel(__global float *cp, size_t cp_size, __global float *map, size_t map_size)
{
    int i = get_global_id(0);

    if (i <= 400 && i > 0) {
        map[i * 3 + 1] = 200.0f;   
    }
    if (i <= 8000 && i > 7600) {
        map[i * 3 + 1] = 100.0f;
    }
}