#include "leviathan/lvpch.h"
#include "leviathan/core/time.h"

namespace lv {
    namespace time {
        time_point epoch, last;
        duration dt;

        void start() {
            epoch = last = clock::now();
        }

        void reset() {
            auto now = clock::now();
            dt = now - last;
            last = now;
        }

        duration render_delta_time() {
            return dt;
        }

        duration elapsed() {
            return clock::now() - epoch;
        }
    }
}