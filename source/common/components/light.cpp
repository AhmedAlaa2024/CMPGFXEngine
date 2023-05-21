
#include "light.hpp"
#include "../deserialize-utils.hpp"
#include "./component-deserializer.hpp"
#include <glm/gtx/euler_angles.hpp>
#include <iostream>

namespace our
{
    // deserialize the data from a JSON object
    void LightComponent::deserialize(const nlohmann::json &data)
    {   // checks if the input data is a JSON object, and returns if it is not
        if (!data.is_object())
            return;
        // The color variable of the LightComponent is deserialized from the "color" field in the input JSON object, or set to a default blue color if the field is not present
        color = glm::vec3(data.value("color", glm::vec3(0,0,1)));
        // The type of the light source is deserialized from the "lighttype" field in the input JSON object
        // set to a default directional light type if the field is not present or invalid
        // The type is represented as an integer, with 0 for directional, 1 for point, and 2 for spotlight
        std::string typeStr = data.value("lighttype", "DIRECTIONAL");
        if (typeStr == "DIRECTIONAL")
            type = 0;

        else if (typeStr == "POINT")
            type = 1;

        else if (typeStr == "SPOT")
            type =2;

        // If the light type is not directional, the attenuation values are deserialized from the "attenuation" field in the input JSON object, or set to a default value of (1,1,1) if the field is not present
        if (type != 0)
        {
        attenuation = glm::vec3(data.value("attenuation", glm::vec3(1, 1, 1)));
        }
        // If the light type is a spotlight, the cone angle values are deserialized from the "cone_angles.in" and "cone_angles.out" fields in the input JSON object, or set to default values of 10 and 80 degrees, respectively, if the fields are notpresent
        if (type ==2)
        { 
        // The glm::radians function is used to convert the input angle values from degrees to radians
        cone_angles.x = glm::radians((float)data.value("cone_angles.in",10));
        cone_angles.y = glm::radians((float)data.value("cone_angles.out",80));
        }
        // The deserialization is complete :)
    }
    
}