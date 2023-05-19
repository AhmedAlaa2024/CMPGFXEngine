#pragma once

#include "../ecs/world.hpp"
#include "../components/physics.hpp"
#include "../components/movement.hpp"

#include <glm/glm.hpp>

namespace our
{

  // The physics system is responsible for updating the physics of every entity which contains a PhysicsComponent.
  class PhysicsSystem
  {
  public:
    // This should be called every frame to update all entities containing a PhysicsComponent.
    void update(World *world, float deltaTime)
    {
      // For each entity in the world
      for (auto entity : world->getEntities())
      {
        // Get the physics component if it exists
        PhysicsComponent *physics = entity->getComponent<PhysicsComponent>();
        MovementComponent *movement = entity->getComponent<MovementComponent>();
        
        // If the physics component exists
        if (physics)
        {
          std::cout << "Physics component exists" << std::endl;
          for (auto collidedEntity : world->getEntities()) {
            if (entity == collidedEntity) continue;
            PhysicsComponent *collidedPhysics = collidedEntity->getComponent<PhysicsComponent>();
            float distance = glm::distance(entity->localTransform.position, collidedEntity->localTransform.position);
            if (distance < 10) {
              std::cout << "Collision occured between: " << entity->name << " and " << collidedEntity->name << std::endl;
            }
            std::cout << "Distance: " << distance << std::endl;
          }
        }
      }
    }
  };

}