#pragma once

#include "leviathan/lvpch.h"

#include "leviathan/layer.h"

namespace lv {
    class DebugGLLayer : public Layer {
    public:
        virtual void init() noexcept override;
        virtual void render() noexcept override;

    private:
        GLuint vao, vbo, ibo;
    };
}
