#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include <iostream>

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Events/CollisionEvent.h"

class CollisionSystem: public System {
    public:
        CollisionSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<BoxColliderComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            
        }

        void Update(std::unique_ptr<Registry>& registry, std::unique_ptr<EventBus>& eventBus) {
            auto entities = GetSystemEntities();

            for (auto i = entities.begin(); i != entities.end(); i++) {
                Entity a = *i;
                const TransformComponent aTransform = a.GetComponent<TransformComponent>();
                const BoxColliderComponent aBoxCollider = a.GetComponent<BoxColliderComponent>();

                for (auto j = i; j != entities.end(); j++) {
                    Entity b = *j;

                    if (a == b) {
                        continue;
                    }

                    const TransformComponent bTransform = b.GetComponent<TransformComponent>();
                    const BoxColliderComponent bBoxCollider = b.GetComponent<BoxColliderComponent>();

                    bool boxCollisionHappened = CheckAABBCollision(
                        aTransform.position.x + aBoxCollider.offset.x,
                        aTransform.position.y + aBoxCollider.offset.y,
                        aBoxCollider.width,
                        aBoxCollider.height,
                        bTransform.position.x + bBoxCollider.offset.x,
                        bTransform.position.y + bBoxCollider.offset.y,
                        bBoxCollider.width,
                        bBoxCollider.height
                    );

                    if (boxCollisionHappened) {
                        std::cout << "Emiting an event since entities " << a.GetId() << " and " << b.GetId() << " collided." << std::endl;
                        eventBus->EmitEvent<CollisionEvent>(a, b);
                    }
                }
            }
        }

        bool CheckAABBCollision(double aX, double aY, double aW, double aH, double bX, double bY, double bW, double bH) {
            return (
                aX < bX + bW &&
                aX + aW > bX &&
                aY < bY + bH &&
                aY + aH > bY
            );
        }
};

#endif