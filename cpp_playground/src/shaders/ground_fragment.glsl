#version 410 core

in vec4 pos;

out vec4 color;

vec3 checker(in float u, in float v)
{
  float checkSize = 50;
  float fmodResult = mod(floor(checkSize * u) + floor(checkSize * v), 2.0);
  float fin = max(sign(fmodResult), 0.0);
  return vec3(fin, fin, fin);
}

void main() {
	color = vec4(pos.y / 50.0f, pos.y/ 50.0f, pos.y/ 50.0f, 1.0f);
//     float u = (pos.x + 200.0f) / 400.0f;
//     float v = (pos.z + 200.0f) / 400.0f;

//     color = mix(vec4(checker(u, v), 1.0f), pos, 0.2);
}
