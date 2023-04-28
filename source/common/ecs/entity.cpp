#include "entity.hpp"
#include "../deserialize-utils.hpp"
#include "../components/component-deserializer.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function returns the transformation matrix from the entity's local space to the world space
    // Remember that you can get the transformation matrix from this entity to its parent from "localTransform"
    // To get the local to world matrix, you need to combine this entities matrix with its parent's matrix and
    // its parent's parent's matrix and so on till you reach the root.
    glm::mat4 Entity::getLocalToWorldMatrix() const {
        //TODO: (Req 8) Write this function
        // get the local Transform of entity, which is the matrix that transform a point in the local space of entity
        glm::mat4 Transformation_matrix = this->localTransform.toMat4();
        // get entity parent
        Entity* parent_space = this->parent;
        // while an entity has a parent
        while(parent_space){
            // multiply entity local transform by its parent local transform to get child-to-parent space transformation matrix, to tranform entity in the parent space
            Transformation_matrix = parent_space->localTransform.toMat4() * Transformation_matrix; 
            // get parent of parent 
            parent_space = parent_space->parent;
        }
        // return transformation matrix from the entity's local space to the world space
        return Transformation_matrix;
    }

    // Deserializes the entity data and components from a json object
    void Entity::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        name = data.value("name", name);
        localTransform.deserialize(data);
        if(data.contains("components")){
            if(const auto& components = data["components"]; components.is_array()){
                for(auto& component: components){
                    deserializeComponent(component, this);
                }
            }
        }
    }

}