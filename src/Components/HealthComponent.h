#ifndef HEALTH_COMPONENT
#define HEALTH_COMPONENT

class HealthComponent {
    public:
        int health;
        
        HealthComponent(int health = 0) {
            this->health = health;
        }
};

#endif
