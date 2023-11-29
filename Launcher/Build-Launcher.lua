project "Launcher"
   kind "WindowedApp"
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

	postbuildcommands 
	{
	  "{COPY} %{cfg.targetdir}/../SantaGame/SantaGame.dll %{cfg.targetdir}"
	}
	
   filter "system:windows"
       systemversion "latest"
       defines {}

   filter "configurations:Debug"
       defines { "DEBUG", "GAME_EXPORTS" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE", "GAME_EXPORTS" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Distribution"
       defines { "DISTRIBUTION", "GAME_EXPORTS" }
       runtime "Release"
       optimize "On"
       symbols "Off"