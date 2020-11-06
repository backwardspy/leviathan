#include "leviathan/lvpch.h"

namespace lv {
    Log::Log() :
        core { spdlog::stderr_color_mt("leviathan") },
        client { spdlog::stderr_color_mt("app") }
    {
        core->set_level(spdlog::level::trace);
        client->set_level(spdlog::level::trace);
    }

    Log& Log::log() {
        static Log log;
        return log;
    }
}