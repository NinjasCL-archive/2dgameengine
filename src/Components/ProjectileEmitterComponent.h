#ifndef PROJECTILEEMITTERCOMPONENT_H
#define PROJECTILEEMITTERCOMPONENT_H

#include <glm/glm.hpp>

class ProjectileEmitterComponent {
    public:
        glm::vec2 projectileVelocity;
        double projectileDuration;
        bool shouldLoop;
        bool isFriendly;
        
        ProjectileEmitterComponent(glm::vec2 projectileVelocity = glm::vec2(0), double projectileDuration = 0.0, bool shouldLoop = false, bool isFriendly = false) {
            this->projectileVelocity = projectileVelocity;
            this->projectileDuration = projectileDuration;
            this->shouldLoop = shouldLoop;
            this->isFriendly = isFriendly;
        }
};

#endif