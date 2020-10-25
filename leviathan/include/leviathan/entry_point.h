#pragma once

#include "application.h"

extern std::unique_ptr<lv::Application> lv::CreateApplication() noexcept;

int main(int argc, const char* const argv[]) noexcept {
    auto app = lv::CreateApplication();
    return app->run();
}
