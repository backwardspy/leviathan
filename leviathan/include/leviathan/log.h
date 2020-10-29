#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "exc.h"

namespace lv {
    class Log {
    public:
        enum class Level {
            Trace, Debug, Info, Warn, Error, Critical
        };

        Log(const Log&&) = delete;
        void operator =(const Log&&) = delete;

        static Log& log() noexcept;

        // configuration
        static void set_level(Level level) { log().client->set_level(to_spdlog_level(level)); }
        static void set_core_level(Level level) { log().core->set_level(to_spdlog_level(level)); }

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

        template<class... Args>
        static void log(Level level, Args... args) { log().client->log(to_spdlog_level(level), args...); }

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

        template<class... Args>
        static void core_log(Level level, Args... args) { log().core->log(to_spdlog_level(level), args...); }

    private:
        Log() noexcept;

        static constexpr spdlog::level::level_enum to_spdlog_level(Level level) {
            switch (level) {
                case Log::Level::Trace: return spdlog::level::trace;
                case Log::Level::Debug: return spdlog::level::debug;
                case Log::Level::Info: return spdlog::level::info;
                case Log::Level::Warn: return spdlog::level::warn;
                case Log::Level::Error: return spdlog::level::err;
                case Log::Level::Critical: return spdlog::level::critical;
                default: throw exc::EnumeratorNotImplementedError {};
            }
        }

        std::shared_ptr<spdlog::logger> core, client;
    };
}
