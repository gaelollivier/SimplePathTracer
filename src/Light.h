//
//  Light.h
//  SimplePathTracer
//
//  Created by gael on 26/03/13.
//
//

#ifndef __SimplePathTracer__Light__
#define __SimplePathTracer__Light__

#include "Node.h"

class Light : public Node {
public:
    
    Light(vec3 position, vec3 color, float radius);
    ~Light(void);
    
    virtual void update(void);
    
    vec3    getColor(void) const;
    float   getRadius(void) const;
    vec3    getSampledPosition(void);
    
    virtual float intersectWithRay(const Ray& ray);
    
    virtual bool hasBoundingBox(void) const;
    
private:
    vec3    _color;
    float   _radius;
};

#endif /* defined(__SimplePathTracer__Light__) */
