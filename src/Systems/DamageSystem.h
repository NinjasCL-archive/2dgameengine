#ifndef DAMAGESYSTEM_H
#define DAMAGESYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Components/HealthComponent.h"
#include <glm/glm.hpp>

class DamageSystem: public System {
    public:
        DamageSystem() {
            RequireComponent<HealthComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->ListenToEvent<CollisionEvent>(this, &DamageSystem::OnCollision);
        }

        void Update(std::unique_ptr<Registry>& registry) {
        
        }

        void OnCollision(CollisionEvent& event) {
            Entity a = event.a;
            Entity b = event.b;
            std::cout << "Damage system detected collision between entity " << a.GetId() << " and " << b.GetId() << std::endl;
            
            a.Kill();
            b.Kill();
        }
};

#endif