//
//  Cube.h
//  SimplePathTracer
//
//  Created by gael on 27/03/13.
//
//

#ifndef __SimplePathTracer__Cube__
#define __SimplePathTracer__Cube__

#include "Object.h"

class Cube : public Object {
public:
    
    Cube(vec3 min, vec3 max, Material* material=NULL);
    Cube(vec3 center, float size, Material* material=NULL);
    
    virtual ~Cube(void);
    
    virtual void update(void);
    
    virtual float intersectWithRay(const Ray& ray);
    
    virtual vec3 normalAtPoint(const vec3& point);
    
private:
    vec3    _min;
    vec3    _max;
};

#endif /* defined(__SimplePathTracer__Cube__) */
