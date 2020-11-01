#pragma once

#include "lvpch.h"

#include "events.h"
#include "keys.h"
#include "buttons.h"
#include "window.h"

namespace lv {
    class Input : public IEventListener {
    public:
        static void init(EventBus& event_bus, const Window&) noexcept;

        static bool is_key_pressed(KeyCode code) noexcept;
        static bool is_key_just_pressed(KeyCode code) noexcept;
        static bool is_key_just_released(KeyCode code) noexcept;

        static bool is_button_pressed(ButtonCode code) noexcept;
        static bool is_button_just_pressed(ButtonCode code) noexcept;
        static bool is_button_just_released(ButtonCode code) noexcept;

        static glm::vec2 get_mouse_position() noexcept;

        static void end_frame() noexcept;

        void handle(const Event&) noexcept override;
    private:
        explicit Input(EventBus& event_bus, const Window&) noexcept;

    private:
        static std::unique_ptr<Input> instance;

        const Window& window;

        static const size_t KeyCodeCount = (size_t) KeyCode::EntryCount;
        static const size_t ButtonCodeCount = (size_t) ButtonCode::EntryCount;
        std::array<bool, KeyCodeCount> cur_keys, prev_keys;
        std::array<bool, ButtonCodeCount> cur_buttons, prev_buttons;
    };
}
