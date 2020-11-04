#pragma once

#include "lvpch.h"
#include "keys.h"
#include "buttons.h"
#include "enum_bitmask.h"

namespace lv {
    enum class EventCategory : unsigned int {
        Window = 1 << 0,
        Keyboard = 1 << 1,
        Mouse = 1 << 2,
    };
    LV_ENABLE_BITMASK_OPERATORS(EventCategory);

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

        EventCategory category;

        bool is_in_category(EventCategory category) const {
            return (this->category & category) == category;
        }

        struct WindowEvent {
            int width, height;
        };

        struct KeyEvent {
            KeyCode code;
            unsigned int scancode;
            bool shift, control, alt, super;
        };

        struct ButtonEvent {
            ButtonCode code;
        };

        struct MouseEvent {
            glm::vec2 coord;
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
        Event ev { Event::Type::WindowClosed, EventCategory::Window };
        return ev;
    }
    constexpr Event window_resized_event(int width, int height) {
        Event ev { Event::Type::WindowResized, EventCategory::Window };
        ev.window = { width, height };
        return ev;
    }
    constexpr Event key_pressed_event(KeyCode code, unsigned int scancode, bool shift, bool control, bool alt, bool super) {
        Event ev { Event::Type::KeyPressed, EventCategory::Keyboard };
        ev.key = { code, scancode, shift, control, alt, super };
        return ev;
    }
    constexpr Event key_released_event(KeyCode code, unsigned int scancode, bool shift, bool control, bool alt, bool super) {
        Event ev { Event::Type::KeyReleased, EventCategory::Keyboard };
        ev.key = { code, scancode, shift, control, alt, super };
        return ev;
    }
    constexpr Event button_pressed_event(ButtonCode code) {
        Event ev { Event::Type::ButtonPressed, EventCategory::Mouse };
        ev.button = { code };
        return ev;
    }
    constexpr Event button_released_event(ButtonCode code) {
        Event ev { Event::Type::ButtonReleased, EventCategory::Mouse };
        ev.button = { code };
        return ev;
    }
    constexpr Event mouse_moved_event(glm::vec2 position) {
        Event ev { Event::Type::MouseMoved, EventCategory::Mouse };
        ev.mouse = { position };
        return ev;
    }
    constexpr Event mouse_scrolled_event(glm::vec2 offset) {
        Event ev { Event::Type::MouseScrolled, EventCategory::Mouse };
        ev.mouse = { offset };
        return ev;
    }
    constexpr Event text_entered_event(unsigned int codepoint) {
        Event ev { Event::Type::TextEntered, EventCategory::Keyboard };
        ev.text = { codepoint };
        return ev;
    }

    class IEventListener {
    public:
        virtual void handle(Event const&) = 0;

        virtual ~IEventListener() = default;
    };

    class EventBus {
    public:
        void add_listener(IEventListener&);
        void push(Event&&);
        void drain();

    private:
        std::queue<Event> events {};
        // TODO: is reference_wrapper the right thing to use here?
        std::vector<std::reference_wrapper<IEventListener>> listeners {};
    };
}
