#version 330 core

in Varyings {
    vec4 color;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;

void main(){
    //TODO: (Req 7) Modify the following line to compute the fragment color
    // by multiplying the tint with the vertex color
    // tint is a vec4 representing the tint color
    // vertexColor is a vec4 representing the vertex color 
    // These two colors are multiplied together to obtain the final color of the fragment
    frag_color = tint * fs_in.color;
}