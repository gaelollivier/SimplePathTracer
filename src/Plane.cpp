//
//  Plane.cpp
//  SimplePathTracer
//
//  Created by gael on 26/03/13.
//
//

#include "Plane.h"

Plane::Plane(void) :
    Plane(vec3(), vec3(), NULL)
{
}

Plane::Plane(vec3 position, vec3 normal, Material* material) :
    Object(position, material), _normal(normalize(normal))
{
}

Plane::~Plane(void) {
}

vec3 Plane::getNormal(void) const {
    return _normal;
}

void Plane::setNormal(const vec3& normal) {
    _normal = normal;
}

float Plane::intersectWithRay(const Ray& ray) {
    // From http://en.wikipedia.org/wiki/Line-plane_intersection
    float denom = dot(ray.direction, _normal);
    if (denom != 0.0) {
        return dot((_absolutePosition - ray.origin), _normal) / denom;
    }
    return -1.0;
}

vec3 Plane::normalAtPoint(const vec3&) {
    return _normal;
}

bool Plane::hasBoundingBox(void) const {
    return false;
}