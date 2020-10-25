#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace lv {
    class Log {
    public:
        enum class Level {
            Trace, Debug, Info, Warn, Error, Critical
        };

        Log(const Log&&) = delete;
        void operator =(const Log&&) = delete;

        static Log& log() noexcept;

        static void init() noexcept;

        // configuration
        static void set_level(Level level) noexcept;
        static void set_core_level(Level level) noexcept;

        // client application logger
        template<class... Args>
        static void trace(Args... args) noexcept { log().client->trace(args...); }

        template<class... Args>
        static void debug(Args... args) noexcept { log().client->debug(args...); }

        template<class... Args>
        static void info(Args... args) noexcept { log().client->info(args...); }

        template<class... Args>
        static void warn(Args... args) noexcept { log().client->warn(args...); }

        template<class... Args>
        static void error(Args... args) noexcept { log().client->error(args...); }

        template<class... Args>
        static void critical(Args... args) noexcept { log().client->critical(args...); }

        // engine logger
        template<class... Args>
        static void core_trace(Args... args) noexcept { log().core->trace(args...); }

        template<class... Args>
        static void core_debug(Args... args) noexcept { log().core->debug(args...); }

        template<class... Args>
        static void core_info(Args... args) noexcept { log().core->info(args...); }

        template<class... Args>
        static void core_warn(Args... args) noexcept { log().core->warn(args...); }

        template<class... Args>
        static void core_error(Args... args) noexcept { log().core->error(args...); }

        template<class... Args>
        static void core_critical(Args... args) noexcept { log().core->critical(args...); }

    private:
        Log() noexcept;

        std::shared_ptr<spdlog::logger> core, client;
    };
}
