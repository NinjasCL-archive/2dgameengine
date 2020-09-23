#ifndef KEYPRESSEDEVENT_H
#define KEYPRESSEDEVENT_H

#include <string>
#include <SDL2/SDL.h>
#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

class KeyPressedEvent: public Event {
    public:
        SDL_Keycode symbol;
        
        KeyPressedEvent(SDL_Keycode symbol) {
            this->symbol = symbol;
        }
};

#endif