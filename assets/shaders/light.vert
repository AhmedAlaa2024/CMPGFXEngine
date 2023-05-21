#version 330
// The "layout" keyword is used to specify the location of the input vertex attribute for the vertex position
layout(location = 0) in vec3 position;
// The "layout" keyword is used to specify the location of the input vertex attribute for the vertex color
layout(location = 1) in vec4 color;
// The "layout" keyword is used to specify the location of the input vertex attribute for the vertex texture coordinates
layout(location = 2) in vec2 tex_coord;
// The "layout" keyword is used to specify the location of the input vertex attribute for the vertex normal
layout(location = 3) in vec3 normal;
// A uniform matrix is defined for the view-projection transformation matrix
uniform mat4 VP;
// A uniform vector is defined for the camera position
uniform vec3 camera_position;
// A uniform matrix is defined for the model transformation matrix
uniform mat4 M;
// A uniform matrix is defined for the inverse transpose of the model transformation matrix, which is used to transform the normal vector into world space
uniform mat4 M_IT;

out Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} vs_out;

void main() {
    // The world position of the vertex is calculated by multiplying the model transformation matrix by the vertex position
    vec3 world = (M * vec4(position, 1.0)).xyz;
    // The world position of the vertex is multiplied by the view-projection transformation matrix to obtain the final position of the vertex on screen
    // The gl_Position built-in variable is set to the final screen position of the vertex, which will be used to rasterize the triangle
    gl_Position = VP * vec4(world, 1.0);
    // The output variable for the vertex color is set to the input vertex color
    vs_out.color = color;
    // The output variable for the vertex texture coordinates is set to the inputtexture coordinates
    vs_out.tex_coord = tex_coord;
    // The vertex normal is transformed into world space by multiplying it with the inverse transpose of the model transformation matrix, and then normalizing the resulting vector
    vs_out.normal = normalize((M_IT * vec4(normal, 0.0)).xyz);
    // The view direction is calculated by subtracting the camera position from the world position of the vertex
    vs_out.view = camera_position - world;
    // The world position of the vertex is assigned to the output variable for the world position
    vs_out.world = world;
}