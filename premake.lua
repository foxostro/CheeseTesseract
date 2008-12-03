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
		"external/crossplatform/GLFT/",
		"external/crossplatform/TreeLib/",
		"external/windows/boost/include/",
		"external/windows/cg/include/",
		"external/windows/DevIL/include/",
		"external/windows/fmod/inc/",
		"external/windows/freetype/include/",
		"external/windows/freetype/include/freetype2",
		"external/windows/glew/include/",
		"external/windows/ode/include/",
		"external/windows/SDL/include/"
	}

	package.libpaths = {
		"external/windows/cg/lib/",
		"external/windows/DevIL/lib/",
		"external/windows/fmod/lib/",
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
		"external/crossplatform/GLFT/",
		"external/crossplatform/TreeLib/",
		"external/windows/fmod/inc/"
	}

	package.links = {
		"freetype",
		"cg",
		"cgGL",
--		"libfmod-3.75.so",
		"ode",
		"SDL",
		"DevIL",
		"ilu",
		"ilut",
		"glew",
		"GL",
		"GLU",
	}
	
	package.buildoptions = { "`sdl-config --cflags` `freetype-config --cflags` `ode-config --cflags`" }
else
	error("Unsupported Operating System: " .. OS)
end

