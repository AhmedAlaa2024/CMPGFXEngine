#version 330 core

in Varyings {
    vec4 color;
    vec2 tex_coord;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;
uniform sampler2D tex;

void main(){
    //TODO: (Req 7) Modify the following line to compute the fragment color
    // by multiplying the tint with the vertex color and with the texture color 
    
    // In this formula, tint is a vec4 representing the tint color. A tint color is a single color that is applied uniformly to an object to change its overall appearance.
    // In this formula, vertexColor is a vec4 representing the vertex color. a vertex color is a color value that is associated with each vertex of an object. Vertex colors are typically interpolated across the surface of the object, resulting in a smooth gradient of colors.
    // In this formula, textureColor is a vec4 representing the color of the texture sample at the current fragment.
    // The multiplication of these three vec4 values results in a new vec4 value, finalColor, which represents the final color of the fragment.
    // This color is then assigned to the built-in output variable gl_FragColor.
    // texture() function to retrieve the color of the texture sample at the current fragment/tex_coord.
    frag_color = tint * fs_in.color * texture(tex, fs_in.tex_coord);
}