#include "leviathan/lvpch.h"

#include "leviathan/input.h"

namespace lv {
    scope<Input> Input::instance;

    void Input::init(EventBus& event_bus, Window const& window) {
        if (instance) return;
        instance = scope<Input>(new Input(event_bus, window)); // make_scope can't access private constructor.
    }

    Input::Input(EventBus& event_bus, Window const& window) :
        window(window) {
        std::fill(std::begin(cur_keys), std::end(cur_keys), false);
        std::fill(std::begin(prev_keys), std::end(prev_keys), false);
        std::fill(std::begin(cur_buttons), std::end(cur_buttons), false);
        std::fill(std::begin(prev_buttons), std::end(prev_buttons), false);

        // we have static lifetime, so we never remove this listener again.
        // by the time our destructor is called, this event bus is long gone.
        event_bus.add_listener(this);
    }

    bool Input::is_key_pressed(KeyCode code) {
        return instance->cur_keys[(size_t) code];
    }

    bool Input::is_key_just_pressed(KeyCode code) {
        return instance->cur_keys[(size_t) code] && !instance->prev_keys[(size_t) code];
    }

    bool Input::is_key_just_released(KeyCode code) {
        return !instance->cur_keys[(size_t) code] && instance->prev_keys[(size_t) code];
    }

    int Input::key_axis(KeyCode positive_code, KeyCode negative_code) {
        return
            (is_key_pressed(positive_code) ? 1 : 0) -
            (is_key_pressed(negative_code) ? 1 : 0);
    }

    bool Input::is_button_pressed(ButtonCode code) {
        return instance->cur_buttons[(size_t) code];
    }

    bool Input::is_button_just_pressed(ButtonCode code) {
        return instance->cur_buttons[(size_t) code] && !instance->prev_buttons[(size_t) code];
    }

    bool Input::is_button_just_released(ButtonCode code) {
        return !instance->cur_buttons[(size_t) code] && instance->prev_buttons[(size_t) code];
    }

    glm::vec2 Input::get_mouse_position() {
        return instance->window.get_mouse_position();
    }

    void Input::end_frame() {
        std::copy(
            std::begin(instance->cur_keys),
            std::end(instance->cur_keys),
            std::begin(instance->prev_keys)
        );
        std::copy(
            std::begin(instance->cur_buttons),
            std::end(instance->cur_buttons),
            std::begin(instance->prev_buttons)
        );
    }

    void Input::handle(Event const& event) {
        switch (event.type) {
            case Event::Type::KeyPressed: cur_keys[(size_t) event.key.code] = true; return;
            case Event::Type::KeyReleased: cur_keys[(size_t) event.key.code] = false; return;
            case Event::Type::ButtonPressed: cur_buttons[(size_t) event.button.code] = true; return;
            case Event::Type::ButtonReleased: cur_buttons[(size_t) event.button.code] = false; return;
        }
    }
}