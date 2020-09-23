#ifndef PROJECTILESYSTEM_H
#define PROJECTILESYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ProjectileEmitterComponent.h"

class ProjectileSystem: public System {
    public:
        ProjectileSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<ProjectileEmitterComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->ListenToEvent<KeyPressedEvent>(this, &ProjectileSystem::OnKeyPressed);
        }

        void OnKeyPressed(KeyPressedEvent& event) {
            if (event.symbol == SDLK_SPACE) {
                for (auto entity: GetSystemEntities()) {
                    const ProjectileEmitterComponent projectileComponent = entity.GetComponent<ProjectileEmitterComponent>();
                    const TransformComponent transformComponent = entity.GetComponent<TransformComponent>();
                    const RigidBodyComponent rigidBodyComponent = entity.GetComponent<RigidBodyComponent>();

                    glm::vec2 projectilePosition = transformComponent.position;
                    // If parent entity has sprite, start the projectile position in the middle of the entity
                    if (entity.HasComponent<SpriteComponent>()) {
                        SpriteComponent spriteComponent = entity.GetComponent<SpriteComponent>();
                        projectilePosition.x += static_cast<int>((transformComponent.scale.x * spriteComponent.width) / 2);
                        projectilePosition.y += static_cast<int>((transformComponent.scale.y * spriteComponent.height) / 2);
                    }

                    glm::vec2 projectileVelocity = projectileComponent.projectileVelocity;
                    // If parent entity direction is controlled by the keyboard keys, modify the direction of the projectile accordingly
                    if (entity.HasComponent<KeyboardControlledComponent>()) {
                        glm::vec2 normalizedVelocityDirection = glm::normalize(rigidBodyComponent.velocity); // normalize to get a vector with length 1
                        projectileVelocity.x = normalizedVelocityDirection.x * projectileVelocity.x;         // multiply by the velocity intensity factor of the projectile
                        projectileVelocity.y = normalizedVelocityDirection.y * projectileVelocity.y;         // multiply by the velocity intensity factor of the projectile
                    }

                    Entity projectile = entity.registry->CreateEntity();
                    projectile.AddComponent<TransformComponent>(projectilePosition, glm::vec2(1, 1), 0.0);
                    projectile.AddComponent<RigidBodyComponent>(projectileVelocity);
                    projectile.AddComponent<SpriteComponent>("bullet-texture", 4, 4, 5);
                    projectile.AddComponent<BoxColliderComponent>(glm::vec2(0), 4, 4);
                }
            }
        }

        void Update(std::unique_ptr<Registry>& registry) {
            // TODO: emit projectiles from enemies
        }
};

#endif