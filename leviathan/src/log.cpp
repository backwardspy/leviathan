#include "leviathan/lvpch.h"

#include "leviathan/log.h"

namespace lv {
    Log::Log() noexcept :
        core { spdlog::stderr_color_mt("leviathan") },
        client { spdlog::stderr_color_mt("app") }
    {
        core->set_level(spdlog::level::trace);
        client->set_level(spdlog::level::trace);
    }

    Log& Log::log() noexcept {
        static Log log;
        return log;
    }
}