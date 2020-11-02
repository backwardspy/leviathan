#pragma once

#include <exception>

namespace lv {
    namespace exc {
        struct LeviathanException : public std::exception {
            const char* what() const override {
                return "An unrecoverable error occurred inside Leviathan. Please report this.";
            }
        };

        struct RenderContextException : public LeviathanException {
            const char* what() const override {
                return "Failed to initialize render context.";
            }
        };

        struct EnumeratorNotImplementedError : public LeviathanException {
            const char* what() const override {
                return "An enumerator has not been implemented.";
            }
        };

        struct EntityLimitReached : public LeviathanException {
            const char* what() const override {
                return "The entity limit has been reached.";
            }
        };

        struct EntityNotFound : public LeviathanException {
            const char* what() const override {
                return "The specified entity does not exist.";
            }
        };

        struct ComponentNotFound : public LeviathanException {
            const char* what() const override {
                return "The specified component does not exist.";
            }
        };
    }
}
