#pragma once

#include "lvpch.h"
#include "keys.h"
#include "buttons.h"

namespace lv {
    struct Event {
        enum class Type {
            WindowClosed,
            WindowResized,
            KeyPressed,
            KeyReleased,
            ButtonPressed,
            ButtonReleased,
            MouseMoved,
            MouseScrolled,
            TextEntered,
        };
        Type type;

        struct WindowEvent {
            int width, height;
        };

        struct KeyEvent {
            KeyCode code;
            bool shift, control, alt, super;
        };

        struct ButtonEvent {
            ButtonCode code;
        };

        struct MouseEvent {
            double x, y;
        };

        struct TextEvent {
            unsigned int codepoint;
        };

        union {
            WindowEvent window;
            KeyEvent key;
            ButtonEvent button;
            MouseEvent mouse;
            TextEvent text;
        };
    };

    constexpr Event window_closed_event() {
        Event ev { Event::Type::WindowClosed };
        return ev;
    }
    constexpr Event window_resized_event(int width, int height) {
        Event ev { Event::Type::WindowResized };
        ev.window = { width, height };
        return ev;
    }
    constexpr Event key_pressed_event(KeyCode code, bool shift, bool control, bool alt, bool super) {
        Event ev { Event::Type::KeyPressed };
        ev.key = { code, shift, control, alt, super };
        return ev;
    }
    constexpr Event key_released_event(KeyCode code, bool shift, bool control, bool alt, bool super) {
        Event ev { Event::Type::KeyReleased };
        ev.key = { code, shift, control, alt, super };
        return ev;
    }
    constexpr Event button_pressed_event(ButtonCode code) {
        Event ev { Event::Type::ButtonPressed };
        ev.button = { code };
        return ev;
    }
    constexpr Event button_released_event(ButtonCode code) {
        Event ev { Event::Type::ButtonReleased };
        ev.button = { code };
        return ev;
    }
    constexpr Event mouse_moved_event(double x, double y) {
        Event ev { Event::Type::MouseMoved };
        ev.mouse = { x, y };
        return ev;
    }
    constexpr Event mouse_scrolled_event(double x_offset, double y_offset) {
        Event ev { Event::Type::MouseScrolled };
        ev.mouse = { x_offset, y_offset };
        return ev;
    }
    constexpr Event text_entered_event(unsigned int codepoint) {
        Event ev { Event::Type::TextEntered };
        ev.text = { codepoint };
        return ev;
    }

    class IEventListener {
    public:
        virtual void handle(const Event&) noexcept = 0;

        virtual ~IEventListener() noexcept;
    };

    class EventBus {
    public:
        EventBus() noexcept;

        void add_listener(IEventListener&) noexcept;
        void emplace(Event&&) noexcept;
        void drain() noexcept;

    private:
        std::queue<Event> events;
        std::vector<std::reference_wrapper<IEventListener>> listeners;
    };
}
