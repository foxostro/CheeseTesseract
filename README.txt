Cheese Tesseract (working title) by Andrew Fox

License
========
A strong effort has been made to properly track authorship of source files in
this project. Information pertaining to license and authorship has been included
at the top of each file that cannot be credited entirely to Andrew Fox. In some
cases, these groups of files have been segregated in isolated directores
alongside a README or README.txt file which includes license and authorship
information.

Source files authored by Andrew Fox may be marked as such, and may include a
license notification of their own. However, source files without such a header
may be assumed to have been authored by Andrew Fox and are subject to the MIT
license specified in the LICENSE.txt file which accompanies this notice.
  

Dependencies
==============
Some dependencies have been included in the external/ directory. Specifically,
all Windows dependencies have been included in this manner. However, to build
in Linux, several additional libraries and software packages are expected to
be available:

boost 1.34.1         <http://www.boost.org>
DevIL 1.6.7          <http://openil.sourceforge.net>
FMOD 3.7.5           <http://www.fmod.org>
Freetype2            <http:/www.freetype.org/>
GLEW                 <http:/glew.sourceforge.net/>
NVIDIA Cg Toolkit    <http://developer.nvidia.com/object/cg_toolkit.html/>
Open Dynamics Engine <http://www.ode.org/>
SDL 1.2.13           <http://www.libsdl.org/>


Building the Game
==================
Use premake to generate a makefile, Visual Studio Solution, etc to build the
project.

For example, in Linux, you might run `premake --target gnu --os linux` to
generate a make file. Then, the project may be built by simply running `make`.

Users of Microsoft's Visual Studio may be interested in running
`premake --target vs2008 --os windows` to generate a Visual Studio Solution
file. The project may then be built through the solution file as might normally
be expected.
