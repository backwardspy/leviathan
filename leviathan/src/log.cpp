#include "leviathan/lvpch.h"

#include "leviathan/log.h"

namespace lv {
    Log::Log() noexcept :
        core { spdlog::stderr_color_mt("leviathan") },
        client { spdlog::stderr_color_mt("app") }
    {
    }

    Log& Log::log() noexcept {
        static Log log;
        return log;
    }

    void Log::init() noexcept {
        set_level(Log::Level::Trace);
        set_core_level(Log::Level::Trace);
    }

    void Log::set_level(Log::Level level) noexcept {
        switch (level) {
            case Log::Level::Trace: log().client->set_level(spdlog::level::trace); break;
            case Log::Level::Debug: log().client->set_level(spdlog::level::debug); break;
            case Log::Level::Info: log().client->set_level(spdlog::level::info); break;
            case Log::Level::Warn: log().client->set_level(spdlog::level::warn); break;
            case Log::Level::Error: log().client->set_level(spdlog::level::err); break;
            case Log::Level::Critical: log().client->set_level(spdlog::level::critical); break;
        }
    }

    void Log::set_core_level(Log::Level level) noexcept {
        switch (level) {
            case Log::Level::Trace: log().core->set_level(spdlog::level::trace); break;
            case Log::Level::Debug: log().core->set_level(spdlog::level::debug); break;
            case Log::Level::Info: log().core->set_level(spdlog::level::info); break;
            case Log::Level::Warn: log().core->set_level(spdlog::level::warn); break;
            case Log::Level::Error: log().core->set_level(spdlog::level::err); break;
            case Log::Level::Critical: log().core->set_level(spdlog::level::critical); break;
        }
    }
}