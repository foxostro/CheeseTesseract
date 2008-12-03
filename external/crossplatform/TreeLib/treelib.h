#ifndef treelibH
#define treelibH


#define TL_ERROR                        0x0300
#define TL_VERSION                      0x0301
#define TL_FACES                        0x1100
#define TL_NODES                        0x1101
#define TL_LEAVES                       0x1102
#define TL_BOX_LOWER_CORNER             0x1400
#define TL_BOX_UPPER_CORNER             0x1401
#define TL_CENTER                       0x1402
#define TL_RADIUS                       0x1200
#define TL_SEED                         0x2100
#define TL_SHADOW                       0x2101
#define TL_AUTO_BALANCE                 0x2102
#define TL_ROTATE_LEAVES                0x2103
#define TL_TEXTURE_PACK                 0x2300
#define TL_LOD                          0x2200
#define TL_LEAF_SIZE                    0x2201
#define TL_BASE_LENGTH                  0x2202
#define TL_BASE_RADIUS                  0x2203
#define TL_VARIANCE                     0x2204
#define TL_SUCCESSOR_RADIUS_SCALE       0x2205
#define TL_SUCCESSOR_LENGTH_SCALE       0x2206
#define TL_BRANCH_RADIUS_SCALE          0x2207
#define TL_BRANCH_LENGTH_SCALE          0x2208
#define TL_BRANCH_MIN_DEVIATION         0x2209
#define TL_VERTICAL_ORIENTATION         0x2210
#define TL_SHADOW_CONE_STEEPNESS        0x2211

#define TL_VERTEX_PATH					0x1403		//{x,y,z,nx,ny,nz,u,v}
#define	TL_VERTEX_PATH_LENGTH			0x1103
#define TL_INDEX_PATH					0x1500
#define TL_INDEX_PATH_LENGTH			0x1104
#define TL_LEAF_PATH					0x1404		//{x,y,z}


/* shadow constants */

#define TL_NO_SHADOW                    0
#define TL_FLAT_BLOB_SHADOW             1
#define TL_CONICAL_BLOB_SHADOW          2

namespace TreeLib
{
    typedef int     Attribute;
    typedef void*   Tree;

    typedef bool        (*_init)(const char*);                              //_ZN7TreeLib4initEPKc
    typedef Tree        (*_createTree)();                                   //_ZN7TreeLib10createTreeEv
    typedef Tree        (*_createTreeFromSeed)(int);                        //_ZN7TreeLib18createTreeFromSeedEi
    typedef void        (*_discardTree)(Tree);                              //_ZN7TreeLib11discardTreeEPv
    typedef void        (*_rebuildTree)(Tree);                              //_ZN7TreeLib11rebuildTreeEPv
    typedef float       (*_getAttributef)(Tree,Attribute);                  //_ZN7TreeLib13getAttributefEPvi
    typedef const float*(*_getAttributefv)(Tree,Attribute);                 //_ZN7TreeLib14getAttributefvEPvi
    typedef int         (*_getAttributei)(Tree,Attribute);                  //_ZN7TreeLib13getAttributeiEPvi
	typedef const int*	(*_getAttributeiv)(Tree,Attribute);					//_ZN7TreeLib14getAttributeivEPvi
    typedef const char* (*_getAttributecv)(Tree,Attribute);                 //_ZN7TreeLib14getAttributecvEPvi

    typedef bool        (*_setAttributef)(Tree,Attribute,float);            //_ZN7TreeLib13setAttributefEPvif
    typedef bool        (*_setAttributefv)(Tree,Attribute,const float*);    //_ZN7TreeLib14setAttributefvEPviPKf
    typedef bool        (*_setAttributei)(Tree,Attribute,int);              //_ZN7TreeLib13setAttributeiEPvii
    typedef bool        (*_setAttributecv)(Tree,Attribute,const char*);     //_ZN7TreeLib14setAttributecvEPviPKc
    typedef void        (*_pushAttributes)();                               //_ZN7TreeLib14pushAttributesEv
    typedef void        (*_popAttributes)();                                //_ZN7TreeLib13popAttributesEv
    typedef bool        (*_renderTree)(Tree);                         		//_ZN7TreeLib10renderTreeEPv

	typedef	bool		(*_renderCompositionLeaves)();						//_ZN7TreeLib23renderCompositionLeavesEv
	typedef	bool		(*_renderCompositionShadows)();						//_ZN7TreeLib24renderCompositionShadowsEv
	typedef	bool		(*_renderCompositionBranches)();					//_ZN7TreeLib25renderCompositionBranchesEv
	typedef	void		(*_flushComposition)();								//_ZN7TreeLib16flushCompositionEv
	typedef	bool		(*_renderComposition)();							//_ZN7TreeLib17renderCompositionEv
	typedef	bool		(*_putTree)(Tree);									//_ZN7TreeLib7putTreeEPv


}


#endif

