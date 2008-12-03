#ifndef _DIMMER_H_
#define _DIMMER_H_

/** Dims the screen */
class Dimmer
{
public:
	/** Constructs the Dimmer */
	Dimmer();

	/** Draw a dimming polygon over the screen */
	void draw() const;

	/** The blur is enabled as long as this is positive, non-zero */
	static float alphaBlur;
};

#endif
