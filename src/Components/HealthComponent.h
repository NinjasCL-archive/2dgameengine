#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

class HealthComponent {
    public:
        int health;
        
        HealthComponent(int health = 0) {
            this->health = health;
        }
};

#endif