//
//  Camera.cpp
//  SimplePathTracer
//
//  Created by gael on 27/03/13.
//
//

#include "Camera.h"

#include "Renderer.h"

Camera::Camera(void) :
    Camera(vec2(), vec3(), vec3())
{
}

Camera::Camera(vec2 size, vec3 position, vec3 target) :
    Node(position), _size(size), _target(target),
    _fovy(45.0), _upVector(0.0, 1.0, 0.0), _nearClippingPlaneDistance(0.1),
    _focusDistance(0.0), _aperture(0.0)
{
}

Camera::~Camera(void) {
}

vec2 Camera::getSize(void) const {
    return _size;
}

void Camera::setSize(const vec2& size) {
    _size = size;
}

vec3 Camera::getTarget(void) const {
    return _target;
}

void Camera::setTarget(const vec3& target) {
    _target = target;
}

float Camera::getFovy(void) const {
    return _fovy;
}

void Camera::setFovy(float fovy) {
    _fovy = fovy;
}

vec3 Camera::getUpVector(void) const {
    return _upVector;
}

void Camera::setUpVector(const vec3& upVector) {
    _upVector = upVector;
}

float Camera::getNearClippingPlane(void) const {
    return _nearClippingPlaneDistance;
}

void Camera::setNearClippingPlane(float nearClippingPlane) {
    _nearClippingPlaneDistance = nearClippingPlane;
}

float Camera::getAperture(void) const {
    return _aperture;
}

void Camera::setAperture(float aperture) {
    _aperture = aperture;
}

float Camera::getFocusDistance(void) const {
    return _focusDistance;
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