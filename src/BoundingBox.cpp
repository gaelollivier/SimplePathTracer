//
//  BoundingBox.cpp
//  SimplePathTracer
//
//  Created by gael on 28/03/13.
//
//

#include "BoundingBox.h"

#include "Renderer.h"

BoundingBox::BoundingBox(void) :
    min(), max()
{
}

BoundingBox::BoundingBox(vec3 min, vec3 max) :
    min(min), max(max)
{
}

BoundingBox::~BoundingBox(void) {
    
}

vec3 BoundingBox::getCenter(void) const {
    return (max - min) / 2.0f;
}

bool BoundingBox::intersectWithBox(const BoundingBox& other) const {
    if (   min.x > other.max.x || max.x < other.min.x
        || min.y > other.max.y || max.y < other.min.y
        || min.z > other.max.z || max.z < other.min.z)
        return false;
    return true;
}

float BoundingBox::intersectWithRay(const Ray& ray, bool* isInBox) const {
    // From http://madebyevan.com/webgl-path-tracing/webgl-path-tracing.js
    vec3 tMin = (min - ray.origin) / ray.direction;
    vec3 tMax = (max - ray.origin) / ray.direction;
    vec3 t1 = glm::min(tMin, tMax);
    vec3 t2 = glm::max(tMin, tMax);
    float tNear = glm::max(glm::max(t1.x, t1.y), t1.z);
    float tFar = glm::min(glm::min(t2.x, t2.y), t2.z);
    if (tNear > tFar)
        return -1.0f;
    if (tNear < Renderer::Epsilon) {
        if (isInBox)
            *isInBox = true;
        return  tFar;
    }
    return tNear;
}