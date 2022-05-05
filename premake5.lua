PROJECT_NAME   = "raytracing"
WORKSPACE_NAME = PROJECT_NAME

OUT_DIR             = "%{cfg.system}_%{cfg.architecture}/%{cfg.buildcfg}/"

workspace (WORKSPACE_NAME)
	startproject (PROJECT_NAME)

	configurations
	{
		"Debug",
		"Release"
	}

	architecture "x86_64"

project (PROJECT_NAME)
    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. OUT_DIR .. PROJECT_NAME)
    objdir ("obj/" .. OUT_DIR .. PROJECT_NAME)

    files
    {
        "src/**.h",
        "src/**.cpp",
        "src/**.c"
    }

    filter "configurations:Debug"
        kind "ConsoleApp"
        symbols "on"
        staticruntime "off"
        runtime "Debug"

    filter "configurations:Release"
        kind "ConsoleApp"
        optimize "on"
        symbols "off"
        staticruntime "off"
        runtime "Release"
