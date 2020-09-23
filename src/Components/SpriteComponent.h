#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>

class SpriteComponent {
    public:
        std::string assetId;
        int width;
        int height;
        int zIndex;
        SDL_Rect srcRect;
        bool isFixed;
        
        SpriteComponent(std::string assetId = "", int width = 0, int height = 0, int zIndex = 0, bool isFixed = false, int srcRectX = 0, int srcRectY = 0) {
            this->assetId = assetId;
            this->width = width;
            this->height = height;
            this->zIndex = zIndex;
            this->isFixed = isFixed;
            this->srcRect = {srcRectX, srcRectY, width, height};
        }
};

#endif
