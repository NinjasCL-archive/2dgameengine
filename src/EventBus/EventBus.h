#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <iostream>

#include <map>
#include <list>
#include <typeindex>
#include "EventCallback.h"
#include "Event.h"
#include <SDL2/SDL.h>

typedef std::list<std::unique_ptr<IEventCallback>> HandlerList;

class EventBus {
    private:
        std::map<std::type_index, std::unique_ptr<HandlerList>> listeners;

    public:
        EventBus() {
            SDL_Log("EventBus constructor invoked...");
        };

        ~EventBus() {
            SDL_Log("EventBus destructor invoked...");
        }

        void ShowListenerList() {
            std::cout << "--------------------------------------------------------------" << std::endl;
            for (auto l = listeners.begin(); l != listeners.end(); l++) {
                std::cout << "Listener for " << l->first.name() << " has " << l->second.get()->size() << " events to be handled." << std::endl;
             }
        }

        void Reset() {
            listeners.clear();
        }
        
        ///////////////////////////////////////////////////////////////////////
        // Emit an event.
        // In our implementation, as soon as something emits an event we go
        // ahead and execute all listeners callback functions.
        // Example: bus->EmitEvent<Collision>(player, enemy);
        ///////////////////////////////////////////////////////////////////////
        template <typename TEvent, typename ...TArgs>
        void EmitEvent(TArgs&& ...args) {
            auto handlers = listeners[typeid(TEvent)].get();
            if (handlers) {
                // In our implementation, as soon as something emits an event we go ahead and execute all listeners callback functions
                for (auto iterator = handlers->begin(); iterator != handlers->end(); iterator++) {
                    auto handler = iterator->get();
                    TEvent event(std::forward<TArgs>(args) ...);
                    handler->Execute(event);
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////
        // Subscribe to an event type.
        // In our implementation, a listener subscribes to an event and is only
        // active until the end of the current update frame.
        // Example: bus->ListenToEvent<Collision>(this, &Game::OnCollision);
        ///////////////////////////////////////////////////////////////////////
        template <typename TEvent, typename TOwner>
        void ListenToEvent(TOwner* ownerInstance, void (TOwner::*callbackFunction)(TEvent&)) {
            if (!listeners[typeid(TEvent)].get()) {
                listeners[typeid(TEvent)] = std::make_unique<HandlerList>();
            }
            auto listener = std::make_unique<EventCallback<TOwner, TEvent>>(ownerInstance, callbackFunction);
            listeners[typeid(TEvent)]->push_back(std::move(listener));
    }
};

#endif