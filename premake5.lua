workspace "MarsEngine"
	architecture "x64"
	startproject "Martian"

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
			"MarsEngine/src/Vender",
			"MarsEngine/src"
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
		kind "StaticLib"
		language "C++"
		staticruntime "Off"

		targetdir ("bin/" .. outputdir .. "/bin")
		objdir("bin/" .. outputdir .."/obj/%{prj.name}")

		files
		{
			
			"MarsEngine/src/Addons/**.cpp",
			"MarsEngine/src/Addons/**.h",
			"MarsEngine/src/Core/**.cpp",
			"MarsEngine/src/Core/**.h",
			"MarsEngine/src/GL/**.h",
			"MarsEngine/src/RenderAPI/**.cpp",
			"MarsEngine/src/RenderAPI/**.h",
			"MarsEngine/src/Utils/**.h",
			"MarsEngine/src/Utils/**.cpp",
			"MarsEngine/src/Vender/GLFW/**.h",
			"MarsEngine/src/Vender/stb/stb_image.h",
			"MarsEngine/src/Window/**.h",
			"MarsEngine/src/Window/**.cpp",
			"MarsEngine/src/MarsHeader.h"
		}

		includedirs
		{
			"MarsEngine/src/Vender/imgui",
			"MarsEngine/src"
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
				"GLEW_STATIC",
				"_CRT_SECURE_NO_WARNINGS"
			}

		filter "configurations:Debug"
			defines "ME_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "ME_RELEASE"
			optimize "On"

	project "Martain"
		location "Martian"
		kind "ConsoleApp"
		language "C++"
		staticruntime "Off"

		targetdir ("bin/" .. outputdir .. "/bin")
		objdir("bin/" .. outputdir .."/obj/%{prj.name}")

		files
		{
			"Martian/**.h",
			"Martian/**.cpp"
		}

		includedirs
		{
			"MarsEngine/src/Vender",
			"MarsEngine/src/Vender/imgui",
			"MarsEngine/src"
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
				"GLEW_STATIC",
				"_CRT_SECURE_NO_WARNINGS"
			}
		
			libdirs
			{
				"bin/" .. outputdir .. "/lib"
			}

			links
			{
				"ImGui",
				"MarsEngine",
				"opengl32",
				"glew32s",
				"glfw3"
			}
		
		filter "configurations:Debug"
			defines "ME_DEBUG"
			symbols "On"

		filter "configurations:Release"
			defines "ME_RELEASE"
			optimize "On"