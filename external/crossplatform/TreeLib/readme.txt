Delta-Works TreeLib (c) 2008 Stefan Elsen


Q/A

What is TreeLib?

TreeLib is a shared library for Linux and Windows to handle trees. The library handles generation, automated detail approximation and rendering in OpenGL.
An existing OpenGL context is required for rendering.


Are there any restrictions on how to create the OpenGL context?

No. Any context will do. With whatever foundation or method created.
In general You should be able to use whatever language as long as it can create an OpenGL context and open DLLs/SOs. The provided headers are for C/C++ only so far though. You may have to improvise.


Is there a version of TreeLib for Direct3d?

No and no.


What extensions does TreeLib use/require?

TreeLib does not use/require any extensions of the standard OpenGL interface. Used components are textures and display lists.


Is TreeLib free?

The library itself is free for use in non-commercial projects. Authors of commercial projects may acquire a license for their project.


Is TreeLib open source?

Not for the time being.


Can generated trees be exported?

This version of the library is not designed for exporting. It merely presents a just-do-it approach to adding trees to an OpenGL scenery.
The leaf-batches of the trees are rendered facing the viewer and would look less nice in a static mesh anyway.


How many trees can i use simultaniously?

As many as you like/have memory for.


Is TreeLib multi thread safe?

Mostly. For the sake of memory and speed efficiency i avoided the use of mutexes per tree.
To see what functions are thread safe in what manner check the function specifications below.


May I reuse the provided textures?

The textures are not copyrighted and may be reused or modified as you see fit.
To extract the textures from the archive use the texture archive manager available at
http://www.delta-works.org/download/?&details=Texture%20Archive
It's fairly straightforward i believe. A complete documentation is in work.
Also a unix command line tool is in work.



TreeLib contents:

The core consists of the shared library treelib.dll (treelib.so for linux/unix) and the texture archive treelib.mta. These files should be located in the same folder when linking.
Additionally the files treelib.h and interface.h/.cpp help simplifying the linking process for C++ applications.



Linking TreeLib:

You should start by putting all files into a sub-folder of the project that will access the library. For easier linking it's useful to include the files interface.h and interface.cpp into your project.
The function TreeLib::load(filename) handles all necessary linking and returns true on success.
Alternatively you can link the library and extract the pointers manually.
The linux .so file will probably cause a lot of difficulties at this point. I'm sticking to the problem and may get a solution up some time in the future.


Generating and rendering trees:

There are currently two ways of creating a tree:
void*tree = TreeLib::createTree() will create a pure random tree and
void*tree = TreeLib::createTreeFromSeed(int) will (re)create a tree from its seed.
When a new tree is created, it receives a copy of the global configuration as its own.
By default generated trees are about the size of real trees with 1.0f being 1 meter. This can be altered using the respective settings.
To render a tree simply call TreeLib::renderTree(tree);
If a tree is no longer needed then it can be discarded using the TreeLib::discard(tree); function.







Examples:


Linking, creating and rendering:

if (!TreeLib::load("treelib/treelib.dll"))
{
	cout << "Error while linking:\n"<<TreeLib::error()<<endl;
	return -1;
}

TreeLib::Tree my_tree = TreeLib::createTree();
if (!my_tree)
{
	cout << "Error while creating a tree:\n"<<TreeLib::error()<<endl;
	return -1;
}


if (!TreeLib::renderTree(my_tree))
{
	cout << "Error while rendering:\n"<<TreeLib::error()<<endl;
	return -1;
}



Disabling shadows on a tree:

TreeLib::setAttributei(my_tree,TL_SHADOW,TL_NO_SHADOW);



Decreasing the level of detail of trees:

TreeLib::setAttributef(NULL,TL_LOD,0.75); //this will affect all trees that are created from this point forward

or on a specific tree:

TreeLib::setAttribute(my_tree,TL_LOD,0.75);
TreeLib::rebuildTree(my_tree);


Toggling auto balance:

bool balancing = TreeLib::getAttributei(my_tree,TL_AUTO_BALANCE);
TreeLib::setAttributei(my_tree,TL_AUTO_BALANCE,!balancing);
TreeLib::rebuildTree(my_tree);



Rendering multiple instances of a tree:

glPushMatrix();
for (int i = 0; i < 10; i++)
{
	glTranslatef(10.0f,0,0);
	TreeLib::putTree(my_tree);
}
glPopMatrix();
TreeLib::renderComposition();



Function specifications:

bool	TreeLib::init(const char*path);

Initializes the library. [path] should point to to the filename of the library or its folder ("./TreeLib/treelib.dll", "./TreeLib/", "./TreeLib" are valid).
The library must be initialized before trees can be generated and/or rendered. If you used the provided interface.h/.cpp files then TreeLib::load(...) will automatically call this function.
This function does not execute any OpenGL commands. A valid context is thus not needed at this point.

Return value: true on success.
Note: use TreeLib::getAttributecv(NULL,TL_ERROR) to receive an error string.


TreeLib::Tree	TreeLib::createTree();

Creates a new tree using the current global configuration and a random seed.
This function does not execute any OpenGL commands. A valid context is thus not needed at this point.
Trees can be created simultaniously by different threads. You should however not attempt to create trees and alter the global configuration at the same time or your application might crash.

Return value: pointer to the new tree or NULL if an error occured.
Note: use TreeLib::getAttributecv(NULL,TL_ERROR) to receive an error string.


TreeLib::Tree	TreeLib::createTreeFromSeed(int seed);

Creates a new tree using the current global configuration and the specified seed. This function does not execute any OpenGL commands. A valid context is thus not needed at this point.
Trees can be created simultaniously by different threads. You should however not attempt to create trees and alter the global configuration at the same time or your application might crash.

Return value: pointer to the new tree or NULL if an error occured.
Note: use TreeLib::getAttributecv(NULL,TL_ERROR) to receive an error string.


void	TreeLib::discardTree(TreeLib::Tree tree)

Discards a created tree. This function requires a valid OpenGL context if the tree was rendered at least once.
The specified tree is required to be valid. No validation check is performed to check its integrity. The pointer may be NULL though causing the function to return.


void	TreeLib::rebuildTree(TreeLib::Tree tree)

Rebuilds the specified tree using the current tree configuration. If the configuration was not changed then the resulting tree will be identical to the original created one. This function does not execute any OpenGL commands. A valid context is thus not needed at this point.
The specified tree is required to be valid. No validation check is performed to check its integrity. The pointer may be NULL though causing the function to return.



float			TreeLib::getAttributef(TreeLib::Tree tree, TreeLib::Attribute attribute)
const float*	TreeLib::getAttributefv(TreeLib::Tree tree, TreeLib::Attribute attribute)
int				TreeLib::getAttributei(TreeLib::Tree tree, TreeLib::Attribute attribute)
const char*		TreeLib::getAttributecv(TreeLib::Tree tree, TreeLib::Attribute attribute)

Retrieves the respective attribute from the current configuration. If the specified tree is not NULL then the configuration of the specified tree is queried, otherwise the global configuration.
Certain attributes can only be queried from trees, others only globaly. For more information on this check the attribute specification. This function does not execute any OpenGL commands. A valid context is thus not needed at this point.
The specified tree is required to be valid or NULL. No validation check is performed to check its integrity.

Return value: value of the respective attribute or 0/NULL if the attribute was not readable.
Note: use TreeLib::getAttributecv(NULL,TL_ERROR) to receive an error string.



bool TreeLib::setAttributef(TreeLib::Tree tree, TreeLib::Attribute attribute, float value)
bool TreeLib::setAttributefv(TreeLib::Tree tree, TreeLib::Attribute attribute, const float*value)
bool TreeLib::setAttributei(TreeLib::Tree tree, TreeLib::Attribute attribute, int value)
bool TreeLib::setAttributecv(TreeLib::Tree tree, TreeLib::Attribute attribute, const char*value)

Alters the respective attribute of the current configuration. If the specified tree is not NULL then the configuration of the specified tree is altered, otherwise the global configuration.
Changes to the global configuration will affect all subsequently created trees.
Certain attributes can only be altered locally, others only globaly. For more information on this check the attribute specifications.
Note that modifications to a tree's configuration take effect when it is rebuilt via rebuildTree(tree) the next time.
This function does not execute any OpenGL commands. A valid context is thus not needed at this point.
The specified tree is required to be valid or NULL. No validation check is performed to check its integrity.

Return value: true on success, false otherwise.
Note: use TreeLib::getAttributecv(NULL,TL_ERROR) to receive an error string.



void	TreeLib::pushAttributes()

Pushes the current global configuration on the top of the configuration stack.
This function does not execute any OpenGL commands. A valid context is thus not needed at this point.
The function returns if the configuration stack is full.



void	TreeLib::popAttributes()

Overwrites the current global configuration with the configuration stored on the top of the configuration stack.
The function returns if no configurations were stacked.



bool	TreeLib::renderTree(TreeLib::Tree tree)

This function is deprecated. Use TreeLib::putTree(tree) and TreeLib::renderComposition() instead.



bool	TreeLib::putTree(TreeLib::Tree tree)

Inserts the specified tree into the current composition using the currently loaded modelview matrix.
The same tree may be inserted any number of times at different locations.
The function queries the currently loaded OpenGL modelview matrix and thus requires a valid OpenGL context to operate.
The specified tree is required to be valid. No validation check is performed to check its integrity. The pointer may be NULL though causing the function to simply return.
This function is thread-safe and can be called from different threads using the same tree or different trees.



void	TreeLib::flushComposition()

Clears the active composition without rendering.
This function is thread-safe and can be called from different threads.



bool	TreeLib::renderComposition()

Renders and flushes the active composition. The library uses on the fly compilation meaning that detail levels are not generated until needed.
For rendering a valid OpenGL context is required or the function will fail.
This function is thread-safe and can be called from different threads.
You should, however, not attempt to modify the tree while rendering it or your application may crash.
Once rendered, the active composition is empty and has to be re-composed using TreeLib::putTree(...).



bool	TreeLib::renderCompositionBranches()

Renders the branches of the currently active composition. The library uses on the fly compilation meaning that detail levels are not generated until needed.
For rendering a valid OpenGL context is required or the function will fail.
This function is thread-safe and can be called from different threads.
You should, however, not attempt to modify the tree while rendering it or your application may crash.
Unlike TreeLib::renderComposition(), this function does not flush the active composition.
The client application is responsible for manually flushing the composition by calling TreeLib::flushComposition().



bool	TreeLib::renderCompositionLeaves()

Renders the leaves of the currently active composition.
For rendering a valid OpenGL context is required or the function will fail.
This function is thread-safe and can be called from different threads.
You should, however, not attempt to modify the tree while rendering it or your application may crash.
Unlike TreeLib::renderComposition(), this function does not flush the active composition.
The client application is responsible for manually flushing the composition by calling TreeLib::flushComposition().



bool	TreeLib::renderCompositionShadows()

Renders the shadows of the currently active composition.
For rendering a valid OpenGL context is required or the function will fail.
This function is thread-safe and can be called from different threads.
You should, however, not attempt to modify the tree while rendering it or your application may crash.
Unlike TreeLib::renderComposition(), this function does not flush the active composition.
The client application is responsible for manually flushing the composition by calling TreeLib::flushComposition().




Attribute specifications:

TL_ERROR

Global only attribute. Read only. getAttributecv(...) only.
Returns a string describing the most recently occured error.


TL_VERSION

Global only attribute. Read only. getAttributecv(...) or getAttributei(...).
getAttributecv(...) returns a string representation of the current version.
getAttributei(...) returns an integer containing the current version number.


TL_FACES

Tree only attribute. Read only. getAttributei(...) or getAttributef(...)
Returns an integer containing the number of triangles of the branch-surface of the specified tree (not including the number of leaves it has).
This value is zero until the tree is rendered the first time.
If the tree is rendered in different detail steps then the returned value will be the sum of all faces in all generated detail steps to this point.


TL_NODES

Tree only attribute. Read only. getAttributei(...) or getAttributef(...)
Returns an integer containing the number of nodes the specified tree has.


TL_LEAVES

Tree only attribute. Read only. getAttributei(...) or getAttributef(...)
Returns an integer containing the number of leaf batches the specified tree has.


TL_BOX_LOWER_CORNER

Tree only attribute. Read only. getAttributefv(...) only.
Returns a pointer to the vector containing the lower corner of the surrounding bounding box.
The returned pointer contains three elements. Note: can be NULL if an error occured.


TL_BOX_UPPER_CORNER

Tree only attribute. Read only. getAttributefv(...) only.
Returns a pointer to the vector containing the upper corner of the surrounding bounding box.
The returned pointer contains three elements. Note: can be NULL if an error occured.


TL_CENTER

Tree only attribute. Read only. getAttributefv(...) only.
Returns a pointer to the vector containing the center of the surrounding bounding box.
The returned pointer contains three elements. Note: can be NULL if an error occured.


TL_RADIUS

Tree only attribute. Read only. getAttributef(...) or getAttributei(...).
Returns a float containing the distance from the center of the tree to its outer most leaf/branch.


TL_SEED

Tree only attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the seed value of the respective tree. Setting or querying the global seed has no effect.
After setting the tree seed the tree needs to be rebuild for the changes to take effect.
Note that two trees using the same seed will only be identical if all other attributes are the same, too.


TL_SHADOW

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the shadow setting of the global or local tree configuration.
Currently supported values are:
	TL_NO_SHADOW		shadows disabled
	TL_FLAT_BLOB_SHADOW	simple planar blob shadow below the tree and leaf batches
	TL_CONICAL_BLOB_SHADOW	simple conical blob shadow below the tree and leaf batches
The global shadow attribute is TL_CONICAL_BLOB_SHADOW by default.


TL_AUTO_BALANCE

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the auto-balance setting of the global or local tree configuration.
After changing the auto balance attribute the tree needs to be rebuild for the changes to take effect.
Can be 0 (disabled) or 1 (enabled).
If auto balancing is enabled then an updated tree will be bend so that the weight is roughly balanced.
Auto balancing is enabled by default.


TL_ROTATE_LEAVES

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the rotate-leaves setting of the global or local tree configuration.
Can be 0 (disabled) or 1 (enabled).
If leaf rotation is enabled then tree leaf batches will be rotated so that they appear less repetitive.
Leaf rotation is enabled by default.


TL_BARK_TEXTURE_OVERRIDE

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the bark texture override of the global or local tree configuration.
This option allows to override the texture of the texture archive and replace it with a custom opengl texture id.
0 by default causing TreeLib to retrieve its textures from the archive.


TL_LEAF_TEXTURE_OVERRIDE

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the leaf texture override of the global or local tree configuration.
This option allows to override the texture of the texture archive and replace it with a custom opengl texture id.
0 by default causing TreeLib to retrieve its textures from the archive.


TL_TEXTURE_PACK

General attribute. Read and write. getAttributecv(...) or setAttributecv(...).
Queries or sets the texture pack setting of the global or local tree configuration.
When rendering a tree two textures are used: one for the bark and one for the leaves. These textures are grouped in folders in the texture archive. Currently only 'default' is supported but this may change over time.
Setting an invalid texture pack will cause the render function to fail.


TL_LOD

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the lod (level of detail) setting of the global or local tree configuration.
After setting the tree lod the tree needs to be rebuild for the changes to take effect.
This value affects the amount of visible faces on all detail levels when rendering the tree.
The level of detail is set to 1.0 by default. Setting it to a higher value will increase the number of faces (setting it to 2.0 will increase the number of faces by a factor of 4, setting it to 0.5 will decrease the number of faces to 1/4).


TL_LEAF_SIZE

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the leaf batch size setting of the global or local tree configuration.
This value affects the visible size of leaf batches while rendering.
0.5 by default.


TL_BASE_LENGTH

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the fractal trunk root length (distance to the next node).
After setting the tree base length the tree needs to be rebuild for the changes to take effect.
Altering this value will cause the tree to stretch or shrink.
1.25 by default.


TL_BASE_RADIUS

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the fractal root node radius.
After setting the tree base radius the tree needs to be rebuild for the changes to take effect.
Altering this value will cause the tree to have thicker or thinner branches.
0.25 by default.


TL_VARIANCE

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the fractal growth noise level.
After setting the tree variance the tree needs to be rebuild for the changes to take effect.
Less variance means more straight branches, more variance means more bent branches.
0.125 by default


TL_SUCCESSOR_RADIUS_SCALE

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the fractal successor radius scale.
After setting the tree successor radius scale the tree needs to be rebuild for the changes to take effect.
0.8 by default


TL_SUCCESSOR_LENGTH_SCALE

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the fractal successor length scale.
After setting the tree successor length scale the tree needs to be rebuild for the changes to take effect.
0.9 by default


TL_BRANCH_RADIUS_SCALE

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the fractal branch radius scale.
After setting the tree branch radius scale the tree needs to be rebuild for the changes to take effect.
0.56 by default


TL_BRANCH_LENGTH_SCALE

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the fractal branch length scale.
After setting the tree branch length scale the tree needs to be rebuild for the changes to take effect.
0.81 by default


TL_BRANCH_MIN_DEVIATION

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the minimum angle between branch siblings (0-360).
After setting the tree branch minimum deviation the tree needs to be rebuild for the changes to take effect.
40 by default
Warning: setting this value higher than 80 may cause tree generation to get stuck in an endless loop.


TL_VERTICAL_ORIENTATION

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the vertical growth orientation.
After setting the tree vertical orientation the tree needs to be rebuild for the changes to take effect.
Assigning a positive value will cause the tree to grow more upwards, giving it a negative value will cause the branches to lean more towards the ground.
2.5 by default


TL_SHADOW_CONE_STEEPNESS

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the shadow cone steepness.
This value takes effect if shadow is set to TL_CONICAL_BLOB_SHADOW. It specifies the steepness of the shadow cone per unit length.
0.1 by default


TL_MAX_VIEWING_DISTANCE

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the maximum viewing distance.
This value specifies the absolute maximum viewing distance of a tree. If a trees effective distances exceeds this value then
it will not be put into the scenery and thus not rendered.
600 by default


TL_SCALE

General attribute. Read and write. getAttributef(...), getAttributei(...), setAttributef(...), setAttributei(...).
Queries or sets the general scenery scale.
After changing the scale attribute the tree needs to be rebuild for the changes to take effect.
This value affects all other linear values, be they tree trunk/branch length, shadow size or max viewing distance.
1 by default



Additional attributes:
The following attributes will only work if the library has been compiled for extended functionality. The normal free version will return errors if you try to access any of these.

TL_VERTEX_PATH

Tree only attribute. Read only. getAttributefv(...) only.
Queries the vertex data of the specified tree in the current level of detail. The returned float array contains position, normal and texcoords for each vertex in the following order:
x1, y1, z1, nx1, ny1, nz1, tx1, ty1,    x2, y2, z2, nx2, ny2, nz2, tx2, ty2, ...
The returned array is allocated per tree and is valid until the tree is rebuilt or deleted. Do not attempt to manually delete this array.
To query the number of vertices in the path use TL_VERTEX_PATH_LENGTH. The total length of the returned array is vertices*8.


TL_VERTEX_PATH_LENGTH

Tree only attribute. Read only. getAttributei(...) only.
Queries the number of vertices in the vertex path returned by getAttributefv(tree,TL_VERTEX_PATH).
The returned value remains correct until the tree is rebuilt.


TL_INDEX_PATH

Tree only attribute. Read only. getAttributeiv(...) only.
Queries the index data of the specified tree in the current level of detail. The returned int array contains the vertex indices of the tree branch triangles.
Each three indices of the returned array form one triangle.
The returned array is allocated per tree and is valid until the tree is rebuilt or deleted. Do not attempt to manually delete this array.
To query the number of indices in the path use TL_INDEX_PATH_LENGTH.


TL_INDEX_PATH_LENGTH

Tree only attribute. Read only. getAttributei(...) only.
Queries the number of indices in the index path returned by getAttributeiv(tree,TL_INDEX_PATH).
The returned value remains correct until the tree is rebuild. Note that the returned int contains the number of indices, not triangles.
The number of triangles is indices/3.


TL_LEAF_PATH

Tree only attribute. Read only. getAttributefv(...) only.
Queries the leaf data of the specified tree. The returned array contains the central position of each leaf in x-y-z-order, with each three floats per leaf.
The returned array is allocated per tree and is valid until the tree is rebuilt or deleted. Do not attempt to manually delete this array.
To query the number of leafs in the path use TL_LEAVES. The total length of the returned array is leaves*3. To query the size of the leaves use TL_LEAF_SIZE.



License:

This is a beta-state of the provided library and may as such be distributed and used as pleased as long as the original runtime-behavior of the library is preservered. Any distribution of the library must be free of charges.
Usage of this library is limited to non-commercial projects/products. If You intend to use it in a commercial project then please contact me via my internet web presence www.delta-works.org.

This library is provided as is, meaning that no guarantee what so ever is issued regarding runtime-stability and/or damage it may or may not cause.

Modification of the binary library-file is strictly forbidden, including but not limited to disassembly and/or reverse-programming. Also distribution without an unchanged copy of this file is forbidden.

All rights and privileges that have not explicitly been granted are reserved.

THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.