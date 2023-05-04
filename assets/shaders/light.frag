// This shader code calculates the lighting for a single light source, based on its color, position, direction, attenuation, spot angle, and type
// It uses the surface color and normal to calculate the diffuse and specular lighting, and applies attenuation and cone angles for spotlights
#version 330

uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform vec3 lightAttenuation;
uniform vec2 lightConeAngle;
uniform int lightType; // 0 = point, 1 = directional, 2 = spot

in Varyings {
    vec4 color;
    vec3 vertexNormal;
    vec3 vertexPosition;
} fs_in;

out vec4 fragColor;

void main()
{
    vec3 surfaceColor = vec3(1.0, 1.0, 1.0); // the color of the surface
    vec3 surfaceNormal = normalize(fs_in.vertexNormal); // the normal of the surface
    vec3 surfaceToLight = vec3(0.0, 0.0, 0.0); // the vector from the surface to the light
    float attenuation = 1.0;
    if (lightType == 0) // point light
    {
        surfaceToLight = lightPosition - fs_in.vertexPosition;
        float distanceToLight = length(surfaceToLight);
        surfaceToLight /= distanceToLight;
        attenuation = 1.0 / dot(lightAttenuation, vec3(distanceToLight*distanceToLight, distanceToLight, 1.0));
    }
    else if (lightType == 1) // directional light
    {
        surfaceToLight = -lightDirection;
    }
    else if (lightType == 2) // spot light
    {
        surfaceToLight = lightPosition - fs_in.vertexPosition;
        float distanceToLight = length(surfaceToLight);
        surfaceToLight /= distanceToLight;
        attenuation = 1.0 / dot(lightAttenuation, vec3(distanceToLight*distanceToLight, distanceToLight, 1.0));
        float spotFactor = dot(surfaceToLight, -lightDirection);
        float angle = acos(spotFactor);
        attenuation *= smoothstep(lightConeAngle.y, lightConeAngle.x, angle);
    }

    // calculate the diffuse and specular lighting
    float diffuseFactor = max(0.0, dot(surfaceNormal, surfaceToLight));

    float specularFactor = 0.0;
    vec3 surfaceToEye = normalize(-fs_in.vertexPosition);
    vec3 reflection = reflect(-surfaceToLight, surfaceNormal);
    specularFactor = pow(max(dot(reflection, surfaceToEye), 0.0), 32.0);

    vec3 diffuseColor = lightColor * surfaceColor * diffuseFactor;
    vec3 specularColor = lightColor * specularFactor;

    fragColor = vec4((diffuseColor + specularColor) * attenuation, 1.0);
}

