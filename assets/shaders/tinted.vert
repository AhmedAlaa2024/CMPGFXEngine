#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out Varyings {
    vec4 color;
} vs_out;

uniform mat4 transform;

void main(){
    //TODO: (Req 7) Change the next line to apply the transformation matrix
    // The vertex position is first converted to a vec4 with a fourth component of 1.0 to make it a position vector
    // then multiplied by the three transformation matrix
    // The resulting transformed position is then assigned to gl_Position, which is used to determine the final position of the vertex on the screen
    gl_Position = transform * vec4(position, 1.0);
    vs_out.color = color;
}