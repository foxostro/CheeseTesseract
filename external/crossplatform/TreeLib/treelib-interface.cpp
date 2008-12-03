#include "treelib-interface.h"



#if defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(_Windows)
    #define SYSTEM_WINDOWS
#elif defined(__linux) || defined(linux) || defined(unix) || defined(__unix__) || defined(__unix)
    #define SYSTEM_UNIX
#else
    #define SYSTEM_UNKNOWN
#endif


#include <stdlib.h>

#ifdef SYSTEM_WINDOWS
    #include <windows.h>
    #include <winbase.h>
    #include <stdio.h>
#elif defined(SYSTEM_UNIX)
    #include <unistd.h>
    #include <pthread.h>
    #include <sys/sem.h>
    #include <sys/ipc.h>
    #include <sys/types.h>
    #include <dlfcn.h>
    #include <string.h>
    #include <fcntl.h>
    #include <stdarg.h>


    #ifndef SEM_R
        #define SEM_A        0200    /* alter permission */
        #define SEM_R       0400    /* read permission */
    #endif
#endif



namespace TreeLib
{
    _init                  init=NULL;
    _createTree            createTree=NULL;
    _createTreeFromSeed    createTreeFromSeed=NULL;
    _discardTree           discardTree=NULL;
    _rebuildTree           rebuildTree=NULL;
    _getAttributef         getAttributef=NULL;
    _getAttributefv        getAttributefv=NULL;
    _getAttributei         getAttributei=NULL;
    _getAttributeiv        getAttributeiv=NULL;
    _getAttributecv        getAttributecv=NULL;
    _setAttributef         setAttributef=NULL;
    _setAttributefv        setAttributefv=NULL;
    _setAttributei         setAttributei=NULL;
    _setAttributecv        setAttributecv=NULL;
    _pushAttributes        pushAttributes=NULL;
    _popAttributes         popAttributes=NULL;
    _renderTree            renderTree=NULL;

    _renderCompositionLeaves	renderCompositionLeaves=NULL;
    _renderCompositionShadows	renderCompositionShadows=NULL;
    _renderCompositionBranches	renderCompositionBranches=NULL;
    _flushComposition			flushComposition=NULL;
    _renderComposition			renderComposition=NULL;
    _putTree					putTree=NULL;

}

#ifdef SYSTEM_WINDOWS
    static HMODULE             module_handle=NULL;
    static char                error_buffer[512];
#else
    static void               *module_handle=NULL;
#endif
static bool         dll_error=false;
static const char*  local_error="No Error";

template <class T> static bool locate(const char*name,T&target)
{
        #ifdef SYSTEM_WINDOWS
            target = (T)GetProcAddress(module_handle,name);
        #else
            target = (T)dlsym(module_handle,name);
        #endif
        return target != NULL;
}

bool    TreeLib::load(const char*filename)
    {
        #ifdef SYSTEM_WINDOWS
            module_handle = LoadLibrary(filename);
        #elif defined(SYSTEM_UNIX)
            module_handle = dlopen(filename,RTLD_LAZY);
        #endif
        if (!module_handle)
        {
            dll_error = true;
            local_error = NULL;
            return false;
        }
        bool success=true;

        success &= locate("_ZN7TreeLib4initEPKc",init);
        success &= locate("_ZN7TreeLib10createTreeEv",createTree);
        success &= locate("_ZN7TreeLib18createTreeFromSeedEi",createTreeFromSeed);
        success &= locate("_ZN7TreeLib11discardTreeEPv",discardTree);
        success &= locate("_ZN7TreeLib11rebuildTreeEPv",rebuildTree);
        success &= locate("_ZN7TreeLib13getAttributefEPvi",getAttributef);
        success &= locate("_ZN7TreeLib14getAttributefvEPvi",getAttributefv);
        success &= locate("_ZN7TreeLib13getAttributeiEPvi",getAttributei);
		success &= locate("_ZN7TreeLib14getAttributeivEPvi",getAttributeiv);
        success &= locate("_ZN7TreeLib14getAttributecvEPvi",getAttributecv);
        success &= locate("_ZN7TreeLib13setAttributefEPvif",setAttributef);
        success &= locate("_ZN7TreeLib13setAttributeiEPvii",setAttributei);
        success &= locate("_ZN7TreeLib14setAttributefvEPviPKf",setAttributefv);
        success &= locate("_ZN7TreeLib14setAttributecvEPviPKc",setAttributecv);
        success &= locate("_ZN7TreeLib14pushAttributesEv",pushAttributes);
        success &= locate("_ZN7TreeLib13popAttributesEv",popAttributes);
        success &= locate("_ZN7TreeLib10renderTreeEPv",renderTree);

        success &= locate("_ZN7TreeLib23renderCompositionLeavesEv",renderCompositionLeaves);
        success &= locate("_ZN7TreeLib24renderCompositionShadowsEv",renderCompositionShadows);
        success &= locate("_ZN7TreeLib25renderCompositionBranchesEv",renderCompositionBranches);
        success &= locate("_ZN7TreeLib16flushCompositionEv",flushComposition);
        success &= locate("_ZN7TreeLib17renderCompositionEv",renderComposition);
        success &= locate("_ZN7TreeLib7putTreeEPv",putTree);




        if (!success)
        {
            dll_error = true;
            local_error = "Failed to link one or more symbols.";
            unload();
            return false;
        }

        dll_error = false;
        return init(filename);
    }

void    TreeLib::unload()
    {
        if (!module_handle)
            return;
        #ifdef SYSTEM_WINDOWS
            FreeLibrary(module_handle);
        #elif defined(SYSTEM_UNIX)
            dlclose(module_handle);
        #endif
        module_handle = NULL;
    }

    const char* TreeLib::error()
    {
        if (dll_error)
        {
            if (local_error)
                return local_error;
            #ifdef SYSTEM_WINDOWS
                LPVOID lpMsgBuf;
                FormatMessage(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    GetLastError(),
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
                    (LPTSTR) &lpMsgBuf,
                    0,
                    NULL
                    );
                size_t len = strlen((char*)lpMsgBuf);
                if (len >= sizeof(error_buffer))
                    len = sizeof(error_buffer)-1;
                memcpy(error_buffer,lpMsgBuf,len);
                error_buffer[len] = 0;
                LocalFree( lpMsgBuf );
                return error_buffer;
            #elif defined(SYSTEM_UNIX)
                return dlerror();
            #endif
        }
        if (!getAttributecv)
            return "Library not loaded";
        return getAttributecv(NULL,TL_ERROR);
    }

