#pragma once

#include "../ecs/world.hpp"
#include "../components/movement.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "../components/physics.hpp"

namespace our
{
    class GameSystem {
    public:
        void update(World* world, float deltaTime) {
            // For each entity in the world
            for(auto entity : world->getEntities()){
                // Get the movement component if it exists
                MovementComponent* movement = entity->getComponent<MovementComponent>();
                PhysicsComponent* physics = entity->getComponent<PhysicsComponent>();
                
                // If the movement component exists
                if(movement){
                    
                }
            }
        }

    };

}
