#include "physics.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

namespace our {
  // Reads linearVelocity & angularVelocity from the given json object
  void PhysicsComponent::deserialize(const nlohmann::json& data){
    if(!data.is_object())
      return;
      
    linearAcceleration = data.value("linearAcceleration", linearAcceleration);
    angularAcceleration = glm::radians(data.value("angularAcceleration", angularAcceleration));
    mass = data.value("mass", mass);
    friction = data.value("friction", friction);
    isStatic = data.value("isStatic", isStatic);
    isCollidable = data.value("isCollidable", isCollidable);
    collisionRestitution = data.value("collisionRestitution", collisionRestitution);
    gravity = data.value("gravity", gravity);
  }
}