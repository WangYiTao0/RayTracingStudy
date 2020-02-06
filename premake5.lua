workspace "RayTracingInOneWeek"
    architecture "x64"
    targetdir "build"
    startproject "Chapter 1"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to 

-- Projects
group "Dependencies"

group ""

include "Chapter 1"
--include "Chapter 2"

