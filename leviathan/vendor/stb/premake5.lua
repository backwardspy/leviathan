target_triple = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "stb"
    kind "StaticLib"
    language "C"

    targetdir ("bin/"..target_triple.."/%{prj.name}")
    objdir ("bin/obj/"..target_triple.."/%{prj.name}")

    files {
        "include/stb/stb_image.h",
    }

    includedirs "include"

    filter "configurations:debug"
        defines "DEBUG"
        symbols "On"

    filter "configurations:release"
        defines "NDEBUG"
        optimize "On"
