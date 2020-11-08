#pragma once

#include "core.h"
#include "application.h"

extern lv::scope<lv::Application> lv::CreateApplication();

int main(int argc, char const* const argv[]) {
#if !defined(DEBUG)
    try {
#endif
        auto app = lv::CreateApplication();
        return app->run();
#if !defined(DEBUG)
    } catch (lv::exc::LeviathanException& ex) {
        lv::Log::core_critical("Leviathan is shutting down due to an unrecoverable error: {}", ex.what());
    } catch (std::exception& ex) {
        lv::Log::core_critical("Leviathan is shutting down due to an unexpected, unhandled exception: {}", ex.what());
    } catch (...) {
        lv::Log::core_critical("Leviathan is shutting down due to a non-standard exception.");
    }
#endif
}
