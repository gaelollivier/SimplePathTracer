//
//  Cube.cpp
//  SimplePathTracer
//
//  Created by gael on 27/03/13.
//
//

#include "Cube.h"

#include "Renderer.h"

Cube::Cube(vec3 min, vec3 max, Material* material) :
    Object((max - min) / 2.0f, material), _min(min), _max(max)
{
}

Cube::Cube(vec3 center, float size, Material* material) :
    Object(center, material), _min(center - size / 2.0f), _max(center + size / 2.0f)
{
}

Cube::~Cube(void) {
}

void Cube::update(void) {
    Object::update();
    _min += _absolutePosition;
    _max += _absolutePosition;
    _boundingBox = BoundingBox(_min, _max);
}

float Cube::intersectWithRay(const Ray& ray) {
    // From http://madebyevan.com/webgl-path-tracing/webgl-path-tracing.js
    vec3 tMin = (_min - ray.origin) / ray.direction;
    vec3 tMax = (_max - ray.origin) / ray.direction;
    vec3 t1 = min(tMin, tMax);
    vec3 t2 = max(tMin, tMax);
    float tNear = max(max(t1.x, t1.y), t1.z);
    float tFar = min(min(t2.x, t2.y), t2.z);
    if (tNear > tFar)
        return -1.0;
    return tNear;
}

vec3 Cube::normalAtPoint(const vec3& point) {
    if(point.x < _min.x + Renderer::Epsilon) return vec3(-1.0, 0.0, 0.0);
    else if(point.x > _max.x - Renderer::Epsilon) return vec3(1.0, 0.0, 0.0);
    else if(point.y < _min.y + Renderer::Epsilon) return vec3(0.0, -1.0, 0.0);
    else if(point.y > _max.y - Renderer::Epsilon) return vec3(0.0, 1.0, 0.0);
    else if(point.z < _min.z + Renderer::Epsilon) return vec3(0.0, 0.0, -1.0);
    else return vec3(0.0, 0.0, 1.0);
}