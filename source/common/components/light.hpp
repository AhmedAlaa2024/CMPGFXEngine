#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp> 

// This component includes properties that are used to define a light
// You can customize these properties to create a specific lighting
// This light component includes properties for color, attenuation, cone angles, and type
namespace our {

    // It also has an enum class for the different types of lights that can be used in the scene
    enum class Type { Point, Directional, Spot };

    class LightComponent : public Component {
    public:
        Type type;
        glm::vec3 color;
        glm::vec3 attenuation;
        glm::vec2 cone_angles;
        // The position and direction of the light are not included in the component, 
        // as they will be calculated from the entity component
    };
}