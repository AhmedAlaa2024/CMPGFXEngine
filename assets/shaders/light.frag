#version 330

#define DIRECTIONAL 0
<<<<<<< Updated upstream
#define POINT       1
#define SPOT        2

struct Light {
=======
#define POINT 1
#define SPOT 2

struct Light{
>>>>>>> Stashed changes
    int type;
    vec3 position;
    vec3 direction;
    vec3 color;
    vec3 attenuation;
    vec2 cone_angles;
};

#define MAX_LIGHTS 8

uniform Light lights[MAX_LIGHTS];
uniform int light_count;

// for ambient_light component dependent on direction
<<<<<<< Updated upstream
struct Sky {
    vec3 top, horizon, bottom;
=======
struct Sky{
    vec3 top,horizon,bottom;
>>>>>>> Stashed changes
};

uniform Sky sky;
// if normal is up, will have ambient light from top
// if normal is down, will have ambient light from bottom
vec3 compute_sky_light(vec3 normal){
<<<<<<< Updated upstream
    vec3 extreme = normal.y > 0 ? sky.top : sky.bottom;
    // interpolate
    return mix(sky.horizon, extreme, normal.y * normal.y);
}

struct Material {
=======
    vec3 extreme=normal.y>0?sky.top:sky.bottom;
    // interpolate
    return mix(sky.horizon,extreme,normal.y*normal.y);
}

struct Material{
>>>>>>> Stashed changes
    sampler2D albedo;
    sampler2D specular;
    sampler2D roughness;
    sampler2D ambient_occlusion;
    sampler2D emissive;
};

uniform Material material;

<<<<<<< Updated upstream
in Varyings {
=======
in Varyings{
>>>>>>> Stashed changes
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
<<<<<<< Updated upstream
} fs_in;

out vec4 frag_color;

float lambert(vec3 normal, vec3 world_to_light_direction) {
    return max(0.0, dot(normal, world_to_light_direction));
}

float phong(vec3 reflected, vec3 view, float shininess) {
    return pow(max(0.0, dot(reflected, view)), shininess);
}

void main() {
    vec3 normal = normalize(fs_in.normal);
    vec3 view = normalize(fs_in.view);
    
    vec3 ambient_light = compute_sky_light(normal);

    vec3 diffuse = texture(material.albedo, fs_in.tex_coord).rgb;
    vec3 specular = texture(material.specular, fs_in.tex_coord).rgb;
    float roughness = texture(material.roughness, fs_in.tex_coord).r;
    vec3 ambient = diffuse * texture(material.ambient_occlusion, fs_in.tex_coord).r;
    vec3 emissive = texture(material.emissive, fs_in.tex_coord).rgb;

    float shininess = 2.0 / pow(clamp(roughness, 0.001, 0.999), 4.0) - 2.0;
    
    vec3 world_to_light_dir;
    vec3 color = emissive + ambient_light * ambient;

    for(int light_idx = 0; light_idx < min(MAX_LIGHTS, light_count); light_idx++){
        Light light = lights[light_idx];
        float attenuation = 1.0;
        if(light.type == DIRECTIONAL){
            world_to_light_dir = -light.direction;
        } else {
            world_to_light_dir = light.position - fs_in.world;
            float d = length(world_to_light_dir);
            world_to_light_dir /= d;

            attenuation = 1.0 / dot(light.attenuation, vec3(d*d, d, 1.0));

            if(light.type == SPOT){
                float angle = acos(dot(light.direction, -world_to_light_dir));
                attenuation *= smoothstep(light.cone_angles.y, light.cone_angles.x, angle);
            }
        }

        vec3 computed_diffuse = light.color * diffuse * lambert(normal, world_to_light_dir);

        vec3 reflected = reflect(-world_to_light_dir, normal);
        vec3 computed_specular = light.color * specular * phong(reflected, view, shininess);

        color += (computed_diffuse + computed_specular) * attenuation;

    }
    
    frag_color = vec4(color, 1.0);
=======
}fs_in;

out vec4 frag_color;

float lambert(vec3 normal,vec3 world_to_light_direction){
    return max(0.,dot(normal,world_to_light_direction));
}

float phong(vec3 reflected,vec3 view,float shininess){
    return pow(max(0.,dot(reflected,view)),shininess);
}

void main(){
    vec3 normal=normalize(fs_in.normal);
    vec3 view=normalize(fs_in.view);
    
    vec3 ambient_light=compute_sky_light(normal);
    
    vec3 diffuse=texture(material.albedo,fs_in.tex_coord).rgb;
    vec3 specular=texture(material.specular,fs_in.tex_coord).rgb;
    float roughness=texture(material.roughness,fs_in.tex_coord).r;
    vec3 ambient=diffuse*texture(material.ambient_occlusion,fs_in.tex_coord).r;
    vec3 emissive=texture(material.emissive,fs_in.tex_coord).rgb;
    
    float shininess=2./pow(clamp(roughness,.001,.999),4.)-2.;
    
    vec3 world_to_light_dir;
    vec3 color=emissive+ambient_light*ambient;
    
    int actual_light_count=MAX_LIGHTS;//min(MAX_LIGHTS,light_count);
    for(int light_idx=0;light_idx<actual_light_count;light_idx++){
        Light light=lights[light_idx];
        float attenuation=1.;
        if(light.type==DIRECTIONAL){
            world_to_light_dir=-light.direction;
        }else{
            world_to_light_dir=light.position-fs_in.world;
            float d=length(world_to_light_dir);
            world_to_light_dir/=d;
            
            attenuation=1./dot(light.attenuation,vec3(d*d,d,1.));
            
            if(light.type==SPOT){
                float angle=acos(dot(light.direction,-world_to_light_dir));
                attenuation*=smoothstep(light.cone_angles.y,light.cone_angles.x,angle);
            }
        }
        
        vec3 computed_diffuse=light.color*diffuse*lambert(normal,world_to_light_dir);
        
        vec3 reflected=reflect(-world_to_light_dir,normal);
        vec3 computed_specular=light.color*specular*phong(reflected,view,shininess);
        
        color+=(computed_diffuse+computed_specular)*attenuation;
    }
    
    frag_color=vec4(color,1.);
>>>>>>> Stashed changes
    // frag_color = vec4(fs_in.normal, 1.0);
}