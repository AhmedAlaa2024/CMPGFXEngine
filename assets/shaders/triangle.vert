#version 330

// This vertex shader should be used to render a triangle whose normalized device coordinates are:
// (-0.5, -0.5, 0.0), ( 0.5, -0.5, 0.0), ( 0.0,  0.5, 0.0)
// And it also should send the vertex color as a varying to the fragment shader where the colors are (in order):
// (1.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.0, 0.0, 1.0)

out Varyings {
    vec3 color;
} vs_out;

// Currently, the triangle is always in the same position, but we don't want that.
// So two uniforms should be added: translation (vec2) and scle (vec2).
// Each vertex "v" should be transformed to be "scale * v + translation".
// The default value for "translation" is (0.0, 0.0) and for "scale" is (1.0, 1.0).
uniforms vec2 translation;
uniforms vec2 scle;
//TODO: (Req 1) Finish this shader

void main(){
  //Question 1- V(3) and translation (vec2) and scle (vec2).
     vec3 positions[3]=vec3[3](
        vec3(-0.5,-0.5,0.0),
          vec3(0.5,-0.5,0.0),
            vec3(0.0,0.5,0.0)
    );
     const vec4 colors[3]=vec4[3](
        vec4(1.0,0.0,0.0),
          vec4(0.0,1.0,0.0),
            vec4(0.0,0.0,1.0)
      );
    gl_Position=vec4(positions[gl_VertexID],1.0);
    vs_out=colors;
}