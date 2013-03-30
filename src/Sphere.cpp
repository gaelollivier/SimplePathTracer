//
//  Sphere.cpp
//  SimplePathTracer
//
//  Created by gael on 26/03/13.
//
//

#include "Sphere.h"

Sphere::Sphere(vec3 position, float radius, Material* material) :
    Object(position, material), _radius(radius)
{
}

Sphere::~Sphere(void) {
}

void Sphere::update(void) {
    Object::update();
    _boundingBox = BoundingBox(_absolutePosition - _radius, _absolutePosition + _radius);
}

float Sphere::intersectWithRay(const Ray& ray) {
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

vec3 Sphere::normalAtPoint(const vec3& point) {
    return normalize(point - _absolutePosition);
}