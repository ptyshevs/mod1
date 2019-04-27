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

/*
 * Type of particle object
 */
#define P_DYNAMIC 1
#define P_STATIC  2
#define P_SOLID   4 // not used currently


void main() {
    if (ptype == P_DYNAMIC)
        discard ;
    vec3 N;
    N.xy = gl_PointCoord* 2.0 - vec2(1.0);
    float mag = dot(N.xy, N.xy);
    if (mag > 1.0) discard;   // kill pixels outside circle
    N.z = sqrt(1.0-mag);

    // calculate lighting
    float diffuse = max(0.0, dot(vec3(0,0,1), N));

    vec3 eye = vec3 (0.0, 0.0, 1.0);
    vec3 halfVector = normalize( eye + vec3(0,0,1));
    float spec = max( pow(dot(N,halfVector), Ns), 0.);
    vec4 S = light_specular*mat_specular* spec;
    float vel_mag = sqrt(dot(velocity, velocity)) * 0.05;
    float v = float(viscosity);
    color = vec4(0.0f, 0.8f, 0.5f, 1.0f) * diffuse + S;
}
