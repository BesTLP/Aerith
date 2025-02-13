workspace "Aerith"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	-- include directories relative to root folder(solution directory)
	IncludeDir = {}
	IncludeDir["GLFW"] = "Aerith/vendor/GLFW/include"

	include "Aerith/vendor/GLFW"

	project "Aerith"
		location "Aerith"
		kind "SharedLib"
		language "C++"

		targetdir("bin/" .. outputdir .. "/%{prj.name}")
		objdir("bin-int/" .. outputdir .. "/%{prj.name}")


		pchheader "AerithPch.h"
		pchsource "Aerith/src/AerithPch.cpp"

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"

		}

		includedirs
		{
			"%{prj.name}/src",
			"%{prj.name}/vendor/spdlog/include",
			"%{IncludeDir.GLFW}"
		}

		links
		{
			"GLFW",
			"opengl32.lib"
		}

		filter "system:windows"
			cppdialect "C++17"
			staticruntime "On"
			systemversion "latest"

			defines
			{
				"AERITH_PLATFORM_WINDOWS",
				"AERITH_BUILD_DLL"
			}

			postbuildcommands
			{
				"{MKDIR} ../bin/" .. outputdir .. "/Sandbox",  -- 创建目标文件夹
				"{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox"  -- 复制文件
			}



			buildoptions "/utf-8"

	filter "configurations:Debug"
		defines "AERITH_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "AERITH_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "AERITH_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

		targetdir("bin/" .. outputdir .. "/%{prj.name}")
		objdir("bin-int/" .. outputdir .. "/%{prj.name}")

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"

		}

		includedirs
		{
			"Aerith/vendor/spdlog/include",
			"Aerith/src"
		}

		links
		{
			"Aerith"
		}
		filter "system:windows"
			cppdialect "C++17"
			staticruntime "On"
			systemversion "latest"

			defines
			{
				"AERITH_PLATFORM_WINDOWS",
			}

			buildoptions "/utf-8"

	filter "configurations:Debug"
		defines "AERITH_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "AERITH_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "AERITH_DIST"
		optimize "On"