#ifndef KEYRELEASEDEVENT_H
#define KEYRELEASEDEVENT_H

#include <string>
#include <SDL2/SDL.h>
#include "../ECS/ECS.h"
#include "../EventBus/Event.h"

class KeyReleasedEvent: public Event {
    public:
        SDL_Keycode symbol;
        
        KeyReleasedEvent(SDL_Keycode symbol) {
            this->symbol = symbol;
        }
};

#endif