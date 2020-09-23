#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <SDL2/SDL.h>
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../AssetStore/AssetStore.h"

struct RenderableEntity {
    TransformComponent transformComponent;
    SpriteComponent spriteComponent;
};

class RenderSystem: public System {
    public:
        RenderSystem() {
            RequireComponent<SpriteComponent>();
            RequireComponent<TransformComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            
        }

        void Update(std::unique_ptr<Registry>& registry, SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore) {
            // Sort entities by zIndex
            std::vector<RenderableEntity> renderableEntities;
            for (auto entity: GetSystemEntities()) {
                RenderableEntity renderableEntity;
                renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
                renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();
                renderableEntities.emplace_back(renderableEntity);
            }
            sort(renderableEntities.begin(), renderableEntities.end(), [](const RenderableEntity& a, const RenderableEntity& b) { 
                return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
            });

            for (auto renderableEntity: renderableEntities) {
                const SpriteComponent sprite = renderableEntity.spriteComponent;
                const TransformComponent transform = renderableEntity.transformComponent;

                // Set the source rectangle of our original sprite texture
                SDL_Rect srcRect = sprite.srcRect;

                // Set the destination rectangle in the x,y position in the renderer
                SDL_Rect dstRect = {
                    static_cast<int>(transform.position.x),
                    static_cast<int>(transform.position.y),
                    static_cast<int>(sprite.width * transform.scale.x),
                    static_cast<int>(sprite.height * transform.scale.y)
                };

                // Draw the sprite texture in the renderer (destination rectangle)
                SDL_RenderCopyEx(
                    renderer,
                    assetStore->GetTexture(sprite.assetId),
                    &srcRect,
                    &dstRect,
                    transform.rotation,
                    NULL,
                    SDL_FLIP_NONE
                );
            }
        }
};

#endif