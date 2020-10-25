#pragma once

namespace lv {
    // these values should mirror GLFW's button values
    enum class ButtonCode {
        MouseButton1 = 0,
        MouseButton2 = 1,
        MouseButton3 = 2,
        MouseButton4 = 3,
        MouseButton5 = 4,
        MouseButton6 = 5,
        MouseButton7 = 6,
        MouseButton8 = 7,

        EntryCount,

        // aliases
        Left = MouseButton1,
        Right = MouseButton2,
        Middle = MouseButton3
    };
}