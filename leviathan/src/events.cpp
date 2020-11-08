#include "leviathan/lvpch.h"

#include "leviathan/events.h"

namespace lv {
    void EventBus::add_listener(IEventListener* const listener) {
        listeners.push_back(listener);
    }

    void EventBus::remove_listener(IEventListener* const listener) {
        listeners.remove(listener);
    }

    void EventBus::push(Event&& event) {
        events.push(std::move(event));
    }

    void EventBus::drain() {
        Event event;
        while (!events.empty()) {
            event = std::move(events.front());
            events.pop();
            for (auto const listener : listeners) listener->handle(event);
        }
    }
}