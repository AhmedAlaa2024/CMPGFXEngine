#version 330
// Three preprocessor directives are defined to represent different types of light sources 
// These directives are used later in the code to specify the type of light source being used
#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2
// A struct is defined to represent a light source
// It contains variables for the:
// type of light, position, direction, color, attenuation, and cone angles (for spotlights).
struct Light{
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 attenuation;
    vec2 cone_angles;
};
// A preprocessor directive is defined to set the maximum number of lights that can be used in the scene
#define MAX_LIGHTS 8
// Two uniforms are defined for an array of lights and the number of lights being used in the scene
uniform Light lights[MAX_LIGHTS];
uniform int light_count;
// A struct is defined to represent the sky, which provides ambient light in the scene
// It contains variables for the top, horizon, and bottom colors
struct Sky{
    vec3 top,horizon,bottom;
};
uniform Sky sky;
// if normal is up, will have ambient light from top
// if normal is down, will have ambient light from bottom
vec3 compute_sky_light(vec3 normal){
    vec3 extreme=normal.y>0?sky.top:sky.bottom;
    // interpolate
    return mix(sky.horizon,extreme,normal.y*normal.y);
}

struct Material{
    sampler2D albedo;
    sampler2D specular;
    sampler2D roughness;
    sampler2D ambient_occlusion;
    sampler2D emissive;
};
//  A uniform is defined for the material properties of the object being rendered, including albedo, specular, roughness, ambient occlusion, and emissive textures
uniform Material material;
// An input struct is defined for the varying values passed in from the vertex shader, including color, texture coordinates, normal, view direction, and world coordinates
in Varyings{
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
}fs_in;
// An output variable is defined to set the final color of the fragment after lighting calculations
out vec4 frag_color;
// Two functions are defined for computing the Lambertian and Phong components of the lighting equation, which are used later inthe code to calculate the diffuse and specular lighting respectively
float lambert(vec3 normal,vec3 world_to_light_direction){
    return max(0.,dot(normal,world_to_light_direction));
}
float phong(vec3 reflected,vec3 view,float shininess){
    return pow(max(0.,dot(reflected,view)),shininess);
}

void main(){
    // normalizing the surface normal and view direction vectors passed in from the vertex shader
    vec3 normal=normalize(fs_in.normal);
    vec3 view=normalize(fs_in.view);
    // The ambient light component is calculated by calling the compute_sky_light function, passing in the surface normal vector
    vec3 ambient_light=compute_sky_light(normal);
    // The albedo, specular, roughness, ambient occlusion, and emissive textures are sampled to obtain their respective colors 
    vec3 diffuse=texture(material.albedo,fs_in.tex_coord).rgb;
    vec3 specular=texture(material.specular,fs_in.tex_coord).rgb;
    float roughness=texture(material.roughness,fs_in.tex_coord).r;
    vec3 ambient=diffuse*texture(material.ambient_occlusion,fs_in.tex_coord).r;
    vec3 emissive=texture(material.emissive,fs_in.tex_coord).rgb;
    // the roughness is used to calculate the shininess value for the Phong lighting component
    float shininess=2./pow(clamp(roughness,.001,.999),4.)-2.;
    
    vec3 world_to_light_dir;
    vec3 color=emissive+ambient_light*ambient;
    
    int actual_light_count = min(MAX_LIGHTS,light_count);
    // A loop is used to iterate through each light in the scene and calculate the contribution of that light to the final color of the fragment
    for(int light_idx=0;light_idx<actual_light_count;light_idx++){
        Light light=lights[light_idx];
        float attenuation=1.;
        // type of light source (directional, point, or spotlight) is used to determine the direction towards the light source
        if(light.type==DIRECTIONAL){
            world_to_light_dir=-light.direction;
        }else{
            world_to_light_dir=light.position-fs_in.world;
            float d=length(world_to_light_dir);
            world_to_light_dir/=d;
            // The attenuation factor is calculated based on the distance between the fragment and the light source
            attenuation=1./dot(light.attenuation,vec3(d*d,d,1.));
            
            if(light.type==SPOT){
                float angle=acos(dot(light.direction,-world_to_light_dir));
                attenuation*=smoothstep(light.cone_angles.y,light.cone_angles.x,angle);
            }
        }
        // The diffuse lighting component is calculated using the Lambertian function, and is multiplied by the light color and material color
        vec3 computed_diffuse=light.color*diffuse*lambert(normal,world_to_light_dir);
        vec3 reflected=reflect(-world_to_light_dir,normal);
        // The specular lighting component is calculated using the Phong function, and is multiplied by the light color and material color
        vec3 computed_specular=light.color*specular*phong(reflected,view,shininess);
        // The computed diffuse and specular lighting components are multiplied by the attenuation factor and added to the existing color
        color+=(computed_diffuse+computed_specular)*attenuation;
    }
    // The final color is assigned to the output variable frag_color
    frag_color=vec4(color,1.);
}