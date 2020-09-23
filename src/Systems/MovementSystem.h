#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

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
                const RigidBodyComponent rigidbody = entity.GetComponent<RigidBodyComponent>();
                TransformComponent& transform = entity.GetComponent<TransformComponent>();
                    
                transform.position.x += rigidbody.velocity.x * deltaTime;
                transform.position.y += rigidbody.velocity.y * deltaTime;
            }
        }
};

#endif