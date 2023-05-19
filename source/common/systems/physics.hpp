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
        std::cout << "PhysicsSystem: " << entity->name << std::endl;
        // std::cout << "PhysicsSystem::update: " << entity->name << std::endl;
        // Get the physics component if it exists
        PhysicsComponent *physics = entity->getComponent<PhysicsComponent>();
        MovementComponent *movement = entity->getComponent<MovementComponent>();
        
        // If the physics component exists
        if (physics)
        {
          // Update the linear and angular velocity based on the linear and angular acceleration, mass, and delta time
          if (!physics->isStatic)
          {

            // Apply linear and angular acceleration
            physics->linearAcceleration += physics->gravity;
            movement->linearVelocity += physics->linearAcceleration * deltaTime;
            movement->angularVelocity += physics->angularAcceleration * deltaTime;

            // Apply friction
            glm::vec3 frictionForce = -physics->friction * movement->linearVelocity;
            physics->linearAcceleration += frictionForce / physics->mass;

            // Apply collision if the entity is collidable and moving
            if(physics->isCollidable && movement->linearVelocity != glm::vec3(0,0,0)){
                movement->linearVelocity *= -physics->collisionRestitution;
            }

            // Update position and rotation based on velocity
            entity->localTransform.position += movement->linearVelocity * deltaTime;
            entity->localTransform.rotation += movement->angularVelocity * deltaTime;
          }
        }
      }
    }
  };

}