//
//  Light.cpp
//  SimplePathTracer
//
//  Created by gael on 26/03/13.
//
//

#include "Light.h"

#include "Renderer.h"

Light::Light(vec3 position, vec3 color, float radius) :
    Node(position), _color(color), _radius(radius)
{
}

Light::~Light(void) {
}

void Light::update(void) {
    Node::update();
    _boundingBox = BoundingBox(_absolutePosition - _radius, _absolutePosition + _radius);
}

vec3 Light::getColor(void) const {
    return _color;
}

float Light::getRadius(void) const {
    return _radius;
}

vec3 Light::getSampledPosition(void) {
    return _absolutePosition + (Renderer::uniformSample()*2.0f - 1.0f) * _radius;
}

float Light::intersectWithRay(const Ray& ray) {
    // From http://en.wikipedia.org/wiki/Line–sphere_intersection
    vec3 oc = ray.origin - _absolutePosition;
    float a = dot(ray.direction, ray.direction);
    float b = 2.0 * dot(ray.direction, oc);
    float c = dot(oc, oc) - _radius * _radius;
    float delta = b*b - 4.0*a*c;
    if (delta > 0.0) {
        float d = (-b - sqrt(delta)) / (2.0*a);
        if (d > 0.0)
            return d;
    }
    return -1.0;
}

bool Light::hasBoundingBox(void) const {
    return true;
}