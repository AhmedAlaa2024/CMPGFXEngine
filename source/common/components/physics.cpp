#include "physics.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

#include <iostream>

namespace our {
  // Reads linearVelocity & angularVelocity from the given json object
  void PhysicsComponent::deserialize(const nlohmann::json& data){
    if(!data.is_object())
      return;
      
    isCollidable = data.value("isCollidable", isCollidable);
    collisionRestitution = data.value("collisionRestitution", collisionRestitution);
  }
}