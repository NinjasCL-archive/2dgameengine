#ifndef KEYBOARDCONTROLSYSTEM_H
#define KEYBOARDCONTROLSYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Events/KeyReleasedEvent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"

class KeyboardControlSystem: public System {
    public:
        KeyboardControlSystem() {
            RequireComponent<KeyboardControlledComponent>();
            RequireComponent<SpriteComponent>();
            RequireComponent<RigidBodyComponent>();
        }

        void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
            eventBus->ListenToEvent<KeyPressedEvent>(this, &KeyboardControlSystem::OnKeyPressed);
            eventBus->ListenToEvent<KeyReleasedEvent>(this, &KeyboardControlSystem::OnKeyReleased);
        }

        void OnKeyPressed(KeyPressedEvent& event) {
            for (auto entity: GetSystemEntities()) {
                const KeyboardControlledComponent keyboardcontrol = entity.GetComponent<KeyboardControlledComponent>();
                SpriteComponent& sprite = entity.GetComponent<SpriteComponent>();
                RigidBodyComponent& rigidbody = entity.GetComponent<RigidBodyComponent>();
                
                switch (event.symbol) {
                    case SDLK_UP:
                        rigidbody.velocity = keyboardcontrol.upVelocity;
                        sprite.srcRect.y = sprite.height * 0;
                        break;
                    case SDLK_RIGHT:
                        rigidbody.velocity = keyboardcontrol.rightVelocity;
                        sprite.srcRect.y = sprite.height * 1;
                        break;
                    case SDLK_DOWN:
                        rigidbody.velocity = keyboardcontrol.downVelocity;
                        sprite.srcRect.y = sprite.height * 2;
                        break;
                    case SDLK_LEFT:
                        rigidbody.velocity = keyboardcontrol.leftVelocity;
                        sprite.srcRect.y = sprite.height * 3;
                        break;
                }
            }
        }

        void OnKeyReleased(KeyReleasedEvent& event) {
            // for (auto entity: GetSystemEntities()) {
            //     RigidBodyComponent& rigidbody = entity.GetComponent<RigidBodyComponent>();

            //     switch (event.symbol) {
            //         case SDLK_UP:
            //             rigidbody.velocity = glm::vec2(0, 0);
            //             break;
            //         case SDLK_RIGHT:
            //             rigidbody.velocity = glm::vec2(0, 0);
            //             break;
            //         case SDLK_DOWN:
            //             rigidbody.velocity = glm::vec2(0, 0);
            //             break;
            //         case SDLK_LEFT:
            //             rigidbody.velocity = glm::vec2(0, 0);
            //             break;
            //     }
            // }
        }

        void Update(std::unique_ptr<Registry>& registry, double deltaTime) {
            
        }
};

#endif