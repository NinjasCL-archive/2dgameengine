#ifndef KEYBOARDCONTROLLED_COMPONENT
#define KEYBOARDCONTROLLED_COMPONENT

#include <set>
#include <string>
#include <glm/glm.hpp>

class KeyboardControlledComponent {
    public:
        glm::vec2 upVelocity;
        glm::vec2 rightVelocity;
        glm::vec2 downVelocity;
        glm::vec2 leftVelocity;
        
        KeyboardControlledComponent(glm::vec2 upVelocity = glm::vec2(0), glm::vec2 rightVelocity = glm::vec2(0), glm::vec2 downVelocity = glm::vec2(0), glm::vec2 leftVelocity = glm::vec2(0)) {
            this->upVelocity = upVelocity;
            this->rightVelocity = rightVelocity;
            this->downVelocity = downVelocity;
            this->leftVelocity = leftVelocity;
        }
};

#endif