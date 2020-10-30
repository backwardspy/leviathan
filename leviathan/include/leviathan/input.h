#pragma once

#include "lvpch.h"

#include "events.h"
#include "keys.h"
#include "buttons.h"

namespace lv {
    class Input : public IEventListener {
    public:
        explicit Input(EventBus& event_bus) noexcept;

        bool is_key_pressed(KeyCode code) const noexcept;
        bool is_key_just_pressed(KeyCode code) const noexcept;
        bool is_key_just_released(KeyCode code) const noexcept;

        bool is_button_pressed(ButtonCode code) const noexcept;
        bool is_button_just_pressed(ButtonCode code) const noexcept;
        bool is_button_just_released(ButtonCode code) const noexcept;

        constexpr glm::vec2 get_mouse_pos() const { return mouse_pos; }

        void end_frame() noexcept;

        void handle(const Event&) noexcept override;

    private:
        static const size_t KeyCodeCount = (size_t) KeyCode::EntryCount;
        static const size_t ButtonCodeCount = (size_t) ButtonCode::EntryCount;
        std::array<bool, KeyCodeCount> cur_keys, prev_keys;
        std::array<bool, ButtonCodeCount> cur_buttons, prev_buttons;
        glm::vec2 mouse_pos;
    };
}
