#version 410 core
#define sl 200
#define hf_sl 100
#define eps 0.0001f

in vec4 pos;

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
	if (abs(volume) < 0.1f)
		discard ;
  if (volume < 0)
    color = vec4(-volume, 1.0f, 0.0f, 1.0f);
  else
	  color = vec4(0.0f, 0.0f, volume, 1.0f);
}
