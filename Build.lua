workspace "DBW"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "DBW-App"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

group "Core"
	include "DBW-Core/Build-Core.lua"
group ""
   include "DBW-App/Build-App.lua"