#include "camera.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 

namespace our {
    // Reads camera parameters from the given json object
    void CameraComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        std::string cameraTypeStr = data.value("cameraType", "perspective");
        if(cameraTypeStr == "orthographic"){
            cameraType = CameraType::ORTHOGRAPHIC;
        } else {
            cameraType = CameraType::PERSPECTIVE;
        }
        near = data.value("near", 0.01f);
        far = data.value("far", 1000.0f);
        fovY = data.value("fovY", 90.0f) * (glm::pi<float>() / 180);
        orthoHeight = data.value("orthoHeight", 1.0f);
    }

    // Creates and returns the camera view matrix
    // the camera view matrix is used to transform the world coordinates of objects to the view coordinates seen by the camera.
    glm::mat4 CameraComponent::getViewMatrix() const {
        auto owner = getOwner();
        auto M = owner->getLocalToWorldMatrix();
        //TODO: (Req 8) Complete this function
        //HINT:
        // In the camera space:
        // - eye is the origin (0,0,0)
        // - center is any point on the line of sight. So center can be any point (0,0,z) where z < 0. For simplicity, we let center be (0,0,-1)
        // - up is the direction (0,1,0)
        // but to use glm::lookAt, we need eye, center and up in the world state.
        // Since M (see above) transforms from the camera to thw world space, you can use M to compute:
        // - the eye position which is the point (0,0,0) but after being transformed by M
        // - the center position which is the point (0,0,-1) but after being transformed by M
        // - the up direction which is the vector (0,1,0) but after being transformed by M
        // then you can use glm::lookAt

        //To create and return the camera view matrix, we used the glm::lookAt() function provided by the GLM library.
        //This function takes three parameters: 
        // but first we need to get coordinates in the world space, so we first multiplied each point and vector by M 
        return glm::lookAt(
            //parameter1: the position of the camera 
             glm::vec3(M * glm::vec4(0, 0, 0,1)),
            // parameter2: the position of the target that the camera is looking at
             glm::vec3(M * glm::vec4(0, 0, -1,1)),
            // parameter3: the up vector that defines the orientation of the camera
             glm::vec3(M * glm::vec4(0, 1, 0,0))
        );
    }

    // Creates and returns the camera projection matrix
    // "viewportSize" is used to compute the aspect ratio
    glm::mat4 CameraComponent::getProjectionMatrix(glm::ivec2 viewportSize) const {
        //TODO: (Req 8) Wrtie this function
        // NOTE: The function glm::ortho can be used to create the orthographic projection matrix
        // It takes left, right, bottom, top. Bottom is -orthoHeight/2 and Top is orthoHeight/2.
        // Left and Right are the same but after being multiplied by the aspect ratio
        // For the perspective camera, you can use glm::perspective
        float aspect_ratio = viewportSize.x / float(viewportSize.y);

        // if CameraType == ORTHOGRAPHIC
        // we use the glm::ortho() function provided by the GLM library.
        // This function takes six parameters: 
        if (cameraType == CameraType::ORTHOGRAPHIC){
            glm::mat4 orthographic = glm::ortho(
            // left
            (-orthoHeight / 2) * aspect_ratio,
            // right
            (orthoHeight / 2) * aspect_ratio,
            // bottom
            -orthoHeight / 2,
            // top
            orthoHeight / 2,
            // the near clipping planes
            near,
            // the far clipping planes
            far);
            // The glm::ortho() function is used to create the projection matrix, which is then returned as a glm::mat4 object
            return orthographic;
        }

        // if CameraType == PERSPECTIVE
        else
        {
        // we use the glm::perspective() function provided by the GLM library. 
        // This function takes four parameters: 
            glm::mat4 projection = glm::perspective(
                // the field of view in degrees 
                fovY,
                // the aspect ratio of the viewport
                aspect_ratio,
                //the near clipping planes 
                near,
                // the far clipping planes.
                far);
            // The glm::perspective() function is used to create the projection matrix, which is then returned as a glm::mat4 object
            return  projection;
        }
    }
}