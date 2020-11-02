#pragma once

#include "leviathan/lvpch.h"

namespace lv {
    namespace time {
        using clock = std::chrono::high_resolution_clock;

        using duration = clock::duration;
        using time_point = clock::time_point;

        using seconds = std::chrono::duration<float>;
        using milliseconds = std::chrono::duration<float, std::milli>;

        constexpr auto to_seconds(duration duration) { return std::chrono::duration_cast<seconds>(duration); }
        constexpr auto to_milliseconds(duration duration) { return std::chrono::duration_cast<milliseconds>(duration); }

        constexpr auto target_fps(float fps) { return std::chrono::duration_cast<duration>(seconds { 1.0f / fps }); }
        constexpr auto delta_time = target_fps(60);

        // prevent "spiral of death" by limiting to this many sim steps per frame.
        constexpr auto max_fixed_steps = 10;

        void start();
        void reset();

        duration render_delta_time();
        duration elapsed();
    }
}
