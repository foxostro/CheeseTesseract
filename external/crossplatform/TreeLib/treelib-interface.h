#ifndef interfaceH
#define interfaceH


#include "treelib.h"


namespace TreeLib
{
    extern _init                  init;
    extern _createTree            createTree;
    extern _createTreeFromSeed    createTreeFromSeed;
    extern _discardTree           discardTree;
    extern _rebuildTree           rebuildTree;
    extern _getAttributef         getAttributef;
    extern _getAttributefv        getAttributefv;
    extern _getAttributei         getAttributei;
	extern _getAttributeiv        getAttributeiv;
    extern _getAttributecv        getAttributecv;
    extern _setAttributef         setAttributef;
    extern _setAttributefv        setAttributefv;
    extern _setAttributei         setAttributei;
    extern _setAttributecv        setAttributecv;
    extern _pushAttributes        pushAttributes;
    extern _popAttributes         popAttributes;
    extern _renderTree            renderTree;

    extern _renderCompositionLeaves		renderCompositionLeaves;
    extern _renderCompositionShadows	renderCompositionShadows;
    extern _renderCompositionBranches	renderCompositionBranches;
    extern _flushComposition			flushComposition;
    extern _renderComposition			renderComposition;
    extern _putTree						putTree;


    bool        load(const char*filename);
    void        unload();
    const char* error();
}



#endif
