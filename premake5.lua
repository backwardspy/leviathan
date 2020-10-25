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
    }
}

include "leviathan/vendor/spdlog"
include "leviathan/vendor/glfw"
include "leviathan/vendor/glad"
include "leviathan/vendor/imgui"

project "leviathan"
    location "leviathan"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"

    disablewarnings {
        "4251",
        "4275",
    }

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
    }

    defines {
        "SPDLOG_COMPILED_LIB",
        "SPDLOG_SHARED_LIB",
    }

    pchheader "%{prj.name}/lvpch.h"
    pchsource "%{prj.name}/src/lvpch.cpp"

    links {
        "spdlog",
        "glfw",
        "glad",
        "imgui",
        "opengl32.lib",
    }

    filter "system:windows"
        defines "LV_BUILD_DLL"

        postbuildcommands ("{COPY} %{cfg.buildtarget.relpath} ../bin/"..target_triple.."/sandbox")
    
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

    disablewarnings {
        "4251",
        "4275",
    }

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
    }

    defines {
        "SPDLOG_COMPILED_LIB",
        "SPDLOG_SHARED_LIB",
    }

    links "leviathan"

    filter "configurations:debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:release"
        defines { "NDEBUG" }
        optimize "On"
