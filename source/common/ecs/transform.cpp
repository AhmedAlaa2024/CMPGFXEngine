#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //TODO: (Req 3) Write th is function
        // Explaination: The following line creates a glm::mat4 matrix S that scales the object
        // based on the current scale vector stored in the Transform object.
        // The glm::scale function takes a matrix and a vector as input, and returns a scaled matrix.
        // The glm::mat4(1.0f) creates an identity matrix to start with,
        // and the this->scale argument scales this matrix by the specified vector.
        glm::mat4 S = glm::scale(glm::mat4(1.0f), this->scale);

        // glm::mat4 R = glm::mat4(1.0f);
        // R = glm::rotate(R, this->rotation[0], glm::vec3(1.0f, 0.0f, 0.0f));
        // R = glm::rotate(R, this->rotation[1], glm::vec3(0.0f, 1.0f, 0.0f));
        // R = glm::rotate(R, this->rotation[2], glm::vec3(0.0f, 0.0f, 1.0f));

        // Explaination: The following line creates a glm::mat4 matrix R that rotates the object
        // based on the current rotation vector stored in the Transform object.
        // The glm::yawPitchRoll function takes three float values representing the yaw, pitch, and roll angles of the object,
        // and returns a matrix that represents this rotation. The this->rotation.y, this->rotation.x, and this->rotation.z values
        // represent the rotation of the object around the y, x, and z axes, respectively.
        glm::mat4 R = glm::yawPitchRoll(this->rotation.y, this->rotation.x, this->rotation.z);

        // Explaination: The following line creates a glm::mat4 matrix T that translates the object
        // based on the current position vector stored in the Transform object.
        // The glm::translate function takes a matrix and a vector as input, and returns a translated matrix.
        // The glm::mat4(1.0f) creates an identity matrix to start with, and the this->position argument translates this matrix by the specified vector.
        glm::mat4 T = glm::translate(glm::mat4(1.0f), this->position);

        // Explaination: The following line creates a glm::mat4 matrix transform that represents the combined scale, rotation, and translation transformations of the object.
        // The order of the transformations is important, as they are applied from right to left.
        // In this case, the scale transformation S is applied first, then the rotation transformation R, and finally the translation transformation T.
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