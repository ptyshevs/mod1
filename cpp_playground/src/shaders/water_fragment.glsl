#version 410 core

in vec4 pos;
//in int is_solid;
in float volume;

out vec4 color;

vec3 checker(in float u, in float v)
{
  float checkSize = 50;
  float fmodResult = mod(floor(checkSize * u) + floor(checkSize * v), 2.0);
  float fin = max(sign(fmodResult), 0.0);
  return vec3(fin, fin, fin);
}

void main() {
  // if (volume < 0.0001f)
  //   discard;
  if (volume > 0.9)
    color = vec4(0.0, 1.0f, 0.0, 1.0f);
  else
    discard ;
}
