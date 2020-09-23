#ifndef ANIMATIONCOMPONENT_H
#define ANIMATIONCOMPONENT_H

#include <SDL2/SDL.h>

class AnimationComponent {
    public:
        int numFrames;
        int currentFrame;
        int frameSpeedRate;
        bool isLoop;
        int startTime;

        AnimationComponent(int numFrames = 1, int frameSpeedRate = 1) {
            this->startTime = SDL_GetTicks();
            this->currentFrame = 1;
            this->numFrames = numFrames;
            this->frameSpeedRate = frameSpeedRate;
        }
};

#endif
