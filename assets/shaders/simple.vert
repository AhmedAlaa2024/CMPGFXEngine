#version 330

uniform float time;
out vec4 vertex_color;

void main() {
  vec3 bias = (vec3(sin(time), cos(time), sin(time)));

  vec3 positions[3] = vec3[](
    vec3(-0.5, -0.5, 0.0) + bias * bias,
    vec3(0.5, -0.5, 0.0) + bias * bias,
    vec3(0.0, 0.5, 0.0) + bias * bias
  );

  const vec4 colors[3] = vec4[3](
    vec4(1.0, 0.0, 0.0, 1.0),
    vec4(0.0, 1.0, 0.0, 1.0),
    vec4(0.0, 0.0, 1.0, 1.0)
  );

  gl_Position = vec4(positions[gl_VertexID] * sin(time), 1.0);
  vertex_color = colors[gl_VertexID];
}
