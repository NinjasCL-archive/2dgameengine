#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <glm/glm.hpp>

class RigidBodyComponent {
    public:
        glm::vec2 velocity;

        RigidBodyComponent(glm::vec2 velocity = glm::vec2(0)) {
            this->velocity = velocity;
        }
};

#endif