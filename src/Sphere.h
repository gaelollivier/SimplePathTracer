//
//  Sphere.h
//  SimplePathTracer
//
//  Created by gael on 26/03/13.
//
//

#ifndef __SimplePathTracer__Sphere__
#define __SimplePathTracer__Sphere__

#include "Object.h"

class Sphere : public Object {
public:
    
    Sphere(vec3 position, float radius, Material* material=NULL);
    
    virtual ~Sphere(void);
    
    virtual void update(void);
    
    virtual float intersectWithRay(const Ray& ray);
    
    virtual vec3 normalAtPoint(const vec3& point);
    
private:
    float  _radius;
};

#endif /* defined(__SimplePathTracer__Sphere__) */
