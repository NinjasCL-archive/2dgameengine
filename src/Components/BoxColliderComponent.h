#ifndef BOXCOLLIDERCOMPONENT_H
#define BOXCOLLIDERCOMPONENT_H

#include <glm/glm.hpp>

class BoxColliderComponent {
    public:
        glm::vec2 offset;
        int width;
        int height;

        BoxColliderComponent(glm::vec2 offset = glm::vec2(0), int width = 0.0, int height = 0.0) {
            this->offset = offset;
            this->width = width;
            this->height = height;
        }
};

#endif
