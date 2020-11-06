#pragma once

#include <exception>

namespace lv {
    namespace exc {
        struct LeviathanException : public std::exception {
            char const* what() const override {
                return "An unrecoverable error occurred inside Leviathan. Please report this.";
            }
        };

        struct RenderContextError : public LeviathanException {
            char const* what() const override {
                return "Failed to initialize render context.";
            }
        };

        struct EnumeratorNotImplemented : public LeviathanException {
            char const* what() const override {
                return "An enumerator has not been implemented.";
            }
        };

        struct EntityLimitReached : public LeviathanException {
            char const* what() const override {
                return "The entity limit has been reached.";
            }
        };

        struct EntityNotFound : public LeviathanException {
            char const* what() const override {
                return "The specified entity does not exist.";
            }
        };

        struct ComponentNotFound : public LeviathanException {
            char const* what() const override {
                return "The specified component does not exist.";
            }
        };

        struct FileReadError : public LeviathanException {
            char const* what() const override {
                return "An error occurred while reading a file.";
            }
        };
    }
}
