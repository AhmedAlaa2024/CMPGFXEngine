#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

// How far (in the texture space) is the distance (on the x-axis) between
// the pixels from which the red/green (or green/blue) channels are sampled
#define STRENGTH 0.005

// Chromatic aberration mimics some old cameras where the lens disperses light
// differently based on its wavelength. In this shader, we will implement a
// cheap version of that effect 

void main(){
    //TODO: Modify this shader to apply chromatic abberation
    // To apply this effect, we only read the green channel from the correct pixel (as defined by tex_coord)
    // To get the red channel, we move by amount STRENGTH to the left then sample another pixel from which we take the red channel
    // To get the blue channel, we move by amount STRENGTH to the right then sample another pixel from which we take the blue channel
     vec3 colors;
     //========================================================================================================================//
     //          move by amount STRENGTH to the right then sample another pixel from which we take the red channel             //
     //          we do move the by anmount strength to the left to sample the pixel to the left of the current pixel          //                                                     //
    //======================================================================================================================//

     colors.r=texture(tex,vec2(tex_coord[0]-STRENGTH,tex_coord[1])).r;
      //read the green channel from the current pixel
     colors.g=texture(tex,tex_coord).g;
       //========================================================================================================================//
     //          move by amount STRENGTH to the right then sample another pixel from which we take the blue channel            //
     //          we do move the by anmount strength to the right to sample the pixel to the right of the current pixel        //                                                     //
    //======================================================================================================================//
     colors.b=texture(tex,vec2(tex_coord[0]+STRENGTH,tex_coord[1])).b;
     frag_color = vec4(colors,1);
}