#pragma once

#include "application.h"
#include "exc.h"

extern std::unique_ptr<lv::Application> lv::CreateApplication();

int main(int argc, const char* const argv[]) noexcept {
    try {
        auto app = lv::CreateApplication();
        return app->run();
    } catch (lv::exc::LeviathanException& ex) {
        lv::Log::core_critical("Leviathan is shutting down due to an unrecoverable error: {}", ex.what());
    } catch (std::exception& ex) {
        lv::Log::core_critical("Leviathan is shutting down due to an unexpected, unhandled exception: {}", ex.what());
    } catch (...) {
        lv::Log::core_critical("Leviathan is shutting down due to a non-standard exception.");
    }
}
