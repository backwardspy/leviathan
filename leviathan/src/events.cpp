#include "leviathan/lvpch.h"

#include "leviathan/events.h"

namespace lv {
    IEventListener::~IEventListener() noexcept {}

    EventBus::EventBus() noexcept : events {}, listeners {} {}

    void EventBus::add_listener(IEventListener& listener) noexcept {
        listeners.emplace_back(listener);
    }

    void EventBus::emplace(Event&& event) noexcept {
        events.emplace(std::move(event));
    }

    void EventBus::drain() noexcept {
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