/*
*/
float inverse_distance_weighting(float3 *a, float3 *b, float s);
/*
*/
// float3 toFloat3(__global float *data, size_t offset);
// float3 toFloat3(__global float * /* data */, size_t /* offset */);

float inverse_distance_weighting(float3 *a, float3 *b, float s)
{
    float3 diff = *a - *b;
    diff = pow(pow(sqrt(diff * diff), 1 + s), 0.5f);
    return diff.x + diff.y + diff.z;
}

__kernel void idw_kernel(__global float *cp, size_t cp_size, __global float *map, size_t map_size)
{
    size_t offset = get_global_id(0)*3;
    float3 curr_point = (float3){map[offset], map[offset + 1], map[offset + 2]};

    float num = 0.0f;
    float denom = 0.0f;
    for (size_t i = 0; i < cp_size; i++) {
        float3 curr_control_point = (float3){cp[i*3], cp[i*3 + 1], cp[i*3 + 2]};
        float w = inverse_distance_weighting(&curr_point, &curr_control_point, 0.1f);

        denom += w;
        num += w * curr_control_point.y;
    }

    map[offset + 1] = (float)num / denom;
}
