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

            if (colliderPhysics->boundryType == our::BoundryType::SPHERIC) {
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
            } else if (colliderPhysics->boundryType == our::BoundryType::CUBOID) {
              glm::vec3 colliderCenter = glm::vec3(0.0f), CollidedCenter = glm::vec3(0.0f);
              glm::vec3 colliderXMin, colliderXMax, colliderYMin, colliderYMax, colliderZMin, colliderZMax;
              glm::vec3 collidedXMin, collidedXMax, collidedYMin, collidedYMax, collidedZMin, collidedZMax;

              glm::mat4 colliderLocalToWorldMat = collider->getLocalToWorldMatrix();
              glm::mat4 collidedLocalToWorldMat = collidedEntity->getLocalToWorldMatrix();

              float colliderMinDim = 0.0f, collidedMinDim = 0.0f, colliderMaxDim = 0.0f, collidedMaxDim = 0.0f;

              if (collider->getComponent<MeshRendererComponent>()) {
                colliderCenter = glm::vec3(colliderLocalToWorldMat * glm::vec4(collider->getComponent<MeshRendererComponent>()->mesh->getCenter(), 1.0f));
                colliderXMin = glm::vec3(colliderLocalToWorldMat * glm::vec4(collider->getComponent<MeshRendererComponent>()->mesh->getXMin(), 1.0f));
                colliderXMax = glm::vec3(colliderLocalToWorldMat * glm::vec4(collider->getComponent<MeshRendererComponent>()->mesh->getXMax(), 1.0f));
                colliderYMin = glm::vec3(colliderLocalToWorldMat * glm::vec4(collider->getComponent<MeshRendererComponent>()->mesh->getYMin(), 1.0f));
                colliderYMax = glm::vec3(colliderLocalToWorldMat * glm::vec4(collider->getComponent<MeshRendererComponent>()->mesh->getYMax(), 1.0f));
                colliderZMin = glm::vec3(colliderLocalToWorldMat * glm::vec4(collider->getComponent<MeshRendererComponent>()->mesh->getZMin(), 1.0f));
                colliderZMax = glm::vec3(colliderLocalToWorldMat * glm::vec4(collider->getComponent<MeshRendererComponent>()->mesh->getZMax(), 1.0f));
                
                colliderMinDim = glm::distance(colliderCenter, colliderXMin);
                colliderMinDim == std::min(colliderMinDim, glm::distance(colliderCenter, colliderXMax));
                colliderMinDim == std::min(colliderMinDim, glm::distance(colliderCenter, colliderYMin));
                colliderMinDim == std::min(colliderMinDim, glm::distance(colliderCenter, colliderYMax));
                colliderMinDim == std::min(colliderMinDim, glm::distance(colliderCenter, colliderZMin));
                colliderMinDim == std::min(colliderMinDim, glm::distance(colliderCenter, colliderZMax));

                colliderMaxDim = glm::distance(colliderCenter, colliderXMin);
                colliderMaxDim == std::max(colliderMaxDim, glm::distance(colliderCenter, colliderXMax));
                colliderMaxDim == std::max(colliderMaxDim, glm::distance(colliderCenter, colliderYMin));
                colliderMaxDim == std::max(colliderMaxDim, glm::distance(colliderCenter, colliderYMax));
                colliderMaxDim == std::max(colliderMaxDim, glm::distance(colliderCenter, colliderZMin));
                colliderMaxDim == std::max(colliderMaxDim, glm::distance(colliderCenter, colliderZMax));
              }

              if (collidedEntity->getComponent<MeshRendererComponent>()) {
                CollidedCenter = glm::vec3(collidedLocalToWorldMat * glm::vec4(collidedEntity->getComponent<MeshRendererComponent>()->mesh->getCenter(), 1.0f));
                collidedXMin = glm::vec3(collidedLocalToWorldMat * glm::vec4(collidedEntity->getComponent<MeshRendererComponent>()->mesh->getXMin(), 1.0f));
                collidedXMax = glm::vec3(collidedLocalToWorldMat * glm::vec4(collidedEntity->getComponent<MeshRendererComponent>()->mesh->getXMax(), 1.0f));
                collidedYMin = glm::vec3(collidedLocalToWorldMat * glm::vec4(collidedEntity->getComponent<MeshRendererComponent>()->mesh->getYMin(), 1.0f));
                collidedYMax = glm::vec3(collidedLocalToWorldMat * glm::vec4(collidedEntity->getComponent<MeshRendererComponent>()->mesh->getYMax(), 1.0f));
                collidedZMin = glm::vec3(collidedLocalToWorldMat * glm::vec4(collidedEntity->getComponent<MeshRendererComponent>()->mesh->getZMin(), 1.0f));
                collidedZMax = glm::vec3(collidedLocalToWorldMat * glm::vec4(collidedEntity->getComponent<MeshRendererComponent>()->mesh->getZMax(), 1.0f));
                
                collidedMinDim = glm::distance(CollidedCenter, collidedXMin);
                collidedMinDim == std::min(collidedMinDim, glm::distance(CollidedCenter, collidedXMax));
                collidedMinDim == std::min(collidedMinDim, glm::distance(CollidedCenter, collidedYMin));
                collidedMinDim == std::min(collidedMinDim, glm::distance(CollidedCenter, collidedYMax));
                collidedMinDim == std::min(collidedMinDim, glm::distance(CollidedCenter, collidedZMin));
                collidedMinDim == std::min(collidedMinDim, glm::distance(CollidedCenter, collidedZMax));

                collidedMaxDim = glm::distance(CollidedCenter, collidedXMin);
                collidedMaxDim == std::max(collidedMaxDim, glm::distance(CollidedCenter, collidedXMax));
                collidedMaxDim == std::max(collidedMaxDim, glm::distance(CollidedCenter, collidedYMin));
                collidedMaxDim == std::max(collidedMaxDim, glm::distance(CollidedCenter, collidedYMax));
                collidedMaxDim == std::max(collidedMaxDim, glm::distance(CollidedCenter, collidedZMin));
                collidedMaxDim == std::max(collidedMaxDim, glm::distance(CollidedCenter, collidedZMax));
              }

              float distance = glm::distance(colliderCenter, CollidedCenter);
              std::cout << "Collision: " << collider->name << " -> " << collidedEntity->name << std::endl;
              std::cout << "Distance: " << distance << std::endl;
              std::cout << "Sum of max dimensions: " << colliderMaxDim + collidedMaxDim << std::endl;

              // [TODO] Using the previous position of the collider and collided entity to check for collision
              if (distance <= colliderMaxDim + collidedMaxDim) {
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