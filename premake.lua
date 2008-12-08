project.name = "CheeseTesseract"
project.config["Debug"].bindir   = "bin/" .. OS .. "_Debug"
project.config["Release"].bindir = "bin/" .. OS .. "_Release"


-- Main Game Application -----------------------------------------------------

package = newpackage()
package.name = "MainApp"
package.kind = "exe"
package.language = "c++"

package.files = {
	matchrecursive("src/*.h", "src/*.cpp"),
	"external/crossplatform/GLFT/GLFT_Font.cpp",
	"external/crossplatform/TreeLib/treelib-interface.cpp"
}

if OS == "windows" then    
	package.includepaths = {
		"external/fmod/inc/",
		"external/crossplatform/GLFT/",
		"external/crossplatform/TreeLib/",
		"external/windows/boost/include/",
		"external/windows/cg/include/",
		"external/windows/DevIL/include/",
		"external/windows/freetype/include/",
		"external/windows/freetype/include/freetype2",
		"external/windows/glew/include/",
		"external/windows/ode/include/",
		"external/windows/SDL/include/"
	}

	package.libpaths = {
		"external/fmod/lib/",
		"external/windows/cg/lib/",
		"external/windows/DevIL/lib/",
		"external/windows/freetype/lib/",
		"external/windows/glew/lib/",
		"external/windows/ode/lib/",
		"external/windows/SDL/lib/"
	}

	package.links = {
		"freetype",
		"cg",
		"cgGL",
		"fmodvc",
		"ode_doubled",
		"SDL",
        "SDLmain",
		"DevIL",
		"ilu",
		"ilut",
		"glew32",
		"opengl32",
		"glu32",
		"winmm"
	}
    
elseif OS == "linux" then
	package.includepaths = {
		"external/fmod/inc/",
		"external/crossplatform/GLFT/",
		"external/crossplatform/TreeLib/"
	}

	package.links = {
		"ode",
		"GL",
		"GLU",
		"GLEW",
		"SDL",
        "SDLmain",
		"IL",
		"ILU",
		"ILUT",
		"Cg",
		"CgGL",
		"freetype",
	}
	
	package.buildoptions = { "-rdynamic `sdl-config --cflags` `freetype-config --cflags` `ode-config --cflags`" }
	package.config["Debug"].linkoptions = { "bin/linux_Debug/libfmod-3.75.so" }
	package.config["Release"].linkoptions = { "bin/linux_Release/libfmod-3.75.so" }
else
	error("Unsupported Operating System: " .. OS)
end

