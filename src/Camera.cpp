//
//  Camera.cpp
//  SimplePathTracer
//
//  Created by gael on 27/03/13.
//
//

#include "Camera.h"

#include "Renderer.h"

Camera::Camera(vec3 position, vec3 target, vec2 size,
               float fovy, const vec3& upVector,
               float nearClippingPlaneDistance) :
    Node(position), _target(target), _size(size),
    _fovy(fovy), _upVector(upVector),
    _nearClippingPlaneDistance(nearClippingPlaneDistance),
    _focusDistance(0.0), _aperture(0.0)
{
}


Camera::~Camera(void) {
}

void Camera::setAperture(float aperture) {
    _aperture = aperture;
}

void Camera::setFocusDistance(float focusDistance) {
    _focusDistance = focusDistance;
}

Ray Camera::getRayAtPoint(const vec2& point) {
    // From http://schabby.de/picking-opengl-ray-tracing/
	vec3 view = normalize(_target - _absolutePosition);
	vec3 h = normalize(cross(view, _upVector));
	vec3 v = normalize(cross(h, view));
	
	float rad = _fovy * M_PI / 180.0;
	float vLength = tan(rad / 2.0) * _nearClippingPlaneDistance;
	float hLength = vLength * (_size.x / _size.y);
	
	h *= hLength;
	v *= vLength;
	
    vec2 screenSpace = 2.0f * point - 1.0f;
    
	vec3 projectPoint = _absolutePosition + view * _nearClippingPlaneDistance +
                        h * screenSpace.x + v * screenSpace.y;
	vec3 direction = normalize(projectPoint - _absolutePosition);
    
    vec3 origin = _absolutePosition;
    
    // Add depth of field !
    if (_aperture > 0.0f) {
        vec3 focusPoint = projectPoint + direction * _focusDistance;
        origin = origin + Renderer::uniformSample() * _aperture;
        direction = normalize(focusPoint - origin);
    }
    
	return Ray(origin, direction);
}

bool Camera::hasBoundingBox(void) const {
    return false;
}