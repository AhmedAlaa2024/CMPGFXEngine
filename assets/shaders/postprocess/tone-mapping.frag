#version 330 core

out vec4 frag_color;
in vec2 tex_coord;

uniform sampler2D tex;

// exposure constrols the brightness of output image
uniform float exposure = 1.0;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(tex, tex_coord).rgb;
  
    // exposure tone mapping
    //applies an exposure adjustment to the HDR color values sampled from the texture
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    //Apply gamma correction to adjust intensity of colors
    //maping high dynamic range to lower dynamic range
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    frag_color = vec4(mapped, 1.0);
} 