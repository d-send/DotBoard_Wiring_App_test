project "DBW-App"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   inheritdependencies "Off"

    files { 
        "Source/**.h",
        "Source/**.hpp",
        "Source/**.cpp",
        "Source/**.c"
    }

   includedirs
   {
      "Source",

	  -- Include Core
	  "../DBW-Core/Source",
      "../Vendor/raylib/include"
   }

   libdirs
   {
        "../Vendor/raylib/lib"
   }

   links
   {
      "DBW-Core",
      "raylib.lib",
      "winmm.lib",
      "user32.lib",
      "gdi32.lib",
      "shell32.lib",
      "SetupAPI.lib",
      "ws2_32.lib"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"