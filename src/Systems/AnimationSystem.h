#ifndef ANIMATIONSYSTEM_H
#define ANIMATIONSYSTEM_H

#include <SDL2/SDL.h>
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Components/AnimationComponent.h"
#include "../Components/SpriteComponent.h"

class AnimationSystem: public System {
    public:
        AnimationSystem() {
            RequireComponent<AnimationComponent>();
            RequireComponent<SpriteComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            
        }

        void Update(std::unique_ptr<Registry>& registry) {
            for (auto entity: GetSystemEntities()) {
                AnimationComponent& animation = entity.GetComponent<AnimationComponent>();
                SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();
                
                animation.currentFrame = ((SDL_GetTicks() - animation.startTime) * animation.frameSpeedRate / 1000) % animation.numFrames;
                sprite.srcRect.x = animation.currentFrame * sprite.width;
            }
        }
};

#endif