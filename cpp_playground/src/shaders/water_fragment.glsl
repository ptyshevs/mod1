#version 410 core
in vec4 position;
in vec4 velocity;
in vec4 force;
in float density;
in float pressure;
in float viscosity;
in float ptype;
in float id;
in float n_neighbors;

float Ns = 250;
vec4 mat_specular=vec4(1);
vec4 light_specular=vec4(1);


out vec4 color;

// should be the same as in camera.hpp
uniform float zNear = 0.1;
uniform float zFar = 5000.0;

float linearDepth(float depth)
{
    depth = 2.0 * depth - 1.0; //depth to NSC
    float zLinear = 2.0 * zNear * zFar / (zFar + zNear - depth * (zFar - zNear));  // NSC to linear (using inverse projection matrix)
    return zLinear;
}

void main() {
    vec3 N;
    N.xy = gl_PointCoord* 2.0 - vec2(1.0);
    float mag = dot(N.xy, N.xy);
    if (mag > 1.0) discard;   // kill pixels outside circle
    N.z = sqrt(1.0-mag);

    // calculate lighting
    float diffuse = max(0.0, dot(vec3(0,0,1), N));

    vec4 pixelPos = vec4(N, 1.0);


    vec3 eye = vec3 (0.0, 0.0, 1.0);
    vec3 halfVector = normalize( eye + vec3(0,0,1));
    float spec = max( pow(dot(N,halfVector), Ns), 0.);
    vec4 S = light_specular*mat_specular* spec;
    float vel_mag = sqrt(dot(velocity, velocity)) * 0.05;
    float v = float(viscosity);
    color = vec4(0.3f + vel_mag, 0.3f + v * 0.1 + vel_mag, 0.5f + vel_mag, 1.0f) * diffuse + S;
    float t = gl_FragCoord.z;
    float t2 = linearDepth(t) / 100;
    color = vec4(t2, t2, t2, 1);

//    vec4 clipSpacePos = MVP * pixelPos;
//    gl_FragDepth = (clipSpacePos.z / clipSpacePos.w) * 0.5f + 0.5f;
//    if (id == 0)
//        color = vec4(0.3f + vel_mag, 0.3f + v * 0.1 + vel_mag, 0.5f + vel_mag, 0.5f) * diffuse + S;
//    else
//        color = vec4(0.5f + vel_mag, 0.2f + vel_mag, 0.2f + vel_mag, 0.5f) * diffuse + S;
}
