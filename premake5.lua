workspace "Aerith"
	architecture "x64"
	startproject "Sandbox"

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
	IncludeDir["Glad"] = "Aerith/vendor/Glad/include"
	IncludeDir["ImGui"] = "Aerith/vendor/imgui"

	group "Dependencies"
		include "Aerith/vendor/GLFW"
		include "Aerith/vendor/Glad"
		include "Aerith/vendor/imgui"
	group ""

	project "Aerith"
		location "Aerith"
		kind "SharedLib"
		language "C++"
		staticruntime "off"

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
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Glad}",
			"%{IncludeDir.ImGui}"
		}

		links
		{
			"GLFW",
			"opengl32.lib",
			"ImGui",
			"Glad"
		}

		filter "system:windows"
			cppdialect "C++17"
			systemversion "latest"

			defines
			{
				"AERITH_PLATFORM_WINDOWS",
				"AERITH_BUILD_DLL",
				"GLFW_INCLUDE_NONE"
			}

			postbuildcommands
			{
				"{MKDIR} ../bin/" .. outputdir .. "/Sandbox",  -- ����Ŀ���ļ���
				"{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox"  -- �����ļ�
			}



			buildoptions "/utf-8"

	filter "configurations:Debug"
		defines "AERITH_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "AERITH_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "AERITH_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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
			systemversion "latest"

			defines
			{
				"AERITH_PLATFORM_WINDOWS",
			}

			buildoptions "/utf-8"

	filter "configurations:Debug"
		defines "AERITH_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "AERITH_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "AERITH_DIST"
		runtime "Release"
		optimize "On"