#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 3) Write th is function
        glm::mat4 S = glm::scale(glm::mat4(1.0f), this->scale);

        // glm::mat4 R = glm::mat4(1.0f);
        // R = glm::rotate(R, this->rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
        // R = glm::rotate(R, this->rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
        // R = glm::rotate(R, this->rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));

        glm::mat4 R = glm::yawPitchRoll(this->rotation.y, this->rotation.x, this->rotation.z);

        glm::mat4 T = glm::translate(glm::mat4(), this->position);

        glm::mat4 transform = T * R * S;

        return transform; 
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
    }

}