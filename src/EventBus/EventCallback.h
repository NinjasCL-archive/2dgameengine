#ifndef MEMBERCALLBACK_H
#define MEMBERCALLBACK_H

#include "Event.h"

class IEventCallback {
    private:
        virtual void Call(Event& e) = 0;

    public:
        virtual ~IEventCallback() = default;

        void Execute(Event& e) {
            Call(e);
        }
};

template <typename TOwner, typename TEvent>
class EventCallback: public IEventCallback {
    private:
        typedef void (TOwner::*CallbackFunction)(TEvent&);
        
        TOwner* ownerInstance;
        CallbackFunction callbackFunction;

        virtual void Call(Event& event) override {
            std::invoke(callbackFunction, ownerInstance, static_cast<TEvent&>(event));
        }

    public:
        EventCallback(TOwner* ownerInstance, CallbackFunction callbackFunction) {
            this->ownerInstance = ownerInstance;
            this->callbackFunction = callbackFunction;
        }

        virtual ~EventCallback() override = default;
};

#endif