#include <leviathan/leviathan.h>

class Sandbox : public lv::Application {};

std::unique_ptr<lv::Application> lv::CreateApplication() noexcept {
    return std::make_unique<Sandbox>();
}