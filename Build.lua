-- premake5.lua
workspace "Square Engine"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "SantaGame"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

include "SquareEngine/Build-SquareEngine.lua"
include "SantaGame/Build-SantaGame.lua"