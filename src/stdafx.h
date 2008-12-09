#if defined(_MSC_VER)
#pragma once
#endif

#ifndef _STDAFX_H_
#define _STDAFX_H_

/* Special directives for Microsoft Visual Studio */
#if defined(_MSC_VER)
#include "msvc_rules.h"
#endif

/* SDL main */
#include <SDL/SDL.h>

/* Engine Libraries */
#include "Core.h"

/* OpenGL Library */
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

/* ODE Physics Engine */
#include "ode_wrapper.h"

#endif
