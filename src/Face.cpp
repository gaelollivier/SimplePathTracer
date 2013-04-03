//
//  Face.cpp
//  SimplePathTracer
//
//  Created by gael on 28/03/13.
//
//

#include "Face.h"

#include "Renderer.h"

Face::Face(vec3 p0, vec3 p1, vec3 p2, Material* material) :
    Object(vec3(), material), _points(), _normals(), _textureCoords()
{
    _points[0] = p0;
    _points[1] = p1;
    _points[2] = p2;
}

Face::~Face(void) {
}

void Face::setNormals(const vec3& n1, const vec3& n2, const vec3& n3) {
    _normals[0] = n1;
    _normals[1] = n2;
    _normals[2] = n3;
}

void Face::setTextureCoords(const vec2& t1, const vec2& t2, const vec2& t3) {
    _textureCoords[0] = t1;
    _textureCoords[1] = t2;
    _textureCoords[2] = t3;
}

void Face::computeNormal(void) {
    vec3 normal = normalize(cross(_points[1] - _points[0], _points[2] - _points[0]));
    for (uint8_t i = 0; i < 3; ++i)
        _normals[i] = normal;
}

void Face::update(void) {
    Object::update();
    for (uint8_t i = 0; i < 3; ++i) {
        _absolutePoints[i] = vec3(_transformationMatrix * vec4(_points[i], 1));
        _normals[i] = normalize(vec3(_transformationMatrix * vec4(_normals[i], 1)));
    }
    // Update bouding box
    vec3 pMin = _absolutePoints[0];
    vec3 pMax = _absolutePoints[0];
    for (uint8_t i = 0; i < 3; ++i) {
        pMin = min(pMin, _absolutePoints[i]);
        pMax = max(pMax, _absolutePoints[i]);
    }
    _boundingBox = BoundingBox(pMin, pMax);
}

float Face::intersectWithRay(const Ray& ray) {
    // From http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
    
    float u, v, t;
    vec3 edge1, edge2, tvec, pvec, qvec;
    float det, inv_det;
    
    // find vectors for two edges sharing vert0
    edge1 = _absolutePoints[1] - _absolutePoints[0];
    edge2 = _absolutePoints[2] - _absolutePoints[0];
    
    // Begin calculating determinant - also used to calculate U parameter
    pvec = cross(ray.direction, edge2);
    
    // If determinant is near zero, ray lies in plane of triangle
    det = dot(edge1, pvec);
    
    if (false) { // Back-face culling -- Seams not working...
        if (det < Renderer::Epsilon)
            return -1.0f;
        
        // Calculate distance from point 0 to ray origin
        tvec = ray.origin - _absolutePoints[0];
        
        // Calculate U parameter and test bounds
        u = dot(tvec, pvec);
        if (u < 0.0f || u > det)
            return -1.0f;
        
        // Prepare to test V parameter
        qvec = cross(tvec, edge1);
        
        // Calculate V parameter and test bounds
        v = dot(ray.direction, qvec);
        if (v < 0.0f || u + v > det)
            return 0.0f;
        
        // Calculate t, ray intersects triangle
        t = dot(edge2, qvec);
        t *= 1.0f / det;
        
        return t;
    } else { // No back-face culling
        if (det > -Renderer::Epsilon && det < Renderer::Epsilon)
            return -1.0f;
        inv_det = 1.0 / det;
        
        // Calculate distance from point 0 to ray origin
        tvec = ray.origin - _absolutePoints[0];
        
        // Calculate U parameter and test bounds
        u = dot(tvec, pvec) * inv_det;
        if (u < 0.0f || u > 1.0f)
            return -1.0f;
        
        // Prepare to test V parameter
        qvec = cross(tvec, edge1);
        
        // Calculate V parameter and test bounds
        v = dot(ray.direction, qvec) * inv_det;
        if (v < 0.0f || u + v > 1.0f)
            return -1.0f;
        
        // Calculate t, ray intersects triangle
        t = dot(edge2, qvec) * inv_det;
        return t;
    }
    
    return -1.0;
}

static float calcArea(const vec3& p1, const vec3& p2, const vec3& p3) {
    vec3 a = p1 - p2;
    vec3 b = p1 - p3;
    float na = length(a);
    float nb = length(b);
    float cosA = dot(a, b) / (na * nb);
    
    cosA = acos(cosA);
    return (0.5f * na * nb * sin(cosA));
}

vec3 Face::normalAtPoint(const vec3& point) {
    
    float
    aT = calcArea(_absolutePoints[0], _absolutePoints[1], _absolutePoints[2]),
    aB = calcArea(point, _absolutePoints[0], _absolutePoints[1]),
    aC = calcArea(point, _absolutePoints[2], _absolutePoints[1]),
    aA = aT - aB - aC;
    float
    c1 = aA / aT,
    c2 = aB / aT,
    c3 = aC / aT;
    vec3 normal = _normals[0] * c3 + _normals[1] * c1 + _normals[2] * c2;
    return normalize(normal);
}
