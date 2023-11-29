project "SantaGame"
   kind "SharedLib"
   language "C++"
   cppdialect "C++latest"
   toolset "v142"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"
   disablewarnings { "5105", "4996" }

   files { "Source/**.h", "Source/**.cpp" }

   includedirs
   {
      "Source",
	  
	  -- Include SquareEngine
	  "../SquareEngine/Source"
   }
      
   links
   {
      "SquareEngine"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

   filter "system:windows"
       systemversion "latest"
       defines {}

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