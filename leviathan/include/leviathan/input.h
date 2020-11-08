#pragma once

#include "lvpch.h"
#include "events.h"
#include "keys.h"
#include "buttons.h"
#include "window.h"

namespace lv {
    class Input : public IEventListener {
    public:
        static void init(EventBus& event_bus, Window const&);

        static bool is_key_pressed(KeyCode);
        static bool is_key_just_pressed(KeyCode);
        static bool is_key_just_released(KeyCode);
        static int key_axis(KeyCode positive_code, KeyCode negative_code);

        static bool is_button_pressed(ButtonCode);
        static bool is_button_just_pressed(ButtonCode);
        static bool is_button_just_released(ButtonCode);

        static glm::vec2 get_mouse_position();

        static void end_frame();

        void handle(Event const&) override;
    private:
        explicit Input(EventBus& event_bus, Window const&);

    private:
        static scope<Input> instance;

        Window const& window;

        static size_t const KeyCodeCount = (size_t) KeyCode::EntryCount;
        static size_t const ButtonCodeCount = (size_t) ButtonCode::EntryCount;
        std::array<bool, KeyCodeCount> cur_keys, prev_keys;
        std::array<bool, ButtonCodeCount> cur_buttons, prev_buttons;
    };
}
