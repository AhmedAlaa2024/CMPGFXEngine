#pragma once

#include "../ecs/world.hpp"
#include "../components/physics.hpp"
#include "../components/movement.hpp"
#include "../components/mesh-renderer.hpp"

#include <glm/glm.hpp>

#include <unordered_set>

namespace our
{
  class PhysicsSystem
  {
  public:
    glm::vec3 playerOldPosition = glm::vec3(0.0f, 0.0f, 0.0f);

    void update(World *world, float deltaTime)
    {
      std::unordered_set<Entity *> processedEntities;

      for (auto collider : world->getEntities())
      {
        MovementComponent *colliderMovement = collider->getComponent<MovementComponent>();
        PhysicsComponent *colliderPhysics = collider->getComponent<PhysicsComponent>();

        if (colliderPhysics)
        {
          for (auto collidedEntity : world->getEntities()) {
            if (collider == collidedEntity || processedEntities.count(collider) || processedEntities.count(collidedEntity))
              continue;  // Skip already processed entities
            
            MovementComponent *collidedMovement = collider->getComponent<MovementComponent>();
            PhysicsComponent *collidedPhysics = collidedEntity->getComponent<PhysicsComponent>();

            if (!collidedPhysics)
              continue;

            glm::vec3 colliderCenter = glm::vec3(0.0f), CollidedCenter = glm::vec3(0.0f);
            float colliderRadius = 0.0f, collidedRadius = 0.0f;

            if (collider->getComponent<MeshRendererComponent>()) {
              colliderCenter = collider->getComponent<MeshRendererComponent>()->mesh->getCenter();
              colliderRadius = collider->getComponent<MeshRendererComponent>()->mesh->getRadius();
            }

            if (collidedEntity->getComponent<MeshRendererComponent>()) {
              colliderCenter = collider->getComponent<MeshRendererComponent>()->mesh->getCenter();
              colliderRadius = collider->getComponent<MeshRendererComponent>()->mesh->getRadius();
            }

            glm::vec3 colliderLiveCenter = glm::vec3(collider->getLocalToWorldMatrix() * glm::vec4(colliderCenter, 1.0f));
            glm::vec3 collidedLiveCenter = glm::vec3(collidedEntity->getLocalToWorldMatrix() * glm::vec4(CollidedCenter, 1.0f));

            float distance = glm::distance(colliderLiveCenter, collidedLiveCenter);

            // Extract the scale from the world transformation matrix
            glm::mat4 localToWorldMat = collider->getLocalToWorldMatrix();
            glm::vec3 scale;
            scale.x = glm::length(glm::vec3(localToWorldMat[0][0], localToWorldMat[0][1], localToWorldMat[0][2]));
            scale.y = glm::length(glm::vec3(localToWorldMat[1][0], localToWorldMat[1][1], localToWorldMat[1][2]));
            scale.z = glm::length(glm::vec3(localToWorldMat[2][0], localToWorldMat[2][1], localToWorldMat[2][2]));

            std::cout << "Scale of Collider: " << scale.x << "," << scale.y << "," << scale.z << std::endl;

            // Find the maximum scale component
            float maxScale = glm::max(scale.x, glm::max(scale.y, scale.z));

            // Transform the local radius to world space
            colliderRadius = colliderRadius * maxScale;

            // Extract the scale from the world transformation matrix
            localToWorldMat = collidedEntity->getLocalToWorldMatrix();
            scale.x = glm::length(glm::vec3(localToWorldMat[0][0], localToWorldMat[0][1], localToWorldMat[0][2]));
            scale.y = glm::length(glm::vec3(localToWorldMat[1][0], localToWorldMat[1][1], localToWorldMat[1][2]));
            scale.z = glm::length(glm::vec3(localToWorldMat[2][0], localToWorldMat[2][1], localToWorldMat[2][2]));

            // Find the maximum scale component
            maxScale = glm::max(scale.x, glm::max(scale.y, scale.z));

            // Transform the local radius to world space
            collidedRadius = collidedRadius * maxScale;

            float sumOfRadius = colliderRadius + collidedRadius;
            
            // glm::vec3 direction = glm::normalize(colliderLiveCenter - collidedLiveCenter);

              std::cout << "Collision: " << collider->name << " -> " << collidedEntity->name << std::endl;
              std::cout << "Distance: " << distance << std::endl;
              std::cout << "Sum of Radius: " << sumOfRadius << std::endl;
              std::cout << "Scale of Collided Entity: " << scale.x << "," << scale.y << "," << scale.z << std::endl;
            if (distance <= sumOfRadius) {
              if (colliderMovement)
                collider->localTransform.position -= deltaTime * colliderMovement->linearVelocity;
              else if (collidedMovement)
                collidedEntity->localTransform.position -= deltaTime * collidedMovement->linearVelocity;
              else {
                if (collider->name == "sword") {
                  collider->parent->localTransform.position = this->playerOldPosition;
                } else if (collidedEntity->name == "sword") {
                  collidedEntity->parent->localTransform.position = this->playerOldPosition;
                }
              }

              colliderPhysics->isCollided = true;
            } else {
              colliderPhysics->isCollided = false;
            }

            // Add the entity pair to the set of processed entities
            processedEntities.insert(collider);
            processedEntities.insert(collidedEntity);
          }
        }
      }
    }
  };
}