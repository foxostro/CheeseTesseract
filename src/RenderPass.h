#if defined(_MSC_VER)
#pragma once
#endif

#ifndef _RENDER_PASS_H_
#define _RENDER_PASS_H_

enum RENDER_PASS {
	OPAQUE_PASS=0,    /** Renders opaque objects */
	TRANSPARENT_PASS, /** Renders transparent objects */
	PARTICLE_PASS,    /** Renders particles */
	NUM_PASSES
};

#endif // _RENDER_PASS_H_