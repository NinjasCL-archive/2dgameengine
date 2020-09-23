#ifndef CAMERAMOVEMENTSYSTEM_H
#define CAMERAMOVEMENTSYSTEM_H

#include <SDL2/SDL.h>
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/TransformComponent.h"

class CameraMovementSystem: public System {
    public:
        CameraMovementSystem() {
            RequireComponent<CameraFollowComponent>();
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            
        }

        void Update(std::unique_ptr<Registry>& registry, SDL_Rect& camera) {
            for (auto entity: GetSystemEntities()) {
                const TransformComponent transform = entity.GetComponent<TransformComponent>();
                const SpriteComponent sprite = entity.GetComponent<SpriteComponent>();
                
                // Change the camera to follow the entity that has a CameraFollow component attached to it
                if (entity.HasComponent<CameraFollowComponent>()) {
                    if (transform.position.x + (camera.w / 2) < Game::mapWidth) {
                        // Prevent from moving when camera middle reaches the limit of the map right boundary
                        camera.x = static_cast<int>(transform.position.x - (Game::windowWidth / 2));
                    }

                    if (transform.position.y + (camera.h / 2) < Game::mapHeight) {
                        // Prevent from moving when camera movement reaches the limit of the map bottom boundary
                        camera.y = static_cast<int>(transform.position.y - (Game::windowHeight / 2));
                    }

                    // Keep camera rectangle view inside screen limits
                    camera.x = camera.x < 0 ? 0 : camera.x;
                    camera.y = camera.y < 0 ? 0 : camera.y;
                    camera.x = camera.x > camera.w ? camera.w : camera.x;
                    camera.y = camera.y > camera.h ? camera.h : camera.y;
                }
            }
        }
};

#endif