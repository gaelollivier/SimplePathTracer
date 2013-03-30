//
//  Plane.h
//  SimplePathTracer
//
//  Created by gael on 26/03/13.
//
//

#ifndef __SimplePathTracer__Plane__
#define __SimplePathTracer__Plane__

#include "Object.h"

class Plane : public Object {
public:
    
    Plane(vec3 position, vec3 normal, Material* material=NULL);
    
    virtual ~Plane(void);
    
    virtual float intersectWithRay(const Ray& ray);
    virtual vec3 normalAtPoint(const vec3&);
    
    virtual bool hasBoundingBox(void) const;
    
private:
    vec3    _normal;
};


#endif /* defined(__SimplePathTracer__Plane__) */
