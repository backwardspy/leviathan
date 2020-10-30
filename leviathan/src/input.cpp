#include "leviathan/lvpch.h"

#include "leviathan/input.h"

namespace lv {
    Input::Input(EventBus& event_bus) noexcept {
        std::fill(std::begin(cur_keys), std::end(cur_keys), false);
        std::fill(std::begin(prev_keys), std::end(prev_keys), false);
        std::fill(std::begin(cur_buttons), std::end(cur_buttons), false);
        std::fill(std::begin(prev_buttons), std::end(prev_buttons), false);
        event_bus.add_listener(*this);
    }

    bool Input::is_key_pressed(KeyCode code) const noexcept {
        return cur_keys[(size_t) code];
    }

    bool Input::is_key_just_pressed(KeyCode code) const noexcept {
        return cur_keys[(size_t) code] && !prev_keys[(size_t) code];
    }

    bool Input::is_key_just_released(KeyCode code) const noexcept {
        return !cur_keys[(size_t) code] && prev_keys[(size_t) code];
    }

    bool Input::is_button_pressed(ButtonCode code) const noexcept {
        return cur_buttons[(size_t) code];
    }

    bool Input::is_button_just_pressed(ButtonCode code) const noexcept {
        return cur_buttons[(size_t) code] && !prev_buttons[(size_t) code];
    }

    bool Input::is_button_just_released(ButtonCode code) const noexcept {
        return !cur_buttons[(size_t) code] && prev_buttons[(size_t) code];
    }

    void Input::end_frame() noexcept {
        std::copy(std::begin(cur_keys), std::end(cur_keys), std::begin(prev_keys));
        std::copy(std::begin(cur_buttons), std::end(cur_buttons), std::begin(prev_buttons));
    }

    void Input::handle(const Event& event) noexcept {
        switch (event.type) {
            case Event::Type::KeyPressed: cur_keys[(size_t) event.key.code] = true; return;
            case Event::Type::KeyReleased: cur_keys[(size_t) event.key.code] = false; return;
            case Event::Type::ButtonPressed: cur_buttons[(size_t) event.button.code] = true; return;
            case Event::Type::ButtonReleased: cur_buttons[(size_t) event.button.code] = false; return;
            case Event::Type::MouseMoved: mouse_pos = event.mouse.coord; return;
        }
    }
}