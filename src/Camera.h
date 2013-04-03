//
//  Camera.h
//  SimplePathTracer
//
//  Created by gael on 27/03/13.
//
//

#ifndef __SimplePathTracer__Camera__
#define __SimplePathTracer__Camera__

#include "Node.h"
#include "Ray.h"

class Camera : public Node {
public:
    
    Camera(void);
    Camera(vec2 size, vec3 position, vec3 target);
    virtual ~Camera(void);
    
    Ray getRayAtPoint(const vec2& point);
    
    virtual bool hasBoundingBox(void) const;
    
    vec2    getSize(void) const;
    void    setSize(const vec2& size);
    
    vec3    getTarget(void) const;
    void    setTarget(const vec3& target);
    
    float   getFovy(void) const;
    void    setFovy(float fovy);
    
    vec3    getUpVector(void) const;
    void    setUpVector(const vec3& upVector);
    
    float   getNearClippingPlane(void) const;
    void    setNearClippingPlane(float nearClippingPlane);
    
    float   getAperture(void) const;
    void    setAperture(float aperture);
    
    float   getFocusDistance(void) const;
    void    setFocusDistance(float focusDistance);
    
private:
    vec2    _size;
    vec3    _target;
    float   _fovy;
    vec3    _upVector;
    float   _nearClippingPlaneDistance;
    float   _focusDistance;
    float   _aperture;
};

#endif /* defined(__SimplePathTracer__Camera__) */
