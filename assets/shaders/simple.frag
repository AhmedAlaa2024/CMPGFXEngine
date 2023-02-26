#version 330

uniform float time;
in vec4 vertex_color;
out vec4 frag_color;

void main() {
  vec3 color_wave = (vec3(sin(time), sin(2 * time), sin(3 * time)) * 0.5) + 0.5;
  frag_color = vertex_color * vec4(color_wave , 1.0);
}
