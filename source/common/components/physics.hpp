#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp>

namespace our {

  // This component denotes that the PhyscisSystem will describe the owning entity by a certain physcis effects like gravity, collision, etc.
  class PhysicsComponent : public Component {
  public:
    // glm::vec3 linearAcceleration = {0, 0, 0}; // The acceleration of the entity in 3D space
    // glm::vec3 angularAcceleration = {0, 0, 0}; // The angular acceleration of the entity in 3D space
    // float mass = 1.0f; // The mass of the entity, which determines how it responds to forces
    // float friction = 0.0f; // The friction of the entity, which determines how it interacts with surfaces
    // bool isStatic = false; // Whether the entity is static and should not move
    bool isCollidable = true; // Whether the entity is collidable and should collide with other entities
    float collisionRestitution = 0.0f; // The restitution of the entity, which determines how it bounces off of other entities
    // glm::vec3 gravity = {0, -9.8f, 0}; // The gravity of the entity, which determines how it falls

    // The ID of this component type is "Physics"
    static std::string getID() { return "Physics"; }

    // Reads position, velocity, mass, friction, and isStatic from the given json object
    void deserialize(const nlohmann::json& data) override;
};
}