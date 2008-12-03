#include "stdafx.h"
#include "GraphicsDevice.h"
#include "Fog.h"

Fog::Fog(function<void (float,float)> _setClippingPlanes)
: density(1),
  nearClip(1),
  farClip(60),
  fogColor(white),
  setClippingPlanes(_setClippingPlanes)
{}

PropertyBag Fog::save() const
{
	PropertyBag xml;

	xml.add("near",    nearClip);
	xml.add("far",     farClip);
	xml.add("red",     fogColor.r);
	xml.add("green",   fogColor.g);
	xml.add("blue",    fogColor.b);

	return xml;
}

void Fog::load(const PropertyBag &xml)
{
	nearClip = xml.getFloat("near");
	farClip = xml.getFloat("far");
	fogColor.r = xml.getFloat("red");
	fogColor.g = xml.getFloat("green");
	fogColor.b = xml.getFloat("blue");
}

void Fog::activate() const
{
	// set the fog type
	glFogi(GL_FOG_MODE, GL_LINEAR);

	// set fog color
	glFogfv(GL_FOG_COLOR, fogColor);
	glClearColor(fogColor.r, fogColor.g, fogColor.b, 1.0f);

	// set density
	glFogf(GL_FOG_DENSITY,density);

	// set extents
	glFogf(GL_FOG_START, nearClip);
	glFogf(GL_FOG_END, farClip);
	//setClippingPlanes(nearClip, farClip);

	// set hint
	glHint(GL_FOG_HINT, GL_NICEST);

	// and switch on fog
	glEnable(GL_FOG);
}

void Fog::deactivate() const
{
	glDisable(GL_FOG);
}
