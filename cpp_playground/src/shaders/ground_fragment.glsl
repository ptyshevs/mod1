#version 410 core

in vec4 pos;

out vec4 color;

vec3 checker(in float u, in float v)
{
  float checkSize = 100;
  float fmodResult = mod(floor(checkSize * u) + floor(checkSize * v), 2.0);
  float fin = max(sign(fmodResult), 0.0);
  return vec3(fin, fin, fin);
}

void main() {
    float u = (pos.x + 200.0f) / 400.0f;
    float v = (pos.z + 200.0f) / 400.0f;
    
    color = vec4(checker(u, v), 1.0f);
}
