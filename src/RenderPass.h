#pragma once

enum RENDER_PASS
{
	OPAQUE_PASS=0,    /** Renders opaque objects */
	TRANSPARENT_PASS, /** Renders transparent objects */
	PARTICLE_PASS,    /** Renders particles */
	NUM_PASSES
};
