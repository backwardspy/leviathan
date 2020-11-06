workspace "leviathan"
    architecture "x86_64"
    configurations { "debug", "release" }
    startproject "sandbox"
    flags "MultiProcessorCompile"

target_triple = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

vendor = {
    includedirs = {
        spdlog = "%{wks.location}/leviathan/vendor/spdlog/include",
        glfw = "%{wks.location}/leviathan/vendor/glfw/include",
        glad = "%{wks.location}/leviathan/vendor/glad/include",
        imgui = "%{wks.location}/leviathan/vendor/imgui",
        glm = "%{wks.location}/leviathan/vendor/glm",
        stb = "%{wks.location}/leviathan/vendor/stb/include",
    }
}

group "dependencies"
    include "leviathan/vendor/spdlog"
    include "leviathan/vendor/glfw"
    include "leviathan/vendor/glad"
    include "leviathan/vendor/imgui"
    include "leviathan/vendor/glm"
    include "leviathan/vendor/stb"
group ""

project "leviathan"
    location "leviathan"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/"..target_triple.."/%{prj.name}")
    objdir ("bin/obj/"..target_triple.."/%{prj.name}")

    files {
        "%{prj.name}/include/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs {
        "%{prj.name}/include",
        vendor.includedirs.spdlog,
        vendor.includedirs.glfw,
        vendor.includedirs.glad,
        vendor.includedirs.imgui,
        vendor.includedirs.glm,
        vendor.includedirs.stb,
    }

    defines {
        "LV_GRAPHICS_OPENGL",
        "SPDLOG_COMPILED_LIB",
        "_CRT_SECURE_NO_WARNINGS",  -- stops noisy warnings about imgui
    }

    pchheader "%{prj.name}/lvpch.h"
    pchsource "%{prj.name}/src/lvpch.cpp"

    links {
        "spdlog",
        "glfw",
        "glad",
        "imgui",
        "glm",
        "opengl32.lib",
    }

    filter "configurations:debug"
        defines "DEBUG"
        symbols "On"

    filter "configurations:release"
        defines "NDEBUG"
        optimize "On"

project "sandbox"
    location "sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/"..target_triple.."/%{prj.name}")
    objdir ("bin/obj/"..target_triple.."/%{prj.name}")

    files {
        "%{prj.name}/include/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs {
        "leviathan/include",
        vendor.includedirs.spdlog,
        vendor.includedirs.glfw,
        vendor.includedirs.glad,
        vendor.includedirs.imgui,
        vendor.includedirs.glm,
        vendor.includedirs.stb,
    }

    defines {
        "LV_GRAPHICS_OPENGL",
        "SPDLOG_COMPILED_LIB",
    }

    links {
        "leviathan",
    }

    filter "configurations:debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:release"
        defines { "NDEBUG" }
        optimize "On"
