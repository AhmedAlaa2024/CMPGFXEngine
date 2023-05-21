#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {

  // An enum that defines the type of the camera (ORTHOGRAPHIC or PERSPECTIVE) 
  enum class BoundryType {
    CUBOID,
    SPHERIC
  };

  // This component denotes that the PhyscisSystem will describe the owning entity by a certain physcis effects like gravity, collision, etc.
  class PhysicsComponent : public Component {
  public:
    BoundryType boundryType = BoundryType::CUBOID; // The type of the boundry of the entity
    glm::vec3 center = glm::vec3(0.0f); // The center of the entity, which determines how it collides with other entities
    float radius = 1.0f; // The radius of the entity, which determines how it collides with other entities
    bool isCollidable = true; // Whether the entity is collidable and should collide with other entities
    float collisionRestitution = 0.0f; // The restitution of the entity, which determines how it bounces off of other entities

    bool isCollided = false;


    // The ID of this component type is "Physics"
    static std::string getID() { return "Physics"; }

    // Reads position, velocity, mass, friction, and isStatic from the given json object
    void deserialize(const nlohmann::json& data) override;
};
}