#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;

out Varyings {
    vec4 color;
    vec2 tex_coord;
} vs_out;

uniform mat4 transform;

void main(){
    //TODO: (Req 7) Change the next line to apply the transformation matrix
    // here we assigned the transformed vertex position to the built-in variable gl_Position, which is used to determine the final position of the vertex on the screen
    // To apply a transformation matrix to the vertex position:
    // The vertex position is first converted to a vec4 with a fourth component of 1.0 to make it a position vector
    // then multiplied by the transformation matrix using matrix multiplication
    gl_Position = transform * vec4(position, 1.0);
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;
}