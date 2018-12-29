/*
*/
typedef struct {
    int         map_size;
    int         cp_size;
}               t_constants;

/*
*/
float inverse_distance_weighting(float3 a, float3 b, float s);
/*
*/

float inverse_distance_weighting(float3 a, float3 b, float s)
{
    float3 diff = a - b;
    diff *= diff;
    float sqsum = diff.x + diff.z;
    return 1 / (pow(sqsum, (1 + s)) + 0.0001f);
}

__kernel void idw_kernel(__global float *control_points,
                         __global float *read_map,
                         __global float *write_map,
                         __constant t_constants *constants)
{
    const t_constants _constants = *constants;

    if (get_global_id(0) >= (size_t)_constants.map_size) { return; }

    size_t offset = get_global_id(0) * 4;

    float3 current_point = (float3) {   read_map[offset],
                                        read_map[offset + 1],
                                        read_map[offset + 2] };

    float num = 0.0f;
    float denom = 0.0f;
    for (size_t i = 0; i < (size_t)_constants.cp_size; i++) {
        int index = i * 3;
        float3 current_control_point = (float3){    control_points[index],
                                                    control_points[index + 1],
                                                    control_points[index + 2] };

        float w = inverse_distance_weighting(current_point, current_control_point, 0.3f);

        denom += w;
        num += w * current_control_point.y;
    }

    write_map[offset + 1] = num / denom;
}
