#include "leviathan/lvpch.h"

#include "leviathan/events.h"

namespace lv {
    void EventBus::add_listener(IEventListener& listener) {
        listeners.emplace_back(listener);
    }

    void EventBus::push(Event&& event) {
        events.push(std::move(event));
    }

    void EventBus::drain() {
        Event event;
        while (!events.empty()) {
            event = std::move(events.front());
            events.pop();
            for (auto& listener : listeners) {
                listener.get().handle(event);
            }
        }
    }
}