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
    
    Camera(vec3 position, vec3 target, vec2 size,
           float fovy=45, const vec3& upVector=vec3(0, 1, 0),
           float nearClippingPlaneDistance=0.1);
    virtual ~Camera(void);
    
    Ray getRayAtPoint(const vec2& point);
    
    virtual bool hasBoundingBox(void) const;
    
    void setAperture(float aperture);
    void setFocusDistance(float focusDistance);
    
private:
    vec3    _target;
    vec2    _size;
    float   _fovy;
    vec3    _upVector;
    float   _nearClippingPlaneDistance;
    float   _focusDistance;
    float   _aperture;
};

#endif /* defined(__SimplePathTracer__Camera__) */
