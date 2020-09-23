#ifndef RENDERCOLLIDERSYSTEM_H
#define RENDERCOLLIDERSYSTEM_H

#include <SDL2/SDL.h>
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"

class RenderColliderSystem: public System {
    public:
        RenderColliderSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<BoxColliderComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            
        }

        void Update(std::unique_ptr<Registry>& registry, SDL_Renderer* renderer) {
            for (auto entity: GetSystemEntities()) {
                const TransformComponent transform = entity.GetComponent<TransformComponent>();
                const BoxColliderComponent collider = entity.GetComponent<BoxColliderComponent>();

                SDL_Rect boxColliderRectangle = {
                    static_cast<int>(transform.position.x + collider.offset.x),
                    static_cast<int>(transform.position.y + collider.offset.y),
                    static_cast<int>(collider.width),
                    static_cast<int>(collider.height)
                };
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &boxColliderRectangle);
            }
        }
};

#endif