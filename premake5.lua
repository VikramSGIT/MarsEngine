workspace "MarsEngine"
	architecture "x64"
	startproject "MarsEngine"

	configurations
	{
		"Debug",
		"Release"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

	project "ImGui"
		location "MarsEngine/src/Vender/imgui"
		kind "StaticLib"
		language "C++"
		staticruntime "Off"

		targetdir ("bin/" .. outputdir .. "/lib")
		objdir("bin/" .. outputdir .."/obj/%{prj.name}")

		files
		{
			"MarsEngine/src/Vender/imgui/backends/imgui_impl_glfw.cpp",
			"MarsEngine/src/Vender/imgui/backends/imgui_impl_glfw.h",
			"MarsEngine/src/Vender/imgui/backends/imgui_impl_opengl3.cpp",
			"MarsEngine/src/Vender/imgui/backends/imgui_impl_opengl3.h",
			"MarsEngine/src/Vender/imgui/*.cpp",
			"MarsEngine/src/Vender/imgui/*.h"
		}

		includedirs
		{
			"MarsEngine/src/Vender/imgui",
			"MarsEngine/src/Vender/",
			"MarsEngine/src/"
		}

		flags
		{
			"MultiProcessorCompile"
		}

		filter "system:windows"
			cppdialect "C++17"
			staticruntime "On"

			defines
			{
				"IMGUI_IMPL_OPENGL_LOADER_GLEW",
				"GLEW_STATIC"
			}

		filter "configurations:Debug"
			defines "ME_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "ME_RELEASE"
			optimize "On"

	project "MarsEngine"
		location "MarsEngine"
		kind "ConsoleApp"
		language "C++"
		staticruntime "Off"

		targetdir ("bin/" .. outputdir .. "/bin")
		objdir("bin/" .. outputdir .."/obj/%{prj.name}")

		files
		{
			"MarsEngine/src/Core/**.cpp",
			"MarsEngine/src/Core/**.h",
			"MarsEngine/src/GL/**.h",
			"MarsEngine/src/RenderAPI/**.cpp",
			"MarsEngine/src/RenderAPI/**.h",
			"MarsEngine/src/Utils/**.h",
			"MarsEngine/src/Utils/**.cpp",
			"MarsEngine/src/Vender/GLFW/**.h",
			"MarsEngine/src/Vender/OGLM/**.h",
			"MarsEngine/src/Vender/stb/stb_image.h",
			"MarsEngine/src/Window/**.h",
			"MarsEngine/src/Window/**.cpp",
			"MarsEngine/src/MarsHeader.h"
		}

		includedirs
		{
			"MarsEngine/src/Vender/imgui",
			"MarsEngine/src/"
		}

		flags
		{
			"MultiProcessorCompile"
		}

		filter "system:windows"
			cppdialect "C++17"
			

			defines
			{
				"ME_PLATFORM_WINDOWS",
				"IMGUI_IMPL_OPENGL_LOADER_GLEW",
				"GLFW_INCLUDE_NONE",
				"GLEW_STATIC"
			}

			libdirs
			{
				"bin/" .. outputdir .. "/lib"
			}

			links
			{
				"ImGui",
				"opengl32",
				"user32",
				"gdi32",
				"shell32",
				"glfw3",
				"glew32s"
			}

		filter "configurations:Debug"
			defines "ME_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "ME_RELEASE"
			optimize "On"