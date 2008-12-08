#include "stdafx.h"
#include "GraphicsDevice.h"
#include "Camera.h"

void Camera::clear() {
	eye = vec3(1,0,0);
	center = vec3(0,0,0);
	up = vec3(0,0,1);
}

void Camera::setCamera() const {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	gluLookAt(eye.x, eye.y, eye.z,
	          center.x, center.y, center.z,
	          up.x, up.y, up.z);
}

void Camera::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) {
	this->eye = eye;
	this->center = center;
	this->up = up;
}

mat3 Camera::getOrientation() const {
	const mat4 modl = GraphicsDevice::getModelViewMatrix();
	mat3 orientation;
	orientation.setAxisX(modl.getAxisX());
	orientation.setAxisY(modl.getAxisY());
	orientation.setAxisZ(modl.getAxisZ());
	return orientation;
}
