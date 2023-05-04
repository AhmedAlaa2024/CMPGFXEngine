#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp> 

// This component includes properties that are used to define a light
// You can customize these properties to create a specific lighting
namespace our {

    class LightComponent : public Component {
    public:
        int type;
        glm::vec3 color;
        glm::vec3 attenuation;
        glm::vec2 cone_angles;
    };
}