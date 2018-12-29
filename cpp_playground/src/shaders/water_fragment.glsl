#version 410 core
#define sl 200
#define hf_sl 100
#define eps 0.1f

in vec4 pos;

in float volume;

out vec4 color;


void main() {
	if (volume < eps)
		discard ;
  // if (volume < 0)
  //   color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
  // else if (volume > 1)
  //   color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
  // else
//	color = vec4(0.0f, 0.0f, 0.6f + volume, 0.01f);
//	if (volume > 0.0f)
//    	color = vec4(volume, 0.0f, 0.0f, 1.0f);
	color = vec4(0.0f, 0.0f, volume, 0.3f);
}
