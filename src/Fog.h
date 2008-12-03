#ifndef FOG_H
#define FOG_H

#include "PropertyBag.h"
#include "color.h"

/** Controls OpenGL fog settings */
class Fog
{
private:
	/** Fog Density */
	float density;

	/** Near distance of fog */
	float nearClip;

	/** Far distance of fog */
	float farClip;

	/** Fog color */
	color fogColor;

	/** Function to set the clipping planes for the fog */
	function<void (float,float)> setClippingPlanes;

public:
	/**
	Constructor
	@param setClippingPlanes Function to set the clipping planes for the fog
	*/
	Fog(function<void (float,float)> setClippingPlanes);

	/**
	Saves the fog to XML
	@return XML reprentation of the fog
	*/
	PropertyBag save() const;

	/**
	Loads the fog settings from XML
	@param xml XML source
	*/
	void load(const PropertyBag &xml);

	/** Activate the fog settings */
	void activate() const;

	/** Deactivate fog */
	void deactivate() const;
};

#endif
