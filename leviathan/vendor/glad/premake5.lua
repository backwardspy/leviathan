target_triple = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "glad"
    kind "StaticLib"
    language "C"

    targetdir ("bin/"..target_triple.."/%{prj.name}")
    objdir ("bin/obj/"..target_triple.."/%{prj.name}")

    files {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c",
    }

    includedirs "include"

    filter "configurations:debug"
        defines "DEBUG"
        symbols "On"

    filter "configurations:release"
        defines "NDEBUG"
        optimize "On"
