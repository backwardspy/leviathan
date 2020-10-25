#pragma once

#include "lvpch.h"

#include "events.h"
#include "keys.h"
#include "buttons.h"

namespace lv {
    class Input : public IEventListener {
    public:
        Input(EventBus& event_bus) noexcept;

        bool is_key_pressed(KeyCode code) const noexcept;
        bool is_key_just_pressed(KeyCode code) const noexcept;
        bool is_key_just_released(KeyCode code) const noexcept;

        bool is_button_pressed(ButtonCode code) const noexcept;
        bool is_button_just_pressed(ButtonCode code) const noexcept;
        bool is_button_just_released(ButtonCode code) const noexcept;

        constexpr double get_mouse_x() const { return mouse_x; }
        constexpr double get_mouse_y() const { return mouse_y; }

        void end_frame() noexcept;

        virtual void handle(const Event&) noexcept override;

    private:
        static const size_t KeyCodeCount = (size_t) KeyCode::EntryCount;
        static const size_t ButtonCodeCount = (size_t) ButtonCode::EntryCount;
        std::array<bool, KeyCodeCount> cur_keys, prev_keys;
        std::array<bool, ButtonCodeCount> cur_buttons, prev_buttons;
        double mouse_x, mouse_y;
    };
}
