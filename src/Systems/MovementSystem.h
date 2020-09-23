#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include <iostream>

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

class MovementSystem: public System {
    public:
        MovementSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<RigidBodyComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            
        }

        void Update(std::unique_ptr<Registry>& registry, double deltaTime) {
            for (auto entity: GetSystemEntities()) {
                // Physical body movement
                const RigidBodyComponent rigidbody = entity.GetComponent<RigidBodyComponent>();
                TransformComponent& transform = entity.GetComponent<TransformComponent>();
                    
                transform.position.x += rigidbody.velocity.x * deltaTime;
                transform.position.y += rigidbody.velocity.y * deltaTime;

                // Kill entities that move beyond the limits of the map
                if (transform.position.x < 0 || transform.position.x > Game::mapWidth || transform.position.y < 0 || transform.position.y > Game::mapHeight) {
                    std::cout << "Killing entity " << entity.GetId() << " because it went outside the boundaries of the map." << std::endl;
                    entity.Kill();
                }
            }
        }
};

#endif